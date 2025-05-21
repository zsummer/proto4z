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



#pragma once
#ifndef _ANY_H_
#define _ANY_H_

#include <map>
#include <vector>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#ifndef WIN32
#include <sys/stat.h>
#else
#include <direct.h>
#endif



//store type enum
enum AnyType : short
{
    GT_DataComment,
    GT_DataEnum,
    GT_DataArray,
    GT_DataMap,
    GT_DataConstValue,
    GT_DataPacket,
};

//tag
enum MemberTag : short
{
    MT_NORMAL,
    MT_DB_KEY,
    MT_DB_UNI,
    MT_DB_IDX,
    MT_DB_AUTO,
    MT_DB_IGNORE,
    MT_DB_BLOB,
};

//comment
struct DataComment
{
    std::string desc_;
};



//array type
struct DataArray
{
    std::string type_;
    std::string arrayName_;
    std::string desc_;
};

//dict type
struct DataMap
{
    std::string typeKey_;
    std::string typeValue_;
    std::string mapName_;
    std::string desc_;
};

//const type
struct DataConstValue
{
    std::string type_;
    std::string name_;
    std::string value_;
    std::string desc_;
};

//include file name, without suffix
struct DataEnum
{
    std::string type_;
    std::string name_;
    std::string desc_;
    std::vector<DataConstValue> members_;
};

//struct type
struct DataStruct
{
    std::string name_;
    std::string desc_;
    std::string store_;
    bool hadLog4z_ = false;
    struct DataMember
    {
        std::string type_;
        std::string name_;
        std::string desc_;
        short tag_; //MemberTag
    };
    std::vector<DataMember> members_;
};

//proto type
struct DataPacket
{
    DataConstValue const_;
    DataStruct struct_;
};



//general store type
struct AnyData
{
    AnyType type_;
    DataComment _comment;
    DataConstValue const_;
    DataEnum enum_;
    DataArray array_;
    DataMap map_;
    DataPacket proto_;
};




#endif
