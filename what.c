#include <ctype.h>  // isalnum
#include <stdio.h>  // printf
#include <stdlib.h> // atoi
#include <string.h> // memset
#define MAX 100
typedef struct user_info
{
    int id;
    int password;
    int express_num;
} user_info;


typedef struct temp_ID
{
    int user_id;
    int exp_id;
} temp_ID;

typedef user_info user[MAX];

typedef struct express_info
{
    int id;
    int user_id;
    int isIn;
    int date;
    int time;
} express_info;

typedef express_info express[MAX];

user_info string_change(char *arr)
{
    user_info temp;
    int flag=0;
    for (int i = 0; i < strlen(arr); ++i) {
    
            if(arr[i]==' '&&flag==0){
                flag=1;
                continue;
            }
            if(flag==0) 
            {
            continue;
            }else{
                int j;
                j=i;
                int sum=0;
                while(arr[j]!=' '&&j<strlen(arr)){
                    sum=sum*10+arr[j]-'0';
                    j++;
                }
                if(j<strlen(arr)){
                    temp.id=sum;
                    //printf("%d\n",temp.id);
                }else{
                    temp.password=sum;
                    //printf("%d\n",temp.password);
                }
                i=j;
        }
    }
    return temp;
}

int back_ID(char *arr)
{
    int flag=0;
    for (int i = 0; i < strlen(arr); ++i) {
    
            if(arr[i]==' '&&flag==0){
                flag=1;
                continue;
            }
            if(flag==0) 
            {
            continue;
            }else{
                int j;
                j=i;
                int sum=0;
                while(arr[j]!=' '&&j<strlen(arr)){
                    sum=sum*10+arr[j]-'0';
                    j++;
                }
                return sum;
        }
    }
}
int first_ID(char *arr)
{
    int sum=0;
    for(int i=0;i<strlen(arr);++i)
    {
        int j=i;
        while()
    }
}
temp_ID store(char *arr)
{
    temp_ID temp;
    int flag=0;
    for (int i = 0; i < strlen(arr); ++i) {
    
            if(arr[i]==' '&&flag==0){
                flag=1;
                continue;
            }
            if(flag==0) 
            {
            continue;
            }else{
                int j;
                j=i;
                int sum=0;
                while(arr[j]!=' '&&j<strlen(arr)){
                    sum=sum*10+arr[j]-'0';
                    j++;
                }
                if(j<strlen(arr)){
                    temp.user_id=sum;
                    //printf("%d\n",temp.id);
                }else{
                    temp.exp_id=sum;
                    //printf("%d\n",temp.password);
                }
                i=j;
        }
    }
    return temp;
}
int main() {
    char arr[256] ="3123 100273 55123434";
    char a[255]="2 32";
    char b[256]="2 37 12";
    user_info temp;
    temp=string_change(arr);
    printf("%d %d\n",temp.id,temp.password);
    printf("%d\n",back_ID(a));
    temp_ID temp2;
    temp2=store(b);
    printf("%d %d\n",temp2.user_id,temp2.exp_id);
    

    return 0;
}