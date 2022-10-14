// include

/*server.c*/

#define USER_NUMBER 100

typedef struct user_info
{
    int id;
    int password;
} user_info;

typedef user_info user[USER_NUMBER];

// 根据id查找用户，查找成功返回用户数据的下标，查找失败返回-1
int find_user(user data, int user_num, int id)
{
    for (int i=0; i<user_num; i++)
    {
        if (data[i].id == id)
            return i;
    }
    return -1;
}


// 添加用户信息，成功返回目前用户数量，用户已存在返回-1，超出最大用户限制返回0
int add_user(user data, int user_num, int user_maxnum, int id, int password)
{
    if (user_num >= user_maxnum)
        return 0;
    if (find_user(data, user_num, id) == -1)
        return -1;
    user_info cnt_user;
    cnt_user.id = id;
    cnt_user.password = password;
    data[user_num] = cnt_user;
    user_num++;
    return user_num;
}

// 检查用户输入的密码是否正确，用户不存在返回-1，用户密码错误返回0，用户密码正确返回用户数据下标
int check_user(user data, int user_num, int id, int password)
{
    int user_index = find_user(data, user_num, id);
    if (user_index == -1)
        return -1;
    if (data[user_index].password != password)
        return 0;
    return user_index;
}

int main()
{
    return 0;
}