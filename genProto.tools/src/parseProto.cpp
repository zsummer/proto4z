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
 * Copyright (C) 2014-2016 YaweiZhang <yawei.zhang@foxmail.com>.
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
#include "parseProto.h"
#include <time.h>
#include <algorithm>
#include "genCPP.h"
#include "genCSharp.h"
#include "genLUA.h"




std::list<AnyData> parseProto(std::string fileName, ParseCache & cache)
{
    unsigned short minProtoID = 0;
    unsigned short maxProtoID = 0;
    bool hadLog4z = false;
    std::list<AnyData> anydata;
    std::string filename = fileName + ".xml";

    if (!accessFile(filename))
    {
        E(filename << " not found.");
    }


    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS)
    {
        doc.PrintError();
        E(filename << " Load Error. ");
    }

    do
    {
        XMLElement * ele = doc.FirstChildElement("ProtoTraits");
        if (ele == NULL)
        {
            E("doc.FirstChildElement(\"ProtoTraits\") Error.");
        }
        auto minNo = ele->FirstChildElement("MinNo");
        auto maxNo = ele->FirstChildElement("MaxNo");
        auto lgz = ele->FirstChildElement("UseLog4z");
        if (!minNo || !minNo->GetText() || !maxNo || !maxNo->GetText())
        {
            E("FirstChildElement(\"MinNo\") || FirstChildElement(\"MaxNo\")  Error");
        }
        if (lgz && (strcmp(lgz->GetText(), "1") == 0 || strcmp(lgz->GetText(), "true") == 0))
        {
            hadLog4z = true;
        }
        
        minProtoID = atoi(minNo->GetText());
        maxProtoID = atoi(maxNo->GetText());
        if (cache.getCurrentProtoID() < minProtoID)
        {
            cache.setCurrentProtoID(minProtoID);
        }
        if (cache.getCurrentProtoID() > maxProtoID)
        {
            E("Current cache Proto No Error. NextNumber=" << cache.getCurrentProtoID() << ", minNo=" << minProtoID << ", maxNo=" << maxProtoID);
        }
        if (minProtoID >= maxProtoID)
        {
            E("must set valid MinNo and MaxNo. cur min=" << minProtoID << ", cur max=" << maxProtoID);
        }
    } while (0);
    LOGI("parseConfig [" << filename << "] NextNumber=" << cache.getCurrentProtoID() << ", minProtoNo=" << minProtoID << ", maxProtoNo=" << maxProtoID);

    if (true)
    {
        XMLElement * ele = doc.FirstChildElement("Proto");
        if (ele == NULL)
        {
            E("doc.FirstChildElement(\"Proto\") Error.");
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
                if (!ele->Attribute("type") || !ele->Attribute("name") || !ele->Attribute("value"))
                {
                    E("Attribute Error. ");
                }
                dc.type_ = trim(ele->Attribute("type"));
                dc.name_ = trim(ele->Attribute("name"));
                dc.value_ = trim(ele->Attribute("value"));
                
                if (ele->Attribute("desc"))
                {
                    dc.desc_ = ele->Attribute("desc");
                }
                AnyData info;
                info.const_ = dc;
                info.type_ = GT_DataConstValue;
                anydata.push_back(info);
            }
            else if (stype == "enum")
            {
                AnyData enumData;
                enumData.type_ = GT_DataEnum;
                int lastID = 0;
                if (!ele->Attribute("type"))
                {
                    E("Attribute Error. ");
                }
                enumData.enum_.type_ = trim(ele->Attribute("type"));
                if (ele->Attribute("name"))
                {
                    enumData.enum_.name_ = trim(ele->Attribute("name"));
                }
                if (ele->Attribute("desc"))
                {
                    enumData.enum_.desc_ = trim(ele->Attribute("desc"));
                }
                
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
                        E("Attribute Error. ");
                    }
                    dc.type_ = enumData.enum_.type_;
                    dc.name_ = trim(member->Attribute("name"));
                    if (member->Attribute("value"))
                    {
                        int v = atoi(trim(member->Attribute("value")).c_str());
                        if (v < lastID)
                        {
                            E("enum value is invalid.");
                        }
                        lastID = v;
                    }
                    dc.value_ = toString(lastID);
                    lastID++;
                    if (member->Attribute("desc"))
                    {
                        dc.desc_ = member->Attribute("desc");
                    }
                    enumData.enum_.members_.push_back(dc);
                    member = member->NextSiblingElement("member");
                } while (true);
                anydata.push_back(enumData);
            }
            //数组类型
            else if (stype == "array")
            {
                DataArray ar;
                if (!ele->Attribute("type") || !ele->Attribute("name"))
                {
                    E("Attribute Error. ");
                }
                ar.type_ = trim(ele->Attribute("type"));
                ar.arrayName_ = trim(ele->Attribute("name"));
                if (ele->Attribute("desc"))
                {
                    ar.desc_ = ele->Attribute("desc");
                }
                AnyData info;
                info.type_ = GT_DataArray;
                info.array_ = ar;
                anydata.push_back(info);
            }
            //K-V类型
            if (stype == "map")
            {
                DataMap dm;
                if (!ele->Attribute("key") || !ele->Attribute("value") || !ele->Attribute("name"))
                {
                    E("Attribute Error. ");
                }
                dm.typeKey_ = trim(ele->Attribute("key"));
                dm.typeValue_ = trim(ele->Attribute("value"));
                dm.mapName_ = trim(ele->Attribute("name"));
                if (ele->Attribute("desc"))
                {
                    dm.desc_ = ele->Attribute("desc");
                }
                AnyData info;
                info.type_ = GT_DataMap;
                info.map_ = dm;
                anydata.push_back(info);
            }
            //结构体类型
            else if (stype == "struct" || stype == "proto" || stype == "packet")
            {
                DataPacket dp;
                if (!ele->Attribute("name"))
                {
                    E("Attribute Error. ");
                }
                dp.struct_.name_ = ele->Attribute("name");
                if (ele->Attribute("desc"))
                {
                    dp.struct_.desc_ = ele->Attribute("desc");
                }
                if (ele->Attribute("store"))
                {
                    if (compareStringIgnCase(ele->Attribute("store"), "InnoDB"))
                    {
                        dp.struct_.store_ = "InnoDB";
                    }
                    else
                    {
                        dp.struct_.store_ = "MyISAM";
                    }
                    
                }
                dp.struct_.hadLog4z_ = hadLog4z;

                dp.const_.type_ = ProtoIDType;
                dp.const_.name_ = dp.struct_.name_;
                dp.const_.desc_ = dp.struct_.desc_;
                dp.const_.value_ = toString(cache.genProtoID(dp.struct_.name_, minProtoID, maxProtoID));

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
                        E("Attribute Error. ");
                    }
                    dm.type_ = trim(member->Attribute("type"));
                    dm.name_ = trim(member->Attribute("name"));
                    dm.tag_ = MT_NORMAL;
                    if (member->Attribute("tag"))
                    {
                        std::string tagText = member->Attribute("tag");
                        auto spt = splitString<std::string>(tagText, ",", " ");
                        for (auto tag : spt)
                        {
                            if (compareStringIgnCase(tag, "key"))
                            {
                                dm.tag_ = setBitFlag(dm.tag_, MT_DB_KEY);
                            }
                            else if (compareStringIgnCase(tag, "uni"))
                            {
                                dm.tag_ = setBitFlag(dm.tag_, MT_DB_UNI);
                            }
                            else if (compareStringIgnCase(tag, "idx"))
                            {
                                dm.tag_ = setBitFlag(dm.tag_, MT_DB_IDX);
                            }
                            else if (compareStringIgnCase(tag, "auto"))
                            {
                                dm.tag_ = setBitFlag(dm.tag_, MT_DB_AUTO);
                            }
                            else if (compareStringIgnCase(tag, "ignore"))
                            {
                                dm.tag_ = setBitFlag(dm.tag_, MT_DB_IGNORE);
                            }
                            else if (compareStringIgnCase(tag, "blob"))
                            {
                                dm.tag_ = setBitFlag(dm.tag_, MT_DB_BLOB);
                            }
                            else
                            {
                                E("Attribute Error. unknown tag");
                            }
                        }
                    }

                    if (member->Attribute("desc"))
                    {
                        dm.desc_ = member->Attribute("desc");
                    }
                    dp.struct_.members_.push_back(dm);
                    member = member->NextSiblingElement("member");

                } while (true);

                AnyData info;
                info.type_ = GT_DataPacket;
                info.proto_ = dp;
                anydata.push_back(info);
            }
            
            ele = ele->NextSiblingElement();

        } while (true);
    }

    for (auto &info : anydata)
    {
        if (info.type_ != GT_DataPacket)
        {
            continue;
        }
        int curTagIndex = 0;
//        info.proto_.struct_.tag_ = 0;
        for (const auto & m : info.proto_.struct_.members_)
        {
//            info.proto_.struct_.tag_ |= (1ULL << curTagIndex);
            curTagIndex++;
        }
    }
    return anydata;
}

GenBase * createGenerate(SupportLanguageType t)
{
    if (t == SL_CPP)
    {
        return new GenCPP();
    }
    else if (t == SL_CSHARP)
    {
        return new GenCSharp();
    }
    else if (t == SL_LUA)
    {
        return new GenLUA();
    }
    return nullptr;
}

void destroyGenerate(GenBase * gen)
{
    if (gen)
    {
        delete gen;
    }
}
