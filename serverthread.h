void * service_thread(void *p)
{
    int *user_num = &USER_NUM;
    int *manager_num = &MANAGER_NUM;
    int *exp_num = &EXP_NUM;
    user_info* user_data = USER_DATA;
    user_info* manager_data = MANAGER_DATA;
    express_info* exp_data = EXPRESS_DATA;
    send_message message;
    thread_data t_data;
    /*获取线程使用的线程缓冲区的序号*/
	t_data.pstruct=(thread_buff_struct *)p;
	t_data.buff_index=t_data.pstruct->buff_index;
	t_data.pstruct->tid=pthread_self();

	/*从线程缓冲区中获取通信使用的套接字描述符*/
	t_data.conn_fd=t_data.pstruct->conn_fd;

	/*打印远端主机地址*/
	t_data.peer_name_len=sizeof(t_data.peer_name);
	t_data.ret=getpeername(t_data.conn_fd,(struct sockaddr*)&t_data.peer_name, &t_data.peer_name_len);
	if(t_data.ret==-1) 
	thread_err("* 获取远端主机地址出错 *",t_data.buff_index);

	sprintf(t_data.msg,"* 新连接-->线程ID：%d, 线程缓冲区索引号：%d, 远端地址：%s, 端口号：%d *\n",(unsigned short)t_data.pstruct->tid, t_data.buff_index, inet_ntoa(t_data.peer_name.sin_addr), ntohs(t_data.peer_name.sin_port));
	add_info(t_data.msg);

    while (1)
    {
        /*从网络中获取数据记录*/
		t_data.ret=recv(t_data.conn_fd,t_data.recv_buf,sizeof(send_message),0);
		/*接收出错*/
		if(t_data.ret==-1) {
			sprintf(t_data.msg,"* 线程：%d 在连接：%d 接收出错。连接将关闭。 *\n",(unsigned short)t_data.pstruct->tid, t_data.conn_fd);
			add_info(t_data.msg);
			thread_err(t_data.msg, t_data.buff_index);
		}
		/*ret==0说明客户端连接已关闭*/
		if(t_data.ret==0) {
			sprintf(t_data.msg,"* 线程  %d  的连接( ID：%d ) 客户端已关闭。服务器端连接也将关闭。 *\n",(unsigned short)t_data.pstruct->tid, t_data.conn_fd);
			add_info(t_data.msg);
			close(t_data.conn_fd);
			free_buff(t_data.buff_index);
			pthread_exit(NULL);
		}
        memcpy(&message, t_data.recv_buf, sizeof(message));
        int user_id = message.user_id;
        int exp_id = message.exp_id;
        int password = message.password;
        switch (message.type)
        {
            case USER_SIGN_UP: user_sign_up(user_data, user_num, user_id, password, &t_data); break;
            case USER_SIGN_IN: user_sign_in(user_data, *user_num, user_id, password, &t_data); break;
            case USER_CHANGE_PASSWORD: user_change_password(user_data, *user_num, user_id, password, &t_data); break;
            case MANAGER_SIGN_UP: manager_sign_up(manager_data, manager_num, user_id, password, &t_data); break;
            case MAGAGER_SIGN_IN: manager_sign_in(manager_data, *manager_num, user_id, password, &t_data); break;
            case MAGAGER_CHANGE_PASSWORD: manager_change_password(manager_data, *manager_num, user_id, password, &t_data); break;
            case CHECK_ALL_EXPRESSES: user_check_expresses(exp_data, *exp_num, user_data, *user_num, user_id, &t_data); break;
            case GET_ALL_EXPRESS: get_all_expresses_out(exp_data, *exp_num, user_data, *user_num, user_id, &t_data); break;
            case ADD_EXPRESS: manager_add_express(exp_data, user_data, exp_num, MAX_EXPRESS_NUM, *user_num, user_id, &t_data); break;
            default: break;
        }
    }
}