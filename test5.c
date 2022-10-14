
//#define __ROOM_H
//#include "globalapi.h"
# include <stdbool.h>
# include <time.h>
# include <stdio.h>

#define MAX  100//总数

/*用户的一个简单描述，id表示账号，password表示密码,express_num表示快递数量*/
struct user_info {
	int id;
	int password;
    int express_num;
	// pthread_mutex_t room_mutex;
};
struct express_info{
    int id;
    int user_id;
    int flag;
    int date;
};
struct user_info user[MAX],manger[MAX];
struct express_info node[MAX];

/*从user读出并初始化user数组*/
int read_user(struct user_info*user,int max,int now)
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
        printf("%d %d %d\n",id,password,express_num);
        now++;
    }
	fclose(fp);
    return now;
}
/*写入user文件*/
int write_user(struct user_info*user,int max,int begin,int end)
{
    int id,password,express_num;
    FILE *fp;
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
int read_manger(struct user_info*user,int max,int now)
{
    int id,password,express_num;
	FILE *fp;
    if ((fp = fopen("manger.txt", "r")) == NULL)
    {
    return 0;
    }
	while(fscanf(fp,"%d %d %d\n",&id,&password,&express_num)!=EOF)
    {
        user[now].id=id;
        user[now].password=password;
        user[now].express_num=express_num;
        printf("%d %d %d\n",id,password,express_num);
        now++;
    }
	fclose(fp);
    return now;
}
/*写入manger文件*/
int write_manger(struct user_info*user,int max,int begin,int end)
{
    int id,password,express_num;
    FILE *fp;
    if ((fp = fopen("manger.txt", "w")) == NULL)
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
/*从node中读出来*/
int read_node(struct express_info*user,int max,int now)
{
    int id,user_id,flag,date;
	FILE *fp;
    if ((fp = fopen("node.txt", "r")) == NULL)
    {
    return 0;
    }
	while(fscanf(fp,"%d %d %d %d\n",&id,&user_id,&flag,&date)!=EOF)
    {
        user[now].id=id;
        user[now].user_id=user_id;
        user[now].flag=flag;
        user[now].date=date;
        now++;
    }
	fclose(fp);
    return now;
}
/*写到node中*/
int write_node(struct express_info*user,int max,int begin,int end)
{
    int id,user_id,flag,date;
    FILE *fp;
    if ((fp = fopen("node.txt", "w")) == NULL)
    {
    return 0;
    }
    for(int i=begin;i<end;i++)
    {
        id=user[i].id;
        user_id=user[i].user_id;
        flag=user[i].flag;
        date=user[i].date;
        fprintf(fp,"%d %d %d %d\n",id,user_id,flag,date);
    }
    fclose(fp);
    return 1;
}

/*写到日志里*/
int write_record(char *a)
{
    FILE *fp;
    if ((fp = fopen("record.txt", "a")) == NULL)
    {
    return 0;
    }
    fprintf(fp,"%s\n",a);
    return 1;
}

int main()
{
    char a[10]="hgsgdy";
    char b[10]="isds";
    user[0].id=2;
    user[0].password=13;
    user[0].express_num=2;
    user[1].id=3;
    user[1].password=23;
    user[1].express_num=4;
    write_user(user,100,0,2);
    read_user(user,100,0);
    write_manger(user,100,0,2);
    read_manger(user,100,0);
    write_record(a);
    write_record(b);
}
