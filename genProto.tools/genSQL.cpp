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
std::string getCPPType(std::string type);

bool genSQLFile(std::string path, std::string filename, std::string attr, std::vector<StoreInfo> & stores)
{
	std::string macroFileName = std::string("_") + filename + "_H_";
	std::transform(macroFileName.begin(), macroFileName.end(), macroFileName.begin(), [](char ch){ return std::toupper(ch); });


	std::string text = LFCR + "#ifndef " + macroFileName + LFCR;
	text += "#define " + macroFileName + LFCR + LFCR;

	for (auto &info : stores)
	{
		if ((info._type == GT_DataStruct || info._type == GT_DataProto) && info._proto._struct._isStore)
		{
			DataStruct::DataMember key;
			for (auto m : info._proto._struct._members)
			{
				if (m._isKey)
				{
					if (key._isKey)
					{
						LOGE("struct have multi key. struct name=" << info._proto._struct._name << ", key1=" << key._name << ", key2=" << m._name);
					}
					else
					{
						key = m;
					}
				}
			}
			if (!key._isKey)
			{
				LOGE("struct have no key. struct name=" << info._proto._struct._name);
				continue;
			}
			//build
			text += LFCR;
			text += "inline std::vector<std::string> " + info._proto._struct._name + "_BUILD()" + LFCR;
			text += "{" + LFCR;
			text += "\tstd::vector<std::string> ret;" + LFCR;
			text += "\tret.push_back(\"desc `tb_" + info._proto._struct._name + "`\");" + LFCR;
			text += "\tret.push_back(\"CREATE TABLE `tb_" + info._proto._struct._name + "` (`" + key._name + "`";
			if (key._type == "string")
			{
				text += " varchar(255) NOT NULL DEFAULT '' , ";
			}
			else if (key._type == "i8" || key._type == "i16" || key._type == "i32" || key._type == "i64")
			{
				text += " bigint(20) NOT NULL DEFAULT '0' ,  ";
			}
			else if (key._type == "ui8" || key._type == "ui16" || key._type == "ui32" || key._type == "ui64")
			{
				text += " bigint(20) unsigned NOT NULL DEFAULT '0' , ";
			}
			text += " PRIMARY KEY(`" + key._name + "`) ) ENGINE = MyISAM DEFAULT CHARSET = utf8\");" + LFCR;

			for (auto& m : info._proto._struct._members)
			{
				if (m._isKey)
				{
					continue;
				}

				text += "\tret.push_back(\"alter table `tb_" + info._proto._struct._name + "` add `" + m._name + "` ";
				if (m._type == "string")
				{
					text += " varchar(255) NOT NULL DEFAULT '' ";
				}
				else if (m._type == "i8" || m._type == "i16" || m._type == "i32" || m._type == "i64")
				{
					text += " bigint(20) NOT NULL DEFAULT '0' ";
				}
				else if (m._type == "ui8" || m._type == "ui16" || m._type == "ui32" || m._type == "ui64")
				{
					text += " bigint(20) unsigned NOT NULL DEFAULT '0' ";
				}
				else
				{
					text += " blob ";
				}
				text += "\");" + LFCR;
			}
		
			text += "\treturn std::move(ret);" + LFCR;
			text += "}" + LFCR;


			//load
			text += LFCR;
			text += "inline std::string " + info._proto._struct._name + "_LOAD( unsigned long long curLoaded)" + LFCR;
			text += "{" + LFCR;
			text += "\tzsummer::mysql::DBQuery q(\" select ";
			for (auto& m: info._proto._struct._members)
			{
				text += "`" + m._name + "`";
				text += ",";
			}
			text[text.length() - 1] = ' ';
			text += "from `tb_" + info._proto._struct._name + "` order by `" + key._name + "` desc limit ?, 1000 \");" + LFCR;
			text += "\tq << curLoaded;" + LFCR;
			text += "\treturn q.popSQL();" + LFCR;
			text += "}" + LFCR;

			//select
			text += LFCR;
			text += "inline std::string " + info._proto._struct._name + "_SELECT(" + getCPPType(key._type) + " " + key._name + ")" + LFCR;
			text += "{" + LFCR;
			text += "\tzsummer::mysql::DBQuery q(\" select ";
			for (auto& m : info._proto._struct._members)
			{
				text += "`" + m._name + "`";
				text += ",";
			}
			text[text.length() - 1] = ' ';
			text += "from `tb_" + info._proto._struct._name + "` where `" + key._name + "` = ? \");" + LFCR;
			text += "\tq << " + key._name + ";" + LFCR;
			text += "\treturn q.popSQL();" + LFCR;
			text += "}" + LFCR;

			//fetch
			text += LFCR;
			text += "inline std::map<" + getCPPType(key._type) + ", " + info._proto._struct._name + "> " + info._proto._struct._name + "_FETCH(zsummer::mysql::DBResultPtr ptr)" + LFCR;
			text += "{" + LFCR;
			text += "\tstd::map<" + getCPPType(key._type) + ", " + info._proto._struct._name + "> ret;" + LFCR;
			text += "\tif (ptr->getErrorCode() != zsummer::mysql::QEC_SUCCESS)" + LFCR;
			text += "\t{" + LFCR;
			text += "\t\t"  "LOGE(\"fetch info from db found error. ErrorCode=\"  <<  ptr->getErrorCode() << \", Error=\" << ptr->getLastError());" + LFCR;
			text += "\t\t"  "return ret;" + LFCR;
			text += "\t}" + LFCR;
			text += "\ttry" + LFCR;
			text += "\t{" + LFCR;

			text += "\t\t"  "while (ptr->haveRow())" + LFCR;
			text += "\t\t{" + LFCR;
			text += "\t\t\t" + info._proto._struct._name + " " + "info;" + LFCR;
			for (auto& m : info._proto._struct._members)
			{
				if (m._type == "ui8" || m._type == "ui16" || m._type == "ui32" || m._type == "ui64"
					|| m._type == "i8" || m._type == "i16" || m._type == "i32" || m._type == "i64"
					|| m._type == "double" || m._type == "float" || m._type == "string")
				{
					text += "\t\t\t*ptr >> info." + m._name + ";" + LFCR;
				}
				else
				{
					text += "\t\t\ttry" + LFCR;
					text += "\t\t\t{" + LFCR;

					text += "\t\t\t\t"  "std::string blob;" + LFCR;
					text += "\t\t\t\t"  "*ptr >> blob;" + LFCR;
					text += "\t\t\t\t"  "if(!blob.empty())" + LFCR;
					text += "\t\t\t\t"  "{" + LFCR;
					text += "\t\t\t\t\t"  "zsummer::proto4z::ReadStream rs(blob.c_str(), blob.length(), false);" + LFCR;
					text += "\t\t\t\t\t"  "rs >> info." + m._name + ";" + LFCR;
					text += "\t\t\t\t"  "}" + LFCR;

					text += "\t\t\t}" + LFCR;
					text += "\t\t\tcatch(std::runtime_error e)" + LFCR;
					text += "\t\t\t{" + LFCR;
					text += "\t\t\t\t"  "LOGW(\"fetch blob catch one runtime warning. what=\" << e.what());" + LFCR;
					text += "\t\t\t}" + LFCR;

				}
			}
			text += "\t\t\tret[info." + key._name + "] = info;" + LFCR;
			text += "\t\t}" + LFCR;

			text += "\t}" + LFCR;
			text += "\tcatch(std::runtime_error e)" + LFCR;
			text += "\t{" + LFCR;
			text += "\t\t"  "LOGE(\"fetch info catch one runtime error. what=\" << e.what());" + LFCR;
			text += "\t\t"  "return ret;" + LFCR;
			text += "\t}" + LFCR;

			text += "\treturn std::move(ret);" + LFCR;
			text += "}" + LFCR;
			text += LFCR;

			//update
			text += LFCR;
			text += "inline std::string " + info._proto._struct._name + "_UPDATE( const " + info._proto._struct._name + " & info) " + LFCR;
			text += "{" + LFCR;
			text += "\tzsummer::mysql::DBQuery q(\" insert into tb_" + info._proto._struct._name + "(" + key._name + ") values(?) on duplicate key update ";
			for (auto& m : info._proto._struct._members)
			{
				if (m._isKey)
				{
					continue;
				}
				text += "`" + m._name + "` = ?,";
			}
			text[text.length() - 1] = ' ';
			text += " \");" + LFCR;
			text += "\tq << info." + key._name + ";" + LFCR;

			for (auto& m : info._proto._struct._members)
			{
				if (m._isKey)
				{
					continue;
				}
				else if (m._type == "ui8" || m._type == "ui16" || m._type == "ui32" || m._type == "ui64"
					|| m._type == "i8" || m._type == "i16" || m._type == "i32" || m._type == "i64"
					|| m._type == "double" || m._type == "float" || m._type == "string")
				{
					text += "\tq << info." + m._name + ";" + LFCR;
				}
				else
				{
					text += "\t"  "try" + LFCR;
					text += "\t"  "{" + LFCR;
					text += "\t\t"  "zsummer::proto4z::WriteStream ws(0);" + LFCR;
					text += "\t\t"  "ws << info." + m._name + ";" + LFCR;
					text += "\t\t"  "q << std::string(ws.getStreamBody(), ws.getStreamBodyLen());" + LFCR;
					text += "\t"  "}" + LFCR;
					text += "\t"  "catch(std::runtime_error e)" + LFCR;
					text += "\t"  "{" + LFCR;
					text += "\t\t"  "LOGW(\"write blob catch one runtime warning. what=\" << e.what());" + LFCR;
					text += "\t\t"  "return "";" + LFCR;
					text += "\t"  "}" + LFCR;
				}
				
			}
			text += "\treturn q.popSQL();" + LFCR;
			text += "}" + LFCR;
			text += LFCR;
		}

	}

	text += LFCR + "#endif" + LFCR;

	std::ofstream os;
	os.open(path + filename + attr, std::ios::binary);
	if (!os.is_open())
	{
		LOGE("genSQLFile open file Error. : " << path + filename + attr);
		return false;
	}
	os.write(text.c_str(), text.length());
	os.close();
	return true;
}



