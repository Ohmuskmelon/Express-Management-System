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

# define RESULT_NUM 10
# define BUFF_SIZE 1024
# define SEND_BUFF_SIZE 4096
# define RECV_BUFF_SIZE 4096

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
// # define BUFF_SIZE 512

# define MAX_USER_NUM 100
# define MAX_EXPRESS_NUM 100
# define MAX_MANAGER_NUM 100

#define MAX  100//总数

int listen_fd, conn_fd;  //监听socket，连接socket
struct sockaddr_in server, cli_addr; //服务器地址信息，客户端地址信息
struct sockaddr tcpaddr;
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

typedef user_info user[MAX_USER_NUM];

typedef struct express_info
{
    int id;
    int user_id;
    int isIn;
    int date;
    int time;
    pthread_mutex_t express_info_mutex;
} express_info;

typedef express_info express[MAX_EXPRESS_NUM];

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

int find_user(user data, int user_num, int user_id);
int add_user(user data, int user_num, int user_maxnum, int user_id, int password);
int check_user(user data, int user_num, int user_id, int password);
int change_user_password(user data, int user_num, int user_id, int password);
int add_express(express exp_data, user user_data, int exp_num, int exp_maxnum, int user_num, int user_id);
int find_user_express(express exp_data, int exp_num, user user_data, int user_num, int user_id, int* result);
int find_single_express(express exp_data, int exp_num, int exp_id);
void remove_express_by_index(express exp_data, int exp_num, int exp_index, user user_data, int user_num);
void remove_express_by_id(express exp_data, int exp_num, int exp_id, user user_data, int user_num);

void user_sign_up(user user_data, int *user_num, int user_id, int password, thread_data *t_data);
int user_sign_in(user user_data, int user_num, int user_id, int password, thread_data *t_data);
int user_change_password(user user_data, int user_num, int user_id, int password, thread_data *t_data);
void manager_sign_up(user user_data, int *user_num, int user_id, int password, thread_data *t_data);
int manager_sign_in(user user_data, int user_num, int user_id, int password, thread_data *t_data);
int manager_change_password(user user_data, int user_num, int user_id, int password, thread_data *t_data);
int user_check_expresses(express exp_data, int exp_num, user user_data, int user_num, int user_id, thread_data *t_data);
void get_single_express_by_index(express exp_data, int exp_num, int exp_index, user user_data, int user_num, int user_id, thread_data *t_data);
void get_all_expresses_out(express exp_data, int exp_num, user user_data, int user_num, int user_id, thread_data *t_data);
int manager_add_express(express exp_data, user user_data, int *exp_num, int exp_maxnum, int user_num, int user_id, thread_data *t_data);

int user_read(user user, int max, int now);
int write_user(user user, int max, int begin, int end);
int read_manger(user user, int max, int now);
void write_manager(user user, int max, int begin, int end);
int express_read(express exp_data, int max, int now);
int write_express(express exp_data, int max, int begin, int end);

void * service_thread(void *p);

void * listen_thread(void *p);
void startserver();
void stopserver();
void otherOperate();
void print_thread();

void add_info(char *src);
void display_info(char *msg);
void free_buff(int index);
static void thread_err(char *s, int index);
void init_thread_buff();
int get_free_buff();
void check_connection(unsigned long ip_addr);
