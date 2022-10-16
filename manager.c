/*manger.c*/
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
#include <time.h>
#include <string.h>

#define SERVER_PORT_NO 8889

/*客户端与服务器端使用的消息类型定义*/
#define INITIAL_VALUE 65535

#define THREAD_BUFF_NUM 128

/*客户端使用的消息代码含义*/
// #define DISCONNECT 0
#define APPLY_ROOM 1
#define INQUIRE_ONE 2
#define INQUIRE_ALL 3

/*服务器端使用的消息代码含义*/
#define APPLY_SUCCEED 255
#define APPLY_FAILED 256
#define INQUIRE_SUCCEED 257
#define UNKNOWN_CODE 258

#define TRUE 1
#define FALSE 0

#define ROOM_NUM 6 //教学楼总数

#define DEFAULT_SIZE 100
#define BUFF_SIZE 512
#define SEND_BUFF_SIZE 4096
#define RECV_BUFF_SIZE 4096

#define DEFAULT_TYPE 0
#define USER_SIGN_UP 1
#define USER_SIGN_IN 2
#define USER_CHANGE_PASSWORD 3
#define MANAGER_SIGN_UP 4
#define MAGAGER_SIGN_IN 5
#define MAGAGER_CHANGE_PASSWORD 6
#define CHECK_ALL_EXPRESSES 7
#define GET_SINGLE_EXPRESS 8
#define GET_ALL_EXPRESS 9
#define ADD_EXPRESS 10
#define OK 11
#define NOT_OK 12
#define NOT_EXITS 13
#define ALREADY_EXITS 14
#define WRONG_PASSWORD 15
#define ALL_EXPRESSES_DATA 16
#define ONE_EXPRESSES_DATA 17
#define DISCONNECT 18

/*定义线程缓冲区的使用状态*/
#define BUFF_OCCUPIED 1
#define BUFF_FREED 0
// #define BUFF_SIZE 512

#define USER_NUMBER 100
#define EXPRESS_NUMBER 100

#define MAX 100 //总数

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
    int isIn;
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

typedef struct send_message
{
    int type;
    int user_id;
    int exp_id;
    int password;
} send_message;

typedef struct recv_message
{
    int type;
    express_info express_list[10];
    int list_num;
} recv_message;

int re_connect();

int socket_fd;             //连接socket
struct sockaddr_in server; //服务器地址信息，客户端地址信息
int ret, i;
int flag = 1;
char c;
int isconnected = FALSE; //是否已连接服务器

//消息内容输出函数
void display_info(char *msg)
{
    printf("%s\n", msg);
}

/*连接服务器操作*/
void connectserver()
{
    char msg[512]; //提示信息
    int i;

    if (!isconnected)
    {
        /*创建套接字*/
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd < 0)
        {
            sprintf(msg, "创建套接字出错！ \n");
            display_info(msg);
            return;
        }
        /*设置接收、发送超时值*/
        struct timeval time_out;
        time_out.tv_sec = 5;
        time_out.tv_usec = 0;
        setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &time_out, sizeof(time_out));

        /*填写服务器的地址信息*/
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr("127.0.0.1"); // htonl(INADDR_ANY);
        server.sin_port = htons(SERVER_PORT_NO);

        /*连接服务器*/
        ret = connect(socket_fd, (struct sockaddr *)&server, sizeof(server));
        if (ret < 0)
        {
            sprintf(msg, "连接服务器 %d 出错！\n", SERVER_PORT_NO);
            display_info(msg);
            close(socket_fd);
            re_connect();
            return;
        }

        //成功后输出提示信息
        printf("\t\t连接服务器成功！\n");

        isconnected = TRUE;
    }
}

/*断开连接操作*/
void disconnect()
{
    char msg[512];
    if (isconnected)
    {
        close(socket_fd);
        sprintf(msg, "断开连接成功！\n");
        display_info(msg);
        isconnected = FALSE;
    }
}

/*manger_sign_up函数*/
int manger_signup(int id, int password)
{
    char msg[512];
    char send_buf[512], recv_buf[512];

    send_message temp;
    recv_message temo;

    temp.type = MANAGER_SIGN_UP;
    temp.user_id = id;
    temp.exp_id = 0;
    temp.password = password;
    memcpy(send_buf, &temp, sizeof(send_message));
    int ret = send(socket_fd, send_buf, sizeof(send_message), 0);
    /*发送出错*/
    if (ret == -1)
    {
        printf("\t发送失败！请重新发送！\n");
        return (1);
    }
    ret = recv(socket_fd, recv_buf, sizeof(recv_message), 0);
    if (ret == -1)
    {
        printf("\t服务器端接收失败！请重新发送！\n");
        return (1);
    }
    memcpy(&temo, recv_buf, sizeof(recv_message));
    if (temo.type == OK)
        sprintf(msg, "恭喜你，注册成功！");
    else if (temo.type == ALREADY_EXITS)
        sprintf(msg, "账号已经存在");
    else
        sprintf(msg, "不好意思，服务器出错了。");
    display_info(msg);
}

// manger_sign_in函数
int manger_signin(int id, int password)
{
    char msg[512];
    char send_buf[512], recv_buf[512];

    send_message temp;
    recv_message temo;

    temp.type = MAGAGER_SIGN_IN;
    temp.user_id = id;
    temp.password = password;
    memcpy(send_buf, &temp, sizeof(send_message));
    int ret = send(socket_fd, send_buf, sizeof(send_message), 0);
    /*发送出错*/
    if (ret == -1)
    {
        printf("\t发送失败！请重新发送！\n");
        return (1);
    }
    ret = recv(socket_fd, recv_buf, sizeof(recv_message), 0);
    if (ret == -1)
    {
        printf("\t服务器端接收失败！请重新发送！\n");
        return (1);
    }
    memcpy(&temo, recv_buf, sizeof(recv_message));
    if (temo.type == OK)
        sprintf(msg, "恭喜你，登录成功！");
    else if (temo.type == NOT_EXITS)
        sprintf(msg, "不好意思，账号不存在。");
    else if (temo.type == WRONG_PASSWORD)
        sprintf(msg, "不好意思，密码错误");
    else
    {
        sprintf(msg, "不好意思，服务器出错了。");
    }
    display_info(msg);
}
// manger_change_pass函数
int manger_changepass(int id, int password)
{
    char msg[512];
    char send_buf[512], recv_buf[512];

    send_message temp;
    recv_message temo;

    temp.type = MAGAGER_CHANGE_PASSWORD;
    temp.user_id = id;
    temp.exp_id = 0;
    temp.password = password;
    memcpy(send_buf, &temp, sizeof(send_message));
    int ret = send(socket_fd, send_buf, sizeof(send_message), 0);
    /*发送出错*/
    if (ret == -1)
    {
        printf("\t发送失败！请重新发送！\n");
        return (1);
    }
    ret = recv(socket_fd, recv_buf, sizeof(recv_message), 0);
    if (ret == -1)
    {
        printf("\t服务器端接收失败！请重新发送！\n");
        return (1);
    }
    memcpy(&temo, recv_buf, sizeof(recv_message));
    if (temo.type == OK)
        sprintf(msg, "恭喜你，修改密码成功！");
    else if (temo.type == NOT_EXITS)
        sprintf(msg, "不好意思，账号不存在。");
    else
    {
        sprintf(msg, "不好意思，服务器出错了。");
    }
    display_info(msg);
}

// manger add single up

int manger_addsingle(int od)
{
    char msg[512];
    char send_buf[512], recv_buf[512];

    send_message temp;
    recv_message temo;

    temp.type = ADD_EXPRESS;
    temp.user_id = od;
    temp.exp_id = 0;
    temp.password = 0;
    memcpy(send_buf, &temp, sizeof(send_message));
    int ret = send(socket_fd, send_buf, sizeof(send_message), 0);
    /*发送出错*/
    if (ret == -1)
    {
        printf("\t发送失败！请重新发送！\n");
        return (1);
    }
    ret = recv(socket_fd, recv_buf, sizeof(recv_message), 0);
    if (ret == -1)
    {
        printf("\t服务器端接收失败！请重新发送！\n");
        return (1);
    }
    memcpy(&temo, recv_buf, sizeof(recv_message));
    if (temo.type == OK)
        sprintf(msg, "快递已添加成功");
    else
    {
        sprintf(msg, "不好意思，服务器出错了。");
    }
    display_info(msg);
}

void otherOperate()
{
    int user_id, password, expid;
    int isIn = FALSE;
    char s;
    printf("\t \t与服务器断开连接请按 q\n");
    printf("\t \t 注册管理员账号并登录请按 u\n");
    printf("\t \t 登录管理员账号请按 i\n");
    printf("\t \t 更改管理员账号密码请按 c\n");
    printf("\t \t 添加快递信息请按 e\n");

    while ((s = getchar()) != 'q')
    {
        printf("\t \t与服务器断开连接请按 q\n");
        printf("\t \t 注册管理员账号并登录请按 u\n");
        printf("\t \t 登录管理员账号请按 i\n");
        printf("\t \t 更改管理员账号密码请按 c\n");
        printf("\t \t 添加快递信息请按 e\n");
        switch (s)
        {
        case 'u':
        {
            printf("请输入账号：\n");
            scanf("%d", &user_id);
            printf("请输入密码：\n");
            scanf("%d", &password);
            manger_signup(user_id, password);
            isIn = TRUE;
            break;
        }
        case 'i':
        {
            printf("请输入账号：\n");
            scanf("%d", &user_id);
            printf("请输入密码：\n");
            scanf("%d", &password);
            manger_signin(user_id, password);
            isIn = TRUE;
            break;
        }
        case 'c':
        {
            printf("请输入账号：\n");
            scanf("%d", &user_id);
            printf("请输入密码：\n");
            scanf("%d", &password);
            manger_changepass(user_id, password);
            isIn = TRUE;
            break;
        }
        case 'e':
        {
            if (isIn == FALSE)
            {
                printf("您还未登录！\n");
                break;
            }
            printf("请输入添加快递的用户：\n");
            scanf("%d", &expid);
            manger_addsingle(expid);
            break;
        }
        case 'q':
        {
            disconnect();
            break;
        }
        default:
            break;
        }
    }
}

//是否重连？
int re_connect()
{
    if (ret < 0)
    {
        printf("重连？【y/n】");
        c = getchar();
        getchar();
        if (c == 'y')
        {
            connectserver();
            return 1;
        }
        else
            return 0;
    }
}

int main(int argc, char *argv[])
{
    printf("\n-------------------------------客户端主界面----------------------------------\n\n");
    connectserver();
    if (ret < 0)
        return 0; //连接服务器失败，退出
    otherOperate();
    disconnect();
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    return 0;
}