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
            text += "/*" + info._comment._desc + "*/" + LFCR;
        }
        else if (info._type == GT_DataInclude)
        {
            text += "#include <" + info._include._filename + ".h> ";
            if (!info._include._desc.empty())
            {
                text += "//" + info._include._desc + " ";
            }
            text += LFCR;
        }
        else if (info._type == GT_DataConstValue)
        {
            text += "const " + getRealType(info._const._type) + " " + info._const._name + " = " + info._const._value + "; ";
            if (!info._const._desc.empty())
            {
                text += "//" + info._const._desc + " ";
            }
            text += LFCR;
        }
        else if (info._type == GT_DataArray)
        {
            text += LFCR + "typedef std::vector<" + getRealType(info._array._type) + "> " + info._array._arrayName + "; ";
            if (!info._array._desc.empty())
            {
                text += "//" + info._array._desc + " ";
            }
            text += LFCR;
        }
        else if (info._type == GT_DataMap)
        {
            text += LFCR + "typedef std::map<"
                + getRealType(info._map._typeKey) + ", " + getRealType(info._map._typeValue)
                + "> " + info._map._mapName + "; ";
            if (!info._map._desc.empty())
            {
                text += "//" + info._map._desc + " ";
            }
            text += LFCR;
        }
        else if (info._type == GT_DataStruct || info._type == GT_DataProto)
        {
            text += LFCR;
            //write ProtoID
            if (info._type == GT_DataProto)
            {
                text += "const " + getRealType(info._proto._const._type) + " " 
                    + info._proto._const._name + " = " + info._proto._const._value + "; ";
                if (!info._proto._const._desc.empty())
                {
                    text += "//" + info._proto._const._desc + " ";
                }
                text += LFCR;
            }

            //write struct
            text += "struct " + info._proto._struct._name;
            if (!info._proto._struct._desc.empty())
            {
                text += " //" + info._proto._struct._desc + " ";
            }
            text += LFCR;
            text += "{" + LFCR;


            for (const auto & m : info._proto._struct._members)
            {
                text += "    " + getRealType(m._type) + " " + m._name + "; ";
                if (m._tag == MT_DELETE)
                {
                    text += "//[already deleted] ";
                }
                if (!m._desc.empty())
                {
                    text += "//" + m._desc + " ";
                }
                text += LFCR;
            }

            if (!info._proto._struct._members.empty())
            {    //struct init
                text += "    " + info._proto._struct._name + "()" + LFCR;
                text += "    {" + LFCR;
                for (const auto &m : info._proto._struct._members)
                {
                    std::string def = getTypeDefault(m._type);
                    if (!def.empty())
                    {
                        text += "        " + m._name + " = " + def + ";" + LFCR;
                    }
                }
                text += "    }" + LFCR;
            }

            if (!info._proto._struct._members.empty())
            {    //struct init
                text += "    " + info._proto._struct._name + "(";
                for (size_t i = 0; i < info._proto._struct._members.size(); i++)
                {
                    const auto & m = info._proto._struct._members[i];
                    if (i != 0) text += ", ";
                    text += "const " + getRealType(m._type) + " & " + m._name;
                }
                text += ")" + LFCR;
                text += "    {" + LFCR;
                for (const auto &m : info._proto._struct._members)
                {
                    text += "        this->" + m._name + " = " + m._name + ";" + LFCR;  
                }
                text += "    }" + LFCR;
            }

            if (info._type == GT_DataProto)
            {
                text += std::string("    static const ") + getRealType(ProtoIDType) + " GetProtoID() { return " + info._proto._const._value + ";}" + LFCR;
                text += std::string("    static const ") + getRealType("string") + " GetProtoName() { return \""
                    + info._proto._const._name + "\";}" + LFCR;
            }
            text += "};" + LFCR;


            //input stream operator
            text += "inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const " + info._proto._struct._name + " & data)" + LFCR;
            text += "{" + LFCR;
#ifdef __WITH_TAG
            text += "    unsigned long long tag = " + boost::lexical_cast<std::string, unsigned long long>(info._proto._struct._tag) + "ULL;" + LFCR;
            text += "    if (zsummer::proto4z::__localEndianType() != zsummer::proto4z::LittleEndian) tag = zsummer::proto4z::byteRevese(tag);" + LFCR;
            text += "    ws << (zsummer::proto4z::Integer)0;" + LFCR;
            text += "    zsummer::proto4z::Integer offset = ws.getStreamLen();" + LFCR;
            text += "    ws << tag;" + LFCR;
#endif
            for (const auto &m : info._proto._struct._members)
            {
                if (m._tag == MT_DELETE)
                {
                    text += "//    ws << data." + m._name + "; //[already deleted] " + LFCR;
                }
                else
                {
                    text += "    ws << data." + m._name + "; " + LFCR;
                }
            }
#ifdef __WITH_TAG
            text += "    ws.fixOriginalData(offset - 4, ws.getStreamLen() - offset);" + LFCR;
#endif
            text += "    return ws;" + LFCR;
            text += "}" + LFCR;


            //output stream operator
            text += "inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, " + info._proto._struct._name + " & data)" + LFCR;
            text += "{" + LFCR;
#ifdef __WITH_TAG
            text += "    zsummer::proto4z::Integer sttLen = 0;" + LFCR;
            text += "    rs >> sttLen;" + LFCR;
            text += "    zsummer::proto4z::Integer cursor = rs.getStreamUnreadLen();" + LFCR;
            text += "    unsigned long long tag = 0;" + LFCR;
            text += "    rs >> tag;" + LFCR;
            text += "    if (zsummer::proto4z::__localEndianType() != zsummer::proto4z::LittleEndian) tag = zsummer::proto4z::byteRevese(tag);" + LFCR;
#endif
            int curTagIndex = 0;
            for (const auto &m : info._proto._struct._members)
            {
#ifdef __WITH_TAG
                text += "    if ( (1ULL << " + boost::lexical_cast<std::string, int>(curTagIndex)+") & tag)" + LFCR;
                text += "    {" + LFCR;
#endif

#ifndef __WITH_TAG
                if (m._tag == MT_DELETE)
                    text += "//        rs >> data." + m._name + "; " + LFCR;
                else
#endif
                    text += "        rs >> data." + m._name + "; " + LFCR;



#ifdef __WITH_TAG
                text += "    }" + LFCR;
#endif
                curTagIndex++;
            }

#ifdef __WITH_TAG
            text += "    cursor = cursor - rs.getStreamUnreadLen();" + LFCR;
            text += "    rs.skipOriginalData(sttLen - cursor);" + LFCR;
#endif
            text += "    return rs;" + LFCR;
            text += "}" + LFCR;

            //input log4z operator
            if (false)
            {
                text += "inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const " + info._proto._struct._name + " & info)" + LFCR;
                text += "{" + LFCR;
                bool bFirst = true;
                for (const auto &m : info._proto._struct._members)
                {
                    if (bFirst)
                    {
                        bFirst = false;
                        text += "    stm << \"" + m._name + "=\"" + " << info." + m._name;
                    }
                    else
                    {
                        text += " << \", " + m._name + "=\"" + " << info." + m._name;
                    }
                }
                text += ";" + LFCR;
                text += "    return stm;" + LFCR;
                text += "}" + LFCR;
            }
        }

    }
    text += LFCR + "#endif" + LFCR;

    return std::move(text);
}
