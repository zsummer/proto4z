 
namespace Proto4z  
{ 
    class STATIC_EC_SUCCESS //success  
    { 
        public static System.UInt16 value = 0;  
    } 
    class STATIC_EC_ERROR //error  
    { 
        public static System.UInt16 value = 1;  
    } 
    class STATIC_EC_INVALID_PARAM //invalid param  
    { 
        public static System.UInt16 value = 1000;  
    } 
    class STATIC_EC_UNKNOWN_ID //unknwon id  
    { 
        public static System.UInt16 value = 1001;  
    } 
 
    class TestIntegerData: Proto4z.IProtoObject //测试  
    {     
        public System.Char _char;  
        public System.Byte _uchar;  
        public System.Int16 _short;  
        public System.UInt16 _ushort;  
        public System.Int32 _int;  
        public System.UInt32 _uint;  
        public System.Int64 _i64;  
        public System.UInt64 _ui128;  
        public System.UInt64 _ui64;  
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var data = new System.Collections.Generic.List<byte>(); 
            data.AddRange(Proto4z.BaseProtoObject.encodeI8(_char)); 
            data.AddRange(Proto4z.BaseProtoObject.encodeUI8(_uchar)); 
            data.AddRange(Proto4z.BaseProtoObject.encodeI16(_short)); 
            data.AddRange(Proto4z.BaseProtoObject.encodeUI16(_ushort)); 
            data.AddRange(Proto4z.BaseProtoObject.encodeI32(_int)); 
            data.AddRange(Proto4z.BaseProtoObject.encodeUI32(_uint)); 
            data.AddRange(Proto4z.BaseProtoObject.encodeI64(_i64)); 
//            data.AddRange(Proto4z.BaseProtoObject.encodeUI64(_ui128)); 
            data.AddRange(Proto4z.BaseProtoObject.encodeUI64(_ui64)); 
            var ret = new System.Collections.Generic.List<byte>(); 
            ret.AddRange(data); 
            return ret; 
        } 
        public int __decode(byte[] binData, ref int pos) 
        { 
            _char = '\0'; 
                _char = Proto4z.BaseProtoObject.decodeI8(binData, ref pos); 
            _uchar = 0; 
                _uchar = Proto4z.BaseProtoObject.decodeUI8(binData, ref pos); 
            _short = 0; 
                _short = Proto4z.BaseProtoObject.decodeI16(binData, ref pos); 
            _ushort = 0; 
                _ushort = Proto4z.BaseProtoObject.decodeUI16(binData, ref pos); 
            _int = 0; 
                _int = Proto4z.BaseProtoObject.decodeI32(binData, ref pos); 
            _uint = 0; 
                _uint = Proto4z.BaseProtoObject.decodeUI32(binData, ref pos); 
            _i64 = 0; 
                _i64 = Proto4z.BaseProtoObject.decodeI64(binData, ref pos); 
            _ui128 = 0; 
                _ui128 = Proto4z.BaseProtoObject.decodeUI64(binData, ref pos); 
            _ui64 = 0; 
                _ui64 = Proto4z.BaseProtoObject.decodeUI64(binData, ref pos); 
            return pos; 
        } 
    } 
 
    class TestFloatData: Proto4z.IProtoObject //测试  
    {     
        public System.Single _float;  
        public System.Double _double;  
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var data = new System.Collections.Generic.List<byte>(); 
            data.AddRange(Proto4z.BaseProtoObject.encodeSingle(_float)); 
            data.AddRange(Proto4z.BaseProtoObject.encodeDouble(_double)); 
            var ret = new System.Collections.Generic.List<byte>(); 
            ret.AddRange(data); 
            return ret; 
        } 
        public int __decode(byte[] binData, ref int pos) 
        { 
            _float = 0.0F; 
                _float = Proto4z.BaseProtoObject.decodeSingle(binData, ref pos); 
            _double = 0.0; 
                _double = Proto4z.BaseProtoObject.decodeDouble(binData, ref pos); 
            return pos; 
        } 
    } 
 
    class TestStringData: Proto4z.IProtoObject //测试  
    {     
        public System.String _string;  
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var data = new System.Collections.Generic.List<byte>(); 
            data.AddRange(Proto4z.BaseProtoObject.encodeString(_string)); 
            var ret = new System.Collections.Generic.List<byte>(); 
            ret.AddRange(data); 
            return ret; 
        } 
        public int __decode(byte[] binData, ref int pos) 
        { 
                _string = Proto4z.BaseProtoObject.decodeString(binData, ref pos); 
            return pos; 
        } 
    } 
 
    class IntArray : System.Collections.Generic.List<System.UInt32>, Proto4z.IProtoObject  
    { 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var ret = new System.Collections.Generic.List<byte>(); 
            System.Int32 len = (System.Int32)this.Count; 
            ret.AddRange(Proto4z.BaseProtoObject.encodeI32(len)); 
            for (int i = 0; i < this.Count; i++ ) 
            { 
                ret.AddRange(Proto4z.BaseProtoObject.encodeUI32(this[i]));  
            } 
            return ret; 
        } 
 
        public int __decode(byte[] binData, ref int pos) 
        { 
            System.Int32 len = Proto4z.BaseProtoObject.decodeI32(binData, ref pos); 
            if(len > 0) 
            { 
                for (int i=0; i<len; i++) 
                { 
                    this.Add(Proto4z.BaseProtoObject.decodeUI32(binData, ref pos)); 
                } 
            } 
            return pos; 
        } 
    } 
 
    class TestIntegerDataArray : System.Collections.Generic.List<TestIntegerData>, Proto4z.IProtoObject  
    { 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var ret = new System.Collections.Generic.List<byte>(); 
            System.Int32 len = (System.Int32)this.Count; 
            ret.AddRange(Proto4z.BaseProtoObject.encodeI32(len)); 
            for (int i = 0; i < this.Count; i++ ) 
            { 
                ret.AddRange(this[i].__encode()); 
            } 
            return ret; 
        } 
 
        public int __decode(byte[] binData, ref int pos) 
        { 
            System.Int32 len = Proto4z.BaseProtoObject.decodeI32(binData, ref pos); 
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
 
    class TestFloatDataArray : System.Collections.Generic.List<TestFloatData>, Proto4z.IProtoObject  
    { 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var ret = new System.Collections.Generic.List<byte>(); 
            System.Int32 len = (System.Int32)this.Count; 
            ret.AddRange(Proto4z.BaseProtoObject.encodeI32(len)); 
            for (int i = 0; i < this.Count; i++ ) 
            { 
                ret.AddRange(this[i].__encode()); 
            } 
            return ret; 
        } 
 
        public int __decode(byte[] binData, ref int pos) 
        { 
            System.Int32 len = Proto4z.BaseProtoObject.decodeI32(binData, ref pos); 
            if(len > 0) 
            { 
                for (int i=0; i<len; i++) 
                { 
                    var data = new TestFloatData(); 
                    data.__decode(binData, ref pos); 
                    this.Add(data); 
                } 
            } 
            return pos; 
        } 
    } 
 
    class TestStringDataArray : System.Collections.Generic.List<TestStringData>, Proto4z.IProtoObject  
    { 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var ret = new System.Collections.Generic.List<byte>(); 
            System.Int32 len = (System.Int32)this.Count; 
            ret.AddRange(Proto4z.BaseProtoObject.encodeI32(len)); 
            for (int i = 0; i < this.Count; i++ ) 
            { 
                ret.AddRange(this[i].__encode()); 
            } 
            return ret; 
        } 
 
        public int __decode(byte[] binData, ref int pos) 
        { 
            System.Int32 len = Proto4z.BaseProtoObject.decodeI32(binData, ref pos); 
            if(len > 0) 
            { 
                for (int i=0; i<len; i++) 
                { 
                    var data = new TestStringData(); 
                    data.__decode(binData, ref pos); 
                    this.Add(data); 
                } 
            } 
            return pos; 
        } 
    } 
 
    class TestIntegerDataMap : System.Collections.Generic.Dictionary<System.String, TestIntegerData>, Proto4z.IProtoObject  
    { 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var ret = new System.Collections.Generic.List<byte>(); 
            System.Int32 len = (System.Int32)this.Count; 
            ret.AddRange(Proto4z.BaseProtoObject.encodeI32(len)); 
            foreach(var kv in this) 
            { 
                ret.AddRange(Proto4z.BaseProtoObject.encodeString(kv.Key)); 
                ret.AddRange(kv.Value.__encode()); 
            } 
            return ret; 
        } 
 
        public int __decode(byte[] binData, ref int pos) 
        { 
            System.Int32 len = Proto4z.BaseProtoObject.decodeI32(binData, ref pos); 
            if(len > 0) 
            { 
                for (int i=0; i<len; i++) 
                { 
                    var key = Proto4z.BaseProtoObject.decodeString(binData, ref pos); 
                    var val = new TestIntegerData(); 
                    val.__decode(binData, ref pos); 
                    this.Add(key, val); 
                } 
            } 
            return pos; 
        } 
    } 
 
    class TestFloatDataMap : System.Collections.Generic.Dictionary<System.String, TestFloatData>, Proto4z.IProtoObject  
    { 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var ret = new System.Collections.Generic.List<byte>(); 
            System.Int32 len = (System.Int32)this.Count; 
            ret.AddRange(Proto4z.BaseProtoObject.encodeI32(len)); 
            foreach(var kv in this) 
            { 
                ret.AddRange(Proto4z.BaseProtoObject.encodeString(kv.Key)); 
                ret.AddRange(kv.Value.__encode()); 
            } 
            return ret; 
        } 
 
        public int __decode(byte[] binData, ref int pos) 
        { 
            System.Int32 len = Proto4z.BaseProtoObject.decodeI32(binData, ref pos); 
            if(len > 0) 
            { 
                for (int i=0; i<len; i++) 
                { 
                    var key = Proto4z.BaseProtoObject.decodeString(binData, ref pos); 
                    var val = new TestFloatData(); 
                    val.__decode(binData, ref pos); 
                    this.Add(key, val); 
                } 
            } 
            return pos; 
        } 
    } 
 
    class TestStringDataMap : System.Collections.Generic.Dictionary<System.String, TestStringData>, Proto4z.IProtoObject  
    { 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var ret = new System.Collections.Generic.List<byte>(); 
            System.Int32 len = (System.Int32)this.Count; 
            ret.AddRange(Proto4z.BaseProtoObject.encodeI32(len)); 
            foreach(var kv in this) 
            { 
                ret.AddRange(Proto4z.BaseProtoObject.encodeString(kv.Key)); 
                ret.AddRange(kv.Value.__encode()); 
            } 
            return ret; 
        } 
 
        public int __decode(byte[] binData, ref int pos) 
        { 
            System.Int32 len = Proto4z.BaseProtoObject.decodeI32(binData, ref pos); 
            if(len > 0) 
            { 
                for (int i=0; i<len; i++) 
                { 
                    var key = Proto4z.BaseProtoObject.decodeString(binData, ref pos); 
                    var val = new TestStringData(); 
                    val.__decode(binData, ref pos); 
                    this.Add(key, val); 
                } 
            } 
            return pos; 
        } 
    } 
 
    class EchoPack: Proto4z.IProtoObject 
    {     
        static public System.UInt16 getProtoID() { return 30000; } 
        static public string getProtoName() { return "EchoPack"; } 
        public TestIntegerDataArray _iarray;  
        public TestFloatDataArray _farray;  
        public TestStringDataArray _sarray;  
        public TestIntegerDataMap _imap;  
        public TestFloatDataMap _fmap;  
        public TestStringDataMap _smap;  
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var data = new System.Collections.Generic.List<byte>(); 
            data.AddRange(_iarray.__encode()); 
            data.AddRange(_farray.__encode()); 
            data.AddRange(_sarray.__encode()); 
            data.AddRange(_imap.__encode()); 
            data.AddRange(_fmap.__encode()); 
            data.AddRange(_smap.__encode()); 
            var ret = new System.Collections.Generic.List<byte>(); 
            ret.AddRange(data); 
            return ret; 
        } 
        public int __decode(byte[] binData, ref int pos) 
        { 
            _iarray = new TestIntegerDataArray(); 
                _iarray.__decode(binData, ref pos); 
            _farray = new TestFloatDataArray(); 
                _farray.__decode(binData, ref pos); 
            _sarray = new TestStringDataArray(); 
                _sarray.__decode(binData, ref pos); 
            _imap = new TestIntegerDataMap(); 
                _imap.__decode(binData, ref pos); 
            _fmap = new TestFloatDataMap(); 
                _fmap.__decode(binData, ref pos); 
            _smap = new TestStringDataMap(); 
                _smap.__decode(binData, ref pos); 
            return pos; 
        } 
    } 
 
    class Pulse: Proto4z.IProtoObject 
    {     
        static public System.UInt16 getProtoID() { return 30001; } 
        static public string getProtoName() { return "Pulse"; } 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var data = new System.Collections.Generic.List<byte>(); 
            var ret = new System.Collections.Generic.List<byte>(); 
            ret.AddRange(data); 
            return ret; 
        } 
        public int __decode(byte[] binData, ref int pos) 
        { 
            return pos; 
        } 
    } 
 
} 
 
 
