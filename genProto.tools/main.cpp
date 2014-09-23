/*
 * genProto License
 * -----------
 * 
 * genProto is licensed under the terms of the MIT license reproduced below.
 * This means that genProto is free software and can be used for both academic
 * and commercial purposes at absolutely no cost.
 * 
 * 
 * ===============================================================================
 * 
 * Copyright (C) 2014 YaweiZhang <yawei_zhang@foxmail.com>.
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


#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "utility.h"
#include "log4z.h"
#include "tinyxml2.h"

#ifndef WIN32
#include <sys/stat.h>
#else
#include <direct.h>
#endif

struct DataArray
{
	std::string type;
	std::string arrayName;
	std::string desc;
};

struct DataMap
{
	std::string typeKey;
	std::string typeValue;
	std::string mapName;
	std::string desc;
};

struct DataConstValue 
{
	std::string type;
	std::string name;
	std::string value;
	std::string desc;
};

struct DataMember 
{
	std::string type;
	std::string name;
	std::string desc;
};
struct ProtoStruct
{
	std::string from;
	std::string to;
	std::string name;
	unsigned short protoID;
	std::string desc;
	std::vector<DataMember> members;

};

struct GeneralStruct 
{
	std::string name;
	std::string desc;
	std::vector<DataMember> members;
};

const std::string LFCR = "\r\n";

std::string genProtoIDName(const ProtoStruct & ps){return "ID_" + ps.from + "2" + ps.to + "_" + ps.name;}
std::string genProtoStructName(const ProtoStruct & ps){ return "Proto"  + ps.name; }



std::string WriteCppConstValue(const DataConstValue & v)
{
	std::string text;
	std::string type = v.type;
	if (type == "string")
	{
		type = "std::string";
	}
	text += "const " + type + " " + v.name + " = " + v.value + "; ";
	if (!v.desc.empty())
	{
		text += "//" + v.desc;
	}
	text += LFCR;
	
	return text;
}

std::string WriteCppArray(const DataArray & a)
{
	std::string text;
	std::string type = a.type;
	if (type == "string")
	{
		type = "std::string";
	}
	text += "typedef std::vector<" + type + "> " + a.arrayName + "; "; 
	if (!a.desc.empty())
	{
		text += "//" + a.desc;
	}
	text += LFCR;
	return text;
}

std::string WriteCppMap(const DataMap & dm)
{
	std::string text;
	std::string typeKey = dm.typeKey;
	std::string typeValue = dm.typeValue;
	if (typeKey == "string")
	{
		typeKey = "std::string";
	}
	if (typeValue == "string")
	{
		typeValue = "std::string";
	}
	text += "typedef std::map<" + typeKey + ", " + typeValue + "> " + dm.mapName + "; ";
	if (!dm.desc.empty())
	{
		text += "//" + dm.desc;
	}
	text += LFCR;
	return text;
}

std::string WriteCppDataMember(const std::vector<DataMember> & ms)
{
	std::string text;
	for (const auto & m:ms)
	{
		std::string type = m.type;
		if (type == "string")
		{
			type = "std::string";
		}
		text += "\t" + type + " " + m.name + "; ";
		if (!m.desc.empty())
		{
			text += "//" + m.desc;
		}
		text += LFCR;
	}
	return text;
}

std::string WriteCppDataStructStream(const std::string &name, const std::vector<DataMember> & ms)
{
	//输入流
	std::string text;
	text += LFCR;
	text = "template<class T>" + LFCR;
	text += "T & operator << (T & t, const " + name + " & data)" + LFCR;
	text += "{" + LFCR;

	for (const auto &m : ms)
	{
		text += "\tt << data." + m.name + ";" + LFCR;
	}
	text += "\treturn t;" + LFCR;
	text += "}" + LFCR;

	//输出流
	text += "template<class T>" + LFCR;
	text += "T & operator >> (T & t, " + name + " & data)" + LFCR;
	text += "{" + LFCR;
	for (const auto &m : ms)
	{
		text += "\tt >> data." + m.name + ";" + LFCR;
	}
	text += "\treturn t;" + LFCR;
	text += "}" + LFCR;
	return text;
}



std::string WriteCppStruct(const GeneralStruct & gs)
{
	std::string text;
	text += "struct " + gs.name;
	if (!gs.desc.empty())
	{
		text += " //" + gs.desc;
	}
	text += LFCR;

	text += "{" + LFCR;
	text += WriteCppDataMember(gs.members);
	text += "};" + LFCR;
	return text;
}



std::string WriteCppProto(const ProtoStruct & ps)
{
	std::string text;
	DataConstValue proto;
	proto.type = "ui16";
	proto.name = genProtoIDName(ps);
	proto.value = boost::lexical_cast<std::string>(ps.protoID);
	proto.desc = ps.desc;
	text += WriteCppConstValue(proto);

	text += "struct " + genProtoStructName(ps);
	if (!ps.desc.empty())
	{
		text += " //" + ps.desc;
	}
	text += LFCR;

	text += "{" + LFCR;
	text += WriteCppDataMember(ps.members);
	text += "};" + LFCR;
	return text;
}


using namespace tinyxml2;

class genProto
{
public:
	std::map<std::string, unsigned short> m_mapCacheNo;
	bool LoadCache(std::string filename)
	{
		LOGI("LoadCache [" << filename << "] ...");
		std::string cachename = filename + ".cache";
		if (!zsummer::utility::GetFileStatus(cachename, 6))
		{
			LOGD("LoadCache [" << cachename << " not found.");
			return true;
		}

		tinyxml2::XMLDocument doc;
		if (doc.LoadFile(cachename.c_str()) != tinyxml2::XML_SUCCESS)
		{
			LOGE(cachename << " Load Error. ");
			doc.PrintError();
			return false;
		}
		XMLElement * cacheEles = doc.FirstChildElement("CacheNo");
		if (cacheEles == NULL)
		{
			LOGE("doc.FirstChildElement(\"CacheNo\") Error.");
			doc.PrintError();
			return false;
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
				return false;
			}
			m_mapCacheNo.insert(std::make_pair(key, atoi(No)));
			if (m_curNo <= atoi(No))
			{
				m_curNo = atoi(No) + 1;
			}
			
			next = next->NextSiblingElement();
		} while (true);
		return true;
	}

	unsigned short m_minNo = 0;
	unsigned short m_maxNo = 0;
	unsigned short m_curNo = 0;
	bool m_bUseLog4z = false;

	bool gen(std::string filename)
	{
		LOGI("gen [" << filename << "] ...");
		//检测文件状态
		if (!zsummer::utility::GetFileStatus(filename, 6))
		{
			LOGD(filename << " not found.");
			return false;
		}
		//读取文件
		tinyxml2::XMLDocument doc;
		if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS)
		{
			LOGE(filename << " Load Error. ");
			doc.PrintError();
			return false;
		}

		//解析traits
		{
			XMLElement * ele = doc.FirstChildElement("ProtoTraits");
			if (ele == NULL)
			{
				LOGE("doc.FirstChildElement(\"ProtoTraits\") Error.");
				doc.PrintError();
				return false;
			}
			auto MinNo = ele->FirstChildElement("MinNo");
			auto MaxNo = ele->FirstChildElement("MaxNo");
			
			if (!MinNo || !MinNo->GetText() || !MaxNo || ! MaxNo->GetText())
			{
				LOGE("FirstChildElement(\"MinNo\") || FirstChildElement(\"MaxNo\")  Error");
				doc.PrintError();
				return false;
			}
			m_minNo = atoi(MinNo->GetText());
			m_maxNo = atoi(MaxNo->GetText());
			if (m_curNo < m_minNo)
			{
				m_curNo = m_minNo;
			}
			if (m_curNo > m_maxNo)
			{
				LOGE("Current cache Proto No Error. CurNo=" << m_curNo << ", minNo=" << m_minNo << ", maxNo=" << m_maxNo);
				return false;
			}
		}
		LOGI("gen [" << filename << "] CurProtoNo=" << m_curNo << ", minProtoNo=" << m_minNo << ", maxProtoNo=" << m_maxNo);


		//解析proto
		{
			std::string cppFileName = filename;
			cppFileName = cppFileName.substr(0, cppFileName.length() - 4);
			std::string macroFileName = "_";
			macroFileName += cppFileName;
			macroFileName += "_H_";
			cppFileName += ".h";

			std::ofstream os;
			os.open(std::string("C++/") + cppFileName, std::ios::binary);
			if (!os.is_open())
			{
				throw std::runtime_error("open cppFileName Error");
			}
			std::string filehead = "#ifndef " + macroFileName + LFCR;
			filehead += "#define " + macroFileName + LFCR;

			os.write(filehead.c_str(), filehead.length());

			zsummer::utility::CreateDir("C++");


			XMLElement * ele = doc.FirstChildElement("Proto");
			if (ele == NULL)
			{
				LOGE("doc.FirstChildElement(\"Proto\") Error.");
				doc.PrintError();
				return false;
			}
			ele = doc.FirstChildElement();
			do 
			{
				if (ele == NULL)
				{
					break;
				}
				std::string stype = ele->Name();
				if (stype == "const")
				{
					DataConstValue dc;
					

					dc.type = ele->Attribute("type");
					dc.name = ele->Attribute("name"); 
					dc.value = ele->Attribute("value"); 
					if (ele->Attribute("desc"))
					{
						dc.desc = ele->Attribute("desc");
					}
					std::string text = LFCR;
					text += WriteCppConstValue(dc);
					os.write(text.c_str(), text.length());
					os.flush();
				}
				//结构体类型
				if (stype == "struct" || stype == "proto")
				{
					ProtoStruct  proto;
					GeneralStruct stt;



					if (stype == "proto")
					{
						
						proto.name = ele->Attribute("name");
						proto.from = ele->Attribute("from");
						proto.to = ele->Attribute("to");
						if (ele->Attribute("desc"))
						{
							proto.desc = ele->Attribute("desc");
						}
						
						std::string idName = genProtoIDName(proto);

						unsigned short No = m_curNo;
						auto iterNo = m_mapCacheNo.find(idName);
						if (iterNo == m_mapCacheNo.end())
						{
							m_mapCacheNo[idName] = No;
							m_curNo++;
						}
						else
						{
							No = iterNo->second;
						}
						if (No >= m_maxNo)
						{
							LOGE("proto No. overflow. curNo=" << m_curNo << ", maxNo=" << m_maxNo);
							return false;
						}
						proto.protoID = No;
					}
					else
					{
						stt.name = ele->Attribute("name");
						if (ele->Attribute("desc"))
						{
							stt.desc = ele->Attribute("desc");
						}
					}
					XMLElement * member = doc.FirstChildElement("member");
					do 
					{
						if (member == NULL)
						{
							break;

						}
						DataMember dm;
						dm.type = member->Attribute("type");
						dm.name = member->Attribute("name");
						if (member->Attribute("desc"))
						{
							stt.desc = member->Attribute("desc");
						}
						if (stype == "proto")
						{
							proto.members.push_back(dm);
						}
						else
						{
							stt.members.push_back(dm);
						}
					} while (true);
					
					std::string text = LFCR;
					if (stype == "proto")
					{
						text += WriteCppProto(proto);
						text += WriteCppDataStructStream(genProtoStructName(proto), proto.members);
					}
					else
					{
						text += WriteCppStruct(stt);
						text += WriteCppDataStructStream(stt.name, stt.members);
					}
					os.write(text.c_str(), text.length());
					os.flush();
				}
				//数组类型
				if (stype == "array")
				{
					DataArray ar;
					ar.type = ele->Attribute("type");
					ar.arrayName = ele->Attribute("name");
					if (ele->Attribute("desc"))
					{
						ar.desc = ele->Attribute("desc");
					}
					std::string text = LFCR;
					text += WriteCppArray(ar);
					os.write(text.c_str(), text.length());
					os.flush();
				}
				//K-V类型
				if (stype == "map")
				{
					DataMap dm;
					dm.typeKey = ele->Attribute("key");
					dm.typeValue = ele->Attribute("value");
					dm.mapName = ele->Attribute("name");
					if (ele->Attribute("desc"))
					{
						dm.desc = ele->Attribute("desc");
					}

					std::string text = LFCR;
					text += WriteCppMap(dm);
					os.write(text.c_str(), text.length());
					os.flush();
				}
				ele = ele->NextSiblingElement();

			} while (true);

			std::string fileEnd = LFCR;
			fileEnd += "#endif" + LFCR;
			os.write(fileEnd.c_str(), fileEnd.length());
			os.close();
			
		}

		return true;
	}



	bool WriteNoCache(std::string filename)
	{
		LOGI("WriteNoCache [" << filename + ".cache" << "] ...");
		std::ofstream os;
		os.open(filename + ".cache", std::ios::binary);
		if (!os.is_open())
		{
			LOGE(filename + ".cache" << " can not open!.");
			return false;
		}
		std::string text = "<?xml version=\"1.0\" encoding=\"UTF - 8\"?>\n<CacheNo>\n";
		for (auto &pr : m_mapCacheNo)
		{
			text += "\t<cache key = \"" + pr.first + "\" No = \"" + boost::lexical_cast<std::string>(pr.second) + "\" /> \n";
		}
		text += "</CacheNo>\n"; 
		os.write(text.c_str(), text.length());
		os.close();
		return true;
	}

};










using namespace zsummer::utility;

int main(int argc, char *argv[])
{
	zsummer::log4z::ILog4zManager::GetInstance()->Start();
	std::vector<_FileInfo> files;
	if (!SearchFiles("./", files))
	{
		LOGE("SearchFiles error.");
		return 0;
	}
	LOGA("FOUND FILE COUNT = " << files.size());
	std::cout << "\r\n" << std::endl;
	for (auto & file : files)
	{
		std::string filename = file.filename;
		if (filename.size() <= 4)
		{
			//LOGD("Skip [" << filename << "] File not a xml .");
			//zsummer::utility::SleepMillisecond(80);
			continue;
		}
		std::string xmlattr = filename.substr(filename.length() - 4, 4);
		if (xmlattr != ".xml")
		{
			//LOGD("Skip [" << filename << "] File not a xml .");
			//zsummer::utility::SleepMillisecond(80);
			continue;
		}
		
		genProto gen;
		if (!gen.LoadCache(filename))
		{
			LOGE("LoadCache Error. filename=" << filename);
			return 0;
		}
		zsummer::utility::SleepMillisecond(150);

		if (!gen.gen(filename))
		{
			LOGE("gen Error. filename=" << filename);
			return 0;
		}
		zsummer::utility::SleepMillisecond(120);

		if (!gen.WriteNoCache(filename))
		{
			LOGE("WriteNoCache Error. filename=" << filename);
			return 0;
		}
		std::cout << "\r\n\r\n" << std::endl;
		zsummer::utility::SleepMillisecond(222);

	}
	
	LOGA("All Success.");


	return 0;
}






