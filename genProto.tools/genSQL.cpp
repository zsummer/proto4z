/*
 * ZSUMMER License
 * -----------
 * 
 * ZSUMMER is licensed under the terms of the MIT license reproduced below.
 * This means that ZSUMMER is free software and can be used for both academic
 * and commercial purposes at absolutely no cost.
 * 
 * 
 * ===============================================================================
 * 
 * Copyright (C) 2014-2015 YaweiZhang <yawei.zhang@foxmail.com>.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * ===============================================================================
 * 
 * (end of COPYRIGHT)
 */

#include "genSQL.h"
std::string GenSQL::getRealType(const std::string & xmltype)
{
    if (xmltype == "i8") return "char";
    else if (xmltype == "ui8") return "unsigned char";
    else if (xmltype == "i16") return "short";
    else if (xmltype == "ui16") return "unsigned short";
    else if (xmltype == "i32") return "int";
    else if (xmltype == "ui32") return "unsigned int";
    else if (xmltype == "i64") return "long long";
    else if (xmltype == "ui64") return "unsigned long long";
    else if (xmltype == "float") return "float";
    else if (xmltype == "double") return "double";
    else if (xmltype == "string") return "std::string";
    return xmltype;
}

std::string getMysqlType(const DataStruct::DataMember & m)
{
    if (m._type == "string")
    {
        return " varchar(255) NOT NULL DEFAULT '' ";
    }
    else if (m._type == "i8" || m._type == "i16" || m._type == "i32" || m._type == "i64")
    {
        if (m._tag == MT_DB_AUTO)
        {
            return " bigint(20) NOT NULL AUTO_INCREMENT ";
        }
        return " bigint(20) NOT NULL DEFAULT '0' ";
    }
    else if (m._type == "ui8" || m._type == "ui16" || m._type == "ui32" || m._type == "ui64")
    {
        if (m._tag == MT_DB_AUTO)
        {
            return " bigint(20) unsigned NOT NULL AUTO_INCREMENT ";
        }
        return " bigint(20) unsigned NOT NULL DEFAULT '0' ";
    }
    return " blob ";
}

std::string GenSQL::genRealContent(const std::list<AnyData> & stores)
{
    std::string macroFileName = std::string("_") + _filename + "SQL_H_";
    std::transform(macroFileName.begin(), macroFileName.end(), macroFileName.begin(), [](char ch){ return std::toupper(ch); });


    std::string text = LFCR + "#ifndef " + macroFileName + LFCR;
    text += "#define " + macroFileName + LFCR + LFCR;

    for (auto &info : stores)
    {
        if ((info._type == GT_DataStruct || info._type == GT_DataProto) && info._proto._struct._isStore)
        {
            std::string dbtable = "`tb_" + info._proto._struct._name + "`";

            //build
            text += LFCR;
            text += "inline std::vector<std::string> " + info._proto._struct._name + "_BUILD()" + LFCR;
            text += "{" + LFCR;
            text += "    std::vector<std::string> ret;" + LFCR;
            text += "    ret.push_back(\"desc " + dbtable + "\");" + LFCR;
            text += "    ret.push_back(\"CREATE TABLE " + dbtable + " (" ; 
            std::for_each(info._proto._struct._members.begin(), info._proto._struct._members.end(), [&text](const DataStruct::DataMember & m)
            {
                if (m._tag != MT_DB_IGNORE) text += "        `" + m._name + "`" + getMysqlType(m) + ",";
            });
            if (text.back() == ',') text.pop_back();

            text += "        PRIMARY KEY(";
            std::for_each(info._proto._struct._members.begin(), info._proto._struct._members.end(), [&text](const DataStruct::DataMember & m)
            {
                if (m._tag == MT_DB_KEY || m._tag == MT_DB_AUTO) text += "`" + m._name + "`,";
            });
            if (text.back() == ',') text.pop_back();
            text += ") ";


            text +=" ) ENGINE = MyISAM DEFAULT CHARSET = utf8\");" + LFCR;

            for (auto& m : info._proto._struct._members)
            {
                if (m._tag != MT_DB_IGNORE)
                {
                    text += "    ret.push_back(\"alter table `tb_" + info._proto._struct._name + "` add `" + m._name + "` " + getMysqlType(m) + "\");" + LFCR;
                    text += "    ret.push_back(\"alter table `tb_" + info._proto._struct._name + "` change `" + m._name + "` " + " `" + m._name + "` " + getMysqlType(m) + "\");" + LFCR;
                }
            }
        
            text += "    return std::move(ret);" + LFCR;
            text += "}" + LFCR;



            //data control language
            text += LFCR;
            text += "inline std::vector<std::string> " + info._proto._struct._name + "_DCL()" + LFCR;
            text += "{" + LFCR;
            //select
            text += "    ret.push_back(\"select ";
            for (auto& m : info._proto._struct._members)
            {
                if (m._tag != MT_DB_IGNORE)
                {
                    text += "`" + m._name + "`";
                    text += ",";
                }
            }
            if (text.back() == ',') text.pop_back();
            text += " from " + dbtable + " where ";
            for (auto& m : info._proto._struct._members)
            {
                if (m._tag == MT_DB_KEY || m._tag == MT_DB_AUTO)
                {
                    text += "`" + m._name + "` = ? and ";
                }
            }
            if (text.back() == ' ' && text.at(text.length() - 2) == 'd' && text.at(text.length() - 3) == 'n' && text.at(text.length() - 4) == 'a')
            {
                text.pop_back();
                text.pop_back();
                text.pop_back();
                text.pop_back();
            }
            text += "\");" + LFCR;

            //insert
            text += "    ret.push_back(\"insert into " + dbtable + "(";
            for (auto& m : info._proto._struct._members)
            {
                if (m._tag != MT_DB_IGNORE  && m._tag != MT_DB_AUTO)
                {

                    text += "`" + m._name + "`";
                    text += ",";
                }
            }
            if (text.back() == ',') text.pop_back();
            text += ") values(";
            for (auto& m : info._proto._struct._members)
            {
                if (m._tag != MT_DB_IGNORE  && m._tag != MT_DB_AUTO)
                {
                    text += "?,";
                }
            }
            if (text.back() == ',') text.pop_back();

            text += ")\");" + LFCR;
            //delete
            text += "    ret.push_back(\"delete from " + dbtable + " where ";
            for (auto& m : info._proto._struct._members)
            {
                if (m._tag == MT_DB_KEY)
                {
                    text += "`" + m._name + "` = ?,";
                }
            }
            text.pop_back();

            text += " \");" + LFCR;

            //update
            text += "    ret.push_back(\"insert into " + dbtable + "(";
            for (auto& m : info._proto._struct._members)
            {
                if (m._tag == MT_DB_KEY)
                {
                    text += m._name;
                    text += ",";
                }
            }
            text.pop_back();
            text += ") values(";
            for (auto& m : info._proto._struct._members)
            {
                if (m._tag == MT_DB_KEY )
                {
                    text += "?,";
                }
            }
            text.pop_back();
            text += " ) on duplicate key update ";
            for (auto& m : info._proto._struct._members)
            {
                if (m._tag == MT_NORMAL)
                {
                    text += "`" + m._name + "` = ?,";
                }
            }
            text.pop_back();

            text += " \");" + LFCR;


            text += "    return std::move(ret);" + LFCR;
            text += "}" + LFCR;


            

            //fetch
            text += LFCR;
            text += "inline bool " + info._proto._struct._name + "_FETCH(zsummer::mysql::DBResultPtr ptr, " + info._proto._struct._name + " & info)" + LFCR;
            text += "{" + LFCR;
            text += "    if (ptr->getErrorCode() != zsummer::mysql::QEC_SUCCESS)" + LFCR;
            text += "    {" + LFCR;
            text += "        "  "LOGE(\"fetch info from db found error. ErrorCode=\"  <<  ptr->getErrorCode() << \", Error=\" << ptr->getErrorMsg());" + LFCR;
            text += "        "  "return false;" + LFCR;
            text += "    }" + LFCR;
            text += "    try" + LFCR;
            text += "    {" + LFCR;

            text += "        "  "if (ptr->haveRow())" + LFCR;
            text += "        {" + LFCR;
            for (auto& m : info._proto._struct._members)
            {
                if (m._tag == MT_DB_IGNORE )
                {
                    continue;
                }
                if (m._type == "ui8" || m._type == "ui16" || m._type == "ui32" || m._type == "ui64"
                    || m._type == "i8" || m._type == "i16" || m._type == "i32" || m._type == "i64"
                    || m._type == "double" || m._type == "float" || m._type == "string")
                {
                    text += "            *ptr >> info." + m._name + ";" + LFCR;
                }
                else
                {
                    text += "            try" + LFCR;
                    text += "            {" + LFCR;

                    text += "                "  "std::string blob;" + LFCR;
                    text += "                "  "*ptr >> blob;" + LFCR;
                    text += "                "  "if(!blob.empty())" + LFCR;
                    text += "                "  "{" + LFCR;
                    text += "                    "  "zsummer::proto4z::ReadStream rs(blob.c_str(), (zsummer::proto4z::Integer)blob.length(), false);" + LFCR;
                    text += "                    "  "rs >> info." + m._name + ";" + LFCR;
                    text += "                "  "}" + LFCR;

                    text += "            }" + LFCR;
                    text += "            catch(std::runtime_error e)" + LFCR;
                    text += "            {" + LFCR;
                    text += "                "  "LOGW(\"fetch blob catch one runtime warning. what=\" << e.what());" + LFCR;
                    text += "            }" + LFCR;

                }
            }

            text += "            return true; " + LFCR;
            text += "        }" + LFCR;

            text += "    }" + LFCR;
            text += "    catch(std::runtime_error e)" + LFCR;
            text += "    {" + LFCR;
            text += "        "  "LOGE(\"fetch info catch one runtime error. what=\" << e.what());" + LFCR;
            text += "        "  "return false;" + LFCR;
            text += "    }" + LFCR;

            text += "    return false;" + LFCR;
            text += "}" + LFCR;
            text += LFCR;

            


            text += LFCR;
        }

    }

    text += LFCR + "#endif" + LFCR;

    return std::move(text);
}



