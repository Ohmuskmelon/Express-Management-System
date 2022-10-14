# include <stdbool.h>
# include <time.h>

time_t now;

/*server.c*/

# define USER_NUMBER 100
# define EXPRESS_NUMBER 100

int express_id = 0;

typedef struct user_info
{
    int id;
    int password;
    int express_num;
} user_info;

typedef user_info* user;

typedef struct express_info
{
    int id;
    int user_id;
    bool isIn;
    int date;
    int time;
} express_info;

typedef express_info* express;


// 根据id查找用户，查找成功返回用户数据的下标，查找失败返回-1
int find_user(user data, int user_num, int user_id)
{
    for (int i=0; i<user_num; i++)
    {
        if (data[i].id == user_id)
            return i;
    }
    return -1;
}

// 添加用户信息，成功返回目前用户数量，用户已存在返回-1，超出最大用户限制返回0
int add_user(user data, int user_num, int user_maxnum, int user_id, int password)
{
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
    return user_num;
}

// 检查用户输入的密码是否正确，用户不存在返回-1，用户密码错误返回0，用户密码正确返回用户数据下标
int check_user(user data, int user_num, int user_id, int password)
{
    int user_index = find_user(data, user_num, user_id);
    if (user_index == -1)
        return -1;
    if (data[user_index].password != password)
        return 0;
    return user_index;
}

// 添加快递，无法添加快递返回-1，添加成功返回目前快递数量
int add_express(express exp_data, user user_data, int exp_num, int exp_maxnum, int user_num, int user_id)
{
    if (exp_num >= exp_maxnum)
        return -1;
    express_info cnt_exp;
    cnt_exp.id = exp_num;
    cnt_exp.isIn = true;
    cnt_exp.user_id = user_id;
    struct tm *tm_now = localtime(&now);
    cnt_exp.date = tm_now->tm_year*10000+tm_now->tm_mon*100+tm_now->tm_mday;
    cnt_exp.time = tm_now->tm_hour*100+tm_now->tm_min;
    exp_data[exp_num] = cnt_exp;
    exp_num++;
    user_data[find_user(user_data, user_num, user_id)].express_num++;
    return exp_num;
}

// 查找用户所有的快递，返回一个装有用户快递数据下标的数组，用户快递数量为0返回NULL
int* find_user_express(express exp_data, int exp_num, user user_data, int user_num, int user_id)
{
    int user_exp_num = user_data[find_user(user_data, user_num, user_id)].express_num;
    if (user_exp_num == 0)
        return NULL;
    int find_num = 0;
    int result[user_exp_num];
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
    return result;
}

// 根据id查找快递，查找成功返回快递数据的下标，查找失败返回-1
int find_single_express(express exp_data, int exp_num, int exp_id)
{
    for(int i=0; i<exp_num; i++)
    {
        if (exp_data[i].id == exp_id)
            return i;
    }
    return -1;
}
 // 通过索引移除快递，同时减少相应用户的快递数量，请保证数据正确
int remove_express_by_index(express exp_data, int exp_num, int exp_index, user user_data, int user_num, int user_id)
{
    exp_data[exp_index].isIn = false;
    user_data[find_user(user_data, user_num, user_id)].express_num--;
}

 // 通过id移除快递，同时减少相应用户的快递数量，请保证数据正确
int remove_express_by_id(express exp_data, int exp_num, int exp_id, user user_data, int user_num, int user_id)
{
    exp_data[find_single_express(exp_data, exp_num, exp_id)].isIn = false;
    user_data[find_user(user_data, user_num, user_id)].express_num--;
}


int main()
{
    express e;
    return 0;
}