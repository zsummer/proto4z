Welcome to the proto4z wiki!  
## Introduction:    
[![Build Status](https://travis-ci.org/zsummer/proto4z.svg?branch=master)](https://travis-ci.org/zsummer/proto4z)  
proto4z  is an efficient serialization library for C++, lua, C#, It's like Google protocol buffers.  
proto4z是一个序列化工具库, 可以通过一次编写xml格式的数据结构, 使用genProto工具一键生成对应的C++,lua,C#的原生数据结构和对应的序列化/反序列化代码, 并且会额外生成C++使用的MYSQL增删改查的接口代码.  
proto4z提供C++的HTTP协议处理接口.  

##example  
  
###xml configure  
```
<?xml version="1.0" encoding="utf-8"?>
<ProtoTraits>
    <MinNo>30000</MinNo>
    <MaxNo>32000</MaxNo>
</ProtoTraits>
<Proto>
    <!-- 整形-->
    <struct name="TestIntegerData" store="true" desc="测试">
        <member name="_uint"    type="ui32" tag="key"    desc=""/>
        <member name="_i64"    type="i64" tag="key"     desc=""/>
        <member name="_ui64"    type="ui64"    tag="ignore" desc=""/>
    </struct>
    <!-- 数组类型-->
    <array name="TestIntegerDataArray" type="TestIntegerData" desc=""/>
    <!-- 协议类型-->
    <proto  name="EchoPack" desc= "">
        <member name="_iarray" type="TestIntegerDataArray"     desc=""/>
        <member name="_f" type="float"     desc=""/>
        <member name="_s" type="string"     desc=""/>
    </proto>
</Proto>
```
###lua code  
**generated code** 
```  
Proto4z.TestIntegerData = {} --测试 
Proto4z.TestIntegerData.__getName = "TestIntegerData" 
Proto4z.TestIntegerData[1] = {name="_uint", type="ui32" }  
Proto4z.TestIntegerData[2] = {name="_i64", type="i64" }  
Proto4z.TestIntegerData[3] = {name="_ui64", type="ui64" }  
 
Proto4z.TestIntegerDataArray = {}  
Proto4z.TestIntegerDataArray.__getName = "TestIntegerDataArray" 
Proto4z.TestIntegerDataArray.__getDesc = "array" 
Proto4z.TestIntegerDataArray.__getTypeV = "TestIntegerData" 
 
Proto4z.register(30000,"EchoPack") 
Proto4z.EchoPack = {}  
Proto4z.EchoPack.__getID = 30000 
Proto4z.EchoPack.__getName = "EchoPack" 
Proto4z.EchoPack[1] = {name="_iarray", type="TestIntegerDataArray" }  
Proto4z.EchoPack[2] = {name="_f", type="float" }  
Proto4z.EchoPack[3] = {name="_s", type="string" }  
```   
**test code**  
```
package.path =package.path ..  ";../../?.lua;../genCode/lua/?.lua"
--require
require("proto4z")
require("TestProto")
local dump = Proto4z.dump
local proto = Proto4z

local echo = {  _iarray = {{_uint=6,_i64=12345678.2,_ui64=12345678},{_uint=6,_i64="123456789000000.2",_ui64="1123122345678.0"}},
				_f =2.235,
				_s = "abcdefg"
				}


	local data = proto.encode(echo, "EchoPack")
	local dr = proto.decode(data, proto.getName(proto.EchoPack.__getID))

dump(dr)
```

###c++ code  
**generated code**
```
#ifndef _TESTPROTO_H_ 
#define _TESTPROTO_H_ 
struct TestIntegerData //测试  
{ 
    unsigned int _uint;  
    long long _i64;  
    unsigned long long _ui64;  
    TestIntegerData() 
    { 
        _uint = 0; 
        _i64 = 0; 
        _ui64 = 0; 
    } 
    TestIntegerData(const unsigned int & _uint, const long long & _i64, const unsigned long long & _ui64) 
    { 
        this->_uint = _uint; 
        this->_i64 = _i64; 
        this->_ui64 = _ui64; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const TestIntegerData & data) 
{ 
    ws << data._uint;  
    ws << data._i64;  
    ws << data._ui64;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, TestIntegerData & data) 
{ 
    rs >> data._uint;  
    rs >> data._i64;  
    rs >> data._ui64;  
    return rs; 
} 
 
 
typedef std::vector<TestIntegerData> TestIntegerDataArray;  
struct EchoPack 
{ 
    static const unsigned short GetProtoID() { return 30000;} 
    static const std::string GetProtoName() { return "ID_EchoPack";} 
    TestIntegerDataArray _iarray;  
    float _f;  
    std::string _s;  
    EchoPack() 
    { 
        _f = 0.0; 
    } 
    EchoPack(const TestIntegerDataArray & _iarray, const float & _f, const std::string & _s) 
    { 
        this->_iarray = _iarray; 
        this->_f = _f; 
        this->_s = _s; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const EchoPack & data) 
{ 
    ws << data._iarray;  
    ws << data._f;  
    ws << data._s;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, EchoPack & data) 
{ 
    rs >> data._iarray;  
    rs >> data._f;  
    rs >> data._s;  
    return rs; 
} 
#endif 
```
**test code**
```
    try
    {
        WriteStream ws(100);
        EchoPack echo({ { 1, 2, 3 }, { 2, 8, 0 } }, 1.0f, "a");
        ws << echo;
        ReadStream rs(ws.getStream(), ws.getStreamLen());
        EchoPack result;
        rs >> result;
        cout << "success" << endl;
    }
    catch (std::runtime_error & e)
    {
        cout << "error:" << e.what() << endl;
    }
```
###c# code
**generated code**
```
namespace Proto4z  
{ 
 
    public class TestIntegerData: Proto4z.IProtoObject //测试  
    {     
        //members   
        public uint _uint;  
        public long _i64;  
        public ulong _ui64;  
        public TestIntegerData()  
        { 
            _uint = 0;  
            _i64 = 0;  
            _ui64 = 0;  
        } 
        public TestIntegerData(uint _uint, long _i64, ulong _ui64) 
        { 
            this._uint = _uint; 
            this._i64 = _i64; 
            this._ui64 = _ui64; 
        } 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var data = new System.Collections.Generic.List<byte>(); 
            data.AddRange(Proto4z.BaseProtoObject.encodeUI32(this._uint)); 
            data.AddRange(Proto4z.BaseProtoObject.encodeI64(this._i64)); 
            data.AddRange(Proto4z.BaseProtoObject.encodeUI64(this._ui64)); 
            return data; 
        } 
        public int __decode(byte[] binData, ref int pos) 
        { 
            this._uint = Proto4z.BaseProtoObject.decodeUI32(binData, ref pos); 
            this._i64 = Proto4z.BaseProtoObject.decodeI64(binData, ref pos); 
            this._ui64 = Proto4z.BaseProtoObject.decodeUI64(binData, ref pos); 
            return pos; 
        } 
    } 
 
 
    public class TestIntegerDataArray : System.Collections.Generic.List<TestIntegerData>, Proto4z.IProtoObject  
    { 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var ret = new System.Collections.Generic.List<byte>(); 
            int len = (int)this.Count; 
            ret.AddRange(Proto4z.BaseProtoObject.encodeI32(len)); 
            for (int i = 0; i < this.Count; i++ ) 
            { 
                ret.AddRange(this[i].__encode()); 
            } 
            return ret; 
        } 
 
        public int __decode(byte[] binData, ref int pos) 
        { 
            int len = Proto4z.BaseProtoObject.decodeI32(binData, ref pos); 
            if(len > 0) 
            { 
                for (int i=0; i<len; i++) 
                { 
                    var data = new TestIntegerData(); 
                    data.__decode(binData, ref pos); 
                    this.Add(data); 
                } 
            } 
            return pos; 
        } 
    } 
 
    public class EchoPack: Proto4z.IProtoObject 
    {     
        //proto id   
        public const ushort protoID = 30000;  
        static public ushort getProtoID() { return 30000; } 
        static public string getProtoName() { return "EchoPack"; } 
        //members   
        public TestIntegerDataArray _iarray;  
        public float _f;  
        public string _s;  
        public EchoPack()  
        { 
            _iarray = new TestIntegerDataArray();  
            _f = 0.0f;  
            _s = "";  
        } 
        public EchoPack(TestIntegerDataArray _iarray, float _f, string _s) 
        { 
            this._iarray = _iarray; 
            this._f = _f; 
            this._s = _s; 
        } 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var data = new System.Collections.Generic.List<byte>(); 
            if (this._iarray == null) this._iarray = new TestIntegerDataArray(); 
            data.AddRange(this._iarray.__encode()); 
            data.AddRange(Proto4z.BaseProtoObject.encodeSingle(this._f)); 
            data.AddRange(Proto4z.BaseProtoObject.encodeString(this._s)); 
            return data; 
        } 
        public int __decode(byte[] binData, ref int pos) 
        { 
            this._iarray = new TestIntegerDataArray(); 
            this._iarray.__decode(binData, ref pos); 
            this._f = Proto4z.BaseProtoObject.decodeSingle(binData, ref pos); 
            this._s = Proto4z.BaseProtoObject.decodeString(binData, ref pos); 
            return pos; 
        } 
    } 
} 
```
**test code**
```
    EchoPack pack = new EchoPack(null, 1.0f, "aaa");
    pack._iarray = new TestIntegerDataArray();
    pack._iarray.Add(new TestIntegerData(1, 2, 3));
    pack._iarray.Add(new TestIntegerData(2, 3, 4));
    var binData = pack.__encode().ToArray();
    var result = new EchoPack();
    int pos = 0;
    result.__decode(binData, ref pos);
```



#About The Author  
Auther: YaweiZhang  
Mail: yawei.zhang@foxmail.com  
GitHub: https://github.com/zsummer  
