

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
    else if (xmltype == "s8") return "char";
    else if (xmltype == "u8") return "unsigned char";
    else if (xmltype == "s16") return "short";
    else if (xmltype == "u16") return "unsigned short";
    else if (xmltype == "s32") return "int";
    else if (xmltype == "u32") return "unsigned int";
    else if (xmltype == "s64") return "long long";
    else if (xmltype == "u64") return "unsigned long long";
    else if (xmltype == "f32") return "float";



    return xmltype;
}

std::string getMysqlType(const DataStruct::DataMember & m)
{
    if (m.type_ == "string" && getBitFlag(m.tag_, MT_DB_BLOB))
    {
        return " longblob NOT NULL ";
    }
    else if (m.type_ == "string")
    {
        return " varchar(255) NOT NULL DEFAULT '' ";
    }
    else if (m.type_ == "i8" || m.type_ == "i16" || m.type_ == "i32" || m.type_ == "i64")
    {
        if (getBitFlag(m.tag_, MT_DB_AUTO))
        {
            return " bigint(20) NOT NULL AUTO_INCREMENT ";
        }
        return " bigint(20) NOT NULL DEFAULT '0' ";
    }
    else if (m.type_ == "ui8" || m.type_ == "ui16" || m.type_ == "ui32" || m.type_ == "ui64")
    {
        if (getBitFlag(m.tag_, MT_DB_AUTO))
        {
            return " bigint(20) unsigned NOT NULL AUTO_INCREMENT ";
        }
        return " bigint(20) unsigned NOT NULL DEFAULT '0' ";
    }
    else if (m.type_ == "float" || m.type_ == "double")
    {
        return " double NOT NULL DEFAULT '0' ";
    }
    return " longblob NOT NULL ";
}


std::string GenCPP::genRealContent(const std::list<AnyData> & stores)
{
    std::string macroFileName = std::string("_") + _filename  + "_H_";
    std::transform(macroFileName.begin(), macroFileName.end(), macroFileName.begin(), [](char ch){ return std::toupper(ch); });


    std::string text = LFCR + "#ifndef " + macroFileName + LFCR;
    text += "#define " + macroFileName + LFCR + LFCR;

    for (auto &info : stores)
    {
        if (info.type_ == GT_DataComment)
        {
            text += LFCR;
            text += "/*" + info._comment.desc_ + "*/" + LFCR;
        }
        else if (info.type_ == GT_DataConstValue)
        {
            text += LFCR;
            text += genDataConst(info.const_);
        }
        else if (info.type_ == GT_DataEnum)
        {
            text += LFCR;
            text += genDataEnum(info.enum_);
        }
        else if (info.type_ == GT_DataArray)
        {
            text += LFCR;
            text += genDataArray(info.array_);
        }
        else if (info.type_ == GT_DataMap)
        {
            text += LFCR;
            text += genDataMap(info.map_);
        }
        else if (info.type_ == GT_DataPacket)
        {
            text += LFCR;
            text += genDataPacket(info.proto_);
        }

    }
    text += LFCR + "#endif" + LFCR;

    return text;
}


std::string GenCPP::genDataConst(const DataConstValue & dc)
{
    std::string text;
    text += "const " + getRealType(dc.type_) + " " + dc.name_ + " = " + dc.value_ + "; ";
    if (!dc.desc_.empty())
    {
        text += "//" + dc.desc_ + " ";
    }
    text += LFCR;
    return text;
}
std::string GenCPP::genDataEnum(const DataEnum & de)
{
    std::string text;
    text += "enum " + de.name_ + " : " + getRealType(de.type_) + LFCR;
    text += "{" + LFCR;
    for (auto m : de.members_)
    {
        text += "    " + m.name_ + " = " + m.value_ + ", ";
        if (!m.desc_.empty())
        {
            text += "//" + m.desc_ + " ";
        }
        text += LFCR;
    }
    text += "};" + LFCR;
    return text;
}
std::string GenCPP::genDataArray(const DataArray & da)
{
    std::string text;
    text += LFCR + "typedef std::vector<" + getRealType(da.type_) + "> " + da.arrayName_ + "; ";
    if (!da.desc_.empty())
    {
        text += "//" + da.desc_ + " ";
    }
    text += LFCR;
    return text;
}
std::string GenCPP::genDataMap(const DataMap & dm)
{
    std::string text;
    text += LFCR + "typedef std::map<"
        + getRealType(dm.typeKey_) + ", " + getRealType(dm.typeValue_)
        + "> " + dm.mapName_ + "; ";
    if (!dm.desc_.empty())
    {
        text += "//" + dm.desc_ + " ";
    }
    text += LFCR;
    return text;
}
std::string GenCPP::genDataPacket(const DataPacket & dp)
{
    std::string text;

    //write struct
    text += "struct " + dp.struct_.name_;
    if (!dp.struct_.desc_.empty())
    {
        text += " //" + dp.struct_.desc_ + " ";
    }
    text += LFCR;
    text += "{" + LFCR;

    std::string dbtable = "`tb_" + dp.struct_.name_ + "`";
    text += std::string("    static const ") + getRealType(ProtoIDType) + " getProtoID() { return " + dp.const_.value_ + ";}" + LFCR;
    text += std::string("    static const ") + getRealType("string") + " getProtoName() { return \"" + dp.struct_.name_ + "\";}" + LFCR;
    if (!dp.struct_.store_.empty())
    {
        text += "    inline std::vector<std::string>  getDBBuild();" + LFCR;
        text += "    inline std::string  getDBInsert();" + LFCR;
        text += "    inline std::string  getDBDelete();" + LFCR;
        text += "    inline std::string  getDBUpdate();" + LFCR;
        text += "    inline std::string  getDBSelect();" + LFCR;
        text += "    inline std::string  getDBSelectPure();" + LFCR;
        text += "    inline bool fetchFromDBResult(zsummer::mysql::DBResult &result);" + LFCR;
    }

    for (const auto & m : dp.struct_.members_)
    {
        text += "    " + getRealType(m.type_) + " " + m.name_ + "; ";
        if (!m.desc_.empty())
        {
            text += "//" + m.desc_ + " ";
        }
        text += LFCR;
    }

    if (!dp.struct_.members_.empty())
    {    //struct init
        text += "    " + dp.struct_.name_ + "()" + LFCR;
        text += "    {" + LFCR;
        for (const auto &m : dp.struct_.members_)
        {
            std::string def = getTypeDefault(m.type_);
            if (!def.empty())
            {
                text += "        " + m.name_ + " = " + def + ";" + LFCR;
            }
        }
        text += "    }" + LFCR;
    }

    if (!dp.struct_.members_.empty())
    {    //struct init
        text += "    " + dp.struct_.name_ + "(";
        for (size_t i = 0; i < dp.struct_.members_.size(); i++)
        {
            const auto & m = dp.struct_.members_[i];
            if (i != 0) text += ", ";
            text += "const " + getRealType(m.type_) + " & " + m.name_;
        }
        text += ")" + LFCR;
        text += "    {" + LFCR;
        for (const auto &m : dp.struct_.members_)
        {
            text += "        this->" + m.name_ + " = " + m.name_ + ";" + LFCR;
        }
        text += "    }" + LFCR;
    }
    text += "};" + LFCR;

    if (!dp.struct_.store_.empty())
    {
        //build
        text += LFCR;
        text += "std::vector<std::string>  " + dp.struct_.name_ + "::getDBBuild()" + LFCR;
        text += "{" + LFCR;
        text += "    std::vector<std::string> ret;" + LFCR;
//        text += "    ret.push_back(\"desc " + dbtable + "\");" + LFCR;
        text += "    ret.push_back(\"CREATE TABLE IF NOT EXISTS " + dbtable + " (";
        for (const auto &m : dp.struct_.members_)
        {
            if (m.tag_ != 0 && !getBitFlag(m.tag_, MT_DB_IGNORE)) text += "        `" + m.name_ + "`" + getMysqlType(m) + ",";
        }
        if (true)
        {
            std::string primaryKey;
            for(const auto &m : dp.struct_.members_)
            {
                if (getBitFlag(m.tag_, MT_DB_KEY)) mergeToString(primaryKey, ",", std::string("`") + m.name_ + "`");
            }
            if (!primaryKey.empty())
            {
                text += "        PRIMARY KEY(";
                text += primaryKey + "),";
            }
            for (const auto &m : dp.struct_.members_)
            {
                if (getBitFlag(m.tag_, MT_DB_UNI))
                {
                    text += "        UNIQUE KEY `";
                    text += m.name_ + "` (`" + m.name_ + "`),";
                }
                else if (getBitFlag(m.tag_, MT_DB_IDX))
                {
                    text += "        KEY `";
                    text += m.name_ + "` (`" + m.name_ + "`),";
                }
            }
        }
        if (text.back() == ',') text.pop_back();
        text += " ) ENGINE = " + dp.struct_.store_ + " DEFAULT CHARSET = utf8\");" + LFCR;

        for (auto& m : dp.struct_.members_)
        {
            if (m.tag_ == 0 || !getBitFlag(m.tag_, MT_DB_IGNORE))
            {
                text += "    ret.push_back(\"alter table `tb_" + dp.struct_.name_ + "` add `" + m.name_ + "` " + getMysqlType(m) + "\");" + LFCR;
                text += "    ret.push_back(\"alter table `tb_" + dp.struct_.name_ + "` change `" + m.name_ + "` " + " `" + m.name_ + "` " + getMysqlType(m) + "\");" + LFCR;
            }
        }
        text += "    return ret;" + LFCR;
        text += "}" + LFCR;

        //select
        text += "std::string  " + dp.struct_.name_ + "::getDBSelect()" + LFCR;
        text += "{" + LFCR;
        text += "    zsummer::mysql::DBQuery q;" + LFCR;
        text += "    q.init(\"select ";
        for (auto& m : dp.struct_.members_)
        {
            if (!getBitFlag(m.tag_, MT_DB_IGNORE))
            {
                text += "`" + m.name_ + "`";
                text += ",";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += " from " + dbtable + " where ";
        for (auto& m : dp.struct_.members_)
        {
            if (getBitFlag(m.tag_, MT_DB_KEY))
            {
                text += "`" + m.name_ + "` = ? and ";
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
        for (auto& m : dp.struct_.members_)
        {
            if (getBitFlag(m.tag_, MT_DB_KEY))
            {
                text += "    q << this->" + m.name_ + ";" + LFCR;
            }
        }
        text += "    return q.pickSQL();" + LFCR;
        text += "}" + LFCR;

        //select pure
        text += "std::string  " + dp.struct_.name_ + "::getDBSelectPure()" + LFCR;
        text += "{" + LFCR;
        text += "    return \"select ";
        for (auto& m : dp.struct_.members_)
        {
            if (!getBitFlag(m.tag_, MT_DB_IGNORE))
            {
                text += "`" + m.name_ + "`";
                text += ",";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += " from " + dbtable + " \";" + LFCR;
        text += "}" + LFCR;


        //insert
        text += "std::string  " + dp.struct_.name_ + "::getDBInsert()" + LFCR;
        text += "{" + LFCR;
        text += "    zsummer::mysql::DBQuery q;" + LFCR;
        text += "    q.init(\"insert into " + dbtable + "(";
        for (auto& m : dp.struct_.members_)
        {
            if (!getBitFlag(m.tag_, MT_DB_IGNORE)  && !getBitFlag(m.tag_, MT_DB_AUTO))
            {
                text += "`" + m.name_ + "`";
                text += ",";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += ") values(";
        for (auto& m : dp.struct_.members_)
        {
            if (!getBitFlag(m.tag_, MT_DB_IGNORE) && !getBitFlag(m.tag_, MT_DB_AUTO))
            {
                text += "?,";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += ")\");" + LFCR;
        for (auto& m : dp.struct_.members_)
        {
            if (!getBitFlag(m.tag_, MT_DB_IGNORE) && !getBitFlag(m.tag_, MT_DB_AUTO))
            {
                if (m.type_ == "ui8" || m.type_ == "ui16" || m.type_ == "ui32" || m.type_ == "ui64"
                    || m.type_ == "i8" || m.type_ == "i16" || m.type_ == "i32" || m.type_ == "i64"
                    || m.type_ == "double" || m.type_ == "float" || m.type_ == "string")
                {
                    text += "    q << this->" + m.name_ + ";" + LFCR;
                }
                else
                {
                    text += "    try" + LFCR;
                    text += "    {" + LFCR;
                    text += "        "  "zsummer::proto4z::WriteStream ws(0);" + LFCR;
                    text += "        "  "ws <<  this->" + m.name_ + ";" + LFCR;
                    text += "        "  "q.add(ws.getStreamBody(), ws.getStreamBodyLen());" + LFCR;
                    text += "    }" + LFCR;
                    text += "    catch(const std::exception & e)" + LFCR;
                    text += "    {" + LFCR;
                    text += "        "  "LOGW(\"catch one except error when insert " + dp.struct_.name_ + "." + m.name_ + " error=\" << e.what());" + LFCR;
                    text += "    }" + LFCR;
                }
                
            }
        }
        text += "    return q.pickSQL();" + LFCR;
        text += "}" + LFCR;

        //delete
        text += "std::string  " + dp.struct_.name_ + "::getDBDelete()" + LFCR;
        text += "{" + LFCR;
        text += "    zsummer::mysql::DBQuery q;" + LFCR;
        text += "    q.init(\"delete from " + dbtable + " where ";
        for (auto& m : dp.struct_.members_)
        {
            if (getBitFlag(m.tag_, MT_DB_KEY))
            {
                text += "`" + m.name_ + "` = ?,";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += " \");" + LFCR;
        for (auto& m : dp.struct_.members_)
        {
            if (getBitFlag(m.tag_, MT_DB_KEY))
            {
                text += "    q << this->" + m.name_ + ";" + LFCR;
            }
        }
        text += "    return q.pickSQL();" + LFCR;
        text += "}" + LFCR;

        //update
        text += "std::string  " + dp.struct_.name_ + "::getDBUpdate()" + LFCR;
        text += "{" + LFCR;
        text += "    zsummer::mysql::DBQuery q;" + LFCR;
        text += "    q.init(\"insert into " + dbtable + "(";
        for (auto& m : dp.struct_.members_)
        {
            if (getBitFlag(m.tag_, MT_DB_KEY))
            {
                text += m.name_;
                text += ",";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += ") values(";
        for (auto& m : dp.struct_.members_)
        {
            if (getBitFlag(m.tag_, MT_DB_KEY))
            {
                text += "?,";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += " ) on duplicate key update ";
        for (auto& m : dp.struct_.members_)
        {
            if (!getBitFlag(m.tag_, MT_DB_KEY) && !getBitFlag(m.tag_, MT_DB_IGNORE) && !getBitFlag(m.tag_, MT_DB_AUTO))
            {
                text += "`" + m.name_ + "` = ?,";
            }
        }
        if (text.back() == ',') text.pop_back();
        text += " \");" + LFCR;

        for (auto& m : dp.struct_.members_)
        {
            if (getBitFlag(m.tag_, MT_DB_KEY))
            {
                text += "    q << this->" + m.name_ + ";" + LFCR;
            }
        }

        for (auto& m : dp.struct_.members_)
        {
            if (!getBitFlag(m.tag_, MT_DB_KEY) && !getBitFlag(m.tag_, MT_DB_IGNORE) && !getBitFlag(m.tag_, MT_DB_AUTO))
            {
                if (m.type_ == "ui8" || m.type_ == "ui16" || m.type_ == "ui32" || m.type_ == "ui64"
                    || m.type_ == "i8" || m.type_ == "i16" || m.type_ == "i32" || m.type_ == "i64"
                    || m.type_ == "double" || m.type_ == "float" || m.type_ == "string")
                {
                    text += "    q << this->" + m.name_ + ";" + LFCR;
                }
                else
                {
                    text += "    try" + LFCR;
                    text += "    {" + LFCR;
                    text += "        "  "zsummer::proto4z::WriteStream ws(0);" + LFCR;
                    text += "        "  "ws <<  this->" + m.name_ + ";" + LFCR;
                    text += "        "  "q.add(ws.getStreamBody(), ws.getStreamBodyLen());" + LFCR;
                    text += "    }" + LFCR;
                    text += "    catch(const std::exception & e)" + LFCR;
                    text += "    {" + LFCR;
                    text += "        "  "LOGW(\"catch one except error when update " + dp.struct_.name_ + "." + m.name_ + " error=\" << e.what());" + LFCR;
                    text += "    }" + LFCR;
                }
                
            }
        }
        text += "    return q.pickSQL();" + LFCR;
        text += "}" + LFCR;

        //fetch
        text += "bool " + dp.struct_.name_ + "::fetchFromDBResult(zsummer::mysql::DBResult &result)" + LFCR;
        text += "{" + LFCR;
        text += "    if (result.getErrorCode() != zsummer::mysql::QEC_SUCCESS)" + LFCR;
        text += "    {" + LFCR;
        text += "        "  "LOGE(\"error fetch " + dp.struct_.name_ + " from table " + dbtable + " . ErrorCode=\"  <<  result.getErrorCode() << \", Error=\" << result.getErrorMsg() << \", sql=\" << result.peekSQL());" + LFCR;
        text += "        "  "return false;" + LFCR;
        text += "    }" + LFCR;
        text += "    try" + LFCR;
        text += "    {" + LFCR;

        text += "        "  "if (result.haveRow())" + LFCR;
        text += "        {" + LFCR;
        for (auto& m : dp.struct_.members_)
        {
            if (getBitFlag(m.tag_, MT_DB_IGNORE))
            {
                continue;
            }
            if (m.type_ == "ui8" || m.type_ == "ui16" || m.type_ == "ui32" || m.type_ == "ui64"
                || m.type_ == "i8" || m.type_ == "i16" || m.type_ == "i32" || m.type_ == "i64"
                || m.type_ == "double" || m.type_ == "float" || m.type_ == "string")
            {
                text += "            result >> this->" + m.name_ + ";" + LFCR;
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
                text += "                    "  "rs >> this->" + m.name_ + ";" + LFCR;
                text += "                "  "}" + LFCR;
                text += "            }" + LFCR;
                text += "            catch(const std::exception & e)" + LFCR;
                text += "            {" + LFCR;
                text += "                "  "LOGW(\"catch one except error when fetch " + dp.struct_.name_ + "." + m.name_ + "  from table " + dbtable + " . what=\" << e.what() << \"  ErrorCode=\"  <<  result.getErrorCode() << \", Error=\" << result.getErrorMsg() << \", sql=\" << result.peekSQL());" + LFCR;
                text += "            }" + LFCR;
            }
        }

        text += "            return true; " + LFCR;
        text += "        }" + LFCR;

        text += "    }" + LFCR;
        text += "    catch(const std::exception & e)" + LFCR;
        text += "    {" + LFCR;
        text += "        "  "LOGE(\"catch one except error when fetch " + dp.struct_.name_ + " from table " + dbtable + " . what=\" << e.what() << \"  ErrorCode=\"  <<  result.getErrorCode() << \", Error=\" << result.getErrorMsg() << \", sql=\" << result.peekSQL());" + LFCR;
        text += "        "  "return false;" + LFCR;
        text += "    }" + LFCR;

        text += "    return false;" + LFCR;
        text += "}" + LFCR;
    }




    //input stream operator
    text += "inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const " + dp.struct_.name_ + " & data)" + LFCR;
    text += "{" + LFCR;
    for (const auto &m : dp.struct_.members_)
    {
        text += "    ws << data." + m.name_ + "; " + LFCR;
    }

    text += "    return ws;" + LFCR;
    text += "}" + LFCR;


    //output stream operator
    text += "inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, " + dp.struct_.name_ + " & data)" + LFCR;
    text += "{" + LFCR;

    for (const auto &m : dp.struct_.members_)
    {
        text += "    rs >> data." + m.name_ + "; " + LFCR;
    }

    text += "    return rs;" + LFCR;
    text += "}" + LFCR;

    //input log4z operator
    if (dp.struct_.hadLog4z_)
    {
        text += "inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const " + dp.struct_.name_ + " & info)" + LFCR;
        text += "{" + LFCR;
        text += "    stm << \"[\";" + LFCR;
        for (const auto &m : dp.struct_.members_)
        {
            text += "    stm << \"" + m.name_ + "=\"" + " << info." + m.name_ + " << \",\";" + LFCR;
        }
        text += "    stm << \"]\";" + LFCR;
        text += "    return stm;" + LFCR;
        text += "}" + LFCR;
    }
    return text;
}



void writeCPPReflection(std::map<unsigned short, std::string> & keys, std::map<unsigned short, std::string> & errCodes)
{
    std::string trustName = SupportLanguageFilePath[SL_CPP];
    trustName += "/ProtoReflection";
    trustName += SupportLanguageFileSuffix[SL_CPP];
    std::string content;
    content.push_back((char)(unsigned char)0xef);
    content.push_back((char)(unsigned char)0xbb);
    content.push_back((char)(unsigned char)0xbf);
    content += R"---OOO(
#ifndef _ProtoReflection_H_ 
#define _ProtoReflection_H_ 
class ProtoReflection
{
public:
    static std::string getProtoName(ui16 protoID)
    {
        switch (protoID)
        {
            case 0:
            {
                return "InvalidProtoID";
            }
            break;
)---OOO";
    for (auto & kv : keys)
    {
        content += "            case " + toString(kv.first) + ":\r\n            {\r\n                   return \"" + kv.second + "\";\r\n            }\r\n            break;\r\n";
    }
    content += R"---OOO(
        }
        char buf[20];
        printf(buf, protoID);
        return std::string("UnknownProtoID_") + buf;
    }
)---OOO";

    content += R"---OOO(
    static std::string getErrorDesc(ui16 errCode)
    {
        switch (errCode)
        {
)---OOO";
    for (auto & kv : errCodes)
    {
        content += "            case " + toString(kv.first) + ":\r\n            {\r\n                   return \"" + kv.second + "\";\r\n            }\r\n            break;\r\n";
    }
    content += R"---OOO(
        }
        char buf[20];
        printf(buf, errCode);
        return std::string("UnknownErrorCode_") + buf;
    }
};

#endif


)---OOO";


    if (writeFileContent(trustName, content.c_str(), content.length(), false) != content.length())
    {
        E("writeCSharpReflection open file Error ");
    }
}


