//! yawei_zhang@foxmail.com

#include <proto4z.h>
#include <unordered_map>
#include <iostream>
#include <map>
#include <time.h>
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif // WIN32

#ifdef __APPLE__
#include<mach/mach_time.h>
#endif // __APPLE__

using namespace std;
using namespace zsummer::proto4z;

#include "C++/TestProto.h"
#include "TestWeb.h"

void  fillOnePack(EchoPack &pack)
{
    IntegerData idata;
    idata._char = 'a';
    idata._uchar = 100;
    idata._short = 200;
    idata._ushort = 300;
    idata._int = 400;
    idata._uint = 500;
    idata._i64 = 600;
    idata._ui64 = 700;

    FloatData fdata;
    fdata._float = (float)123123.111111;
    fdata._double = 12312312.88888;

    StringData sdata;
    sdata._string = "abcdefg";

    pack._iarray.push_back(idata);
    pack._iarray.push_back(idata);
    pack._farray.push_back(fdata);
    pack._farray.push_back(fdata);
     pack._sarray.push_back(sdata);
     pack._sarray.push_back(sdata);


    pack._imap.insert(std::make_pair(123, idata));
    pack._imap.insert(std::make_pair(223, idata));
    pack._fmap.insert(std::make_pair(32.3, fdata));
    pack._fmap.insert(std::make_pair(42.3, fdata));
     pack._smap.insert(std::make_pair("523", sdata));
     pack._smap.insert(std::make_pair("623", sdata));
}


unsigned int getSteadyTime();



int main()
{


    cout << "check http proto ..." << endl;
    TestWeb th;
    WriteWebStream whGet;
    whGet.addHead("Content-Type", "application/x-www-form-urlencoded");
    whGet.addHead("Host", "www.google.com");
    whGet.get("/");
    th.Test(whGet);

    WriteWebStream whPost;
    whPost.addHead("Content-Type", "application/x-www-form-urlencoded");
    whPost.addHead("Host", "www.google.com");
    whPost.post("/", "index.php?aaa=333");
    th.Test(whPost);

    WriteWebStream whResult;
    whResult.addHead("test", "test");
    whResult.addHead("Content-Type", "application/x-www-form-urlencoded");
    whResult.addHead("Host", "www.google.com");
    whResult.response("200", "");
    th.Test(whResult);

    cout << "check binary proto" << endl;
    try
    {
        WriteStream wwrap(1);
        WriteStream ws(EchoPack::getProtoID());
        EchoPack echo;
        fillOnePack(echo);
        ws << echo;
        wwrap << "3333333333333333333333333333";
        wwrap.AppendRawData(ws.GetStream(), ws.GetStreamLen());
        ReadStream rwrap(wwrap.GetStream(), wwrap.GetStreamLen());
        std::string tmp;
        rwrap >> tmp;
        ReadStream rs(rwrap.GetStreamUnread(), rwrap.GetStreamUnreadLen());
        rs >> echo;
        cout << "success" << endl;
    }
    catch (const std::exception & e)
    {
        cout << "error:" << e.what() << endl;
        return -1;
    }

    try
    {


        SimplePack pack;
        pack.id = 10;
        pack.name = "aaa";
        pack.createTime = time(NULL);
        pack.moneyTree.freeCount = 0;
        pack.moneyTree.lastTime = pack.createTime;
        pack.moneyTree.statSum = 0;
        pack.moneyTree.statCount = 0;
        pack.moneyTree.payCount = 0;



        //序列化
        WriteStream ws(SimplePack::getProtoID());
        ws << pack;
        //反序列化
        ReadStream rs(ws.GetStream(), ws.GetStreamLen());
        rs >> pack;
        cout << "success" << endl;
    }
    catch (const std::exception & e)
    {
        cout << "error:" << e.what() << endl;
        return -1;
    }


    try
    {
        using PairString = std::pair<std::string, std::string>;
        using PairVector = std::pair<std::vector<PairString>, std::vector<PairString>>;
        using DequeVector = std::deque<PairVector>;
        using Map = std::map<std::string, DequeVector>;
        using UnorderedMap = std::unordered_map<int, Map>;

        PairString ps = std::make_pair("111", "111");
        std::vector<PairString> vps;
        vps.push_back(ps);
        PairVector pv = std::make_pair(vps, vps);
        DequeVector dv;
        dv.push_back(pv);

        Map m;
        m["111"] = dv;

        UnorderedMap um;
        um[111] = m;

        WriteStream ws(0);
        ws << um;

        UnorderedMap um2;
        ReadStream rs(ws.GetStream(), ws.GetStreamLen());
        rs >> um2;
        if (um2[111]["111"].at(0).second.at(0).second != "111")
        {
            std::cout << "error" << std::endl;
            return -10;
        }
        else
        {
            std::cout << "success" << std::endl;
        }
        

        
    }
    catch (const std::exception&)
    {
        return -1;
    }
    

#define StressCount 1*10000000
    SimplePack pack;
    pack.id = 10;
    pack.name = "aaa";
    pack.createTime = time(NULL);
    pack.moneyTree.freeCount = 0;
    pack.moneyTree.lastTime = pack.createTime;
    pack.moneyTree.statSum = 0;
    pack.moneyTree.statCount = 0;
    pack.moneyTree.payCount = 0;
    unsigned long long count = 0;
    unsigned int now = getSteadyTime();
    for (int i = 0; i < StressCount; i++)
    {
        WriteStream ws(100);
        ws << pack;
        count += ws.GetStreamLen();
    }
    std::cout << "writeStream used time: " << getSteadyTime() - now << std::endl;
    
    now = getSteadyTime();
    for (int i = 0; i < StressCount; i++)
    {
        WriteStream ws(100);
        ws << pack;
        ReadStream rs(ws.GetStream(), ws.GetStreamLen());
        rs >> pack;
        count += rs.GetStreamLen();
    }
    std::cout << "write and read stream used time: " << getSteadyTime() - now << std::endl;






//     cout << "press any key to continue." << endl;
//     getchar();
    return 0;
}


unsigned int getSteadyTime()
{
#ifdef WIN32
    return ::GetTickCount();
#elif defined(__APPLE__)
    const int64_t kOneMillion = 1000 * 1000;
    mach_timebase_info_data_t timebase_info;
    mach_timebase_info(&timebase_info);
    return (unsigned int)((mach_absolute_time() * timebase_info.numer) / (kOneMillion * timebase_info.denom));
#else
    timespec ts;
    if(clock_gettime(CLOCK_MONOTONIC_RAW, &ts) != 0)
        return 0;
    return ts.tv_sec * 1000 + (ts.tv_nsec / 1000/1000);
#endif
}

