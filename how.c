void f1(char *rev_buf,char *msg){
    int tmp;
    memcpy(&tmp,recv_buf,sizeof(tmp));
    switch (tmp){
        case OK:
        sprintf(msg,"OK");
        break;
        case NOT_OK:
        sprintf(msg,"NOT_OK");
        break;
    }
    display_info(msg);
}
void f2(char *rev_buf,char *msg){
    int tmp;
    memcpy(&tmp,recv_buf,sizeof(tmp));
    switch (tmp){
        case OK:
        sprintf(msg,"OK");
        break;
        case NOT_EXITS:
        sprintf(msg,"NOT_EXITS");
        break;
        case WRONG_PASSWORD:
        sprintf(msg,"WRONG_PASSWORD");
        break;
    }
    display_info(msg);
}
void f3(char *rev_buf,char *msg){
    int tmp;
    memcpy(&tmp,recv_buf,sizeof(tmp));
    switch (tmp){
        case OK:
        sprintf(msg,"OK");
        break;
        case NOT_EXITS:
        sprintf(msg,"NOT_EXITS");
        break;
    }
    display_info(msg);
}
