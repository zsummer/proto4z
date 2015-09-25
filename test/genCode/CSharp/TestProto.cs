 
namespace Proto4z  
{ 
    public class EC_SUCCESS //success  
    { 
        public const System.UInt16 value = 0;  
    } 
    public class EC_ERROR //error  
    { 
        public const System.UInt16 value = 1;  
    } 
    public class EC_INVALID_PARAM //invalid param  
    { 
        public const System.UInt16 value = 1000;  
    } 
    public class EC_UNKNOWN_ID //unknwon id  
    { 
        public const System.UInt16 value = 1001;  
    } 
 
    public class TestIntegerData: Proto4z.IProtoObject //测试  
    {     
        //members   
        public System.Char _char;  
        public System.Byte _uchar;  
        public System.Int16 _short;  
        public System.UInt16 _ushort;  
        public System.Int32 _int;  
        public System.UInt32 _uint;  
        public System.Int64 _i64;  
        public System.UInt64 _ui128;  
        public System.UInt64 _ui64;  
        public TestIntegerData()  
        { 
            _char = '\0';  
            _uchar = 0;  
            _short = 0;  
            _ushort = 0;  
            _int = 0;  
            _uint = 0;  
            _i64 = 0;  
            _ui128 = 0;  
            _ui64 = 0;  
        } 
        public TestIntegerData(System.Char _char, System.Byte _uchar, System.Int16 _short, System.UInt16 _ushort, System.Int32 _int, System.UInt32 _uint, System.Int64 _i64, System.UInt64 _ui128, System.UInt64 _ui64) 
        { 
            this._char = _char; 
            this._uchar = _uchar; 
            this._short = _short; 
            this._ushort = _ushort; 
            this._int = _int; 
            this._uint = _uint; 
            this._i64 = _i64; 
            this._ui128 = _ui128; 
            this._ui64 = _ui64; 
        } 
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
            data.AddRange(Proto4z.BaseProtoObject.encodeUI64(_ui64)); 
            return data; 
        } 
        public int __decode(byte[] binData, ref int pos) 
        { 
            this._char = Proto4z.BaseProtoObject.decodeI8(binData, ref pos); 
            this._uchar = Proto4z.BaseProtoObject.decodeUI8(binData, ref pos); 
            this._short = Proto4z.BaseProtoObject.decodeI16(binData, ref pos); 
            this._ushort = Proto4z.BaseProtoObject.decodeUI16(binData, ref pos); 
            this._int = Proto4z.BaseProtoObject.decodeI32(binData, ref pos); 
            this._uint = Proto4z.BaseProtoObject.decodeUI32(binData, ref pos); 
            this._i64 = Proto4z.BaseProtoObject.decodeI64(binData, ref pos); 
            this._ui64 = Proto4z.BaseProtoObject.decodeUI64(binData, ref pos); 
            return pos; 
        } 
    } 
 
    public class TestFloatData: Proto4z.IProtoObject //测试  
    {     
        //members   
        public System.Single _float;  
        public System.Double _double;  
        public TestFloatData()  
        { 
            _float = 0.0f;  
            _double = 0.0;  
        } 
        public TestFloatData(System.Single _float, System.Double _double) 
        { 
            this._float = _float; 
            this._double = _double; 
        } 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var data = new System.Collections.Generic.List<byte>(); 
            data.AddRange(Proto4z.BaseProtoObject.encodeSingle(_float)); 
            data.AddRange(Proto4z.BaseProtoObject.encodeDouble(_double)); 
            return data; 
        } 
        public int __decode(byte[] binData, ref int pos) 
        { 
            this._float = Proto4z.BaseProtoObject.decodeSingle(binData, ref pos); 
            this._double = Proto4z.BaseProtoObject.decodeDouble(binData, ref pos); 
            return pos; 
        } 
    } 
 
    public class TestStringData: Proto4z.IProtoObject //测试  
    {     
        //members   
        public System.String _string;  
        public TestStringData()  
        { 
            _string = "";  
        } 
        public TestStringData(System.String _string) 
        { 
            this._string = _string; 
        } 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var data = new System.Collections.Generic.List<byte>(); 
            data.AddRange(Proto4z.BaseProtoObject.encodeString(_string)); 
            return data; 
        } 
        public int __decode(byte[] binData, ref int pos) 
        { 
            this._string = Proto4z.BaseProtoObject.decodeString(binData, ref pos); 
            return pos; 
        } 
    } 
 
    public class IntArray : System.Collections.Generic.List<System.UInt32>, Proto4z.IProtoObject  
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
 
    public class TestIntegerDataArray : System.Collections.Generic.List<TestIntegerData>, Proto4z.IProtoObject  
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
 
    public class TestFloatDataArray : System.Collections.Generic.List<TestFloatData>, Proto4z.IProtoObject  
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
 
    public class TestStringDataArray : System.Collections.Generic.List<TestStringData>, Proto4z.IProtoObject  
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
 
    public class TestIntegerDataMap : System.Collections.Generic.Dictionary<System.String, TestIntegerData>, Proto4z.IProtoObject  
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
 
    public class TestFloatDataMap : System.Collections.Generic.Dictionary<System.String, TestFloatData>, Proto4z.IProtoObject  
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
 
    public class TestStringDataMap : System.Collections.Generic.Dictionary<System.String, TestStringData>, Proto4z.IProtoObject  
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
 
    public class EchoPack: Proto4z.IProtoObject 
    {     
        //proto id   
        public const System.UInt16 protoID = 30000;  
        static public System.UInt16 getProtoID() { return 30000; } 
        static public string getProtoName() { return "EchoPack"; } 
        //members   
        public TestIntegerDataArray _iarray;  
        public TestFloatDataArray _farray;  
        public TestStringDataArray _sarray;  
        public TestIntegerDataMap _imap;  
        public TestFloatDataMap _fmap;  
        public TestStringDataMap _smap;  
        public EchoPack()  
        { 
            _iarray = new TestIntegerDataArray();  
            _farray = new TestFloatDataArray();  
            _sarray = new TestStringDataArray();  
            _imap = new TestIntegerDataMap();  
            _fmap = new TestFloatDataMap();  
            _smap = new TestStringDataMap();  
        } 
        public EchoPack(TestIntegerDataArray _iarray, TestFloatDataArray _farray, TestStringDataArray _sarray, TestIntegerDataMap _imap, TestFloatDataMap _fmap, TestStringDataMap _smap) 
        { 
            this._iarray = _iarray; 
            this._farray = _farray; 
            this._sarray = _sarray; 
            this._imap = _imap; 
            this._fmap = _fmap; 
            this._smap = _smap; 
        } 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var data = new System.Collections.Generic.List<byte>(); 
            if (_iarray == null) _iarray = new TestIntegerDataArray(); 
            data.AddRange(_iarray.__encode()); 
            if (_farray == null) _farray = new TestFloatDataArray(); 
            data.AddRange(_farray.__encode()); 
            if (_sarray == null) _sarray = new TestStringDataArray(); 
            data.AddRange(_sarray.__encode()); 
            if (_imap == null) _imap = new TestIntegerDataMap(); 
            data.AddRange(_imap.__encode()); 
            if (_fmap == null) _fmap = new TestFloatDataMap(); 
            data.AddRange(_fmap.__encode()); 
            if (_smap == null) _smap = new TestStringDataMap(); 
            data.AddRange(_smap.__encode()); 
            return data; 
        } 
        public int __decode(byte[] binData, ref int pos) 
        { 
            this._iarray = new TestIntegerDataArray(); 
            this._iarray.__decode(binData, ref pos); 
            this._farray = new TestFloatDataArray(); 
            this._farray.__decode(binData, ref pos); 
            this._sarray = new TestStringDataArray(); 
            this._sarray.__decode(binData, ref pos); 
            this._imap = new TestIntegerDataMap(); 
            this._imap.__decode(binData, ref pos); 
            this._fmap = new TestFloatDataMap(); 
            this._fmap.__decode(binData, ref pos); 
            this._smap = new TestStringDataMap(); 
            this._smap.__decode(binData, ref pos); 
            return pos; 
        } 
    } 
 
    public class Pulse: Proto4z.IProtoObject 
    {     
        //proto id   
        public const System.UInt16 protoID = 30001;  
        static public System.UInt16 getProtoID() { return 30001; } 
        static public string getProtoName() { return "Pulse"; } 
        //members   
        public Pulse()  
        { 
        } 
        public System.Collections.Generic.List<byte> __encode() 
        { 
            var data = new System.Collections.Generic.List<byte>(); 
            return data; 
        } 
        public int __decode(byte[] binData, ref int pos) 
        { 
            return pos; 
        } 
    } 
 
} 
 
 
