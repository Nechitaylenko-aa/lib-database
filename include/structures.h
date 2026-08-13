//
// Created by nechi on 07.03.2022.
//

#ifndef DB_MANAGER_STRUCTURES_H
#define DB_MANAGER_STRUCTURES_H


//#include "../../db_types.h"
#include "../core-types.h"

class CDBStringList;



typedef struct s_local_user {
    Tstring user;
    Tstring user_hash;
    Tstring pass;
    Tstring pass_hash;
}s_local_user;

typedef struct s_file_conn
{
    Tstring   links_file;
    Tstring   events_file;
    Tstring   users_file;
    Tstring   bud_groups_file;
    Tstring   bud_waste_file;
    Tstring   bud_income_file;
    Tstring   bud_wishes_file;
    bool    is_data_encrypt_enabled{false};
}SFilesConnection;




#endif //DB_MANAGER_STRUCTURES_H
