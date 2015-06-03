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

std::string GenCSharp::getRealType(const std::string & xmltype)
{
    if ( xmltype == "i8") return "Proto4z.i8";
    else if ( xmltype == "ui8") return "Proto4z.ui8";
    else if ( xmltype == "i16") return "Proto4z.i16";
    else if ( xmltype == "ui16") return "Proto4z.ui16";
    else if ( xmltype == "i32") return "Proto4z.i32";
    else if ( xmltype == "ui32") return "Proto4z.ui32";
    else if ( xmltype == "i64") return "Proto4z.i64";
    else if ( xmltype == "ui64") return "Proto4z.ui64";
    else if ( xmltype == "float") return "Proto4z.Float";
    else if ( xmltype == "double") return "Proto4z.Double";
    else if ( xmltype == "string") return "Proto4z.String";
    return xmltype;
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
            text += "        public static " + getRealType(info._const._type) + " value = " + info._const._value + "; " + LFCR;
            text += "    }" + LFCR;
        }
        else if (info._type == GT_DataArray)
        {
            text += LFCR + "    class " + info._array._arrayName + " : System.Collections.Generic.List<" + getRealType(info._array._type) + ">, Proto4z.IProtoObject ";
            if (!info._array._desc.empty())
            {
                text += "//" + info._array._desc;
            }
            text += LFCR;
            text += "    {" + LFCR;
            text += "        "   "public System.Collections.Generic.List<byte> __encode()" + LFCR;
            text += "        {" + LFCR;
            text += "            "   "var ret = new System.Collections.Generic.List<byte>();" + LFCR;
            text += "            "   " var len = new Proto4z.ui32((System.UInt32)this.Count);" + LFCR;
            text += "            "   "ret.AddRange(len.__encode());" + LFCR;
            text += "            "   "for (int i = 0; i < this.Count; i++ )" + LFCR;
            text += "            "   "{" + LFCR;
            text += "                "  "ret.AddRange(this[i].__encode());" + LFCR;
            text += "            "   "}" + LFCR;
            text += "            "   "return ret;" + LFCR;
            text += "        }" + LFCR;
            text += LFCR;

            text += "        "   "public int __decode(byte[] binData, ref int pos)" + LFCR;
            text += "        {" + LFCR;
            text += "            "   "var len = new Proto4z.ui32(0);" + LFCR;
            text += "            "   "len.__decode(binData, ref pos);" + LFCR;
            text += "            "  "if(len.val > 0)" + LFCR;
            text += "            "   "{" + LFCR;
            text += "                "  "for (int i=0; i<len.val; i++)" + LFCR;
            text += "                "   "{" + LFCR;
            text += "                    " "var data = new " + getRealType(info._array._type) + "();" + LFCR;
            text += "                    "  " data.__decode(binData, ref pos);" + LFCR;
            text += "                    "  "this.Add(data);" + LFCR;
            text += "                "   "}" + LFCR;
            text += "            "   "}" + LFCR;
            text += "            "   "return pos;" + LFCR;
            text += "        }" + LFCR;
            text += "    }" + LFCR;
        }
        else if (info._type == GT_DataMap)
        {
            text += LFCR + "    class " + info._map._mapName + " : System.Collections.Generic.Dictionary<" + getRealType(info._map._typeKey) + ", " + getRealType(info._map._typeValue) + ">, Proto4z.IProtoObject ";
            if (!info._array._desc.empty())
            {
                text += "//" + info._array._desc;
            }
            text += LFCR;
            text += "    {" + LFCR;
            text += "        "   "public System.Collections.Generic.List<byte> __encode()" + LFCR;
            text += "        {" +  LFCR;
            text += "            "  "var ret = new System.Collections.Generic.List<byte>();" + LFCR;
            text += "            "   " var len = new Proto4z.ui32((System.UInt32)this.Count);" + LFCR;
            text += "            "   "ret.AddRange(len.__encode());" + LFCR;
            text += "            "   "foreach(var kv in this)" + LFCR;
            text += "            "   "{" + LFCR;
            text += "                "   "ret.AddRange(kv.Key.__encode());" + LFCR;
            text += "                "   "ret.AddRange(kv.Value.__encode());" + LFCR;
            text += "            "   "}" + LFCR;
            text += "            "   "return ret;" + LFCR;
            text += "        }" +  LFCR;
            text += LFCR;

            text += "        "   "public int __decode(byte[] binData, ref int pos)" + LFCR;
            text += "        {" + LFCR;
            text += "            "   "var len = new Proto4z.ui32(0);" + LFCR;
            text += "            "   "len.__decode(binData, ref pos);" + LFCR;
            text += "            "   "if(len.val > 0)" + LFCR;
            text += "            "   "{" + LFCR;
            text += "                "   "for (int i=0; i<len.val; i++)" + LFCR;
            text += "                "   "{" + LFCR;
            text += "                    "   "var key = new " + getRealType(info._map._typeKey) + "();" + LFCR;
            text += "                    "   "var val = new " + getRealType(info._map._typeValue) + "();" + LFCR;
            text += "                    "    "key.__decode(binData, ref pos);" + LFCR;
            text += "                    "   "val.__decode(binData, ref pos);" + LFCR;
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
                text += "        " "static public Proto4z.ui16 getProtoID() { return new Proto4z.ui16(" + info._proto._const._value + "); }" + LFCR;
                text += "        " "static public string getProtoName() { return \"" + info._proto._struct._name + "\"; }" + LFCR;
            }

            for (const auto & m : info._proto._struct._members)
            {
                text += "        public " + getRealType(m._type) + " " + m._name + "; ";
                if (!m._desc.empty())
                {
                    text += "//" + m._desc;
                }
                text += LFCR;
            }

            //encode
            text += "        public System.Collections.Generic.List<byte> __encode()" + LFCR;
            text += "        {" + LFCR;
            text += "            "   "Proto4z.ui32 sttLen = 0;" + LFCR;
            text += "            "   "Proto4z.ui64 tag = " + boost::lexical_cast<std::string, unsigned long long>(info._proto._struct._tag) + ";" + LFCR;
            text += "            "   "" + LFCR;
            text += "            "   "var data = new System.Collections.Generic.List<byte>();" + LFCR;
            for (const auto &m : info._proto._struct._members)
            {
                if (m._tag == MT_DELETE)
                {
                    text += "//            "  "data.AddRange(" + m._name + ".__encode());//[already deleted]" + LFCR;
                }
                else
                {
                    text += "            "  "data.AddRange(" + m._name + ".__encode());" + LFCR;
                }
            }
            text += "            " "sttLen = (System.UInt32)data.Count + 8;" + LFCR;
            text += "            "  "var ret = new System.Collections.Generic.List<byte>();" + LFCR;
            text += "            "  "ret.AddRange(sttLen.__encode());" + LFCR;
            text += "            "  "ret.AddRange(tag.__encode());" + LFCR;
            text += "            "  "ret.AddRange(data);" + LFCR;
            text += "            "  "return ret;" + LFCR;
            text += "        }" + LFCR;

            //decode
            text += "        public int __decode(byte[] binData, ref int pos)" + LFCR;
            text += "        {" + LFCR;
            text += "            " "Proto4z.ui32 offset = 0;" + LFCR;
            text += "            " "Proto4z.ui64 tag = 0;" + LFCR;
            text += "            " "offset.__decode(binData, ref pos);" + LFCR;
            text += "            " "offset.val += (System.UInt32)pos;" + LFCR;
            text += "            " "tag.__decode(binData, ref pos);" + LFCR;
            int i = 0;
            for (const auto &m : info._proto._struct._members)
            {
                text += "            " + m._name + " = new " + getRealType(m._type) + "();" + LFCR;
                text += "            " "if ((tag.val & ((System.UInt64)1 << " + boost::lexical_cast<std::string, int>(i) + ")) != 0)" + LFCR;
                text += "            " "{" + LFCR;
                text += "                " + m._name + ".__decode(binData, ref pos);" + LFCR;
                text += "            " "}" + LFCR;
                i++;
            }
            text += "            return (int)offset.val;" + LFCR;
            text += "        }" + LFCR;
            text += "    }" + LFCR;
        }

    }
    text += LFCR;
    text += "}" + LFCR;
    text += LFCR + LFCR;

    return text;
}













