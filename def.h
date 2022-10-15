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
# include <time.h>
# include <string.h>

#define SERVER_PORT_NO 8889

/*客户端与服务器端使用的消息类型定义*/
#define INITIAL_VALUE 65535

#define THREAD_BUFF_NUM 128

/*客户端使用的消息代码含义*/
// #define DISCONNECT 0
#define APPLY_ROOM  1
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

# define DEFAULT_SIZE 100
# define BUFF_SIZE 512
# define SEND_BUFF_SIZE 1024
# define RECV_BUFF_SIZE 512

# define DEFAULT_TYPE 0
# define USER_SIGN_UP 1
# define USER_SIGN_IN 2
# define USER_CHANGE_PASSWORD 3
# define MANAGER_SIGN_UP 4
# define MAGAGER_SIGN_IN 5
# define MAGAGER_CHANGE_PASSWORD 6
# define CHECK_ALL_EXPRESSES 7
# define GET_SINGLE_EXPRESS 8
# define GET_ALL_EXPRESS 9
# define ADD_EXPRESS 10
# define OK 11
# define NOT_OK 12
# define NOT_EXITS 13
# define ALREADY_EXITS 14
# define WRONG_PASSWORD 15
# define ALL_EXPRESSES_DATA 16
# define ONE_EXPRESSES_DATA 17
# define DISCONNECT 18

/*定义线程缓冲区的使用状态*/
#define BUFF_OCCUPIED 1
#define BUFF_FREED 0
# define BUFF_SIZE 512

# define USER_NUMBER 100
# define EXPRESS_NUMBER 100

#define MAX  100//总数

int listen_fd, conn_fd;  //监听socket，连接socket
struct sockaddr_in server, cli_addr; //服务器地址信息，客户端地址信息
int ret, buffer_index, i,cli_len;
unsigned long ip_addr;
int flag=1;
char c;
pthread_t listentid,servicetid; //监听线程ID,服务线程ID；

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
    express_info* express_list;
    int list_num;
} recv_message;


typedef struct thread_buff_struct_t {
	/*线程缓冲区的索引号*/
	int buff_index;
	/*保存对应线程的线程号*/
	int  tid;
	/**/
	unsigned long ip_addr;  //保存对应的客户机的IP地址
	/*该线程使用的连接套接字描述符*/
	int conn_fd;
	/*线程缓冲区的状态*/
	int buff_status;
} thread_buff_struct;

thread_buff_struct thread_buff[THREAD_BUFF_NUM];

pthread_mutex_t buff_mutex;

typedef struct thread_data
{
    int conn_fd;
	int buff_index;
	char send_buf[SEND_BUFF_SIZE],recv_buf[RECV_BUFF_SIZE];
	int ret,i,cnt;
	uint16_t nbyte;
	struct sockaddr_in peer_name;
	int peer_name_len;
	unsigned int required_room_num;
	int pos;
	thread_buff_struct *pstruct;
	char msg[BUFF_SIZE];
}thread_data;


int is_server_opened=FALSE; //服务器端是否开启标志位

int USER_NUM = 0;
int MANAGER_NUM = 0;
int EXP_NUM = 0;

time_t now;

user USER_DATA;
user MANAGER_DATA;

express EXPRESS_DATA;