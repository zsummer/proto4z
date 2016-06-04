
#ifndef _CRT_SECURE_NO_WARNINGS
#define  _CRT_SECURE_NO_WARNINGS
#endif
#include "parseCache.h"
#include <time.h>
#include <algorithm>



void ParseCache::parse(std::string filename)
{
    _fileName = filename;
    std::string configFile = filename + ".xml.cache";
    if (!accessFile(configFile))
    {
        LOGW("ParseCache::parse [" << configFile << " not found.");
        return;
    }
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(configFile.c_str()) != tinyxml2::XML_SUCCESS)
    {
        E(" ParseCache::parse Error. configFile=" << configFile << ", err1=" << doc.GetErrorStr1() << ", err2" << doc.GetErrorStr2());
    }

    for (int i = SL_NORMAL+1; i < SL_END; i++)
    {
        XMLElement * md5 = doc.FirstChildElement(SupportLanguageString[i]);
        if (md5 && md5->GetText())
        {
            _md5Cache[i] = md5->GetText();
        }
    }
    
    XMLElement * cacheEles = doc.FirstChildElement("cacheNumber");
    if (cacheEles == NULL)
    {
        LOGW("ParseCache::parse can not found cacheNumber. configFile=" << configFile);
        return ;
    }

    XMLElement * next = cacheEles->FirstChildElement("cache");
    do
    {
        if (next == NULL)
        {
            break;
        }
        const char * key = next->Attribute("key");
        const char * number = next->Attribute("Number");
        if (key == NULL || number == NULL)
        {
            E("ParseCache::parse cache number is invalid. configFile=" << configFile);
        }


        auto founder = _cacheNumber.find(key);
        if (founder != _cacheNumber.end())
        {
            E("ParseCache::parse dumplicate key on " << key << " from " << configFile);
        }
        _cacheNumber[key] = atoi(number);

        if (_currentProtoID <= atoi(number))
        {
            _currentProtoID = atoi(number) + 1;
        }

        next = next->NextSiblingElement("cache");
    } while (true);
}
bool   ParseCache::write()
{
    std::string filename = _fileName + ".xml.cache";
    LOGI("writeCache [" << filename );
    std::string text = "<?xml version=\"1.0\" encoding=\"UTF - 8\"?>\n\n";
    for (int i = SL_NORMAL + 1; i < SL_END; i++)
    {
        std::string md5;
        if (i != SL_XML)
        {
            md5 = genFileMD5(std::string("./") + SupportLanguageFilePath[i] + "/" + _fileName + SupportLanguageFileSuffix[i]);
        }
        else
        {
            md5 = genFileMD5(std::string("./") + _fileName + SupportLanguageFileSuffix[i]);
        }
        text += std::string() + "<" + SupportLanguageString[i] + ">";
        text += md5;
        text += std::string() + "</" + SupportLanguageString[i] + ">" + LFCR;
    }
    
    text += "<cacheNumber>\n";

    for (auto &pr : _cacheNumber)
    {
        text += "    <cache key = \"" + pr.first +"\" Number = \"" + toString(pr.second) + "\" /> \n";
    }
    text += "</cacheNumber>\n";
    if (writeFileContent(filename, text.c_str(), text.length(), false) != text.length())
    {
        LOGE("write cache file error. filename=" << filename);
    }
    return true;
}

bool ParseCache::isNeedUpdate()
{
    for (int i = SL_NORMAL+1; i < SL_END; i++)
    {
        if (_md5Cache[i].empty())
        {
            return true;
        }
        std::string md5;
        if (i != SL_XML)
        {
            md5 = genFileMD5(std::string("./") + SupportLanguageFilePath[i] + "/" + _fileName + SupportLanguageFileSuffix[i]);
        }
        else
        {
            md5 = genFileMD5(std::string("./") + _fileName + SupportLanguageFileSuffix[i]);
        }
        
        if (md5 != _md5Cache[i])
        {
            return true;
        }
    }
    return false;
}

unsigned short ParseCache::getCacheNumber(std::string key)
{
    auto founder = _cacheNumber.find(key);
    if (founder == _cacheNumber.end())
    {
        return -1;
    }
    return founder->second;
}
bool ParseCache::setCacheNumber(std::string key, unsigned short number)
{
    auto founder = _cacheNumber.find(key);
    if (founder != _cacheNumber.end())
    {
        return false;
    }
    _cacheNumber[key] = number;
    return true;
}

unsigned short ParseCache::genProtoID(std::string key, unsigned short minProtoID, unsigned short maxProtoID)
{
    unsigned short ret = getCacheNumber(key);
    if (ret == (unsigned short)-1)
    {
        ret = _currentProtoID++;
        setCacheNumber(key, ret);
        if (ret < minProtoID || ret >= maxProtoID)
        {
            E("proto number override. key=" << key << ", next number=" << ret);
        }
    }
    return ret;
}

