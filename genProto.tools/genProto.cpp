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
 * Copyright (C) 2012 YaweiZhang <yawei_zhang@foxmail.com>.
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

std::string getCPPType(std::string type)
{
	auto founder = xmlTypeToCPPType.find(type);
	if (founder != xmlTypeToCPPType.end() && !founder->second.empty())
	{
		type = founder->second;
	}
	return type;
}

bool genCppFile(std::string path, std::string filename, std::string attr, std::vector<StoreInfo> & stores)
{
	std::string macroFileName = std::string("_") + filename  + "_H_";
	std::transform(macroFileName.begin(), macroFileName.end(), macroFileName.begin(), [](char ch){ return std::toupper(ch); });


	std::string text = LFCR + "#ifndef " + macroFileName + LFCR;
	text += "#define " + macroFileName + LFCR + LFCR;

	for (auto &info : stores)
	{
		if (info._type == GT_DataInclude)
		{
			text += "#include <" + info._include._filename + ".h> ";
			if (!info._include._desc.empty())
			{
				text += "//" + info._include._desc;
			}
			text += LFCR;
		}
		else if (info._type == GT_DataConstValue)
		{
			text += "const " + getCPPType(info._const._type) + " " + info._const._name + " = " + info._const._value + "; ";
			if (!info._const._desc.empty())
			{
				text += "//" + info._const._desc;
			}
			text += LFCR;
		}
		else if (info._type == GT_DataArray)
		{
			text += LFCR + "typedef std::vector<" + getCPPType(info._array._type) + "> " + info._array._arrayName + "; ";
			if (!info._array._desc.empty())
			{
				text += "//" + info._array._desc;
			}
			text += LFCR;
		}
		else if (info._type == GT_DataMap)
		{
			text += LFCR + "typedef std::map<"
				+ getCPPType(info._map._typeKey) + ", " + getCPPType(info._map._typeValue) 
				+ "> " + info._map._mapName + "; ";
			if (!info._map._desc.empty())
			{
				text += "//" + info._map._desc;
			}
			text += LFCR;
		}
		else if (info._type == GT_DataStruct || info._type == GT_DataProto)
		{
			text += LFCR;
			//write ProtoID
			if (info._type == GT_DataProto)
			{
				text += "const " + getCPPType(info._proto._const._type) + " " 
					+ info._proto._const._name + " = " + info._proto._const._value + "; ";
				if (!info._proto._const._desc.empty())
				{
					text += "//" + info._proto._const._desc;
				}
				text += LFCR;
			}

			//write struct
			text += "struct " + info._proto._struct._name;
			if (!info._proto._struct._desc.empty())
			{
				text += " //" + info._proto._struct._desc;
			}
			text += LFCR;
			text += "{" + LFCR;

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
				
				text += "\t" + getCPPType(m._type) + " " + m._name + "; ";
				if (m._isDel)
				{
					text += "//[already deleted]";
				}
				if (!m._desc.empty())
				{
					text += "//" + m._desc;
				}
				text += LFCR;
			}

			{	//struct init
				std::string defaltInit = "\t" + info._proto._struct._name + "()" + LFCR;
				defaltInit += "\t{" + LFCR;
				std::string memberText;
				for (const auto &m : info._proto._struct._members)
				{
					auto founder = xmlTypeToCPPDefaultValue.find(m._type);
					if (founder != xmlTypeToCPPDefaultValue.end() && !founder->second.empty())
					{
						memberText += "\t\t" + m._name + " = " + founder->second + ";" + LFCR;
					}
				}
				if (!memberText.empty())
				{
					text += defaltInit;
					text += memberText;
					text += "\t}" + LFCR;
				}
			}
			if (info._type == GT_DataProto)
			{
				text += std::string("\tinline ") + getCPPType(ProtoIDType) + " GetProtoID() { return " + info._proto._const._value + ";}" + LFCR;
				text += std::string("\tinline ") + getCPPType("string") + " GetProtoName() { return \""
					+ info._proto._const._name + "\";}" + LFCR;
			}
			text += "};" + LFCR;


			//input stream operator
			text += "zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const " + info._proto._struct._name + " & data)" + LFCR;
			text += "{" + LFCR;
			text += "\tzsummer::proto4z::Integer offset = ws.getStreamLen();" +LFCR;
			text += "\tunsigned long long tag = " + boost::lexical_cast<std::string, unsigned long long>(info._proto._struct._tag) + "ULL;" + LFCR;
			text += "\tws << offset;" + LFCR;
			text += "\tws << tag;" + LFCR;
			for (const auto &m : info._proto._struct._members)
			{
				if (m._isDel)
				{
					text += "//\tws << data." + m._name + "; //[already deleted]" + LFCR;
				}
				else
				{
					text += "\tws << data." + m._name + ";" + LFCR;
				}
			}
			text += "\tws.fixOriginalData(offset, ws.getStreamLen() - offset);" + LFCR;
			text += "\treturn ws;" + LFCR;
			text += "}" + LFCR;


			//output stream operator
			text += "zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, " + info._proto._struct._name + " & data)" + LFCR;
			text += "{" + LFCR;
			text += "\tzsummer::proto4z::Integer cursor = rs.getStreamUnreadLen();" + LFCR;
			text += "\tzsummer::proto4z::Integer sttLen = 0;" + LFCR;
			text += "\tunsigned long long tag = 0;" + LFCR;
			text += "\trs >> sttLen;" + LFCR;
			text += "\trs >> tag;" + LFCR;
			curTagIndex = 0;
			for (const auto &m : info._proto._struct._members)
			{
				text += "\tif ( (1ULL << " + boost::lexical_cast<std::string, int>(curTagIndex)+") & tag)" + LFCR;
				text += "\t{" + LFCR;
				text += "\t\trs >> data." + m._name + "; " + LFCR;
				text += "\t}" + LFCR;
				curTagIndex++;
			}
			text += "\tcursor = cursor - rs.getStreamUnreadLen();" + LFCR;
			text += "\trs.skipOriginalData(sttLen - cursor);" + LFCR;
			text += "\treturn rs;" + LFCR;
			text += "}" + LFCR;
		}

	}
	text += LFCR + "#endif" + LFCR;

	std::ofstream os;
	os.open(path + filename + attr, std::ios::binary);
	if (!os.is_open())
	{
		LOGE("genCppFile open file Error. : " << path + filename + attr);
		return false;
	}
	os.write(text.c_str(), text.length());
	os.close();
	return true;
}


bool genLuaFile(std::string path, std::string filename, std::string attr, std::vector<StoreInfo> & stores)
{
	std::string text;
	for (auto &info : stores)
	{
		if (info._type == GT_DataInclude)
		{
			text += "require (\"" + info._include._filename + "\")";
			if (!info._include._desc.empty())
			{
				text += "--" + info._include._desc;
			}
			text += LFCR;
		}
		else if (info._type == GT_DataConstValue)
		{
			text += "Protoz." + info._const._name + " = " + info._const._value;
			if (!info._const._desc.empty())
			{
				text += "--" + info._const._desc;
			}
			text += LFCR;
		}
		else if (info._type == GT_DataArray)
		{
			text += LFCR;
			text += "Protoz." + info._array._arrayName + " = {} ";
			if (!info._array._desc.empty())
			{
				text += "--" + info._array._desc;
			}
			text += LFCR;

			text += "Protoz." + info._array._arrayName + ".__getName = \"" + info._array._arrayName + "\"" + LFCR;
			text += "Protoz." + info._array._arrayName + ".__getDesc = \"array\"" + LFCR;
			text += "Protoz." + info._array._arrayName + ".__getTypeV = \"" + info._array._type + "\"" + LFCR;
		}
		else if (info._type == GT_DataMap)
		{
			text += LFCR;
			text += "Protoz." + info._map._mapName + " = {} ";
			if (!info._map._desc.empty())
			{
				text += "--" + info._map._desc;
			}
			text += LFCR;

			text += "Protoz." + info._map._mapName + ".__getName = \"" + info._map._mapName + "\"" + LFCR;
			text += "Protoz." + info._map._mapName + ".__getDesc = \"map\"" + LFCR;
			text += "Protoz." + info._map._mapName + ".__getTypeK = \"" + info._map._typeKey + "\"" + LFCR;
			text += "Protoz." + info._map._mapName + ".__getTypeV = \"" + info._map._typeValue + "\"" + LFCR;
		}
		else if (info._type == GT_DataStruct || info._type == GT_DataProto)
		{
			text += LFCR;

			if (info._type == GT_DataProto)
			{
				text += "Protoz.register(" + info._proto._const._value + ",\"" + info._proto._struct._name + "\")" + LFCR;
			}

			text += "Protoz." + info._proto._struct._name + " = {} ";
			if (!info._proto._struct._desc.empty())
			{
				text += "--" + info._proto._struct._desc;
			}
			text += LFCR;
			if (info._type == GT_DataProto)
			{
				text += "Protoz." + info._proto._struct._name + ".__getID = " + info._proto._const._value + "" + LFCR;
			}
			text += "Protoz." + info._proto._struct._name + ".__getName = \"" + info._proto._struct._name + "\"" + LFCR;

	

			for (size_t i = 0; i < info._proto._struct._members.size(); ++i)
			{
				text += "Protoz." + info._proto._struct._name + "[" + boost::lexical_cast<std::string>(i+1) 
					+ "] = {name=\"" + info._proto._struct._members[i]._name +  "\", type=\"" + info._proto._struct._members[i]._type + "\"} ";
				if (!info._proto._struct._members[i]._desc.empty())
				{
					text += "--" + info._proto._struct._members[i]._desc;
				}
				text += LFCR;
			}

			
		}

	}


	std::ofstream os;
	os.open(path + filename + attr, std::ios::binary);
	if (!os.is_open())
	{
		LOGE("genCppFile open file Error. : " << path + filename + attr);
		return false;
	}
	os.write(text.c_str(), text.length());
	os.close();
	return true;
}






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
			text += "\t\t\t"   " var len = new Proto4z.ui16((System.UInt16)this.Count);" + LFCR;
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
			text += "\t\t\t"   "var len = new Proto4z.ui16(0);" + LFCR;
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
			text += "\t\t\t"   " var len = new Proto4z.ui16((System.UInt16)this.Count);" + LFCR;
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
			text += "\t\t\t"   "var len = new Proto4z.ui16(0);" + LFCR;
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
				text += "\t\t public Proto4z.ui16 getProtoID() { return new Proto4z.ui16(" + info._proto._const._value + "); }" + LFCR;
				text += "\t\t  public string getProtoName() { return \"" + info._proto._struct._name + "\"; }" + LFCR;
			}

			for (const auto & m : info._proto._struct._members)
			{
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
			text += "\t\t\t"   "var ret = new System.Collections.Generic.List<byte>();" + LFCR;
			for (const auto &m : info._proto._struct._members)
			{
				text += "\t\t\t"  "ret.AddRange(" + m._name + ".__encode());" + LFCR;
			}
			text += "\t\t\t"  "return ret;" + LFCR;
			text += "\t\t}" + LFCR;

			//decode
			text += "\t\tpublic int __decode(byte[] binData, ref int pos)" + LFCR;
			text += "\t\t{" + LFCR;
			for (const auto &m : info._proto._struct._members)
			{
				text += "\t\t\t" + m._name + " = new " + getCSharpType(m._type) + "();" + LFCR;
				text += "\t\t\t" + m._name + ".__decode(binData, ref pos);" + LFCR;
			}
			text += "\t\t\treturn pos;" + LFCR;
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
		LOGE("genCppFile open file Error. : " << path + filename + attr);
		return false;
	}
	os.write(text.c_str(), text.length());
	os.close();
	return true;
}

















ParseCode genProto::parseCache()
{
	std::string cachename = _fileName + _fileCacheAttr;
	if (!zsummer::utility::GetFileStatus(cachename, 6))
	{
		LOGD("parseCache [" << cachename << " not found.");
		return PC_SUCCESS;
	}


	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(cachename.c_str()) != tinyxml2::XML_SUCCESS)
	{
		LOGE(cachename << " parseCache Error. ");
		doc.PrintError();
		return PC_ERROR;
	}

	XMLElement * md5 = doc.FirstChildElement("md5");
	if (md5 && md5->GetText())
	{
		_md5 = md5->GetText();
	}




	XMLElement * cacheEles = doc.FirstChildElement("CacheNo");
	if (cacheEles == NULL)
	{
		LOGE("doc.FirstChildElement(\"CacheNo\") Error.");
		doc.PrintError();
		return PC_ERROR;
	}

	XMLElement * next = cacheEles->FirstChildElement("cache");
	do
	{
		if (next == NULL)
		{
			break;
		}
		const char * key = next->Attribute("key");
		const char * No = next->Attribute("No");
		if (key == NULL || No == NULL)
		{
			LOGE("cache file is invalid. cachefile=" << cachename);
			doc.PrintError();
			return PC_ERROR;
		}
		DataCache dc;
		dc.protoName = key;
		dc.protoValue = atoi(No);
		_mapCacheNo.insert(std::make_pair(key, dc));
		if (_curNo <= atoi(No))
		{
			_curNo = atoi(No) + 1;
		}

		next = next->NextSiblingElement("cache");
	} while (true);
	return PC_SUCCESS;
}


ParseCode genProto::parseConfig()
{
	std::string filename = _fileName + _fileConfigAttr;
	//检测文件状态
	if (!zsummer::utility::GetFileStatus(filename, 6))
	{
		LOGD(filename << " not found.");
		return PC_ERROR;
	}

	//读取文件
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS)
	{
		LOGE(filename << " Load Error. ");
		doc.PrintError();
		return PC_ERROR;
	}

	//解析traits
		{
			XMLElement * ele = doc.FirstChildElement("ProtoTraits");
			if (ele == NULL)
			{
				LOGE("doc.FirstChildElement(\"ProtoTraits\") Error.");
				doc.PrintError();
				return PC_ERROR;
			}
			auto MinNo = ele->FirstChildElement("MinNo");
			auto MaxNo = ele->FirstChildElement("MaxNo");

			if (!MinNo || !MinNo->GetText() || !MaxNo || !MaxNo->GetText())
			{
				LOGE("FirstChildElement(\"MinNo\") || FirstChildElement(\"MaxNo\")  Error");
				doc.PrintError();
				return PC_ERROR;
			}
			_minNo = atoi(MinNo->GetText());
			_maxNo = atoi(MaxNo->GetText());
			if (_curNo < _minNo)
			{
				_curNo = _minNo;
			}
			if (_curNo > _maxNo)
			{
				LOGE("Current cache Proto No Error. CurNo=" << _curNo << ", minNo=" << _minNo << ", maxNo=" << _maxNo);
				return PC_ERROR;
			}
		}
	LOGI("parseConfig [" << filename << "] CurProtoNo=" << _curNo << ", minProtoNo=" << _minNo << ", maxProtoNo=" << _maxNo);


	//解析proto
	{
		XMLElement * ele = doc.FirstChildElement("Proto");
		if (ele == NULL)
		{
			LOGE("doc.FirstChildElement(\"Proto\") Error.");
			doc.PrintError();
			return PC_ERROR;
		}
		ele = ele->FirstChildElement();
		do
		{
			if (ele == NULL)
			{
				break;
			}
			std::string stype = ele->Name();
			if (stype == "include")
			{
				DataInclude dc;
				if (!ele->Attribute("name"))
				{
					LOGE("Attribute Error. ");
					return PC_ERROR;
				}
				dc._filename = ele->Attribute("name");
				if (ele->Attribute("desc"))
				{
					dc._desc = ele->Attribute("desc");
				}
				StoreInfo info;
				info._include = dc;
				info._type = GT_DataInclude;
				_vctStoreInfo.push_back(info);
			}
			else if (stype == "const")
			{
				DataConstValue dc;
				if (!ele->Attribute("type") || !ele->Attribute("name") || !ele->Attribute("value"))
				{
					LOGE("Attribute Error. ");
					return PC_ERROR;
				}
				dc._type = ele->Attribute("type");
				dc._name = ele->Attribute("name");
				dc._value = ele->Attribute("value");
				
				if (ele->Attribute("desc"))
				{
					dc._desc = ele->Attribute("desc");
				}
				StoreInfo info;
				info._const = dc;
				info._type = GT_DataConstValue;
				_vctStoreInfo.push_back(info);
			}
			else if (stype == "enum")
			{
				std::string enumType;
				int lastID = 0;
				if (!ele->Attribute("type"))
				{
					LOGE("Attribute Error. ");
					return PC_ERROR;
				}
				enumType = ele->Attribute("type");

				XMLElement * member = ele->FirstChildElement("member");
				do
				{
					DataConstValue dc;
					if (member == NULL)
					{
						break;
					}
					if (!member->Attribute("name"))
					{
						LOGE("Attribute Error. ");
						return PC_ERROR;
					}
					dc._type = enumType;
					dc._name = member->Attribute("name");
					if (member->Attribute("value"))
					{
						int v = atoi(member->Attribute("value"));
						if (v < lastID)
						{
							LOGE("enum value is invalid.");
							return PC_ERROR;
						}
						lastID = v;
					}
					dc._value = boost::lexical_cast<std::string>(lastID);
					lastID++;
					if (member->Attribute("desc"))
					{
						dc._desc = member->Attribute("desc");
					}
					StoreInfo info;
					info._const = dc;
					info._type = GT_DataConstValue;
					_vctStoreInfo.push_back(info);
					member = member->NextSiblingElement("member");
				} while (true);

			}
			//数组类型
			else if (stype == "array")
			{
				DataArray ar;
				if (!ele->Attribute("type") || !ele->Attribute("name"))
				{
					LOGE("Attribute Error. ");
					return PC_ERROR;
				}
				ar._type = ele->Attribute("type");
				ar._arrayName = ele->Attribute("name");
				if (ele->Attribute("desc"))
				{
					ar._desc = ele->Attribute("desc");
				}
				StoreInfo info;
				info._type = GT_DataArray;
				info._array = ar;
				_vctStoreInfo.push_back(info);
			}
			//K-V类型
			if (stype == "map")
			{
				DataMap dm;
				if (!ele->Attribute("key") || !ele->Attribute("value") || !ele->Attribute("name"))
				{
					LOGE("Attribute Error. ");
					return PC_ERROR;
				}
				dm._typeKey = ele->Attribute("key");
				dm._typeValue = ele->Attribute("value");
				dm._mapName = ele->Attribute("name");
				if (ele->Attribute("desc"))
				{
					dm._desc = ele->Attribute("desc");
				}
				StoreInfo info;
				info._type = GT_DataMap;
				info._map = dm;
				_vctStoreInfo.push_back(info);
			}
			//结构体类型
			else if (stype == "struct" || stype == "proto")
			{
				DataProto dp;
				if (stype == "proto")
				{
					if (!ele->Attribute("name") || !ele->Attribute("from") || !ele->Attribute("to"))
					{
						LOGE("Attribute Error. ");
						return PC_ERROR;
					}
					std::string name = ele->Attribute("name");
					std::string from = ele->Attribute("from");
					std::string to = ele->Attribute("to");
					std::string desc;
					if (ele->Attribute("desc"))
					{
						desc = ele->Attribute("desc");
					}

					dp._const._type = ProtoIDType;
					dp._const._name = "ID_" + from + "2" + to + "_" + name;
					dp._const._desc = desc;
					unsigned int No = _curNo;
					dp._struct._name = from + "2" + to + "_" + name;
					dp._struct._desc = desc;

					auto iterNo = _mapCacheNo.find(dp._const._name);
					if (iterNo == _mapCacheNo.end())
					{
						DataCache cache;
						cache.protoName = dp._const._name;
						cache.protoValue = _curNo;
						_mapCacheNo[dp._const._name] = cache;
						_curNo++;
					}
					else
					{
						No = iterNo->second.protoValue;
					}
					dp._const._value = boost::lexical_cast<std::string>(No);
					if (No >= _maxNo)
					{
						LOGE("proto No. overflow. curNo=" << _curNo << ", maxNo=" << _maxNo);
						return PC_ERROR;
					}
				}
				else
				{
					if (!ele->Attribute("name"))
					{
						LOGE("Attribute Error. ");
						return PC_ERROR;
					}
					dp._struct._name = ele->Attribute("name");
					if (ele->Attribute("desc"))
					{
						dp._struct._desc = ele->Attribute("desc");
					}
				}
				XMLElement * member = ele->FirstChildElement("member");
				do
				{
					if (member == NULL)
					{
						break;
					}
					DataStruct::DataMember dm;
					if (!member->Attribute("type") || !member->Attribute("name"))
					{
						LOGE("Attribute Error. ");
						return PC_ERROR;
					}
					dm._type = member->Attribute("type");
					dm._name = member->Attribute("name");
					if (member->Attribute("del"))
					{
						if (strcmp(member->Attribute("del"), "true") == 0)
						{
							dm._isDel = true;
						}
						else
						{
							dm._isDel = false;
						}
					}
					else
					{
						dm._isDel = false;
					}
					if (member->Attribute("desc"))
					{
						dm._desc = member->Attribute("desc");
					}
					dp._struct._members.push_back(dm);
					member = member->NextSiblingElement("member");

				} while (true);

				StoreInfo info;
				info._type = GT_DataStruct;
				info._proto = dp;
				if (stype == "proto")
				{
					info._type = GT_DataProto;
				}
				_vctStoreInfo.push_back(info);
			}
			
			ele = ele->NextSiblingElement();

		} while (true);



	}

	return PC_SUCCESS;
}



ParseCode genProto::genCode()
{
	std::string xmlmd5 = genFileMD5(_fileName + _fileConfigAttr);

	{
		if (!zsummer::utility::IsDirectory("C++") && !zsummer::utility::CreateDir("C++"))
		{
			LOGE("CreateDir C++ Error. ");
		}
		if (!zsummer::utility::IsDirectory("lua") && !zsummer::utility::CreateDir("lua"))
		{
			LOGE("CreateDir lua Error. ");
		}
		if (!zsummer::utility::IsDirectory("CSharp") && !zsummer::utility::CreateDir("CSharp"))
		{
			LOGE("CreateDir lua Error. ");
		}
		std::string cppFileName = "C++/" + _fileName + ".h";
		std::string luaFileName = "lua/" + _fileName + ".lua";
		std::string csFileName = "CSharp/" + _fileName + ".CSharp";
		if (xmlmd5 != _md5 || !zsummer::utility::GetFileStatus(cppFileName, 6))
		{
			if (!genCppFile("C++/", _fileName, ".h", _vctStoreInfo))
			{
				return PC_ERROR;
			}
		}
		if (xmlmd5 != _md5 || !zsummer::utility::GetFileStatus(luaFileName, 6))
		{
			if (!genLuaFile("lua/", _fileName, ".lua", _vctStoreInfo))
			{
				return PC_ERROR;
			}
		}
		if (xmlmd5 != _md5 || !zsummer::utility::GetFileStatus(csFileName, 6))
		{
			if (!genCSharpFile("CSharp/", _fileName, ".cs", _vctStoreInfo))
			{
				return PC_ERROR;
			}
		}
	}

	if (xmlmd5 == _md5)
	{
		return PC_NEEDSKIP;
	}
	return PC_SUCCESS;
}



ParseCode genProto::writeCache()
{
	std::string filename = _fileName;
	std::string md5 = genFileMD5(_fileName + _fileConfigAttr);
	filename += _fileCacheAttr;

	LOGI("writeCache [" << filename + ".cache" << "] ...");
	std::ofstream os;
	os.open(filename, std::ios::binary);
	if (!os.is_open())
	{
		LOGE(filename << " can not open!.");
		return PC_ERROR;
	}
	std::string text = "<?xml version=\"1.0\" encoding=\"UTF - 8\"?>\n\n";
	text += "<md5>";
	text += boost::lexical_cast<std::string>(md5);
	text += "</md5>\n\n";
	text += "<CacheNo>\n";
	for (auto &pr : _mapCacheNo)
	{
		text += "\t<cache key = \"" + pr.first +
			"\" No = \"" + boost::lexical_cast<std::string>(pr.second.protoValue) + "\" /> \n";
	}
	text += "</CacheNo>\n";
	os.write(text.c_str(), text.length());
	os.close();
	return PC_SUCCESS;
}

