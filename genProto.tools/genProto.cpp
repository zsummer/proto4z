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

#include "genProto.h"


std::string WriteCppConstValue(const DataConstValue & v)
{
	std::string text;
	std::string type = v.type;
	{
		auto founder = xmlTypeToCppType.find(v.type);
		if (founder != xmlTypeToCppType.end() && !founder->second.empty())
		{
			type = founder->second;
		}
		
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
	{
		auto founder = xmlTypeToCppType.find(a.type);
		if (founder != xmlTypeToCppType.end() && !founder->second.empty())
		{
			type = founder->second;
		}
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
	{
		auto founder = xmlTypeToCppType.find(dm.typeKey);
		if (founder != xmlTypeToCppType.end() && !founder->second.empty())
		{
			typeKey = founder->second;
		}
	}

	std::string typeValue = dm.typeValue;
	{
		auto founder = xmlTypeToCppType.find(dm.typeValue);
		if (founder != xmlTypeToCppType.end() && !founder->second.empty())
		{
			typeValue = founder->second;
		}
	}


	text += "typedef std::map<" + typeKey + ", " + typeValue + "> " + dm.mapName + "; ";
	if (!dm.desc.empty())
	{
		text += "//" + dm.desc;
	}
	text += LFCR;
	return text;
}

std::string WriteCppDataMember(const std::vector<GeneralStruct::DataMember> & ms)
{
	std::string text;
	for (const auto & m : ms)
	{
		std::string type = m.type;
		{
			auto founder = xmlTypeToCppType.find(m.type);
			if (founder != xmlTypeToCppType.end() && !founder->second.empty())
			{
				type = founder->second;
			}
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

std::string WriteCppDataStructStream(const std::string &name, const std::vector<GeneralStruct::DataMember> & ms)
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

	text += "\t" + gs.name + "()" + LFCR;
	text += "\t{" + LFCR;
	for (const auto &m : gs.members)
	{
		{
			auto founder = xmlTypeToCppType.find(m.type);
			if (founder == xmlTypeToCppType.end() || founder->second.empty())
			{
				continue;
			}
		}
		{
			auto founder = xmlTypeToCppDefaultValue.find(m.type);
			if (founder == xmlTypeToCppDefaultValue.end() || founder->second.empty())
			{
				continue;
			}
		}

		text += "\t\t" + m.name + " = " + xmlTypeToCppDefaultValue[m.type] + ";" + LFCR;
	}
	text += "\t}" + LFCR;

	text += WriteCppDataMember(gs.members);
	text += "};" + LFCR;
	return text;
}









bool genProto::LoadCache()
{
	std::string cachename = m_fileName + m_fileCacheAttr;
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

	XMLElement * md5 = doc.FirstChildElement("md5");
	if (md5 && md5->GetText())
	{
		m_md5 = md5->GetText();
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
		DataCache dc;
		dc.protoName = key;
		dc.protoValue = atoi(No);
		m_mapCacheNo.insert(std::make_pair(key, dc));
		if (m_curNo <= atoi(No))
		{
			m_curNo = atoi(No) + 1;
		}

		next = next->NextSiblingElement("cache");
	} while (true);
	return true;
}


bool genProto::LoadConfig()
{
	std::string filename = m_fileName + m_fileConfigAttr;
	//检测文件状态
	if (!zsummer::utility::GetFileStatus(filename, 6))
	{
		LOGD(filename << " not found.");
		return false;
	}
	std::string md5 = genFileMD5(filename);
	if (!m_bForceGen && md5 == m_md5)
	{
		LOGD("skip genProto. file not change. filename=" << filename);
		return true;
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

			if (!MinNo || !MinNo->GetText() || !MaxNo || !MaxNo->GetText())
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
		XMLElement * ele = doc.FirstChildElement("Proto");
		if (ele == NULL)
		{
			LOGE("doc.FirstChildElement(\"Proto\") Error.");
			doc.PrintError();
			return false;
		}
		ele = ele->FirstChildElement();
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
				GeneralInfo info;
				info.dataConstValue = dc;
				info.type = GT_DataConstValue;
				m_genInfo.push_back(info);
			}
			//结构体类型
			if (stype == "struct" || stype == "proto")
			{
				DataConstValue dc;
				GeneralStruct stt;

				if (stype == "proto")
				{

					std::string name = ele->Attribute("name");
					std::string from = ele->Attribute("from");
					std::string to = ele->Attribute("to");
					std::string desc;
					if (ele->Attribute("desc"))
					{
						desc = ele->Attribute("desc");
					}

					dc.type = ProtoIDType;
					dc.name = "ID_" + from + "2" + to + "_" + name;
					unsigned int No = m_curNo;
					stt.name = from + "2" + "_" + name;
					stt.desc = desc;

					auto iterNo = m_mapCacheNo.find(dc.name);
					if (iterNo == m_mapCacheNo.end())
					{
						DataCache cache;
						cache.protoName = dc.name;
						cache.protoValue = m_curNo;
						m_mapCacheNo[dc.name] = cache;
						m_curNo++;
					}
					else
					{
						No = iterNo->second.protoValue;
					}
					dc.value = boost::lexical_cast<std::string>(No);
					if (No >= m_maxNo)
					{
						LOGE("proto No. overflow. curNo=" << m_curNo << ", maxNo=" << m_maxNo);
						return false;
					}
				}
				else
				{
					stt.name = ele->Attribute("name");
					if (ele->Attribute("desc"))
					{
						stt.desc = ele->Attribute("desc");
					}
				}
				XMLElement * member = ele->FirstChildElement("member");
				do
				{
					if (member == NULL)
					{
						break;

					}
					GeneralStruct::DataMember dm;
					dm.type = member->Attribute("type");
					dm.name = member->Attribute("name");
					if (member->Attribute("desc"))
					{
						stt.desc = member->Attribute("desc");
					}
					stt.members.push_back(dm);
					member = ele->NextSiblingElement("member");

				} while (true);

				if (stype == "proto")
				{
					GeneralInfo info;
					info.type = GT_DataConstValue;
					info.dataConstValue = dc;
					m_genInfo.push_back(info);
				}
				GeneralInfo info;
				info.type = GT_GeneralStruct;
				info.genStrunct = stt;
				m_genInfo.push_back(info);




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
				GeneralInfo info;
				info.type = GT_DataArray;
				info.dataArray = ar;
				m_genInfo.push_back(info);
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
				GeneralInfo info;
				info.type = GT_DataMap;
				info.dataMap = dm;
				m_genInfo.push_back(info);
			}
			ele = ele->NextSiblingElement();

		} while (true);



	}

	return true;
}



bool genProto::GenCPP()
{
	std::string md5 = genFileMD5(m_fileName + m_fileConfigAttr);
	if (md5 == m_md5)
	{
		LOGD("skip GenCPP. file not change. filename=" << m_fileName);
		return true;
	}

	std::string cppFileName = m_fileName;
	//		cppFileName = cppFileName.substr(0, cppFileName.length() - 4);
	std::string macroFileName = "_";
	macroFileName += cppFileName;
	macroFileName += "_H_";
	cppFileName += ".h";

	std::ofstream os;
	os.open(std::string("C++/") + cppFileName, std::ios::binary);

	if (!os.is_open())
	{
		LOGE("open cppFileName Error. : " << std::string("C++/") + cppFileName);
		return false;
	}
	std::string filehead = "#ifndef " + macroFileName + LFCR;
	filehead += "#define " + macroFileName + LFCR;
	os.write(filehead.c_str(), filehead.length());

	for (auto &info : m_genInfo)
	{
		if (info.type == GT_DataConstValue)
		{
			std::string text = LFCR;
			text += WriteCppConstValue(info.dataConstValue);
			os.write(text.c_str(), text.length());
			os.flush();
		}

		if (info.type == GT_GeneralStruct)
		{
			std::string text = LFCR;
			text += WriteCppStruct(info.genStrunct);
			text += WriteCppDataStructStream(info.genStrunct.name, info.genStrunct.members);

			os.write(text.c_str(), text.length());
			os.flush();
		}
		if (info.type == GT_DataArray)
		{
			std::string text = LFCR;
			text += WriteCppArray(info.dataArray);
			os.write(text.c_str(), text.length());
			os.flush();
		}
		if (info.type == GT_DataMap)
		{
			std::string text = LFCR;
			text += WriteCppMap(info.dataMap);
			os.write(text.c_str(), text.length());
			os.flush();
		}
	}
	std::string fileEnd = LFCR;
	fileEnd += "#endif" + LFCR;
	os.write(fileEnd.c_str(), fileEnd.length());
	os.close();
	return true;
}



bool genProto::WriteNoCache()
{
	std::string filename = m_fileName;
	std::string md5 = genFileMD5(m_fileName + m_fileConfigAttr);
	if (md5 == m_md5)
	{
		LOGD("skip WriteNoCache. file not change. filename=" << filename);
		return true;
	}
	filename += m_fileCacheAttr;

	LOGI("WriteNoCache [" << filename + ".cache" << "] ...");
	std::ofstream os;
	os.open(filename, std::ios::binary);
	if (!os.is_open())
	{
		LOGE(filename << " can not open!.");
		return false;
	}
	std::string text = "<?xml version=\"1.0\" encoding=\"UTF - 8\"?>\n\n";
	text += "<md5>";
	text += boost::lexical_cast<std::string>(md5);
	text += "</md5>\n\n";
	text += "<CacheNo>\n";
	for (auto &pr : m_mapCacheNo)
	{
		text += "\t<cache key = \"" + pr.first +
			"\" No = \"" + boost::lexical_cast<std::string>(pr.second.protoValue) + "\" /> \n";
	}
	text += "</CacheNo>\n";
	os.write(text.c_str(), text.length());
	os.close();
	return true;
}

