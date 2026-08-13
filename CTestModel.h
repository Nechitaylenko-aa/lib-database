//
// Created by artem on 07.10.23.
//

#ifndef REM_DB_CTESTMODEL_H
#define REM_DB_CTESTMODEL_H

#include "../include/ADatabaseModel.h"



struct s_link
{
    int id{0};
    int id_user{0};
    int id_node{0};
    std::string description{};
    std::string content{};
};

class CTestModel : public ADatabaseModel
{
public:
    CTestModel() = delete;
    CTestModel(const CTestModel &) = delete;
    explicit CTestModel(const std::string &name, CAbstractConnection *connection);
    ~CTestModel() override;

    /// @brief распечатывает в консоли запись с id = index. Если index==0, то все записи.
    void print_row(const Tsize &index);

    bool link_add(s_link * link);

    bool link_update(s_link * link);

    bool link_remove(const long long &id_record);

    Tsize get_links_by_user(const Tsize &id_user, std::vector<s_link*> *links);
    Tsize get_links_by_users_node(const Tsize &id_node, const Tsize &id_user, std::vector<s_link*> *links);

    s_link  get_link_by_id(const long long &id_rec);

private:


    bool prepare_connection();
    void link_to_row(s_link *link, std::vector<CDBValue*> *row);
    void link_from_row(s_link *link, std::vector<CDBValue*> *row);
};


#endif //REM_DB_CTESTMODEL_H
