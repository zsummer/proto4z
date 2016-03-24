

#ifndef _CRT_SECURE_NO_WARNINGS
#define  _CRT_SECURE_NO_WARNINGS
#endif
#include "genBase.h"
#include <time.h>
#include <algorithm>




GenBase::GenBase()
{

}
GenBase::~GenBase()
{

}

void GenBase::init(std::string fileName, SupportLanguageType t)
{
    _filename = fileName;
    _type = t;
    std::string path = SupportLanguageFilePath[t];
    if (!isDirectory(path) && !createDirectory(path))
    {
        E("CreateDir path [" << path << "] Error. ");
    }
}

std::string GenBase::getRealType(const std::string & xmltype)
{
    return xmltype;
}

std::string GenBase::getTypeDefault(const std::string & xmltype)
{
    if (xmltype == "i8") return "0";
    else if (xmltype == "ui8") return "0";
    else if (xmltype == "i16") return "0";
    else if (xmltype == "ui16") return "0";
    else if (xmltype == "i32") return "0";
    else if (xmltype == "ui32") return "0";
    else if (xmltype == "i64") return "0";
    else if (xmltype == "ui64") return "0";
    else if (xmltype == "float") return "0.0";
    else if (xmltype == "double") return "0.0";
    return "";
}

std::string GenBase::genRealContent(const std::list<AnyData> & stores)
{
    return "";
}



void GenBase::write(const std::string & content)
{
    std::string filename = SupportLanguageFilePath[_type];
    filename += "/";
    filename += _filename;
    filename += SupportLanguageFileSuffix[_type];
    std::ofstream os;
    os.open(filename, std::ios::binary);
    if (!os.is_open())
    {
        E("genCppFile open file Error. : " << filename);
    }
    os.write(content.c_str(), content.length());
    os.close();
}

