
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
* Copyright (C) 2013 YaweiZhang <yawei_zhang@foxmail.com>.
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







using System;

using Proto4z;



class STATIC_EC_SUCCESS //global const 
{
    public static ui16 value = new ui16(100);
}
class STATIC_EC_ERROR //global const 
{
    public static ui16 value = new ui16(100);
}
class HeroInfo : IProtoObject//struct
{
    public i32 id;
    public Proto4z.String name;
    public System.Collections.Generic.List<Byte> __encode()
    {
        var ret = new System.Collections.Generic.List<Byte>();
        ret.AddRange(id.__encode());
        ret.AddRange(name.__encode());
        return ret;
    }
    public Int32 __decode(byte[] binData, ref Int32 pos)
    {
        id = new i32(0);
        name = new Proto4z.String("");
        id.__decode(binData, ref pos);
        name.__decode(binData, ref pos);
        return pos;
    }
}

class HeroInfoDict : System.Collections.Generic.Dictionary<i32, HeroInfo> , IProtoObject
{
    public System.Collections.Generic.List<Byte> __encode()
    {
        var ret = new System.Collections.Generic.List<Byte>();
        var len = new ui16((UInt16)this.Count);
        ret.AddRange(len.__encode());
        foreach(var kv in this)
        {
            ret.AddRange(kv.Key.__encode());
            ret.AddRange(kv.Value.__encode());
        }
        return ret;
    }
    public Int32 __decode(byte[] binData, ref Int32 pos)
    {
        var len = new ui16(0);
        len.__decode(binData, ref pos);
        if (len.val > 0)
        {
            for (int i = 0; i < len.val; i++)
            {
                var key = new i32(0);
                var val = new HeroInfo();
                key.__decode(binData, ref pos);
                val.__decode(binData, ref pos);
                this.Add(key, val);
            }
        }
        return pos;
    }
}
class HeroInfoArray : System.Collections.Generic.List<HeroInfo>, IProtoObject
{
    public System.Collections.Generic.List<Byte> __encode()
    {
        var ret = new System.Collections.Generic.List<Byte>();
        var len = new ui16((UInt16)this.Count);
        ret.AddRange(len.__encode());
        for (int i = 0; i < this.Count; i++ )
        {
            ret.AddRange(this[i].__encode());
        }
        return ret;
    }
    public Int32 __decode(byte[] binData, ref Int32 pos)
    {
        var len = new ui16(0);
        len.__decode(binData, ref pos);
        if(len.val > 0)
        {
            for (int i=0; i<len.val; i++)
            {
                var data = new HeroInfo();
                data.__decode(binData, ref pos);
                this.Add(data);
            }
        }
        return pos;
    }
}
class UserInfo : IProtoObject//struct
{
    public ui64 uid;
    public HeroInfo hero;
    public HeroInfoDict dictHeros;
    public HeroInfoArray arrayHeros;
    public System.Collections.Generic.List<Byte> __encode()
    {
        var ret = new System.Collections.Generic.List<Byte>();
        ret.AddRange(uid.__encode());
        ret.AddRange(hero.__encode());
        ret.AddRange(dictHeros.__encode());
        ret.AddRange(arrayHeros.__encode());
        return ret;
    }
    public Int32 __decode(byte[] binData, ref Int32 pos)
    {
        uid = new ui64(0);
        hero = new HeroInfo();
        dictHeros = new HeroInfoDict();
        arrayHeros = new HeroInfoArray();
        uid.__decode(binData, ref pos);
        hero.__decode(binData, ref pos);
        dictHeros.__decode(binData, ref pos);
        arrayHeros.__decode(binData, ref pos);
        return pos;
    }
}

class LS2C_LoginResult: IProtoObject
{
    public ui16 retCode;
    public UserInfo info;
    public System.Collections.Generic.List<Byte> __encode()
    {
        var ret = new System.Collections.Generic.List<Byte>();
        ret.AddRange(retCode.__encode());
        ret.AddRange(info.__encode());
        return ret;
    }
    public Int32 __decode(byte[] binData, ref Int32 pos)
    {
        retCode = new ui16(0);
        info = new UserInfo();
        retCode.__decode(binData, ref pos);
        info.__decode(binData, ref pos);
        return pos;
    }
}




namespace ConsoleApplication2
{
    class Program
    {
        static void Main(string[] args)
        {
            LS2C_LoginResult result = new LS2C_LoginResult();
            result.retCode = new ui16(0);
            result.info = new UserInfo();
            result.info.uid = new ui64(100);
            result.info.hero = new HeroInfo();
            result.info.hero.id = new i32(100);
            result.info.hero.name = new Proto4z.String("hero");

            HeroInfo hero1 = new HeroInfo();
            hero1.id = new i32(100);
            hero1.name = new Proto4z.String("hero1");

            HeroInfo hero2 = new HeroInfo();
            hero2.id = new i32(101);
            hero2.name = new Proto4z.String("hero2");


            result.info.dictHeros = new HeroInfoDict();
            result.info.dictHeros.Add(hero1.id, hero1);
            result.info.dictHeros.Add(hero2.id, hero2);

            result.info.arrayHeros = new HeroInfoArray();
            result.info.arrayHeros.Add(hero1);
            result.info.arrayHeros.Add(hero2);

            RC4Encryption rc4Server = new RC4Encryption();
            RC4Encryption rc4Client = new RC4Encryption();
            rc4Server.makeSBox("zhangyawei");
            rc4Client.makeSBox("zhangyawei");


            var byteData = result.__encode();
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

            var v = new LS2C_LoginResult();

            int pos = 0;
            v.__decode(binData, ref pos);
        }
    }
}
