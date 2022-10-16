void add_info(char *src)
{ 
	printf("%s\n",src);
}

//消息内容输出函数
void display_info(char *msg)
{
	printf("%s\n",msg);
}

/*释放线程缓冲区，对buff_status的访问同样需要使用互斥保护*/
void free_buff(int index)
{
	pthread_mutex_lock(&buff_mutex);
	if(thread_buff[index].buff_status==BUFF_OCCUPIED)
		thread_buff[index].buff_status=BUFF_FREED;
	pthread_mutex_unlock(&buff_mutex);
}
/*thread_err: 服务线程的错误处理函数，由于服务器端使用的是多线程技术，服务线程发生错误时，不能像在多进程的情况下，简单地调用exit()终止进程。在多线程下，服务线程必须将使用的资源释放后，调用pthread_exit()退出，并且在需要进行线程间同步的情况下，还需要做一些线程同步的工作，才能退出。这个特点在多线程编程中是非常重要的。*/
static void thread_err(char *s, int index)
{
	int i;
	char msg[512];
	/*获取空闲的界面输出信息缓冲区，如果没有空闲的,延迟一段时间后继续获取*/
	sprintf(msg,"线程 %d 发生致命错误：,%s\n",(unsigned short)pthread_self(),s);
	add_info(msg);
	//info_print(strmsg,serverwindow);
	/*释放线程使用的线程缓冲区*/
	free_buff(index);
	pthread_exit(NULL);
}



/*初始化线程缓冲区*/
void init_thread_buff()
{
	int index;
	for(index=0; index<THREAD_BUFF_NUM;index++) {
		thread_buff[index].tid=-1;
		thread_buff[index].buff_status=BUFF_FREED;
	}
}

/*分配一个空闲的线程缓冲区，如果没有空闲的缓冲区则返回－1*/
int get_free_buff()
{
	int i,ret;
	/*注意对互斥锁的操作，这些操作必须是成对的（加锁和解锁），否则会发生死锁的情况*/
	pthread_mutex_lock(&buff_mutex);
	for(i=0;i<THREAD_BUFF_NUM; i++)
		if(thread_buff[i].buff_status==BUFF_FREED) {
			ret=i;
			pthread_mutex_unlock(&buff_mutex);
			break;
		}
		if(i==THREAD_BUFF_NUM) {
			ret=-1;
			pthread_mutex_unlock(&buff_mutex);
		}
	return ret;
}

/*检查线程缓冲区中是否有重复连接，因为可能客户端的通信进程终止后重新启动，此时应当终止原来它所对应的服务线程，再重新创建一个服务线程，并为这个新的服务线程分配线程缓冲区。*/
void check_connection(unsigned long ip_addr)
{
	int i,j;
	struct in_addr in;
	char msg[BUFF_SIZE];
	/*检查所有的线程缓冲区*/
	pthread_mutex_lock(&buff_mutex);
	for(i=0;i<THREAD_BUFF_NUM;i++) {
		/*发现重复连接*/
		if((thread_buff[i].buff_status!=BUFF_FREED)&&thread_buff[i].ip_addr==ip_addr) {
			in.s_addr=htonl(ip_addr);
			sprintf(msg,"重复连接：%s，旧连接将关闭！\n",inet_ntoa(in));
			add_info(msg);
			pthread_cancel(thread_buff[i].tid);

			pthread_join(thread_buff[i].tid,NULL);
			/*退出的线程不释放它的缓冲区，释放工作由主线程来处理*/
			thread_buff[i].tid=0;
			thread_buff[i].buff_status=BUFF_FREED;
			close(thread_buff[i].conn_fd);
		}
	}
	pthread_mutex_unlock(&buff_mutex);
}