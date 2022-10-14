void recv_user_sign_up(char *rev_buf,char *msg){
    int tmp;
    memcpy(&tmp,recv_buf,sizeof(tmp));
    switch (tmp){
        case OK:
        sprintf(msg,"恭喜你，注册成功！");
        break;
        case ALREADY_EXITS:
        sprintf(msg,"账号已经存在");
        break;
        case NOT_OK:
        sprintf(msg,"不好意思，服务器出错了。");
        break;
    }
    display_info(msg);
}
void recv_user_sign_in(char *rev_buf,char *msg){
    int tmp;
    memcpy(&tmp,recv_buf,sizeof(tmp));
    switch (tmp){
        case OK:
        sprintf(msg,"恭喜你，登录成功！");
        break;
        case NOT_EXITS:
        sprintf(msg,"不好意思，账号不存在。");
        break;
        case WRONG_PASSWORD:
        sprintf(msg,"不好意思，密码错误");
        break;
        case NOT_OK:
        sprintf(msg,"不好意思，服务器出错了。");
        break;
    }
    display_info(msg);
}
void recv_user_change_pass(char *rev_buf,char *msg){
    int tmp;
    memcpy(&tmp,recv_buf,sizeof(tmp));
    switch (tmp){
        case OK:
        sprintf(msg,"恭喜你，修改密码成功！");
        break;
        case NOT_EXITS:
        sprintf(msg,"不好意思，账号不存在");
        break;
        case NOT_OK:
        sprintf(msg,"不好意思，服务器出错了。");
        break;
    }
    display_info(msg);
}

void recv_manger_sign_up(char *rev_buf,char *msg){
    int tmp;
    memcpy(&tmp,recv_buf,sizeof(tmp));
    switch (tmp){
        case OK:
        sprintf(msg,"恭喜你，注册成功！");
        break;
        case ALREADY_EXITS:
        sprintf(msg,"账号已经存在");
        break;
        case NOT_OK:
        sprintf(msg,"不好意思，服务器出错了。");
        break;
    }
    display_info(msg);
}
void recv_manger_sign_in(char *rev_buf,char *msg){
    int tmp;
    memcpy(&tmp,recv_buf,sizeof(tmp));
    switch (tmp){
        case OK:
        sprintf(msg,"恭喜你，登录成功！");
        break;
        case NOT_EXITS:
        sprintf(msg,"不好意思，账号不存在。");
        break;
        case WRONG_PASSWORD:
        sprintf(msg,"不好意思，密码错误");
        break;
        case NOT_OK:
        sprintf(msg,"不好意思，服务器出错了。");
        break;
    }
    display_info(msg);
}
void recv_manger_change_pass(char *rev_buf,char *msg){
    int tmp;
    memcpy(&tmp,recv_buf,sizeof(tmp));
    switch (tmp){
        case OK:
        sprintf(msg,"恭喜你，修改密码成功！");
        break;
        case NOT_EXITS:
        sprintf(msg,"不好意思，账号不存在");
        break;
        case NOT_OK:
        sprintf(msg,"不好意思，服务器出错了。");
        break;
    }
    display_info(msg);
}

void recv_get_all_exp(char *rev_buf,char *msg){
    int tmp;
    memcpy(&tmp,recv_buf,sizeof(tmp));
    switch (tmp){
        case OK:
        sprintf(msg,"得到所有快递信息！");
        break;
        case NOT_OK:
        sprintf(msg,"不好意思，服务器出错了。");
        break;
    }
    display_info(msg);
}
void recv_add_single_exp(char *rev_buf,char *msg){
    int tmp;
    memcpy(&tmp,recv_buf,sizeof(tmp));
    switch (tmp){
        case OK:
        sprintf(msg,"添加快递成功！");
        break;
        case NOT_OK:
        sprintf(msg,"不好意思，服务器出错了。");
        break;
    }
    display_info(msg);
}