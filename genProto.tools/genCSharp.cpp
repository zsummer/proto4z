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

#ifndef _CRT_SECURE_NO_WARNINGS
#define  _CRT_SECURE_NO_WARNINGS
#endif
#include "genCSharp.h"
#include <time.h>
#include <algorithm>


CSharpType GenCSharp::getCSharpType(const std::string & xmltype)
{
    if (xmltype == "i8") return{ true, "System.Char", "Proto4z.BaseProtoObject.encodeI8", "Proto4z.BaseProtoObject.decodeI8" };
    if (xmltype == "ui8") return{ true, "System.Byte", "Proto4z.BaseProtoObject.encodeUI8", "Proto4z.BaseProtoObject.decodeUI8" };
    if (xmltype == "i16") return{ true, "System.Int16", "Proto4z.BaseProtoObject.encodeI16", "Proto4z.BaseProtoObject.decodeI16" };
    if (xmltype == "ui16") return{ true, "System.UInt16", "Proto4z.BaseProtoObject.encodeUI16", "Proto4z.BaseProtoObject.decodeUI16" };
    if (xmltype == "i32") return{ true, "System.Int32", "Proto4z.BaseProtoObject.encodeI32", "Proto4z.BaseProtoObject.decodeI32" };
    if (xmltype == "ui32") return{ true, "System.UInt32", "Proto4z.BaseProtoObject.encodeUI32", "Proto4z.BaseProtoObject.decodeUI32" };
    if (xmltype == "i64") return{ true, "System.Int64", "Proto4z.BaseProtoObject.encodeI64", "Proto4z.BaseProtoObject.decodeI64" };
    if (xmltype == "ui64") return{ true, "System.UInt64", "Proto4z.BaseProtoObject.encodeUI64", "Proto4z.BaseProtoObject.decodeUI64" };
    if (xmltype == "float") return{ true, "System.Single", "Proto4z.BaseProtoObject.encodeSingle", "Proto4z.BaseProtoObject.decodeSingle" };
    if (xmltype == "double") return{ true, "System.Double", "Proto4z.BaseProtoObject.encodeDouble", "Proto4z.BaseProtoObject.decodeDouble" };
    if (xmltype == "string") return{ true, "System.String", "Proto4z.BaseProtoObject.encodeString", "Proto4z.BaseProtoObject.decodeString" };

    return{ false, xmltype, "", "" };
}

std::string  GenCSharp::getTypeDefault(const std::string & xmltype)
{
    if (xmltype == "i8") return "'\\0'";
    else if (xmltype == "float") return "0.0F";
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
            text += "    class STATIC_" + info._const._name + " ";
            if (!info._const._desc.empty())
            {
                text += "//" + info._const._desc;
            }
            text += LFCR;
            text += "    {" + LFCR;
            text += "        public static " + getCSharpType(info._const._type).realType + " value = " + info._const._value + "; " + LFCR;
            text += "    }" + LFCR;
        }
        else if (info._type == GT_DataArray)
        {
            text += LFCR + "    class " + info._array._arrayName + " : System.Collections.Generic.List<" + getCSharpType(info._array._type).realType + ">, Proto4z.IProtoObject ";
            if (!info._array._desc.empty())
            {
                text += "//" + info._array._desc;
            }
            text += LFCR;
            text += "    {" + LFCR;
            text += "        "   "public System.Collections.Generic.List<byte> __encode()" + LFCR;
            text += "        {" + LFCR;
            text += "            "   "var ret = new System.Collections.Generic.List<byte>();" + LFCR;
            text += "            "   "System.Int32 len = (System.Int32)this.Count;" + LFCR;
            text += "            "   "ret.AddRange(Proto4z.BaseProtoObject.encodeI32(len));" + LFCR;
            text += "            "   "for (int i = 0; i < this.Count; i++ )" + LFCR;
            text += "            "   "{" + LFCR;
            if (getCSharpType(info._array._type).isBase)
            {
                text += "                "  "ret.AddRange(" + getCSharpType(info._array._type).baseEncode + "(this[i])); " + LFCR;
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
            text += "            "   "System.Int32 len = Proto4z.BaseProtoObject.decodeI32(binData, ref pos);" + LFCR;
            text += "            "  "if(len > 0)" + LFCR;
            text += "            "   "{" + LFCR;
            text += "                "  "for (int i=0; i<len; i++)" + LFCR;
            text += "                "   "{" + LFCR;
            if (getCSharpType(info._array._type).isBase)
            {
                text += "                    "  "this.Add(" + getCSharpType(info._array._type).baseDecode + "(binData, ref pos));" + LFCR;
            }
            else
            {
                text += "                    "  "var data = new " + getCSharpType(info._array._type).realType + "();" + LFCR;
                text += "                    "  "data.__decode(binData, ref pos);" + LFCR;
                text += "                    "  "this.Add(data);" + LFCR;
            }
            text += "                "   "}" + LFCR;
            text += "            "   "}" + LFCR;
            text += "            "   "return pos;" + LFCR;
            text += "        }" + LFCR;
            text += "    }" + LFCR;
        }
        else if (info._type == GT_DataMap)
        {
            text += LFCR + "    class " + info._map._mapName + " : System.Collections.Generic.Dictionary<" + getCSharpType(info._map._typeKey).realType + ", " + getCSharpType(info._map._typeValue).realType + ">, Proto4z.IProtoObject ";
            if (!info._array._desc.empty())
            {
                text += "//" + info._array._desc;
            }
            text += LFCR;
            text += "    {" + LFCR;
            text += "        "   "public System.Collections.Generic.List<byte> __encode()" + LFCR;
            text += "        {" +  LFCR;
            text += "            "   "var ret = new System.Collections.Generic.List<byte>();" + LFCR;
            text += "            "   "System.Int32 len = (System.Int32)this.Count;" + LFCR;
            text += "            "   "ret.AddRange(Proto4z.BaseProtoObject.encodeI32(len));" + LFCR;
            text += "            "   "foreach(var kv in this)" + LFCR;
            text += "            "   "{" + LFCR;
            if (getCSharpType(info._map._typeKey).isBase)
            {
                text += "                "   "ret.AddRange(" + getCSharpType(info._map._typeKey).baseEncode + "(kv.Key));" + LFCR;
            }
            else
            {
                text += "                "   "ret.AddRange(kv.Key.__encode());" + LFCR;
            }
            if (getCSharpType(info._map._typeValue).isBase)
            {
                text += "                "   "ret.AddRange(" + getCSharpType(info._map._typeValue).baseEncode + "(kv.Value));" + LFCR;
            }
            else
            {
                text += "                "   "ret.AddRange(kv.Value.__encode());" + LFCR;
            }
            text += "            "   "}" + LFCR;
            text += "            "   "return ret;" + LFCR;
            text += "        }" +  LFCR;
            text += LFCR;

            text += "        "   "public int __decode(byte[] binData, ref int pos)" + LFCR;
            text += "        {" + LFCR;
            text += "            "   "System.Int32 len = Proto4z.BaseProtoObject.decodeI32(binData, ref pos);" + LFCR;
            text += "            "   "if(len > 0)" + LFCR;
            text += "            "   "{" + LFCR;
            text += "                "   "for (int i=0; i<len; i++)" + LFCR;
            text += "                "   "{" + LFCR;
            if (getCSharpType(info._map._typeKey).isBase)
            {
                text += "                    "   "var key = " + getCSharpType(info._map._typeKey).baseDecode + "(binData, ref pos);" + LFCR;
            }
            else
            {
                text += "                    "   "var key = new " + getCSharpType(info._map._typeKey).realType + "();" + LFCR;
                text += "                    "    "key.__decode(binData, ref pos);" + LFCR;
            }
           
            if (getCSharpType(info._map._typeValue).isBase)
            {
                text += "                    "   "var val = " + getCSharpType(info._map._typeValue).baseDecode + "(binData, ref pos);" + LFCR;
            }
            else
            {
                text += "                    "   "var val = new " + getCSharpType(info._map._typeValue).realType + "();" + LFCR;
                text += "                    "    "val.__decode(binData, ref pos);" + LFCR;
            }
            text += "                    "   "this.Add(key, val);" + LFCR;
            text += "                "   "}" + LFCR;
            text += "            "   "}" + LFCR;
            text += "            "   "return pos;" + LFCR;
            text += "        }" + LFCR;
            text += "    }" + LFCR;

        }
        else if (info._type == GT_DataStruct || info._type == GT_DataProto)
        {
            text += LFCR;
            text += "    class " + info._proto._struct._name + ": Proto4z.IProtoObject";
            if (!info._proto._struct._desc.empty())
            {
                text += " //" + info._proto._struct._desc;
            }
            text += LFCR;
            text += "    {    " + LFCR;

            //write ProtoID
            if (info._type == GT_DataProto)
            {
                text += "        " "static public System.UInt16 getProtoID() { return " + info._proto._const._value + "; }" + LFCR;
                text += "        " "static public string getProtoName() { return \"" + info._proto._struct._name + "\"; }" + LFCR;
            }

            for (const auto & m : info._proto._struct._members)
            {
                text += "        public " + getCSharpType(m._type).realType + " " + m._name + "; ";
                if (!m._desc.empty())
                {
                    text += "//" + m._desc;
                }
                text += LFCR;
            }

            //encode
            text += "        public System.Collections.Generic.List<byte> __encode()" + LFCR;
            text += "        {" + LFCR;
#ifdef __WITH_TAG
            text += "            "   "System.Int32 sttLen = 0;" + LFCR;
            text += "            "   "System.UInt64 tag = " + boost::lexical_cast<std::string, unsigned long long>(info._proto._struct._tag) + ";" + LFCR;
            text += "            "   "" + LFCR;
#endif
            text += "            "   "var data = new System.Collections.Generic.List<byte>();" + LFCR;
            for (const auto &m : info._proto._struct._members)
            {
                if (m._tag == MT_DELETE)
                {
                    text += "//";
                }
                if (getCSharpType(m._type).isBase)
                {
                    text += "            "  "data.AddRange(" + getCSharpType(m._type).baseEncode + "(" + m._name + "));" + LFCR;
                }
                else
                {
                    text += "            "  "data.AddRange(" + m._name + ".__encode());" + LFCR;
                }
            }
#ifdef __WITH_TAG
            text += "            " "sttLen = data.Count + 8;" + LFCR;
#endif
            text += "            "  "var ret = new System.Collections.Generic.List<byte>();" + LFCR;
#ifdef __WITH_TAG
            text += "            "  "ret.AddRange(Proto4z.BaseProtoObject.encodeI32(sttLen));" + LFCR;
            text += "            "  "ret.AddRange(Proto4z.BaseProtoObject.encodeUI64(tag));" + LFCR;
#endif
            text += "            "  "ret.AddRange(data);" + LFCR;
            text += "            "  "return ret;" + LFCR;
            text += "        }" + LFCR;

            //decode
            text += "        public int __decode(byte[] binData, ref int pos)" + LFCR;
            text += "        {" + LFCR;
#ifdef __WITH_TAG
            text += "            " "System.Int32 offset = Proto4z.BaseProtoObject.decodeI32(binData, ref pos);" + LFCR;
            text += "            " "offset += pos;" + LFCR;
            text += "            " "System.UInt64 tag = Proto4z.BaseProtoObject.decodeUI64(binData, ref pos);" + LFCR;
#endif
            int i = 0;
            for (const auto &m : info._proto._struct._members)
            {
                if (getCSharpType(m._type).isBase)
                {
                    if (!getTypeDefault(m._type).empty())
                    {
                        text += "            " + m._name + " = " + getTypeDefault(m._type) + ";" + LFCR;
                    }
                }
                else
                {
                    text += "            " + m._name + " = new " + getCSharpType(m._type).realType + "();" + LFCR;
                }
#ifdef __WITH_TAG
                text += "            " "if ((tag & ((System.UInt64)1 << " + boost::lexical_cast<std::string, int>(i) + ")) != 0)" + LFCR;
                text += "            " "{" + LFCR;
#endif
                if (getCSharpType(m._type).isBase)
                {
                    text += "                " + m._name + " = " + getCSharpType(m._type).baseDecode + "(binData, ref pos);" + LFCR;
                }
                else
                {
                    text += "                " + m._name + ".__decode(binData, ref pos);" + LFCR;
                }
#ifdef __WITH_TAG
                text += "            " "}" + LFCR;
#endif
                i++;
            }
#ifdef __WITH_TAG
            text += "            pos = offset;" + LFCR;
#endif
            text += "            return pos;" + LFCR;
            text += "        }" + LFCR;
            text += "    }" + LFCR;
        }

    }
    text += LFCR;
    text += "}" + LFCR;
    text += LFCR + LFCR;

    return text;
}













