# include "def.h"
# include "data.h"
# include "fileio.h"
# include "threadbuff.h"
# include "excute.h"
# include "serverthread.h"
# include "TCP.h"

void init_user_data()
{
    for (int i=0; i<MAX_USER_NUM; i++)
    {
        USER_DATA[i].id = 0;
        USER_DATA[i].express_num = 0;
        USER_DATA[i].isIn = FALSE;
        USER_DATA[i].password = 0;
    }
}

void init_manager_data()
{
    for (int i=0; i<MAX_MANAGER_NUM; i++)
    {
        MANAGER_DATA[i].id = 0;
        MANAGER_DATA[i].express_num = 0;
        MANAGER_DATA[i].isIn = FALSE;
        MANAGER_DATA[i].password = 0;
    }
}

void init_express_data()
{
    for (int i=0; i<MAX_EXPRESS_NUM; i++)
    {
        EXPRESS_DATA[i].id = 0;
        EXPRESS_DATA[i].user_id = 0;
        EXPRESS_DATA[i].isIn = FALSE;
        EXPRESS_DATA[i].date = 0;
        EXPRESS_DATA[i].time = 0;
    }
}

int main ()
{
    init_user_data();
    init_manager_data();
    init_express_data();
    user_info* user_data = USER_DATA;
    user_info* manager_data = MANAGER_DATA;
    express_info* exp_data = EXPRESS_DATA;
    int *user_num = &USER_NUM;
    int *manager_num = &MANAGER_NUM;
    int *exp_num = &EXP_NUM;
    *user_num = 0;
    *manager_num = 0;
    *exp_num = 0;
    *user_num = user_read(user_data, MAX_USER_NUM, *user_num);
    *manager_num = read_manger(manager_data, MAX_MANAGER_NUM, *manager_num);
    *exp_num = express_read(exp_data, MAX_EXPRESS_NUM, *exp_num);
    printf("**************************  服务器端建立连接  ***************************\n");
    printf("\n");
    startserver();
    otherOperate();
    stopserver();
    write_user(user_data, MAX_USER_NUM, 0, *user_num);
    write_manager(manager_data, MAX_MANAGER_NUM, 0, *manager_num);
    write_express(exp_data, MAX_EXPRESS_NUM, 0, *exp_num);
    return 0;
}