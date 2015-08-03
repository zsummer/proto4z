#pragma once
#ifndef _TEST_STREAM_H_
#define _TEST_STREAM_H_
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <typeinfo>
#include <C++/TestProto.h>
using namespace  std;
using namespace zsummer::proto4z;

inline bool operator==(const TestIntegerData & data1, const TestIntegerData &data2)
{
    if (data1._char != data2._char)
    {
        return false;
    }
    if (data1._uchar != data2._uchar)
    {
        return false;
    }
    if (data1._short != data2._short)
    {
        return false;
    }
    if (data1._ushort != data2._ushort)
    {
        return false;
    }
    if (data1._int != data2._int)
    {
        return false;
    }
    if (data1._uint != data2._uint)
    {
        return false;
    }
    if (data1._i64 != data2._i64)
    {
        return false;
    }
    if (data1._ui64 != data2._ui64)
    {
        return false;
    }
    return true;
}
inline bool operator!=(const TestIntegerData & data1, const TestIntegerData &data2) { return !(data1 == data2); }

inline bool operator==(const TestFloatData & data1, const TestFloatData &data2)
{

    if ((unsigned long long)(data1._float*100.0) != (unsigned long long)(data2._float*100.0))
    {
        return false;
    }
    if ((unsigned long long)(data1._double*100.0) != (unsigned long long)(data2._double*100.0))
    {
        return false;
    }
    return true;
}

inline bool operator!=(const TestFloatData & data1, const TestFloatData &data2) { return !(data1 == data2); }

inline bool operator==(const TestStringData & data1, const TestStringData &data2)
{
    if (data1._string != data2._string)
    {
        return false;
    }
    return true;
}
inline bool operator!=(const TestStringData & data1, const TestStringData &data2){ return !(data1 == data2); }

template<class T>
inline bool operator==(const std::vector<T> & data1, const std::vector<T> &data2)
{
    if (data1.size() != data2.size())
    {
        return false;
    }
    for (size_t i = 0; i < data1.size(); i++)
    {
        if (data1[i] != data2[i])
        {
            return false;
        }
    }
    return true;
}
template<class T>
inline bool operator!=(const std::vector<T> & data1, const std::vector<T> &data2){ return !(data1 == data2); }
template<class K, class V>
inline bool operator==(const std::map<K,V> & data1, const std::map<K,V> &data2)
{
    if (data1.size() != data2.size())
    {
        return false;
    }
    for (typename std::map<K, V>::const_iterator iter = data1.begin(); iter != data1.end(); iter++)
    {
        typename std::map<K, V>::const_iterator founder = data2.find(iter->first);
        if (founder == data2.end())
        {
            return false;
        }
        if (founder->second != iter->second)
        {
            return false;
        }
    }
    return true;
}
template<class K, class V>
inline bool operator!=(const std::map<K, V> & data1, const std::map<K, V> &data2){ return !(data1 == data2); }

inline bool operator==(const EchoPack & pack1, const EchoPack &pack2)
{
    if (pack1._iarray != pack2._iarray)
    {
        return false;
    }
    if (pack1._farray != pack2._farray)
    {
        return false;
    }
    if (pack1._sarray != pack2._sarray)
    {
        return false;
    }
    if (pack1._imap != pack2._imap)
    {
        return false;
    }
    if (pack1._fmap != pack2._fmap)
    {
        return false;
    }
    if (pack1._smap != pack2._smap)
    {
        return false;
    }
    return true;
}
inline bool operator!=(const EchoPack & pack1, const EchoPack &pack2){ return !(pack1 == pack2); }

class TestBase
{
public:
    TestBase()
    {
        TestIntegerData idata;
        idata._char = 'z';
        idata._uchar = 234;
        idata._short = -1000;
        idata._ushort = 65500;
        idata._int = -2000;
        idata._uint = 123456789;
        idata._i64 = -3000;
        idata._ui64 = 12345678900ULL;
        TestFloatData fdata;
        fdata._float = (float)1.2345;
        fdata._double = 12345.1235567;
        TestStringData sdata;
        sdata._string = "lovexx";

        _testData._iarray.push_back(idata);
        _testData._iarray.push_back(idata);
        _testData._iarray.push_back(idata);

        _testData._farray.push_back(fdata);
        _testData._farray.push_back(fdata);
        _testData._farray.push_back(fdata);

        _testData._sarray.push_back(sdata);
        _testData._sarray.push_back(sdata);
        _testData._sarray.push_back(sdata);

        _testData._imap.insert(std::make_pair("123", idata));
        _testData._imap.insert(std::make_pair("223", idata));
        _testData._imap.insert(std::make_pair("323", idata));

        _testData._fmap.insert(std::make_pair("123", fdata));
        _testData._fmap.insert(std::make_pair("223", fdata));
        _testData._fmap.insert(std::make_pair("323", fdata));

        _testData._smap.insert(std::make_pair("123", sdata));
        _testData._smap.insert(std::make_pair("223", sdata));
        _testData._smap.insert(std::make_pair("323", sdata));

        _packLen = 0;
        _bodyLen = 0;
    }

    ~TestBase()
    {

    }

    inline bool CheckLenght()
    {
        WriteStream ws(120);
        try
        {
            ws << _testData;
            _packLen = ws.getStreamLen();
            _bodyLen = ws.getStreamBodyLen();
            if (_packLen - _bodyLen != sizeof(Integer) + sizeof(ProtoInteger))
            {
                cout << "CheckLenght -> " " HeadLen is error.  packLen=" << _packLen << ", bodyLen=" << _bodyLen << endl;
                return false;
            }
            if (120 != 
                streamToInteger<ProtoInteger>(ws.getStream() + sizeof(Integer)) )
            {
                cout << "CheckLenght -> " " write protocol ID error.  packLen=" << _packLen << ", bodyLen=" << _bodyLen << endl;
                return false;
            }
        }
        catch (std::runtime_error e)
        {
            cout << "CheckLenght -> "  " write Fail. error msg=" << e.what() << endl;
            return false;
        }
        cout << "CheckLenght -> "  " _packLen=" << _packLen << ", _bodyLen=" << _bodyLen << endl;
        return true;
    }

    inline bool CheckProtocol(WriteStream &ws, const char * desc);

    inline bool CheckAttachProtocol()
    {
        char * writeBuff = new char[_packLen];
        memset(writeBuff, 0, _packLen);
        WriteStream ws(120, writeBuff, _packLen);
        std::string str = "Check Attach Write. protocolTrits=";
        str += ":  ";
        return CheckProtocol(ws, str.c_str());
    }

    inline bool CheckNoAttachProtocol()
    {
        WriteStream ws(120, NULL, _packLen);
        std::string str = "Check NoAttach Write. protocolTrits=";
        str += ":  ";
        return CheckProtocol(ws, str.c_str());
    }

    inline bool CheckRouteProtocol()
    {
        const char * className = "Test";

        try
        {
            //analog recv message buff.
            WriteStream ws(120);
            unsigned int _roomID = 1;
            ws << _roomID << _testData;
            


            //analog recv msg
            std::pair<INTEGRITY_RET_TYPE, Integer> ret = checkBuffIntegrity(ws.getStream(), ws.getStreamLen(), MaxPackLen, MaxPackLen);
            if (ret.first == IRT_CORRUPTION || ret.first == IRT_SHORTAGE)
            {
                cout << "CheckRouteProtocol -> " << className << "  checkBuffIntegrity error" << endl;
                return false;
            }

            //check
            ReadStream rs(ws.getStream(), ws.getStreamLen());
            if (rs.getProtoID() != 120)
            {
                cout << "CheckRouteProtocol -> " << className << "  check Proto ID error" << endl;
                return false;
            }
            //check user data
            unsigned int roomID2 = 0;
            rs >> roomID2;
            if (roomID2 != _roomID)
            {
                cout << "CheckRouteProtocol -> " << className << "  check room ID error" << endl;
                return false;
            }
            //check route
            WriteStream ws2(120);
            ws2.appendOriginalData(rs.getStreamUnread(), rs.getStreamUnreadLen());
            ReadStream rs2(ws2.getStream(), ws2.getStreamLen());
            EchoPack testData;
            rs2 >> testData;
            if (!(testData == _testData))
            {
                cout << "CheckRouteProtocol -> " << className << "  check route error" << endl;
                return false;
            }
            cout << "CheckRouteProtocol -> " << className << "  check route OK" << endl;


            return true;
        }
        catch (std::runtime_error e)
        {
            cout << "CheckRouteProtocol -> " << className << "  check route Failed. msg=" << e.what() << endl;
            return false;
        }
        return true;
            
    }


    Integer _packLen;
    Integer _bodyLen;
    EchoPack _testData;
};








bool TestBase::CheckProtocol(WriteStream &ws, const char * desc)
{
    //check write
    {
        try
        {
            ws << _testData;
            cout << desc << " write data OK." << endl;
            if (ws.getStreamLen() == _packLen)
            {
                cout << desc << " write data  WriteStreamLen OK." << endl;
            }
            else
            {
                cout << desc << " write data  WriteStreamLen Failed. getStreamLen()="
                    << ws.getStreamLen() << ", _packLen=" << _packLen << endl;
                return false;
            }
            if (ws.getStreamBodyLen() == _bodyLen)
            {
                cout << desc << " write data WriteStreamBodyLen OK." << endl;
            }
            else
            {
                cout << desc << " write data WriteStreamBodyLen Failed. getStreamBodyLen()="
                    << ws.getStreamBodyLen() << ", _bodyLen=" << _bodyLen << endl;
                return false;
            }
        }
        catch (std::runtime_error e)
        {
            cout << desc << " write data catch error msg=" << e.what() << endl;
            return false;
        }
        try
        {
            ws << 'c';
            cout << desc << " write data check bound Failed. getStreamBodyLen()="
                << ws.getStreamBodyLen() << ", _bodyLen=" << _bodyLen << endl;
            return false;
        }
        catch (std::runtime_error e)
        {
            cout << desc << " write data check bound OK" << endl;
        }
    }


    //check checkBuffIntegrity
    {
        size_t headLen = sizeof(Integer) + sizeof(ProtoInteger);
        std::pair<INTEGRITY_RET_TYPE, Integer> ret = checkBuffIntegrity(ws.getStream(), 1, _packLen, _packLen);
        if (ret.first == IRT_SHORTAGE && ret.second == headLen - 1)
        {
            cout << desc << " checkBuffIntegrity check header len OK" << endl;
        }
        else
        {
            cout << desc << " checkBuffIntegrity check header len failed" << endl;
            return false;
        }
        ret = checkBuffIntegrity(ws.getStream(), ws.getStreamLen(), ws.getStreamLen(), ws.getStreamLen());
        if (ret.first == IRT_SUCCESS && ret.second == _packLen)
        {
            cout << desc << " checkBuffIntegrity check integrity  OK" << endl;
        }
        else
        {
            cout << desc << " checkBuffIntegrity check integrity  failed" << endl;
            return false;
        }
    }
    

    //check read
    {
        EchoPack readTestData;
        ReadStream rs(ws.getStream(), ws.getStreamLen());

        try
        {
            rs >> readTestData;
            cout << desc << " check Read all data  OK" << endl;
        }
        catch (std::runtime_error e)
        {
            cout << desc << " check Read all data  failed" << e.what() << endl;
            return false;
        }

        if (readTestData == _testData)
        {
            cout << desc << "consistency check OK" << endl;
        }
        else
        {
            cout << desc << "consistency check failed" << endl;
            return false;
        }

        try
        {
            char ch = 'a';
            rs >> ch;
            cout << desc << "read bounds check  ReadStream failed" << endl;
            return false;
        }
        catch (std::runtime_error e)
        {
            cout << desc << "read bounds check  ReadStream OK." << endl;
        }
        cout << desc << "check OK." << endl;


        ReadStream rs2(rs.getStreamBody(), rs.getStreamBodyLen(), false);
        EchoPack readTestData2;
        try
        {

            rs2 >> readTestData2;
            cout << desc << " check Read all data  OK" << endl;
        }
        catch (std::runtime_error e)
        {
            cout << desc << " check Read all data  failed" << e.what() << endl;
            return false;
        }

        if (readTestData2 == _testData)
        {
            cout << desc << "consistency check OK" << endl;
        }
        else
        {
            cout << desc << "consistency check failed" << endl;
            return false;
        }

        try
        {
            char ch = 'a';
            rs2 >> ch;
            cout << desc << "read bounds check  ReadStream failed" << endl;
            return false;
        }
        catch (std::runtime_error e)
        {
            cout << desc << "read bounds check  ReadStream OK." << endl;
        }    
    }
    
    cout << desc << "check OK." << endl;
    return true;
}



#endif


