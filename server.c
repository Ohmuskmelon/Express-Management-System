# include "def.h"
# include "data.h"
# include "fileio.h"
# include "threadbuff.h"
# include "excute.h"
# include "serverthread.h"
# include "TCP.h"


int main ()
{
    user_info* user_data = USER_DATA;
    user_info* manager_data = MANAGER_DATA;
    express_info* exp_data = EXPRESS_DATA;
    int *user_num = &USER_NUM;
    int *manager_num = &MANAGER_NUM;
    int *exp_num = &EXP_NUM;
    *user_num = 0;
    *manager_num = 0;
    *exp_num = 0;
    *user_num = user_read(user_data, USER_NUMBER, *user_num);
    *user_num = user_read(manager_data, USER_NUMBER, *manager_num);
    startserver();
    otherOperate();
    stopserver();
    return 0;
}

