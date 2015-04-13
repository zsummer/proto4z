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
 * Copyright (C) 2014-2015 YaweiZhang <yawei_zhang@foxmail.com>.
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
#include "genProto.h"
#include <time.h>
#include <algorithm>


std::string getCSharpType(std::string type)
{
	auto founder = xmlTypeToCSharpType.find(type);
	if (founder != xmlTypeToCSharpType.end() && !founder->second.empty())
	{
		type = founder->second;
	}
	else
	{
		type = std::string("Proto4z.") + type;
	}
	return type;
}

bool genCSharpFile(std::string path, std::string filename, std::string attr, std::vector<StoreInfo> & stores)
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
			text += "\tclass STATIC_" + info._const._name + " ";
			if (!info._const._desc.empty())
			{
				text += "//" + info._const._desc;
			}
			text += LFCR;
			text += "\t{" + LFCR;
			text += "\t\tpublic static " + getCSharpType(info._const._type) + " value = " + info._const._value + "; " + LFCR;
			text += "\t}" + LFCR;
		}
		else if (info._type == GT_DataArray)
		{
			text += LFCR + "\tclass " + info._array._arrayName + " : System.Collections.Generic.List<" + getCSharpType(info._array._type) + ">, Proto4z.IProtoObject ";
			if (!info._array._desc.empty())
			{
				text += "//" + info._array._desc;
			}
			text += LFCR;
			text += "\t{" + LFCR;
			text += "\t\t"   "public System.Collections.Generic.List<byte> __encode()" + LFCR;
			text += "\t\t{" + LFCR;
			text += "\t\t\t"   "var ret = new System.Collections.Generic.List<byte>();" + LFCR;
			text += "\t\t\t"   " var len = new Proto4z.ui32((System.UInt32)this.Count);" + LFCR;
			text += "\t\t\t"   "ret.AddRange(len.__encode());" + LFCR;
			text += "\t\t\t"   "for (int i = 0; i < this.Count; i++ )" + LFCR;
			text += "\t\t\t"   "{" + LFCR;
			text += "\t\t\t\t"  "ret.AddRange(this[i].__encode());" + LFCR;
			text += "\t\t\t"   "}" + LFCR;
			text += "\t\t\t"   "return ret;" + LFCR;
			text += "\t\t}" + LFCR;
			text += LFCR;

			text += "\t\t"   "public int __decode(byte[] binData, ref int pos)" + LFCR;
			text += "\t\t{" + LFCR;
			text += "\t\t\t"   "var len = new Proto4z.ui32(0);" + LFCR;
			text += "\t\t\t"   "len.__decode(binData, ref pos);" + LFCR;
			text += "\t\t\t"  "if(len.val > 0)" + LFCR;
			text += "\t\t\t"   "{" + LFCR;
			text += "\t\t\t\t"  "for (int i=0; i<len.val; i++)" + LFCR;
			text += "\t\t\t\t"   "{" + LFCR;
			text += "\t\t\t\t\t" "var data = new " + getCSharpType(info._array._type) + "();" + LFCR;
			text += "\t\t\t\t\t"  " data.__decode(binData, ref pos);" + LFCR;
			text += "\t\t\t\t\t"  "this.Add(data);" + LFCR;
			text += "\t\t\t\t"   "}" + LFCR;
			text += "\t\t\t"   "}" + LFCR;
			text += "\t\t\t"   "return pos;" + LFCR;
			text += "\t\t}" + LFCR;
			text += "\t}" + LFCR;
		}
		else if (info._type == GT_DataMap)
		{
			text += LFCR + "\tclass " + info._map._mapName + " : System.Collections.Generic.Dictionary<" + getCSharpType(info._map._typeKey) + ", " + getCSharpType(info._map._typeValue)  + ">, Proto4z.IProtoObject ";
			if (!info._array._desc.empty())
			{
				text += "//" + info._array._desc;
			}
			text += LFCR;
			text += "\t{" + LFCR;
			text += "\t\t"   "public System.Collections.Generic.List<byte> __encode()" + LFCR;
			text += "\t\t{" +  LFCR;
			text += "\t\t\t"  "var ret = new System.Collections.Generic.List<byte>();" + LFCR;
			text += "\t\t\t"   " var len = new Proto4z.ui32((System.UInt32)this.Count);" + LFCR;
			text += "\t\t\t"   "ret.AddRange(len.__encode());" + LFCR;
			text += "\t\t\t"   "foreach(var kv in this)" + LFCR;
			text += "\t\t\t"   "{" + LFCR;
			text += "\t\t\t\t"   "ret.AddRange(kv.Key.__encode());" + LFCR;
			text += "\t\t\t\t"   "ret.AddRange(kv.Value.__encode());" + LFCR;
			text += "\t\t\t"   "}" + LFCR;
			text += "\t\t\t"   "return ret;" + LFCR;
			text += "\t\t}" +  LFCR;
			text += LFCR;

			text += "\t\t"   "public int __decode(byte[] binData, ref int pos)" + LFCR;
			text += "\t\t{" + LFCR;
			text += "\t\t\t"   "var len = new Proto4z.ui32(0);" + LFCR;
			text += "\t\t\t"   "len.__decode(binData, ref pos);" + LFCR;
			text += "\t\t\t"   "if(len.val > 0)" + LFCR;
			text += "\t\t\t"   "{" + LFCR;
			text += "\t\t\t\t"   "for (int i=0; i<len.val; i++)" + LFCR;
			text += "\t\t\t\t"   "{" + LFCR;
			text += "\t\t\t\t\t"   "var key = new " + getCSharpType(info._map._typeKey) + "();" + LFCR;
			text += "\t\t\t\t\t"   "var val = new " + getCSharpType(info._map._typeValue) + "();" + LFCR;
			text += "\t\t\t\t\t"    "key.__decode(binData, ref pos);" + LFCR;
			text += "\t\t\t\t\t"   "val.__decode(binData, ref pos);" + LFCR;
			text += "\t\t\t\t\t"   "this.Add(key, val);" + LFCR;
			text += "\t\t\t\t"   "}" + LFCR;
			text += "\t\t\t"   "}" + LFCR;
			text += "\t\t\t"   "return pos;" + LFCR;
			text += "\t\t}" + LFCR;
			text += "\t}" + LFCR;

		}
		else if (info._type == GT_DataStruct || info._type == GT_DataProto)
		{
			text += LFCR;
			text += "\tclass " + info._proto._struct._name + ": Proto4z.IProtoObject";
			if (!info._proto._struct._desc.empty())
			{
				text += " //" + info._proto._struct._desc;
			}
			text += LFCR;
			text += "\t{\t" + LFCR;

			//write ProtoID
			if (info._type == GT_DataProto)
			{
				text += "\t\t" "static public Proto4z.ui16 getProtoID() { return new Proto4z.ui16(" + info._proto._const._value + "); }" + LFCR;
				text += "\t\t" "static public string getProtoName() { return \"" + info._proto._struct._name + "\"; }" + LFCR;
			}

			info._proto._struct._tag = 0;
			int curTagIndex = 0;
			for (const auto & m : info._proto._struct._members)
			{
				{
					if (!m._isDel)
					{
						info._proto._struct._tag |= (1ULL << curTagIndex);
					}
					curTagIndex++;
				}
				text += "\t\tpublic " + getCSharpType(m._type) + " " + m._name + "; ";
				if (!m._desc.empty())
				{
					text += "//" + m._desc;
				}
				text += LFCR;
			}



			//encode
			text += "\t\tpublic System.Collections.Generic.List<byte> __encode()" + LFCR;
			text += "\t\t{" + LFCR;
			text += "\t\t\t"   "Proto4z.ui32 sttLen = 0;" + LFCR;
			text += "\t\t\t"   "Proto4z.ui64 tag = " + boost::lexical_cast<std::string, unsigned long long>(info._proto._struct._tag) + ";" + LFCR;
			text += "\t\t\t"   "" + LFCR;
			text += "\t\t\t"   "var data = new System.Collections.Generic.List<byte>();" + LFCR;
			for (const auto &m : info._proto._struct._members)
			{
				if (m._isDel)
				{
					text += "//\t\t\t"  "data.AddRange(" + m._name + ".__encode());//[already deleted]" + LFCR;
				}
				else
				{
					text += "\t\t\t"  "data.AddRange(" + m._name + ".__encode());" + LFCR;
				}
			}
			text += "\t\t\t" "sttLen = (System.UInt32)data.Count + 8;" + LFCR;
			text += "\t\t\t"  "var ret = new System.Collections.Generic.List<byte>();" + LFCR;
			text += "\t\t\t"  "ret.AddRange(sttLen.__encode());" + LFCR;
			text += "\t\t\t"  "ret.AddRange(tag.__encode());" + LFCR;
			text += "\t\t\t"  "ret.AddRange(data);" + LFCR;
			text += "\t\t\t"  "return ret;" + LFCR;
			text += "\t\t}" + LFCR;

			//decode
			text += "\t\tpublic int __decode(byte[] binData, ref int pos)" + LFCR;
			text += "\t\t{" + LFCR;
			text += "\t\t\t" "Proto4z.ui32 offset = 0;" + LFCR;
			text += "\t\t\t" "Proto4z.ui64 tag = 0;" + LFCR;
			text += "\t\t\t" "offset.__decode(binData, ref pos);" + LFCR;
			text += "\t\t\t" "offset.val += (System.UInt32)pos;" + LFCR;
			text += "\t\t\t" "tag.__decode(binData, ref pos);" + LFCR;
			int i = 0;
			for (const auto &m : info._proto._struct._members)
			{
				text += "\t\t\t" + m._name + " = new " + getCSharpType(m._type) + "();" + LFCR;
				text += "\t\t\t" "if ((tag.val & ((System.UInt64)1 << " + boost::lexical_cast<std::string, int>(i) + ")) != 0)" + LFCR;
				text += "\t\t\t" "{" + LFCR;
				text += "\t\t\t\t" + m._name + ".__decode(binData, ref pos);" + LFCR;
				text += "\t\t\t" "}" + LFCR;
				i++;
			}
			text += "\t\t\treturn (int)offset.val;" + LFCR;
			text += "\t\t}" + LFCR;
			text += "\t}" + LFCR;
		}

	}
	text += LFCR;
	text += "}" + LFCR;
	text += LFCR + LFCR;

	std::ofstream os;
	os.open(path + filename + attr, std::ios::binary);
	if (!os.is_open())
	{
		LOGE("genCSharpFile open file Error. : " << path + filename + attr);
		return false;
	}
	os.write(text.c_str(), text.length());
	os.close();
	return true;
}













