/* 用户注册处理函数 */
void user_sign_up (user user_data, int *user_num, int user_id, int password, thread_data *t_data)
{
    int type;
    recv_message receive_message;
    int result = add_user(user_data, *user_num, USER_NUMBER, user_id, password);
    switch(result)
    {
        case -1 : type = ALREADY_EXITS; break;
        case 0: type = NOT_OK; break;
        default:
        {
            *user_num = result;
            type = OK;
            break;
        }
    }
    char *result_message;
    switch (type)
    {
        case ALREADY_EXITS: result_message = "账号已存在！"; break;
        case NOT_OK: result_message = "创建失败！"; break;
        default: result_message = "创建成功！"; break;
    }
    receive_message.type = type;
    sprintf(t_data->msg, "ID为%d的用户申请注册！,%s\n", user_id, result_message);
    add_info(t_data->msg);
    memcpy(t_data->send_buf, &receive_message, sizeof(receive_message));
    t_data->ret = send(t_data->conn_fd, t_data->send_buf, sizeof(receive_message), 0);
    if (t_data->ret < 0)
        thread_err("发送数据出错！\n", t_data->buff_index);
}

/* 用户登录处理函数 */
int user_sign_in (user user_data, int user_num, int user_id, int password, thread_data *t_data)
{
    int type;
    recv_message receive_message;
    int result = check_user(user_data, user_num, user_id, password);
    switch (result)
    {
        case -1: type = NOT_EXITS; break;
        case -2: type = WRONG_PASSWORD; break;
        default: type = OK; break;
    }
    char *result_message;
    switch (type)
    {
        case NOT_EXITS: result_message = "用户不存在！"; break;
        case WRONG_PASSWORD: result_message = "密码错误！"; break;
        default: result_message = "登录成功！"; break;
    }
    receive_message.type = type;
    sprintf(t_data->msg, "ID为%d的用户申请登录！,%s\n", user_id, result_message);
    add_info(t_data->msg);
    memcpy(t_data->send_buf, &receive_message, sizeof(receive_message));
    t_data->ret = send(t_data->conn_fd, t_data->send_buf, sizeof(receive_message), 0);
    if (t_data->ret < 0)
        thread_err("发送数据出错！\n", t_data->buff_index);
    return result;
}

/* 用户更改密码处理函数 */
int user_change_password (user user_data, int user_num, int user_id, int password, thread_data *t_data)
{
    int type;
    recv_message receive_message;
    int result = change_user_password(user_data, user_num, user_id, password);
    switch (result)
    {
        case -1: type = NOT_EXITS; break;
        default: type = OK; break;
    }
    char *result_message;
    switch (type)
    {
        case NOT_EXITS: result_message = "用户不存在！"; break;
        default: result_message = "更改密码成功！"; break;
    }
    receive_message.type = type;
    sprintf(t_data->msg, "ID为%d的用户申请更改密码！,%s\n", user_id, result_message);
    add_info(t_data->msg);
    memcpy(t_data->send_buf, &receive_message, sizeof(receive_message));
    t_data->ret = send(t_data->conn_fd, t_data->send_buf, sizeof(receive_message), 0);
    if (t_data->ret < 0)
        thread_err("发送数据出错！\n", t_data->buff_index);
    return result;
}

/* 管理员注册处理函数 */
void manager_sign_up (user user_data, int *user_num, int user_id, int password, thread_data *t_data)
{
    int type;
    recv_message receive_message;
    int result = add_user(user_data, *user_num, USER_NUMBER, user_id, password);
    switch(result)
    {
        case -1 : type = ALREADY_EXITS; break;
        case 0: type = NOT_OK; break;
        default:
        {
            *user_num = result;
            type = OK;
            break;
        }
    }
    char *result_message;
    switch (type)
    {
        case ALREADY_EXITS: result_message = "账号已存在！"; break;
        case NOT_OK: result_message = "创建失败！"; break;
        default: result_message = "创建成功！"; break;
    }
    receive_message.type = type;
    sprintf(t_data->msg, "ID为%d的管理员申请注册！,%s\n", user_id, result_message);
    add_info(t_data->msg);
    memcpy(t_data->send_buf, &receive_message, sizeof(receive_message));
    t_data->ret = send(t_data->conn_fd, t_data->send_buf, sizeof(receive_message), 0);
    if (t_data->ret < 0)
        thread_err("发送数据出错！\n", t_data->buff_index);
}

/* 管理员登录处理函数 */
int manager_sign_in (user user_data, int user_num, int user_id, int password, thread_data *t_data)
{
    int type;
    recv_message receive_message;
    int result = check_user(user_data, user_num, user_id, password);
    switch (result)
    {
        case -1: type = NOT_EXITS; break;
        case -2: type = WRONG_PASSWORD; break;
        default: type = OK; break;
    }
    char *result_message;
    switch (type)
    {
        case NOT_EXITS: result_message = "用户不存在！"; break;
        case WRONG_PASSWORD: result_message = "密码错误！"; break;
        default: result_message = "登录成功！"; break;
    }
    receive_message.type = type;
    sprintf(t_data->msg, "ID为%d的管理员申请登录！,%s\n", user_id, result_message);
    add_info(t_data->msg);
    memcpy(t_data->send_buf, &receive_message, sizeof(receive_message));
    t_data->ret = send(t_data->conn_fd, t_data->send_buf, sizeof(receive_message), 0);
    if (t_data->ret < 0)
        thread_err("发送数据出错！\n", t_data->buff_index);
    return result;
}

/* 管理员更改密码处理函数 */
int manager_change_password (user user_data, int user_num, int user_id, int password, thread_data *t_data)
{
    int type;
    recv_message receive_message;
    int result = change_user_password(user_data, user_num, user_id, password);
    switch (result)
    {
        case -1: type = NOT_EXITS; break;
        default: type = OK; break;
    }
    char *result_message;
    switch (type)
    {
        case NOT_EXITS: result_message = "用户不存在！"; break;
        default: result_message = "更改密码成功！"; break;
    }
    receive_message.type = type;
    sprintf(t_data->msg, "ID为%d的管理员申请更改密码！,%s\n", user_id, result_message);
    add_info(t_data->msg);
    memcpy(t_data->send_buf, &receive_message, sizeof(receive_message));
    t_data->ret = send(t_data->conn_fd, t_data->send_buf, sizeof(receive_message), 0);
    if (t_data->ret < 0)
        thread_err("发送数据出错！\n", t_data->buff_index);
    return result;
}

/* 用户查看快递函数 */
int user_check_expresses(express exp_data, int exp_num, user user_data, int user_num, int user_id, thread_data *t_data)
{
    int result[DEFAULT_SIZE];
    int express_num = find_user_express(exp_data, exp_num, user_data, user_num, user_id, result);
    express_info list[express_num];
    for (int i=0; i<express_num; i++)
    {
        list[i].id = exp_data[result[i]].id;
        list[i].date = exp_data[result[i]].date;
        list[i].time = exp_data[result[i]].time;
    }
    recv_message receive_message;
    receive_message.express_list = list;
    receive_message.type = OK;
    receive_message.list_num = express_num;
    sprintf(t_data->msg, "ID为%d的用户查看了快递信息！\n", user_id);
    add_info(t_data->msg);
    memcpy(t_data->send_buf, &receive_message, sizeof(receive_message));
    t_data->ret = send(t_data->conn_fd, t_data->send_buf, sizeof(receive_message), 0);
    if (t_data->ret < 0)
        thread_err("发送数据出错！\n", t_data->buff_index);
    return express_num;
}

/* 按索引取出单个快递函数 */
void get_single_express_by_index (express exp_data, int exp_num, int exp_index, user user_data, int user_num, int user_id, thread_data *t_data)
{
    recv_message receive_message;
    receive_message.type = OK;
    remove_express_by_index(exp_data, exp_num, exp_index, user_data, user_num);
    int exp_id = exp_data[exp_index].id;
    sprintf(t_data->msg, "ID为%d的用户取出了快递，ID为%d！\n", user_id, exp_id);
    add_info(t_data->msg);
    memcpy(t_data->send_buf, &receive_message, sizeof(receive_message));
    t_data->ret = send(t_data->conn_fd, t_data->send_buf, sizeof(receive_message), 0);
    if (t_data->ret < 0)
        thread_err("发送数据出错！\n", t_data->buff_index);
}

/* 按id取出单个快递 */
void get_single_express_by_id (express exp_data, int exp_num, int exp_id, user user_data, int user_num, int user_id, thread_data *t_data)
{
    recv_message receive_message;
    receive_message.type = OK;
    remove_express_by_id(exp_data, exp_num, exp_id, user_data, user_num);
    sprintf(t_data->msg, "ID为%d的用户取出了快递，ID为%d！\n", user_id, exp_id);
    add_info(t_data->msg);
    memcpy(t_data->send_buf, &receive_message, sizeof(receive_message));
    t_data->ret = send(t_data->conn_fd, t_data->send_buf, sizeof(receive_message), 0);
    if (t_data->ret < 0)
        thread_err("发送数据出错！\n", t_data->buff_index);
}

/* 用户取出所有快递 */
void get_all_expresses_out(express exp_data, int exp_num, user user_data, int user_num, int user_id, thread_data *t_data)
{
    int user_index = find_user(user_data, user_num, user_id);
    int list_num = user_data[user_index].express_num;
    int list[list_num];
    find_user_express(exp_data, exp_num, user_data, user_num, user_id, list);
    for (int i=0; i<list_num; i++)
        get_single_express_by_index(exp_data, exp_num, list[i], user_data, user_num, user_id, t_data);
}

/* 管理员添加快递 */
int manager_add_express (express exp_data, user user_data, int exp_num, int exp_maxnum, int user_num, int user_id, thread_data *t_data)
{
    int type;
    recv_message receive_message;
    int result = add_express(exp_data, user_data, exp_num, EXPRESS_NUMBER, user_num, user_id);
    switch (result)
    {
        case -1: type = NOT_OK; break;
        default: type = OK; break;
    }
    receive_message.type = type;
    if(result != -1)
        sprintf(t_data->msg, "成功为ID为%d的用户添加快递！\n", user_id);
    else
        sprintf(t_data->msg, "ID为%d的用户添加快递失败！\n", user_id);
    add_info(t_data->msg);
    memcpy(t_data->send_buf, &receive_message, sizeof(receive_message));
    t_data->ret = send(t_data->conn_fd, t_data->send_buf, sizeof(receive_message), 0);
    if (t_data->ret < 0)
        thread_err("发送数据出错！\n", t_data->buff_index);
    return result;
}
