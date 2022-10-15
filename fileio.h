/*从user读出并初始化user数组*/
int user_read(user user,int max,int now)
{
    int id,password,express_num;
	FILE *fp;
    if ((fp = fopen("user.txt", "r")) == NULL)
    {
    return 0;
    }
	while(fscanf(fp,"%d %d %d\n",&id,&password,&express_num)!=EOF)
    {
        user[now].id=id;
        user[now].password=password;
        user[now].express_num=express_num;
        now++;
    }
	fclose(fp);
    return now;
}

/*写入user文件*/
int write_user(user user,int max,int begin,int end)
{
    int id,password,express_num;
    FILE *fp;
    struct user_info*temp;
    if ((fp = fopen("user.txt", "w")) == NULL)
    {
    return 0;
    }
    for(int i=begin;i<end;i++)
    {
        id=user[i].id;
        password=user[i].password;
        express_num=user[i].express_num;
        fprintf(fp,"%d %d %d\n",id,password,express_num);
    }
    fclose(fp);
    return 1;
}

/*从manger中读出来*/
void read_manger(user user,int max,int now)
{
    int id,password,express_num;
	FILE *fp;
    if ((fp = fopen("manger.txt", "r")) == NULL)
    {
    printf("打开文件失败！\n");
    }
	while(fscanf(fp,"%d %d %d\n",&id,&password,&express_num)!=EOF)
    {
        user[now].id=id;
        user[now].password=password;
        user[now].express_num=express_num;
        now++;
    }
	fclose(fp);
}

/*写入manger文件*/
void write_manager(user user,int max,int begin,int end)
{
    int id,password,express_num;
    FILE *fp;
    struct user_info*temp;
    if ((fp = fopen("manger.txt", "w")) == NULL)
    {
    printf("打开文件失败！\n");
    }
    for(int i=begin;i<end;i++)
    {
        id=user[i].id;
        password=user[i].password;
        express_num=user[i].express_num;
        fprintf(fp,"%d %d\n",id,password);
    }
    fclose(fp);
}