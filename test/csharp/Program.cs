
/*
* proto4z License
* -----------
*
* proto4z is licensed under the terms of the MIT license reproduced below.
* This means that proto4z is free software and can be used for both academic
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




using Proto4z;
using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;  





namespace ConsoleApplication2
{
    class Client
    {
        class NetHeader : IProtoObject
        {
            public Proto4z.ui32 packLen;
            public Proto4z.ui16 protoID;
            public System.Collections.Generic.List<byte> __encode()
            {
                var ret = new System.Collections.Generic.List<byte>();
                ret.AddRange(packLen.__encode());
                ret.AddRange(protoID.__encode());
                return ret;
            }
            public System.Int32 __decode(byte[] binData, ref System.Int32 pos)
            {
                packLen = new Proto4z.ui32();
                protoID = new Proto4z.ui16();
                packLen.__decode(binData, ref pos);
                protoID.__decode(binData, ref pos);
                return pos;
            }
        }
 

        public void Run(byte[] binData)
        {

            IPAddress ip = IPAddress.Parse("127.0.0.1");
            Socket clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                clientSocket.Connect(new IPEndPoint(ip, 8081)); //zsummerX/example/frameStressTest server default port 
                Console.WriteLine("connect Success.");
            }
            catch
            {
                Console.WriteLine("connect Failed");
                return;
            }
            do
            {
                var sendData = new System.Collections.Generic.List<byte>();

                NetHeader head = new NetHeader();
                head.packLen = (UInt32)(4 + 2 + binData.Length);
                head.protoID = Proto4z.EchoPack.getProtoID();

                sendData.AddRange(head.__encode());
                sendData.AddRange(binData);
                clientSocket.Send(sendData.ToArray());


                
                
                var recvBytes = new byte[2000];
                uint curLen = 0;
                uint needLen = 4 + 2;
                uint recvLen = 0;
                NetHeader recvHead = new NetHeader();
                do
                {
                    recvLen = (uint)clientSocket.Receive(recvBytes, (int)curLen, (int)needLen, System.Net.Sockets.SocketFlags.None);
                    if (recvLen == 0)
                    {
                        return;
                    }
                    curLen += recvLen;
                    needLen -= recvLen;
                    if (needLen == 0 && curLen == 4 + 2) //head already read finish
                    {
                        int pos = 0;
                        recvHead.__decode(recvBytes, ref pos);
                        needLen = recvHead.packLen.val - 4 - 2;
                    }
                    else if (needLen == 0)
                    {
                        if (recvHead.protoID == Proto4z.EchoPack.getProtoID())
                        {
                            Proto4z.EchoPack result = new Proto4z.EchoPack();
                            int pos = 4+2;
                            result.__decode(recvBytes, ref pos);
                            //System.Console.WriteLine("echo =" + result.text.val);
                        }
                        break;
                    }
                    recvLen = 0;
                } while (true);


            } while (true);

        }
    }  

    class Program
    {
        static void Main(string[] args)
        {
            RC4Encryption rc4Server = new RC4Encryption();
            RC4Encryption rc4Client = new RC4Encryption();
            rc4Server.makeSBox("zhangyawei");
            rc4Client.makeSBox("zhangyawei");
            TestIntegerData idata = new TestIntegerData();
            idata._char = 'a';
            idata._uchar = 1;
            idata._short = 5;
            idata._ushort = 20;
            idata._int = 30;
            idata._uint = 40;
            idata._i64 = 60;
            idata._ui64 = 80;

            TestFloatData fdata = new TestFloatData();
            fdata._float = 243.2222F;
            fdata._double = 3423.11123;
            TestStringData sdata = new TestStringData();
            sdata._string = "love";

            EchoPack pack = new EchoPack();
            pack._iarray = new Proto4z.TestIntegerDataArray();
            pack._iarray.Add(idata);
            pack._iarray.Add(idata);
            pack._iarray.Add(idata);
            pack._farray = new Proto4z.TestFloatDataArray();
            pack._farray.Add(fdata);
            pack._farray.Add(fdata);
            pack._farray.Add(fdata);
            pack._sarray = new Proto4z.TestStringDataArray();
            pack._sarray.Add(sdata);
            pack._sarray.Add(sdata);
            pack._sarray.Add(sdata);

            pack._imap = new Proto4z.TestIntegerDataMap();
            pack._imap.Add("123", idata);
            pack._imap.Add("223", idata);

            pack._fmap = new Proto4z.TestFloatDataMap();
            pack._fmap.Add("523", fdata);
            pack._fmap.Add("623", fdata);
            pack._smap = new Proto4z.TestStringDataMap();
            pack._smap.Add("723", sdata);
            pack._smap.Add("823", sdata);
            var byteData = pack.__encode();
            var binData = byteData.ToArray();

            for (int i = 0; i < binData.Length; i++)
            {
                System.Console.WriteLine((int)binData[i]);
            }
            rc4Server.encryption(binData, binData.Length);
            rc4Client.encryption(binData, binData.Length);
            for (int i = 0; i < binData.Length; i++)
            {
                System.Console.WriteLine((int)binData[i]);
            }

            var v = new EchoPack();

            int pos = 0;
            v.__decode(binData, ref pos);


            Client client = new Client();
            client.Run(binData);







        }
    }
}
