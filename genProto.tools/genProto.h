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



#pragma once
#ifndef _GEN_PROTO_H_
#define _GEN_PROTO_H_

#include <map>
#include <vector>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "utility.h"
#include "log4z.h"
#include "tinyxml2.h"
#include "md5.h"
#ifndef WIN32
#include <sys/stat.h>
#else
#include <direct.h>
#endif
#include "genProto.h"


//支持的的数据类型
static  std::map<std::string, std::string> xmlTypeToCppType = {
		{ "i8", "char" },
		{ "ui8", "unsigned char" },
		{ "i16", "short" },
		{ "ui16", "unsigned short" },
		{ "i32", "unsigned int" },
		{ "ui32", "unsigned int" },
		{ "i64", "long long" },
		{ "ui64", "unsigned long long" },
		{ "float", "float" },
		{ "double", "double" },
		{ "string", "std::string" },
};

//数据类型对应的初始化数值
static  std::map<std::string, std::string> xmlTypeToCppDefaultValue = {
		{ "i8", "'\0'" },
		{ "ui8", "0" },
		{ "i16", "0" },
		{ "ui16", "0" },
		{ "i32", "0" },
		{ "ui32", "0" },
		{ "i64", "0" },
		{ "ui64", "0" },
		{ "float", "0.0" },
		{ "double", "0.0" },
		{ "string", "\"\"" },
};


//支持数组类型
struct DataArray
{
	std::string type;
	std::string arrayName;
	std::string desc;
};

//支持字典类型
struct DataMap
{
	std::string typeKey;
	std::string typeValue;
	std::string mapName;
	std::string desc;
};

//支持常量类型
struct DataConstValue
{
	std::string type;
	std::string name;
	std::string value;
	std::string desc;
};

//支持结构体类型
struct GeneralStruct
{
	std::string name;
	std::string desc;
	struct DataMember
	{
		std::string type;
		std::string name;
		std::string desc;
	};
	std::vector<DataMember> members;
};



enum GeneralType
{
	GT_DataArray,
	GT_DataMap,
	GT_DataConstValue,
	GT_GeneralStruct,
};

struct GeneralInfo
{
	GeneralType type;
	DataArray dataArray;
	DataMap dataMap;
	DataConstValue dataConstValue;
	GeneralStruct genStrunct;
};

const std::string ProtoIDType = "ui16";


const std::string LFCR = "\n";



std::string WriteCppConstValue(const DataConstValue & v);

std::string WriteCppArray(const DataArray & a);

std::string WriteCppMap(const DataMap & dm);

std::string WriteCppDataMember(const std::vector<GeneralStruct::DataMember> & ms);

std::string WriteCppDataStructStream(const std::string &name, const std::vector<GeneralStruct::DataMember> & ms);

std::string WriteCppStruct(const GeneralStruct & gs);




using namespace tinyxml2;

class genProto
{
public:
	//初始化信息
	std::string m_fileName;
	std::string m_fileConfigAttr = ".xml";
	std::string m_fileCacheAttr = ".xml.cache";
	bool m_bForceGen = false;

	//cache信息
	unsigned short m_curNo = 0;
	std::string m_md5;
	struct DataCache
	{
		std::string protoName;
		unsigned int protoValue;
	};
	std::map<std::string, DataCache> m_mapCacheNo;

	//xml信息
	unsigned short m_minNo = 0;
	unsigned short m_maxNo = 0;
	std::vector<GeneralInfo> m_genInfo;

	//构造
	genProto(std::string filename, bool bForceGen = false)
	{
		m_fileName = filename;
		m_bForceGen = bForceGen;
	}

	bool GenCPP();

	bool LoadCache();

	bool LoadConfig();

	bool WriteNoCache();

};


#endif
