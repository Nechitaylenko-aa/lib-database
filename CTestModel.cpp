//
// Created by artem on 07.10.23.
//

#include "CTestModel.h"
#include <iostream>

CTestModel::CTestModel(const Tstring &name, CAbstractConnection *connection)
    : ADatabaseModel(name, connection)
{}

CTestModel::~CTestModel()
{
    this->clear_all();
}

bool CTestModel::prepare_connection()
{
    if (!m_connection->is_opened())
    {
        if (!m_connection->open())
        {
            fprintf(stderr, "Can't open connection\n");
            return false;
        }
    }

    clear_all();

    return true;
}


void CTestModel::print_row(const Tsize &index)
{
    if (!prepare_connection())
    {
        return;
    }

    // теперь условие

    SDBCondition    cond;

    if (index > 0)
    {
        cond.oper = EConditionOpers::ECO_EQ;
    }

    else
    {
        cond.oper = EConditionOpers::ECO_GE;
    }

    cond.name = "id";
    cond.value_s = std::to_string(index);

    *request->conditions << cond;

    if (this->read(true))
    {
        Tstring r_string;

        for (auto &row : *answer)
        {
            r_string = "";

            for (auto &value : *row)
            {
                 r_string += value->getValueAsString() + ";\t";
            }

            r_string += "\n";

            fprintf(stdout, "%s", r_string.c_str());
        }
    }

    this->clear_all();
}

bool CTestModel::link_add(s_link *link)
{
    if (!prepare_connection())
    {
        return false;
    }

    link_to_row(link, request->values);

    bool res = add(false);

    if (res)
    {
        link->id = static_cast<int>(get_last_id());
    }

    return res;
}

bool CTestModel::link_update(s_link *link)
{
    SDBCondition cond;
    cond.name = "id";
    cond.value_s = std::to_string(link->id);
    cond.oper = EConditionOpers::ECO_EQ;

    request->conditions->push_back(cond);

    link_to_row(link, request->values);

    bool res = write();

    return res;
}

bool CTestModel::link_remove(const long long int &id_record)
{
    if (!prepare_connection())
    {
        return false;
    }

    * request->conditions << SDBCondition("id", std::to_string(id_record));

    bool res = remove();

    return res;
}

Tsize CTestModel::get_links_by_user(const Tsize &id_user, std::vector<s_link*> *links)
{
    SDBCondition cond;
    cond.name = "id_user";
    cond.value_s = std::to_string(id_user);
    cond.oper = EConditionOpers::ECO_EQ;

    request->conditions->push_back(cond);

    bool res = read(true);

    if (!res)
    {
        return 0;
    }

    for (auto &row : *answer)
    {
        auto link = new s_link();
        link_from_row(link, row);

        links->push_back(link);
    }

    return answer->size();
}

Tsize CTestModel::get_links_by_users_node(const Tsize &id_node, const Tsize &id_user, std::vector<s_link*> *links)
{

    SDBCondition cond("id_user", std::to_string(id_user));

    SDBCondition cond1;
    cond1.name = "id_node";
    cond1.value_s = std::to_string(id_node);
    cond1.oper = EConditionOpers::ECO_EQ;
    cond1.logic = ELogicOpers::ELO_AND;

    *request->conditions << cond << cond1;

    bool res = read(true);

    if (!res)
    {
        return 0;
    }

    for (auto &row : *answer)
    {
        auto link = new s_link();
        link_from_row(link, row);

        links->push_back(link);
    }

    return answer->size();
}

s_link CTestModel::get_link_by_id(const long long int &id_rec)
{
    s_link link{};
    SDBCondition cond;

    cond.name = "id";
    cond.value_s = std::to_string(id_rec);
    cond.oper = EConditionOpers::ECO_EQ;

    this->request->conditions->push_back(cond);

    if (read(true))
    {
        auto row = answer->at(0);
        link_from_row(&link, row);
    }

    clear_all();

    return link;
}

void CTestModel::link_to_row(s_link *link, std::vector<CDBValue *> *row)
{
    // don't write ID if it is autoincrement
    row->push_back(new CDBValue("id_user", link->id_user));
    row->push_back(new CDBValue("id_node", link->id_node));
    row->push_back(new CDBValue("description", link->description));
    row->push_back(new CDBValue("content", link->content));
}

void CTestModel::link_from_row(s_link *link, std::vector<CDBValue *> *row)
{
    link->id = row->at(0)->integer();
    link->id_user = row->at(1)->integer();
    link->id_node = row->at(2)->integer();
    link->description = row->at(3)->string();
    link->content = row->at(4)->string();
}

