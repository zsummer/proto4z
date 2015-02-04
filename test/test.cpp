//! yawei_zhang@foxmail.com

#include "../proto4z.h"

#include <iostream>
#include <time.h>
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif // WIN32


using namespace std;
using namespace zsummer::proto4z;

#include "TestStream.h"
#include "TestHTTP.h"



unsigned int GetTimeMillisecond();

struct StressTest
{
	std::string name;
	std::string mail;
	int id;
	std::string number;
	int type;
};

int main()
{
	unsigned long long tag = 0ULL;
	tag |= (1ULL << 1);
	bool bAllCheckOK = false;
	do 
	{
		TestBase test;
		if (!test.CheckLenght()) break;
		cout << endl;
		if (!test.CheckAttachProtocol()) break;
		cout << endl;
		if (!test.CheckNoAttachProtocol()) break;
		cout << endl;
		if (!test.CheckRouteProtocol()) break;
		cout << endl << endl;
		bAllCheckOK = true;
	} while (0);
	cout << endl;
	if (bAllCheckOK) cout << "all check OK . " << endl;
	else cout << "check failed . " << endl;

	cout << "check http proto ..." << endl;
	TestHTTP th;
	WriteHTTP whGet;
	whGet.addHead("Content-Type", "application/x-www-form-urlencoded");
	whGet.addHead("Host", "www.google.com");
	whGet.get("/");
	th.Test(whGet);

	WriteHTTP whPost;
	whPost.addHead("Content-Type", "application/x-www-form-urlencoded");
	whPost.addHead("Host", "www.google.com");
	whPost.post("/", "index.php?aaa=333");
	th.Test(whPost);

	WriteHTTP whResult;
	whResult.addHead("test", "test");
	whResult.addHead("Content-Type", "application/x-www-form-urlencoded");
	whResult.addHead("Host", "www.google.com");
	whResult.response("200", "");
	th.Test(whResult);



	char buf[1000];
	StressTest t1;
	StressTest t2;
	StressTest t3;
	StressTest t4;
	unsigned int now = GetTimeMillisecond();
	int len = 0;
	for( int i=0; i<100*10000; i++)
	{
		WriteStream ws(100, buf, 1000);
		{
			
			//StressTest t1;
			t1.name = "Alice";
			t1.id = 10000;
			t1.mail = "fdaf";
			t1.number = "12345#@fdsafdsaf.com";
			t1.type = 2;
			//StressTest t2 = t1;
			t2 = t1;
			ws << t1.name << t1.id << t1.mail << t1.number << t1.type << t2.name << t2.id << t2.mail << t2.number << t2.type;
			if(i == 0) len = ws.getStreamLen();
			ReadStream rs(ws.getStream(), ws.getStreamLen());
			StressTest t3;
			StressTest t4;
			rs >> t3.name >> t3.id >> t3.mail >>t3.number >> t3.type  >> t4.name >> t4.id >> t4.mail >> t4.number >> t4.type;
		}
	}	
	std::cout << "all:" << GetTimeMillisecond() - now << std::endl;

	now = GetTimeMillisecond();
	for (int i = 0; i < 100 * 10000; i++)
	{
		WriteStream ws(100, buf, 1000);
		{

			//StressTest t1;
			t1.name = "Alice";
			t1.id = 10000;
			t1.mail = "fdafa@fsadf.com";
			t1.number = "123456789";
			t1.type = 2;
			//StressTest t2 = t1;
			t2 = t1;
			ws << t1.name << t1.id << t1.mail << t1.number << t1.type << t2.name << t2.id << t2.mail << t2.number << t2.type;
		}
	}
	std::cout << "encode:" << GetTimeMillisecond() - now << std::endl;
	
	now = GetTimeMillisecond();
	WriteStream ws(100);
	//StressTest t1;
	t1.name = "Alice";
	t1.id = 10000;
	t1.mail = "fdafa@fsadf.com";
	t1.number = "123456789";
	t1.type = 2;
	//StressTest t2 = t1;
	t2 = t1;
	ws << t1.name << t1.id << t1.mail << t1.number << t1.type << t2.name << t2.id << t2.mail << t2.number << t2.type;
	for (int i = 0; i < 100 * 10000; i++)
	{

		{
			ReadStream rs(ws.getStream(), ws.getStreamLen());
			//StressTest t3;
			//StressTest t4;
			rs >> t3.name >> t3.id >> t3.mail >> t3.number >> t3.type >> t4.name >> t4.id >> t4.mail >> t4.number >> t4.type;
		}
	}
	std::cout << "decode:" << GetTimeMillisecond() - now << std::endl;


	std::cout << len << std::endl;
	cout << "press any key to continue." << endl;
	getchar();
	return 0;
}


unsigned int GetTimeMillisecond()
{
#ifdef WIN32
	return ::GetTickCount();
#else
	struct timeval tm;
	gettimeofday(&tm, NULL);
	return (tm.tv_sec * 1000 + (tm.tv_usec / 1000));
#endif
}

