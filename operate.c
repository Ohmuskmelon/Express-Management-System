#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/un.h>
#include <string.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

time_t now;

/*server.c*/

#define USER_NUMBER 100
#define EXPRESS_NUMBER 100

typedef struct user_message
{
    int type;
    int id;
    int password;
} user_message;

typedef struct user_info
{
    int id;
    int password;
    int express_num;
    pthread_mutex_t user_info_mutex;
} user_info;

typedef user_info user[USER_NUMBER];

typedef struct express_info
{
    int id;
    int user_id;
    int isIn;
    int date;
    int time;
    pthread_mutex_t express_info_mutex;
} express_info;

typedef express_info express[EXPRESS_NUMBER];

/* 根据id查找用户，查找成功返回用户数据的下标，查找失败返回-1 */
int find_user(user data, int user_num, int user_id)
{
    for (int i=0; i<user_num; i++)
    {
        if (data[i].id == user_id)
            return i;
    }
    return -1;
}

/* 添加用户信息，成功返回目前用户数量，用户已存在返回-1，超出最大用户限制返回0 */
int add_user(user data, int user_num, int user_maxnum, int user_id, int password)
{
    int ori_user_num = user_num;
    pthread_mutex_lock(&data[ori_user_num].user_info_mutex);
    if (user_num >= user_maxnum)
        return 0;
    if (find_user(data, user_num, user_id) != -1)
        return -1;
    user_info cnt_user;
    cnt_user.id = user_id;
    cnt_user.password = password;
    cnt_user.express_num = 0;
    data[user_num] = cnt_user;
    user_num++;
    pthread_mutex_unlock(&data[ori_user_num].user_info_mutex);
    return user_num;
}

/* 检查用户输入的密码是否正确，用户不存在返回-1，用户密码错误返回-2，用户密码正确返回用户数据下标 */
int check_user(user data, int user_num, int user_id, int password)
{
    int user_index = find_user(data, user_num, user_id);
    if (user_index == -1)
        return -1;
    if (data[user_index].password != password)
        return -2;
    pthread_mutex_lock(&data[user_num].user_info_mutex);
    pthread_mutex_unlock(&data[user_num].user_info_mutex);
    return user_index;
}

/* 更改用户密码，成功返回用户数据下标，用户不存在返回-1 */
int change_user_password(user data, int user_num, int user_id, int password)
{
    int user_index = find_user(data, user_num, user_id);
    if (user_index == -1)
        return -1;
    pthread_mutex_lock(&data[user_index].user_info_mutex);
    data[user_index].password = password;
    pthread_mutex_unlock(&data[user_num].user_info_mutex);
    return user_index;
}

/* 添加快递，无法添加快递返回-1，添加成功返回目前快递数量 */
int add_express(express exp_data, user user_data, int exp_num, int exp_maxnum, int user_num, int user_id)
{
    int ori_exp_num = user_num;
    pthread_mutex_lock(&exp_data[ori_exp_num].express_info_mutex);
    time_t time_ptr;
    struct tm *tmp_ptr = NULL;
    time(&time_ptr);
    tmp_ptr = localtime(&time_ptr);
    if (exp_num >= exp_maxnum)
        return -1;
    express_info cnt_exp;
    cnt_exp.id = exp_num;
    cnt_exp.isIn = TRUE;
    cnt_exp.user_id = user_id;
    int year = tmp_ptr->tm_year + 1900;
    int month = tmp_ptr->tm_mon + 1;
    int day = tmp_ptr->tm_mday;
    int hour = tmp_ptr->tm_hour;
    int minute = tmp_ptr->tm_min;
    int sec = tmp_ptr->tm_sec;
    cnt_exp.date = year*10000+month*100+day;
    cnt_exp.time = hour*10000+minute*100+sec;
    exp_data[exp_num] = cnt_exp;
    exp_num++;
    user_data[find_user(user_data, user_num, user_id)].express_num++;
    pthread_mutex_unlock(&exp_data[ori_exp_num].express_info_mutex);
    return exp_num;
}

/* 查找用户所有的快递，所有快递的索引号存在result中，成功返回用户快递数量，失败返回0 */
int find_user_express(express exp_data, int exp_num, user user_data, int user_num, int user_id, int* result)
{
    int user_exp_num = user_data[find_user(user_data, user_num, user_id)].express_num;
    if (user_exp_num == 0)
        return 0;
    int find_num = 0;
    for (int i=0; i<exp_num; i++)
    {
        if (find_num == exp_num)
            break;
        if (exp_data[i].user_id == user_id)
        {
            result[find_num] = i;
            find_num++;
        }
    }
    return user_exp_num;
}

/* 根据id查找快递，查找成功返回快递数据的下标，查找失败返回-1 */
int find_single_express(express exp_data, int exp_num, int exp_id)
{
    for(int i=0; i<exp_num; i++)
    {
        if (exp_data[i].id == exp_id)
            return i;
    }
    return -1;
}

/* 通过idex移除快递，同时减少相应用户的快递数量，请保证数据正确 */
void remove_express_by_index(express exp_data, int exp_num, int exp_index, user user_data, int user_num)
{
    pthread_mutex_lock(&exp_data[exp_index].express_info_mutex);
    exp_data[exp_index].isIn = FALSE;
    int user_id = exp_data[exp_index].user_id;
    int user_index = find_user(user_data, user_num, user_id);
    pthread_mutex_lock(&user_data[user_index].user_info_mutex);
    user_data[user_index].express_num--;
    pthread_mutex_unlock(&user_data[user_index].user_info_mutex);
    pthread_mutex_unlock(&exp_data[exp_index].express_info_mutex);
}

/* 通过id移除快递，同时减少相应用户的快递数量，请保证数据正确 */
void remove_express_by_id(express exp_data, int exp_num, int exp_id, user user_data, int user_num)
{
    int exp_index = find_single_express(exp_data, exp_num, exp_id);
    pthread_mutex_lock(&exp_data[exp_index].express_info_mutex);
    exp_data[exp_index].isIn = FALSE;
    int user_id = exp_data[exp_index].user_id;
    int user_index = find_user(user_data, user_num, user_id);
    pthread_mutex_lock(&user_data[user_index].user_info_mutex);
    user_data[user_index].express_num--;
    pthread_mutex_unlock(&user_data[user_index].user_info_mutex);
    pthread_mutex_unlock(&exp_data[exp_index].express_info_mutex);
}

int main()
{
    user u;
    express p;
    int user_num = 0;
    int exp_num = 0;

    user_num = add_user(u, user_num, USER_NUMBER, 12345, 19999);
    user_num = add_user(u, user_num, USER_NUMBER, 222222, 222229);
    printf("creat user\n");
    printf("%d %d\n", u[0].id, u[0].password);
    printf("\n");

    printf("find user 12345:\n");
    printf("%d\n", find_user(u, user_num, 222222));
    printf("\n");

    int check1 = check_user(u, user_num, 12345, 18888);
    int check2 = check_user(u, user_num, 12345, 19999);
    printf("check id:12345 pass:18888 id:12345 pass:19999\n");
    if (check1 == -2)
        printf("Wrong!\n");
    if (check2 >= 0)
        printf("Yes!\n");
    printf("\n");

    int a = change_user_password(u, user_num, 12345, 1111111);
    for (int i = 0; i < user_num; i++)
        printf("%d %d\n", u[i].id, u[i].password);

    exp_num = add_express(p, u, exp_num, EXPRESS_NUMBER, user_num, 12345);
    exp_num = add_express(p, u, exp_num, EXPRESS_NUMBER, user_num, 12345);
    exp_num = add_express(p, u, exp_num, EXPRESS_NUMBER, user_num, 222222);
    exp_num = add_express(p, u, exp_num, EXPRESS_NUMBER, user_num, 12345);
    exp_num = add_express(p, u, exp_num, EXPRESS_NUMBER, user_num, 222222);
    printf("add expresses and check all expresses:\n");
    for (int i = 0; i < exp_num; i++)
        printf("%d %d %d %d %d\n", p[i].id, p[i].user_id, p[i].isIn, p[i].date, p[i].time);
    printf("\n");

    int result[100];
    int renum = find_user_express(p, exp_num, u, user_num, 12345, result);
    express_info list[renum];
    for (int i=0; i<renum; i++)
    {
        list[i].id = p[result[i]].id;
        list[i].user_id = p[result[i]].user_id;
    }
    express_info* output;
    output = list;
    printf("find all expresses' indexes from id:12345:\n");
    for (int i = 0; i < renum; i++)
        printf("%d %d\n", output[i].id, output[i].user_id);
    printf("\n");
    remove_express_by_index(p, exp_num, 0, u, user_num);
    remove_express_by_id(p, exp_num, 4, u, user_num);
    printf("check all expresses:\n");
    for (int i = 0; i < exp_num; i++)
        printf("%d %d %d %d %d\n", p[i].id, p[i].user_id, p[i].isIn, p[i].date, p[i].time);
    return 0;
}