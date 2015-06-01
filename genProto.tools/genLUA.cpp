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


#include "genLUA.h"

std::string GenLUA::genRealContent(const std::list<AnyData> & stores)
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
			text += "Proto4z." + info._const._name + " = " + info._const._value;
			if (!info._const._desc.empty())
			{
				text += "--" + info._const._desc;
			}
			text += LFCR;
		}
		else if (info._type == GT_DataArray)
		{
			text += LFCR;
			text += "Proto4z." + info._array._arrayName + " = {} ";
			if (!info._array._desc.empty())
			{
				text += "--" + info._array._desc;
			}
			text += LFCR;

			text += "Proto4z." + info._array._arrayName + ".__getName = \"" + info._array._arrayName + "\"" + LFCR;
			text += "Proto4z." + info._array._arrayName + ".__getDesc = \"array\"" + LFCR;
			text += "Proto4z." + info._array._arrayName + ".__getTypeV = \"" + info._array._type + "\"" + LFCR;
		}
		else if (info._type == GT_DataMap)
		{
			text += LFCR;
			text += "Proto4z." + info._map._mapName + " = {} ";
			if (!info._map._desc.empty())
			{
				text += "--" + info._map._desc;
			}
			text += LFCR;

			text += "Proto4z." + info._map._mapName + ".__getName = \"" + info._map._mapName + "\"" + LFCR;
			text += "Proto4z." + info._map._mapName + ".__getDesc = \"map\"" + LFCR;
			text += "Proto4z." + info._map._mapName + ".__getTypeK = \"" + info._map._typeKey + "\"" + LFCR;
			text += "Proto4z." + info._map._mapName + ".__getTypeV = \"" + info._map._typeValue + "\"" + LFCR;
		}
		else if (info._type == GT_DataStruct || info._type == GT_DataProto)
		{
			text += LFCR;

			if (info._type == GT_DataProto)
			{
				text += "Proto4z.register(" + info._proto._const._value + ",\"" + info._proto._struct._name + "\")" + LFCR;
			}

			text += "Proto4z." + info._proto._struct._name + " = {} ";
			if (!info._proto._struct._desc.empty())
			{
				text += "--" + info._proto._struct._desc;
			}
			text += LFCR;
			if (info._type == GT_DataProto)
			{
				text += "Proto4z." + info._proto._struct._name + ".__getID = " + info._proto._const._value + "" + LFCR;
			}
			text += "Proto4z." + info._proto._struct._name + ".__getName = \"" + info._proto._struct._name + "\"" + LFCR;

	
			std::string tag = "";
			for (auto &m : info._proto._struct._members)
			{
				if (m._tag == MT_DELETE)
				{
					tag.append("0");
				}
				else
				{
					tag.append("1");
				}
			}
			text += "Proto4z." + info._proto._struct._name + ".__getTag = \"" + tag + "\"" + LFCR;
			for (size_t i = 0; i < info._proto._struct._members.size(); ++i)
			{
				text += "Proto4z." + info._proto._struct._name + "[" + boost::lexical_cast<std::string>(i + 1)
					+ "] = {name=\"" + info._proto._struct._members[i]._name + "\", type=\"" + info._proto._struct._members[i]._type + "\"";
				if (info._proto._struct._members[i]._tag == MT_DELETE)
				{
					text += ", del = true";
				}
				text += " } ";
				if (!info._proto._struct._members[i]._desc.empty())
				{
					text += "--" + info._proto._struct._members[i]._desc;
				}
				text += LFCR;
			}

			
		}

	}


	return std::move(text);
}



