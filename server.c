# include "servicethread.h"
# include "data.h"

int listen_fd, conn_fd;  //监听socket，连接socket
struct sockaddr_in server, cli_addr; //服务器地址信息，客户端地址信息
int ret, buffer_index, i,cli_len;
unsigned long ip_addr;
int flag=1;
char c;
pthread_t listentid,servicetid; //监听线程ID,服务线程ID；

# define BUFF_SIZE 512
# define SEND_BUFF_SIZE 1024
# define RECV_BUFF_SIZE 512

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

int is_server_opened=FALSE; //服务器端是否开启标志位

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

/* 用户注册处理函数 */
void user_sign_up (user user_data, int *user_num, int user_id, int password, thread_data t_data)
{
    int send_message;
    int result = add_user(user_data, user_num, USER_NUMBER, user_id, password);
    switch(result)
    {
        case -1 : send_message = ALREADY_EXITS; break;
        case 0: send_message = NOT_OK; break;
        default:
        {
            user_num = result;
            send_message = OK;
            break;
        }
    }
    char *result_message;
    switch (send_message)
    {
        case ALREADY_EXITS: result_message = "账号已存在！";
        case NOT_OK: result_message = "创建失败！";
        default: result_message = "创建成功！";
    }
    sprintf(t_data.msg, "ID为%d的用户申请注册！,%s\n", user_id, result_message);
    add_info(t_data.msg);
    memcpy(t_data.send_buf, &send_message, sizeof(send_message));
    ret = send(t_data.conn_fd, t_data.send_buf, sizeof(send_message), 0);
    if (ret < 0)
        thread_err("发送数据出错！\n", t_data.buff_index);
}

void user_sign_in (user user_data, int user_num, int user_id, int passward, thread_data t_data)
{
    int send_message;
    int result = check_user(user_data, user_num, user_id, passward);
    switch (result)
    {
        case -1: send_message = 
        case -2:
        default:
    }
}

//消息内容输出函数
void display_info(char *msg)
{
	printf("%s\n",msg);
}

void * listen_thread(void *p)
{
	char msg[BUFF_SIZE];
	while(1) {		

		/*接受远端的TCP连接请求*/
		cli_len=sizeof(cli_addr);
		conn_fd=accept(listen_fd,(struct sockaddr *)&cli_addr, &cli_len);
		if(conn_fd<0)
		continue;

		ip_addr=ntohl(cli_addr.sin_addr.s_addr);
		/*检测重复连接*/
		check_connection(ip_addr);
		/*分配线程缓冲区*/
		buffer_index=get_free_buff();
		if(buffer_index<0) {
			sprintf(msg,"没有空闲的线程缓冲区。\n");
			add_info(msg);
			close(conn_fd);
			continue;
		}

		/*填写服务线程需要的信息*/
		pthread_mutex_lock(&buff_mutex);
		thread_buff[buffer_index].buff_index=buffer_index;
		thread_buff[buffer_index].ip_addr=ip_addr;
		thread_buff[buffer_index].conn_fd=conn_fd;
		thread_buff[buffer_index].buff_status=BUFF_OCCUPIED;
		pthread_mutex_unlock(&buff_mutex);

		/*创建新的服务线程*/
		ret=pthread_create(&servicetid, NULL, service_thread, &thread_buff[buffer_index]);
		if(ret==-1) {
			sprintf(msg,"创建服务线程出错！\n");
			add_info(msg);
			close(conn_fd);
			/*释放线程缓冲区*/
			pthread_mutex_lock(&buff_mutex);
			thread_buff[buffer_index].tid=0;
			thread_buff[buffer_index].buff_status=BUFF_FREED;
			pthread_mutex_unlock(&buff_mutex);
		}
	}
}

//开启服务器操作
void startserver()
{
	char msg[BUFF_SIZE]; //提示信息

	//初始化数据结构
	init_thread_buff();
    // 补充：初始化快递、用户数据

	if(!is_server_opened)
	{
		/*创建套接字*/
		listen_fd=socket(AF_INET,SOCK_STREAM,0);
		if(listen_fd<0) {
			sprintf(msg,"创建监听套接字出错！ \n");
			display_info(msg);
			return;
		}

		/*填写服务器的地址信息*/
		server.sin_family=AF_INET;
		server.sin_addr.s_addr=htonl(INADDR_ANY);
		server.sin_port=htons(SERVER_PORT_NO);

		setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,(void *)&flag,sizeof(int));

		/*绑定端口*/
		ret=bind(listen_fd,(struct sockaddr*)&server, sizeof(server));
		if(ret<0) {
			sprintf(msg,"绑定TCP端口：%d 出错！ \n",SERVER_PORT_NO);
			display_info(msg);
			close(listen_fd);
			return;
		}

		/*转化成倾听套接字*/
		listen(listen_fd,5);
		ret=pthread_create(&listentid, NULL, listen_thread, NULL);
		if(ret==-1) {
			sprintf(msg,"创建监听线程出错！ \n");
			display_info(msg);
			close(listen_fd);
			return;
		}

		//成功后输出提示信息
		sprintf(msg,"             服务器端开启成功！服务器在端口：%d 准备接受连接！ \n",SERVER_PORT_NO);
		display_info(msg);
		is_server_opened=TRUE;

	}
}

/*关闭服务器操作*/
void stopserver()
{

	char msg[512];
	if(is_server_opened)
	{

		pthread_mutex_lock(&buff_mutex);
		for(i=0;i<THREAD_BUFF_NUM;i++) {
			if(thread_buff[i].buff_status!=BUFF_FREED) {
				/*退出服务线程*/
				pthread_cancel(thread_buff[i].tid);
				pthread_join(thread_buff[i].tid,NULL);
				/*退出的线程不释放它的缓冲区，释放工作由主线程来处理*/
				thread_buff[i].tid=0;
				thread_buff[i].buff_status=BUFF_FREED;
				close(thread_buff[i].conn_fd);
			}
		}
		pthread_mutex_unlock(&buff_mutex);

		pthread_cancel(listentid);
		pthread_join(listentid,NULL);
		close(listen_fd);
		sprintf(msg,"\t  服务器端成功关闭！ \n");
		display_info(msg);
		is_server_opened=FALSE;
	}
}

