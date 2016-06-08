

#include "genCPP.h"



std::string GenCPP::getRealType(const std::string & xmltype)
{
    if ( xmltype == "i8") return "char";
    else if ( xmltype == "ui8") return "unsigned char";
    else if ( xmltype == "i16") return "short";
    else if ( xmltype == "ui16") return "unsigned short";
    else if ( xmltype == "i32") return "int";
    else if ( xmltype == "ui32") return "unsigned int";
    else if ( xmltype == "i64") return "long long";
    else if ( xmltype == "ui64") return "unsigned long long";
    else if ( xmltype == "float") return "float";
    else if ( xmltype == "double") return "double";
    else if ( xmltype == "string") return "std::string";
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
        if (getBitFlag(m._tag, MT_DB_AUTO))
        {
            return " bigint(20) NOT NULL AUTO_INCREMENT ";
        }
        return " bigint(20) NOT NULL DEFAULT '0' ";
    }
    else if (m._type == "ui8" || m._type == "ui16" || m._type == "ui32" || m._type == "ui64")
    {
        if (getBitFlag(m._tag, MT_DB_AUTO))
        {
            return " bigint(20) unsigned NOT NULL AUTO_INCREMENT ";
        }
        return " bigint(20) unsigned NOT NULL DEFAULT '0' ";
    }
    return " blob ";
}


std::string GenCPP::genRealContent(const std::list<AnyData> & stores)
{
    std::string macroFileName = std::string("_") + _filename  + "_H_";
    std::transform(macroFileName.begin(), macroFileName.end(), macroFileName.begin(), [](char ch){ return std::toupper(ch); });


    std::string text = LFCR + "#ifndef " + macroFileName + LFCR;
    text += "#define " + macroFileName + LFCR + LFCR;

    for (auto &info : stores)
    {
        if (info._type == GT_DataComment)
        {
            text += LFCR;
            text += "/*" + info._comment._desc + "*/" + LFCR;
        }
        else if (info._type == GT_DataConstValue)
        {
            text += LFCR;
            text += genDataConst(info._const);
        }
        else if (info._type == GT_DataEnum)
        {
            text += LFCR;
            text += genDataEnum(info._enum);
        }
        else if (info._type == GT_DataArray)
        {
            text += LFCR;
            text += genDataArray(info._array);
        }
        else if (info._type == GT_DataMap)
        {
            text += LFCR;
            text += genDataMap(info._map);
        }
        else if (info._type == GT_DataPacket)
        {
            text += LFCR;
            text += genDataPacket(info._proto);
        }

    }
    text += LFCR + "#endif" + LFCR;

    return std::move(text);
}


std::string GenCPP::genDataConst(const DataConstValue & dc)
{
    std::string text;
    text += "const " + getRealType(dc._type) + " " + dc._name + " = " + dc._value + "; ";
    if (!dc._desc.empty())
    {
        text += "//" + dc._desc + " ";
    }
    text += LFCR;
    return text;
}
std::string GenCPP::genDataEnum(const DataEnum & de)
{
    std::string text;
    text += "enum " + de._name + " : " + getRealType(de._type) + LFCR;
    text += "{" + LFCR;
    for (auto m : de._members)
    {
        text += "    " + m._name + " = " + m._value + ", ";
        if (!m._desc.empty())
        {
            text += "//" + m._desc + " ";
        }
        text += LFCR;
    }
    text += "};" + LFCR;
    return text;
}
std::string GenCPP::genDataArray(const DataArray & da)
{
    std::string text;
    text += LFCR + "typedef std::vector<" + getRealType(da._type) + "> " + da._arrayName + "; ";
    if (!da._desc.empty())
    {
        text += "//" + da._desc + " ";
    }
    text += LFCR;
    return text;
}
std::string GenCPP::genDataMap(const DataMap & dm)
{
    std::string text;
    text += LFCR + "typedef std::map<"
        + getRealType(dm._typeKey) + ", " + getRealType(dm._typeValue)
        + "> " + dm._mapName + "; ";
    if (!dm._desc.empty())
    {
        text += "//" + dm._desc + " ";
    }
    text += LFCR;
    return text;
}
std::string GenCPP::genDataPacket(const DataPacket & dp)
{
    std::string text;

    //write struct
    text += "struct " + dp._struct._name;
    if (!dp._struct._desc.empty())
    {
        text += " //" + dp._struct._desc + " ";
    }
    text += LFCR;
    text += "{" + LFCR;

    std::string dbtable = "`tb_" + dp._struct._name + "`";
    text += std::string("    static const ") + getRealType(ProtoIDType) + " getProtoID() { return " + dp._const._value + ";}" + LFCR;
    text += std::string("    static const ") + getRealType("string") + " getProtoName() { return \"" + dp._struct._name + "\";}" + LFCR;
    if (!dp._struct._store.empty())
    {
        text += "    inline const std::vector<std::string>  getDBBuild();" + LFCR;
        text += "    inline std::string  getDBInsert();" + LFCR;
        text += "    inline std::string  getDBDelete();" + LFCR;
        text += "    inline std::string  getDBUpdate();" + LFCR;
        text += "    inline std::string  getDBSelect();" + LFCR;
        text += "    inline std::string  getDBSelectPure();" + LFCR;
        text += "    inline bool fetchFromDBResult(zsummer::mysql::DBResult &result);" + LFCR;
    }

    for (const auto & m : dp._struct._members)
    {
        text += "    " + getRealType(m._type) + " " + m._name + "; ";
        if (!m._desc.empty())
        {
            text += "//" + m._desc + " ";
        }
        text += LFCR;
    }

    if (!dp._struct._members.empty())
    {    //struct init
        text += "    " + dp._struct._name + "()" + LFCR;
        text += "    {" + LFCR;
        for (const auto &m : dp._struct._members)
        {
            std::string def = getTypeDefault(m._type);
            if (!def.empty())
            {
                text += "        " + m._name + " = " + def + ";" + LFCR;
            }
        }
        text += "    }" + LFCR;
    }

    if (!dp._struct._members.empty())
    {    //struct init
        text += "    " + dp._struct._name + "(";
        for (size_t i = 0; i < dp._struct._members.size(); i++)
        {
            const auto & m = dp._struct._members[i];
            if (i != 0) text += ", ";
            text += "const " + getRealType(m._type) + " & " + m._name;
        }
        text += ")" + LFCR;
        text += "    {" + LFCR;
        for (const auto &m : dp._struct._members)
        {
            text += "        this->" + m._name + " = " + m._name + ";" + LFCR;
        }
        text += "    }" + LFCR;
    }
    text += "};" + LFCR;

    if (!dp._struct._store.empty())
    {
        //build
        text += LFCR;
        text += "const std::vector<std::string>  " + dp._struct._name + "::getDBBuild()" + LFCR;
        text += "{" + LFCR;
        text += "    std::vector<std::string> ret;" + LFCR;
//        text += "    ret.push_back(\"desc " + dbtable + "\");" + LFCR;
        text += "    ret.push_back(\"CREATE TABLE IF NOT EXISTS " + dbtable + " (";
        for (const auto &m : dp._struct._members)
        {
            if (m._tag != 0 && !getBitFlag(m._tag, MT_DB_IGNORE)) text += "        `" + m._name + "`" + getMysqlType(m) + ",";
        }
        if (true)
        {
            std::string primaryKey;
            for(const auto &m : dp._struct._members)
            {
                if (getBitFlag(m._tag, MT_DB_KEY)) mergeToString(primaryKey, ",", std::string("`") + m._name + "`");
            }
            if (!primaryKey.empty())
            {
                text += "        PRIMARY KEY(";
                text += primaryKey + "),";
            }
            for (const auto &m : dp._struct._members)
            {
                if (getBitFlag(m._tag, MT_DB_UNI))
                {
                    text += "        UNIQUE KEY `";
                    text += m._name + "` (`" + m._name + "`),";
                }
                else if (getBitFlag(m._tag, MT_DB_IDX))
                {
                    text += "        KEY `";
                    text += m._name + "` (`" + m._name + "`),";
                }
            }
        }
        if (text.back() == ',') text.pop_back();
        text += " ) ENGINE = " + dp._struct._store + " DEFAULT CHARSET = utf8\");" + LFCR;

        for (auto& m : dp._struct._members)
        {
            if (m._tag == 0 || !getBitFlag(m._tag, MT_DB_IGNORE))
            {
                text += "    ret.push_back(\"alter table `tb_" + dp._struct._name + "` add `" + m._name + "` " + getMysqlType(m) + "\");" + LFCR;
                text += "    ret.push_back(\"alter table `tb_" + dp._struct._name + "` change `" + m._name + "` " + " `" + m._name + "` " + getMysqlType(m) + "\");" + LFCR;
            }
        }
        text += "    return std::move(ret);" + LFCR;
        text += "}" + LFCR;

        //select
        text += "std::string  " + dp._struct._name + "::getDBSelect()" + LFCR;
        text += "{" + LFCR;
        text += "    zsummer::mysql::DBQuery q;" + LFCR;
        text += "    q.init(\"select ";
        for (auto& m : dp._struct._members)
        {
            if (!getBitFlag(m._tag, MT_DB_IGNORE))
            {
                text += "`" + m._name + "`";
                text += ",";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += " from " + dbtable + " where ";
        for (auto& m : dp._struct._members)
        {
            if (getBitFlag(m._tag, MT_DB_KEY))
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
        for (auto& m : dp._struct._members)
        {
            if (getBitFlag(m._tag, MT_DB_KEY))
            {
                text += "    q << this->" + m._name + ";" + LFCR;
            }
        }
        text += "    return q.pickSQL();" + LFCR;
        text += "}" + LFCR;

        //select pure
        text += "std::string  " + dp._struct._name + "::getDBSelectPure()" + LFCR;
        text += "{" + LFCR;
        text += "    return \"select ";
        for (auto& m : dp._struct._members)
        {
            if (!getBitFlag(m._tag, MT_DB_IGNORE))
            {
                text += "`" + m._name + "`";
                text += ",";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += " from " + dbtable + " \";" + LFCR;
        text += "}" + LFCR;


        //insert
        text += "std::string  " + dp._struct._name + "::getDBInsert()" + LFCR;
        text += "{" + LFCR;
        text += "    zsummer::mysql::DBQuery q;" + LFCR;
        text += "    q.init(\"insert into " + dbtable + "(";
        for (auto& m : dp._struct._members)
        {
            if (!getBitFlag(m._tag, MT_DB_IGNORE)  && !getBitFlag(m._tag, MT_DB_AUTO))
            {
                text += "`" + m._name + "`";
                text += ",";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += ") values(";
        for (auto& m : dp._struct._members)
        {
            if (!getBitFlag(m._tag, MT_DB_IGNORE) && !getBitFlag(m._tag, MT_DB_AUTO))
            {
                text += "?,";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += ")\");" + LFCR;
        for (auto& m : dp._struct._members)
        {
            if (!getBitFlag(m._tag, MT_DB_IGNORE) && !getBitFlag(m._tag, MT_DB_AUTO))
            {
                text += "    q << this->" + m._name + ";" + LFCR;
            }
        }
        text += "    return q.pickSQL();" + LFCR;
        text += "}" + LFCR;

        //delete
        text += "std::string  " + dp._struct._name + "::getDBDelete()" + LFCR;
        text += "{" + LFCR;
        text += "    zsummer::mysql::DBQuery q;" + LFCR;
        text += "    q.init(\"delete from " + dbtable + " where ";
        for (auto& m : dp._struct._members)
        {
            if (getBitFlag(m._tag, MT_DB_KEY))
            {
                text += "`" + m._name + "` = ?,";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += " \");" + LFCR;
        for (auto& m : dp._struct._members)
        {
            if (getBitFlag(m._tag, MT_DB_KEY))
            {
                text += "    q << this->" + m._name + ";" + LFCR;
            }
        }
        text += "    return q.pickSQL();" + LFCR;
        text += "}" + LFCR;

        //update
        text += "std::string  " + dp._struct._name + "::getDBUpdate()" + LFCR;
        text += "{" + LFCR;
        text += "    zsummer::mysql::DBQuery q;" + LFCR;
        text += "    q.init(\"insert into " + dbtable + "(";
        for (auto& m : dp._struct._members)
        {
            if (getBitFlag(m._tag, MT_DB_KEY))
            {
                text += m._name;
                text += ",";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += ") values(";
        for (auto& m : dp._struct._members)
        {
            if (getBitFlag(m._tag, MT_DB_KEY))
            {
                text += "?,";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += " ) on duplicate key update ";
        for (auto& m : dp._struct._members)
        {
            if (!getBitFlag(m._tag, MT_DB_KEY) && !getBitFlag(m._tag, MT_DB_IGNORE) && !getBitFlag(m._tag, MT_DB_AUTO))
            {
                text += "`" + m._name + "` = ?,";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += " \");" + LFCR;

        for (auto& m : dp._struct._members)
        {
            if (getBitFlag(m._tag, MT_DB_KEY))
            {
                text += "    q << this->" + m._name + ";" + LFCR;
            }
        }

        for (auto& m : dp._struct._members)
        {
            if (!getBitFlag(m._tag, MT_DB_KEY) && !getBitFlag(m._tag, MT_DB_IGNORE) && !getBitFlag(m._tag, MT_DB_AUTO))
            {
                text += "    q << this->" + m._name + ";" + LFCR;
            }
        }
        text += "    return q.pickSQL();" + LFCR;
        text += "}" + LFCR;

        //fetch
        text += "bool " + dp._struct._name + "::fetchFromDBResult(zsummer::mysql::DBResult &result)" + LFCR;
        text += "{" + LFCR;
        text += "    if (result.getErrorCode() != zsummer::mysql::QEC_SUCCESS)" + LFCR;
        text += "    {" + LFCR;
        text += "        "  "LOGE(\"error fetch " + dp._struct._name + " from table " + dbtable + " . ErrorCode=\"  <<  result.getErrorCode() << \", Error=\" << result.getErrorMsg() << \", sql=\" << result.peekSQL());" + LFCR;
        text += "        "  "return false;" + LFCR;
        text += "    }" + LFCR;
        text += "    try" + LFCR;
        text += "    {" + LFCR;

        text += "        "  "if (result.haveRow())" + LFCR;
        text += "        {" + LFCR;
        for (auto& m : dp._struct._members)
        {
            if (getBitFlag(m._tag, MT_DB_IGNORE))
            {
                continue;
            }
            if (m._type == "ui8" || m._type == "ui16" || m._type == "ui32" || m._type == "ui64"
                || m._type == "i8" || m._type == "i16" || m._type == "i32" || m._type == "i64"
                || m._type == "double" || m._type == "float" || m._type == "string")
            {
                text += "            result >> this->" + m._name + ";" + LFCR;
            }
            else
            {
                text += "            try" + LFCR;
                text += "            {" + LFCR;
                text += "                "  "std::string blob;" + LFCR;
                text += "                "  "result >> blob;" + LFCR;
                text += "                "  "if(!blob.empty())" + LFCR;
                text += "                "  "{" + LFCR;
                text += "                    "  "zsummer::proto4z::ReadStream rs(blob.c_str(), (zsummer::proto4z::Integer)blob.length(), false);" + LFCR;
                text += "                    "  "rs >> this->" + m._name + ";" + LFCR;
                text += "                "  "}" + LFCR;
                text += "            }" + LFCR;
                text += "            catch(std::runtime_error e)" + LFCR;
                text += "            {" + LFCR;
                text += "                "  "LOGW(\"catch one except error when fetch " + dp._struct._name + "." + m._name + "  from table " + dbtable + " . what=\" << e.what() << \"  ErrorCode=\"  <<  result.getErrorCode() << \", Error=\" << result.getErrorMsg() << \", sql=\" << result.peekSQL());" + LFCR;
                text += "            }" + LFCR;
            }
        }

        text += "            return true; " + LFCR;
        text += "        }" + LFCR;

        text += "    }" + LFCR;
        text += "    catch(std::runtime_error e)" + LFCR;
        text += "    {" + LFCR;
        text += "        "  "LOGE(\"catch one except error when fetch " + dp._struct._name + " from table " + dbtable + " . what=\" << e.what() << \"  ErrorCode=\"  <<  result.getErrorCode() << \", Error=\" << result.getErrorMsg() << \", sql=\" << result.peekSQL());" + LFCR;
        text += "        "  "return false;" + LFCR;
        text += "    }" + LFCR;

        text += "    return false;" + LFCR;
        text += "}" + LFCR;
    }




    //input stream operator
    text += "inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const " + dp._struct._name + " & data)" + LFCR;
    text += "{" + LFCR;
    for (const auto &m : dp._struct._members)
    {
        text += "    ws << data." + m._name + "; " + LFCR;
    }

    text += "    return ws;" + LFCR;
    text += "}" + LFCR;


    //output stream operator
    text += "inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, " + dp._struct._name + " & data)" + LFCR;
    text += "{" + LFCR;

    for (const auto &m : dp._struct._members)
    {
        text += "    rs >> data." + m._name + "; " + LFCR;
    }

    text += "    return rs;" + LFCR;
    text += "}" + LFCR;

    //input log4z operator
    if (dp._struct._hadLog4z)
    {
        text += "inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const " + dp._struct._name + " & info)" + LFCR;
        text += "{" + LFCR;
        text += "    stm << \"[\\n\";" + LFCR;
        for (const auto &m : dp._struct._members)
        {
            text += "    stm << \"" + m._name + "=\"" + " << info." + m._name + " << \"\\n\";" + LFCR;
        }
        text += "    stm << \"]\\n\";" + LFCR;
        text += "    return stm;" + LFCR;
        text += "}" + LFCR;
    }
    return text;
}



