﻿
#ifndef _CRT_SECURE_NO_WARNINGS
#define  _CRT_SECURE_NO_WARNINGS
#endif
#include "genCSharp.h"
#include <time.h>
#include <algorithm>


CSharpType GenCSharp::getCSharpType(const std::string & xmltype)
{
    if (xmltype == "i8") return{ true, "char", "Proto4z.BaseProtoObject.encodeI8", "Proto4z.BaseProtoObject.decodeI8" };
    if (xmltype == "ui8") return{ true, "byte", "Proto4z.BaseProtoObject.encodeUI8", "Proto4z.BaseProtoObject.decodeUI8" };
    if (xmltype == "i16") return{ true, "short", "Proto4z.BaseProtoObject.encodeI16", "Proto4z.BaseProtoObject.decodeI16" };
    if (xmltype == "ui16") return{ true, "ushort", "Proto4z.BaseProtoObject.encodeUI16", "Proto4z.BaseProtoObject.decodeUI16" };
    if (xmltype == "i32") return{ true, "int", "Proto4z.BaseProtoObject.encodeI32", "Proto4z.BaseProtoObject.decodeI32" };
    if (xmltype == "ui32") return{ true, "uint", "Proto4z.BaseProtoObject.encodeUI32", "Proto4z.BaseProtoObject.decodeUI32" };
    if (xmltype == "i64") return{ true, "long", "Proto4z.BaseProtoObject.encodeI64", "Proto4z.BaseProtoObject.decodeI64" };
    if (xmltype == "ui64") return{ true, "ulong", "Proto4z.BaseProtoObject.encodeUI64", "Proto4z.BaseProtoObject.decodeUI64" };
    if (xmltype == "float") return{ true, "float", "Proto4z.BaseProtoObject.encodeSingle", "Proto4z.BaseProtoObject.decodeSingle" };
    if (xmltype == "double") return{ true, "double", "Proto4z.BaseProtoObject.encodeDouble", "Proto4z.BaseProtoObject.decodeDouble" };
    if (xmltype == "string") return{ true, "string", "Proto4z.BaseProtoObject.encodeString", "Proto4z.BaseProtoObject.decodeString" };

    if (xmltype == "s8") return{ true, "char", "Proto4z.BaseProtoObject.encodeI8", "Proto4z.BaseProtoObject.decodeI8" };
    if (xmltype == "u8") return{ true, "byte", "Proto4z.BaseProtoObject.encodeUI8", "Proto4z.BaseProtoObject.decodeUI8" };
    if (xmltype == "s16") return{ true, "short", "Proto4z.BaseProtoObject.encodeI16", "Proto4z.BaseProtoObject.decodeI16" };
    if (xmltype == "u16") return{ true, "ushort", "Proto4z.BaseProtoObject.encodeUI16", "Proto4z.BaseProtoObject.decodeUI16" };
    if (xmltype == "s32") return{ true, "int", "Proto4z.BaseProtoObject.encodeI32", "Proto4z.BaseProtoObject.decodeI32" };
    if (xmltype == "u32") return{ true, "uint", "Proto4z.BaseProtoObject.encodeUI32", "Proto4z.BaseProtoObject.decodeUI32" };
    if (xmltype == "s64") return{ true, "long", "Proto4z.BaseProtoObject.encodeI64", "Proto4z.BaseProtoObject.decodeI64" };
    if (xmltype == "u64") return{ true, "ulong", "Proto4z.BaseProtoObject.encodeUI64", "Proto4z.BaseProtoObject.decodeUI64" };
    if (xmltype == "f32") return{ true, "float", "Proto4z.BaseProtoObject.encodeSingle", "Proto4z.BaseProtoObject.decodeSingle" };


    return{ false, xmltype, "", "" };
}

std::string  GenCSharp::getTypeDefault(const std::string & xmltype)
{
    if (xmltype == "i8") return "'\\0'";
    else if (xmltype == "float") return "0.0f";
    else if (xmltype == "string") return "\"\"";
    else if (xmltype == "s8") return "'\\0'";
    else if (xmltype == "f32") return "0.0f";
    return GenBase::getTypeDefault(xmltype);
}

std::string GenCSharp::genRealContent(const std::list<AnyData> & stores)
{
    std::string text = LFCR;

    {
        char buf[100];
        time_t now = time(NULL);
        tm *ptm = localtime(&now);
        sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", ptm->tm_year + 1990, ptm->tm_mon + 1, ptm->tm_yday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    }

    text += "namespace Proto4z " + LFCR + "{" + LFCR;

    for (auto &info : stores)
    {
        if (info._type == GT_DataConstValue)
        {
            text += LFCR;
            text += genDataConst(info._const);
        }
        if (info._type == GT_DataEnum)
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
    text += LFCR;
    text += "}" + LFCR;
    text += LFCR + LFCR;

    return text;
}

std::string GenCSharp::genDataConst(const DataConstValue & dc)
{
    std::string text;
    text += "    public class " + dc._name + " ";
    if (!dc._desc.empty())
    {
        text += "//" + dc._desc + " ";
    }
    text += LFCR;
    text += "    {" + LFCR;
    text += "        public const " + getCSharpType(dc._type).realType + " value = " + dc._value + "; " + LFCR;
    text += "    }" + LFCR;
    return text;
}

std::string GenCSharp::genDataEnum(const DataEnum & de)
{
    std::string text;
    text += "    public enum " + de._name + " : " + getCSharpType(de._type).realType + LFCR;
    text += "    {" + LFCR;
    for (auto m : de._members)
    {
        text += "        " + m._name + " = " + m._value + ", ";
        if (!m._desc.empty())
        {
            text += "//" + m._desc + " ";
        }
        text += LFCR;
    }
    text += "    };" + LFCR;
    return text;
}

std::string GenCSharp::genDataArray(const DataArray & da)
{
    std::string text;
    text += LFCR + "    public class " + da._arrayName + " : System.Collections.Generic.List<" + getCSharpType(da._type).realType + ">, Proto4z.IProtoObject ";
    if (!da._desc.empty())
    {
        text += "//" + da._desc + " ";
    }
    text += LFCR;
    text += "    {" + LFCR;
    text += "        "   "public System.Collections.Generic.List<byte> __encode()" + LFCR;
    text += "        {" + LFCR;
    text += "            "   "var ret = new System.Collections.Generic.List<byte>();" + LFCR;
    text += "            "   "int len = (int)this.Count;" + LFCR;
    text += "            "   "ret.AddRange(Proto4z.BaseProtoObject.encodeI32(len));" + LFCR;
    text += "            "   "for (int i = 0; i < this.Count; i++ )" + LFCR;
    text += "            "   "{" + LFCR;
    if (getCSharpType(da._type).isBase)
    {
        text += "                "  "ret.AddRange(" + getCSharpType(da._type).baseEncode + "(this[i])); " + LFCR;
    }
    else
    {
        text += "                "  "ret.AddRange(this[i].__encode());" + LFCR;
    }
    text += "            "   "}" + LFCR;
    text += "            "   "return ret;" + LFCR;
    text += "        }" + LFCR;
    text += LFCR;

    text += "        "   "public int __decode(byte[] binData, ref int pos)" + LFCR;
    text += "        {" + LFCR;
    text += "            "   "int len = Proto4z.BaseProtoObject.decodeI32(binData, ref pos);" + LFCR;
    text += "            "  "if(len > 0)" + LFCR;
    text += "            "   "{" + LFCR;
    text += "                "  "for (int i=0; i<len; i++)" + LFCR;
    text += "                "   "{" + LFCR;
    if (getCSharpType(da._type).isBase)
    {
        text += "                    "  "this.Add(" + getCSharpType(da._type).baseDecode + "(binData, ref pos));" + LFCR;
    }
    else
    {
        text += "                    "  "var data = new " + getCSharpType(da._type).realType + "();" + LFCR;
        text += "                    "  "data.__decode(binData, ref pos);" + LFCR;
        text += "                    "  "this.Add(data);" + LFCR;
    }
    text += "                "   "}" + LFCR;
    text += "            "   "}" + LFCR;
    text += "            "   "return pos;" + LFCR;
    text += "        }" + LFCR;
    text += "    }" + LFCR;
    return text;
}


std::string GenCSharp::genDataMap(const DataMap & dm)
{
    std::string text;
    text += LFCR + "    public class " + dm._mapName + " : System.Collections.Generic.Dictionary<" + getCSharpType(dm._typeKey).realType + ", " + getCSharpType(dm._typeValue).realType + ">, Proto4z.IProtoObject ";
    if (!dm._desc.empty())
    {
        text += "//" + dm._desc + " ";
    }
    text += LFCR;
    text += "    {" + LFCR;
    text += "        "   "public System.Collections.Generic.List<byte> __encode()" + LFCR;
    text += "        {" + LFCR;
    text += "            "   "var ret = new System.Collections.Generic.List<byte>();" + LFCR;
    text += "            "   "int len = (int)this.Count;" + LFCR;
    text += "            "   "ret.AddRange(Proto4z.BaseProtoObject.encodeI32(len));" + LFCR;
    text += "            "   "foreach(var kv in this)" + LFCR;
    text += "            "   "{" + LFCR;
    if (getCSharpType(dm._typeKey).isBase)
    {
        text += "                "   "ret.AddRange(" + getCSharpType(dm._typeKey).baseEncode + "(kv.Key));" + LFCR;
    }
    else
    {
        text += "                "   "ret.AddRange(kv.Key.__encode());" + LFCR;
    }
    if (getCSharpType(dm._typeValue).isBase)
    {
        text += "                "   "ret.AddRange(" + getCSharpType(dm._typeValue).baseEncode + "(kv.Value));" + LFCR;
    }
    else
    {
        text += "                "   "ret.AddRange(kv.Value.__encode());" + LFCR;
    }
    text += "            "   "}" + LFCR;
    text += "            "   "return ret;" + LFCR;
    text += "        }" + LFCR;
    text += LFCR;

    text += "        "   "public int __decode(byte[] binData, ref int pos)" + LFCR;
    text += "        {" + LFCR;
    text += "            "   "int len = Proto4z.BaseProtoObject.decodeI32(binData, ref pos);" + LFCR;
    text += "            "   "if(len > 0)" + LFCR;
    text += "            "   "{" + LFCR;
    text += "                "   "for (int i=0; i<len; i++)" + LFCR;
    text += "                "   "{" + LFCR;
    if (getCSharpType(dm._typeKey).isBase)
    {
        text += "                    "   "var key = " + getCSharpType(dm._typeKey).baseDecode + "(binData, ref pos);" + LFCR;
    }
    else
    {
        text += "                    "   "var key = new " + getCSharpType(dm._typeKey).realType + "();" + LFCR;
        text += "                    "    "key.__decode(binData, ref pos);" + LFCR;
    }

    if (getCSharpType(dm._typeValue).isBase)
    {
        text += "                    "   "var val = " + getCSharpType(dm._typeValue).baseDecode + "(binData, ref pos);" + LFCR;
    }
    else
    {
        text += "                    "   "var val = new " + getCSharpType(dm._typeValue).realType + "();" + LFCR;
        text += "                    "    "val.__decode(binData, ref pos);" + LFCR;
    }
    text += "                    "   "this.Add(key, val);" + LFCR;
    text += "                "   "}" + LFCR;
    text += "            "   "}" + LFCR;
    text += "            "   "return pos;" + LFCR;
    text += "        }" + LFCR;
    text += "    }" + LFCR;
    return text;
}


std::string GenCSharp::genDataPacket(const DataPacket & dp)
{
    std::string text;
    text += "    public class " + dp._struct._name + ": Proto4z.IProtoObject";
    if (!dp._struct._desc.empty())
    {
        text += " //" + dp._struct._desc + " ";
    }
    text += LFCR;
    text += "    {    " + LFCR;

    //write ProtoID

    text += "        " "//proto id  " + LFCR;
    text += "        " "public const ushort protoID = " + dp._const._value + "; " + LFCR;
    text += "        " "static public ushort getProtoID() { return " + dp._const._value + "; }" + LFCR;
    text += "        " "static public string getProtoName() { return \"" + dp._struct._name + "\"; }" + LFCR;


    //members
    text += "        " "//members  " + LFCR;
    for (const auto & m : dp._struct._members)
    {
        text += "        public " + getCSharpType(m._type).realType + " " + m._name + "; ";
        if (!m._desc.empty())
        {
            text += "//" + m._desc + " ";
        }
        text += LFCR;
    }
    //default struct ()
    text += "        public " + dp._struct._name + "() " + LFCR;
    text += "        {" + LFCR;
    for (const auto & m : dp._struct._members)
    {
        text += "            " + m._name + " = ";
        if (getCSharpType(m._type).isBase)
        {
            text += getTypeDefault(m._type);
        }
        else
        {
            text += "new " + getCSharpType(m._type).realType + "()";
        }
        text += "; " + LFCR;
    }
    text += "        }" + LFCR;

    //default struct (....) 
    if (!dp._struct._members.empty())
    {
        text += "        public " + dp._struct._name + "(";
        for (size_t i = 0; i < dp._struct._members.size(); i ++)
        {
            const auto &m = dp._struct._members[i];
            if (i > 0) text += ", ";
            text += "" + getCSharpType(m._type).realType + " " + m._name;
        }
        text += ")" + LFCR;
        text += "        {" + LFCR;
        for (const auto & m : dp._struct._members)
        {
            text += "            this." + m._name + " = " + m._name + ";" + LFCR;
        }
        text += "        }" + LFCR;
    }


    //encode
    text += "        public System.Collections.Generic.List<byte> __encode()" + LFCR;
    text += "        {" + LFCR;
    text += "            "   "var data = new System.Collections.Generic.List<byte>();" + LFCR;
    for (const auto &m : dp._struct._members)
    {
        //null
        if (!getCSharpType(m._type).isBase)
        {
            text += "            if (this." + m._name + " == null) this." + m._name + " = new " + m._type + "();" + LFCR;
        }


        //encode
        if (getCSharpType(m._type).isBase)
        {
            text += "            "  "data.AddRange(" + getCSharpType(m._type).baseEncode + "(this." + m._name + "));" + LFCR;
        }
        else
        {
            text += "            "  "data.AddRange(this." + m._name + ".__encode());" + LFCR;
        }
    }
    text += "            "  "return data;" + LFCR;
    text += "        }" + LFCR;

    //decode
    text += "        public int __decode(byte[] binData, ref int pos)" + LFCR;
    text += "        {" + LFCR;
    for (const auto &m : dp._struct._members)
    {
        if (getCSharpType(m._type).isBase)
        {
            text += "            this." + m._name + " = " + getCSharpType(m._type).baseDecode + "(binData, ref pos);" + LFCR;
        }
        else
        {
            text += "            this." + m._name + " = new " + getCSharpType(m._type).realType + "();" + LFCR;
            text += "            this." + m._name + ".__decode(binData, ref pos);" + LFCR;
        }
    }
    text += "            return pos;" + LFCR;
    text += "        }" + LFCR;
    text += "    }" + LFCR;
    return text;
}


void writeCSharpReflection(std::map<unsigned short, std::string> & keys, std::map<unsigned short, std::string> & errCodes)
{
    std::string trustName = SupportLanguageFilePath[SL_CSHARP];
    trustName += "/ProtoReflection";
    trustName += SupportLanguageFileSuffix[SL_CSHARP];
    std::string content;
    content += R"---OOO(
namespace Proto4z
{
    static class Reflection
    {
        public static ushort getProtoID(string protoName)
        {
            try
            {
                return _nameToID[protoName];
            }
            catch (System.Exception)
            {
            }
            return 0;
        }
        public static string getProtoName(ushort protoID)
        {
            try
            {
                return _idToName[protoID];
            }
            catch (System.Exception)
            {
            }
            return "UnknownProtoID_" + protoID;
        }
        public static string getErrorDesc(ushort errCode)
        {
            try
            {
                return _errCodes[errCode];
            }
            catch (System.Exception)
            {
            }
            return "UnknownErrorCode_" + errCode;
        }
        static System.Collections.Generic.Dictionary<string, ushort> _nameToID = new System.Collections.Generic.Dictionary<string, ushort>();
        static System.Collections.Generic.Dictionary<ushort, string> _idToName = new System.Collections.Generic.Dictionary<ushort, string>();
        static System.Collections.Generic.Dictionary<ushort, string> _errCodes = new System.Collections.Generic.Dictionary<ushort, string>();
        static Reflection()
        {
        

                    )---OOO";


    content += "\r\n";
    for (auto & kv : keys)
    {
        content += "            _nameToID.Add(\"" + kv.second + "\", " + toString(kv.first) + ");\r\n";
        content += "            _idToName.Add(" + toString(kv.first) + ", \"" + kv.second + "\");\r\n";
    }
    for (auto & kv : errCodes)
    {
        content += "            _errCodes.Add(" + toString(kv.first) + ", \"" + kv.second + "\");\r\n";
    }

    content += R"---OOO(
        }
    }
}


)---OOO";



    if (writeFileContent(trustName, content.c_str(), content.length(), false) != content.length())
    {
        E("writeCSharpReflection open file Error ");
    }
}









