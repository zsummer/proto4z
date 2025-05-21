#pragma once
#ifndef _TEST_HTTP_H_
#define _TEST_HTTP_H_
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <typeinfo>
#include <proto4z.h>
using namespace  std;
using namespace zsummer::proto4z;

const int MAX_HTTP_LEN = 1000;
class TestWeb
{
public:
    bool Test(WriteWebStream &wh)
    {
        bool isChunked = false;
        std::string method;
        std::string methodLine;
        std::map<std::string,std::string> head;
        std::string body;
        if (HasWebRawPacket(wh.GetStream(), wh.GetStreamLen(), 1024, isChunked, method, methodLine, head, body).first == kIntegrityIntact)
        {
            if (head.find("Host") != head.end()
                && (method == "GET" || method == "POST" || methodLine == "200"))
            {
                cout << "Check HasWebRawPacket Success" << endl;
            }
            else
            {
                cout << "Check HasWebRawPacket Data error" << endl;
                return false;
            }
        }
        else
        {
            cout << "Check HasWebRawPacket unpack error. ret =" << (kIntegrityShortage ? "kIntegrityShortage":"kIntegrityCorrupted") << endl;
            return false;
        }
        if (HasWebRawPacket(wh.GetStream(), wh.GetStreamLen() - 1, 1024, isChunked, method, methodLine, head, body).first != kIntegrityShortage)
        {
            cout << "Check HasWebRawPacket kIntegrityShortage error" << endl;
            return false;
        }
        if (HasWebRawPacket(wh.GetStream(), wh.GetStreamLen(), wh.GetStreamLen() - 1,  isChunked, method, methodLine, head, body).first != kIntegrityCorrupted)
        {
            cout << "Check HasWebRawPacket kIntegrityCorrupted error" << endl;
            return false;
        }    
        return true;
    }
protected:
private:
};




#endif


