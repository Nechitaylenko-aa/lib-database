
#include "CDataBaseModel.h"
#include <vector>
//#include "octo-encryption-cpp/base64.hpp"
#include "octo-encryption-cpp/base64.hpp"

CDataBaseModel::CDataBaseModel(const Tstring  &name, const bool &is_encrypt)
{
    m_request = new SDBRequest();
    answer  = new std::vector<std::vector<CDBValue*>*>();
    m_fields  = new std::vector<Tstring>();

    this->name = name;
}

CDataBaseModel::~CDataBaseModel()
{
    clear_all();
    delete m_request;
    delete answer;
    delete m_fields;
}

Tstring CDataBaseModel::getConditionsString()
{
    Tstring req;

    //if (m_request->values->isEmpty() || m_request->conditions->empty())
    if (m_request->conditions->isEmpty())
    {
        return "";
    }

    for (int i = 0; i < this->m_request->conditions->Length(); ++i)
    {
        SDBCondition * cond = this->m_request->conditions->getValue(i);

        if (!cond)
        {
            return "";
        }

        if (cond->logic != ELogicOpers::ELO_NO)
        {
            req += " ";
            req += LogicOpersStrings[cond->logic];

        }

        req += " ";
        req += cond->name;
        req += " ";
        req += CondtionOpersStrings[cond->oper];
        req += " ";

        if (cond->isString)
        {
            req += "\'";
            req += cond->value_s;
            req += "\'";
        }
        else
        {
            req += cond->value_s;
        }
    }

    return req;
}

Tstring CDataBaseModel::getValuesString(const bool &isNames)
{
    Tstring req;
    int i = 0;

    if (m_request->values->empty() && m_request->conditions->isEmpty())
    {
        return "";
    }

    for ( ; i < this->m_request->values->size() - 1; ++i)
    {
        CDBValue * val = this->m_request->values->at(i);
        if (!val)
        {
            return "";
        }

        if (isNames)
        {
            req += val->name();
            req += " = ";
        }

        if (val->type() == EValueTypes::EVT_STRING)
        {
            req += "\'";
            Tstring e_str = val->string();
            escape_string(e_str);

            if (m_is_encrypt)
            {
                e_str = octo::encryption::Base64::base64_encode(e_str);
            }

            req += e_str;
            req += "\'";
        }
        else
        {
            req += val->getValueAsString();
        }

        req += ", ";
    }

    CDBValue * val = this->m_request->values->at(i);

    if (!val)
    {
        return "";
    }

    if (isNames)
    {
        req += val->name();
        req += " = ";
    }

    if (val->type() == EValueTypes::EVT_STRING)
    {
        req += "\'";
        Tstring e_s = val->string();
        escape_string(e_s);

        if (m_is_encrypt)
        {
            e_s = octo::encryption::Base64::base64_encode(e_s);
        }

        req += e_s;
        req += "\'";
    }
    else
    {
        req += val->getValueAsString(2, true, EBT_DIGIT);
    }

    return req;
}

Tstring CDataBaseModel::getNamesString()
{
    Tstring req;

    int i = 0;
    if (m_request->values->empty() && m_request->conditions->isEmpty())
    {
        return "";
    }

    for ( ; i < this->m_request->values->size() - 1; ++i)
    {
        CDBValue * val = this->m_request->values->at(i);

        if (!val)
        {
            return "";
        }

        req += val->name();
        req += ", ";
    }

    CDBValue * val = this->m_request->values->at(i);

    if (!val)
    {
        return "";
    }

    req += val->name();

    return req;
}

Tstring CDataBaseModel::getSelectReq(const bool & isAll)
{
    Tstring req = "SELECT ";

    if (isAll)
    {
        req += "* ";
    }
    else
    {
        req += this->getNamesString();
    }

    req += " FROM ";
    req += this->name;

    if(!this->m_request->conditions->isEmpty())
    {
        req += " WHERE ";
        req += this->getConditionsString();
    }

    req += ";";

    return req;
}

Tstring CDataBaseModel::getUpdateReq()
{
    Tstring req;

    if (m_db_type == db_type::dt_mysql)
    {
        req = "UPDATE `" + this->name;
        req += "` SET ";
    }

    if (m_db_type == db_type::dt_postgres)
    {
        req = "UPDATE " + this->name;
        req += " SET ";
    }


    req += this->getValuesString(true);

    if (!this->m_request || !this->m_request->conditions)
    {
        return "";
    }

    if(!this->m_request->conditions->isEmpty())
    {
        req += " WHERE ";
        req += this->getConditionsString();
    }

    req += ";";

    return req;
}

Tstring CDataBaseModel::getInsertReq(const bool &isSimple)
{
    Tstring req = "INSERT INTO " + this->name;

    if (!isSimple)
    {
        req += " (";

        req += this->getNamesString();
        req += ")";
    }

    req += " VALUES ";
    req += " (";

    req += this->getValuesString();

    req += ");";

    return req;
}

Tstring CDataBaseModel::getDeleteReq()
{

    Tstring req = "DELETE FROM " + this->name + " WHERE ";
    req += this->getConditionsString();

    req += ";";

    return req;
}

SDBRequest *CDataBaseModel::request()
{
    return m_request;
}

void CDataBaseModel::clear_all()
{
    clear_answer();
    this->m_request->values->clear();
    this->m_request->conditions->clear();
    this->m_request->other->clear();
}

void CDataBaseModel::clear_answer()
{
    for (auto &row : *answer)
    {
        for (auto &item : *row)
        {
            delete item;
        }
        delete row;
    }
    answer->clear();
}

std::vector<std::vector<CDBValue *> *> *CDataBaseModel::get_answer()
{
    return answer;
}

std::vector<Tstring> *CDataBaseModel::fields()
{
    return m_fields;
}

SDBRequest::~SDBRequest()
{
    for (auto &item : *values)
    {
        delete item;
    }

    delete values;
    delete conditions;
    delete other;
}

SDBRequest::SDBRequest(const std::vector<CDBValue *> &v, const CDBConditionList &c, const CStringsVector &o) :
        conditions(new CDBConditionList(c))
{
    other = new CStringsVector();
    *other = o;

    values = new std::vector<CDBValue*>();
    for (auto &item : v)
    {
        values->push_back(new CDBValue(*item));
    }
}
