 
namespace Proto4z  
{ 
	class STATIC_EC_SUCCESS //success 
	{ 
		public static Proto4z.ui16 value = 0;  
	} 
	class STATIC_EC_ERROR //error 
	{ 
		public static Proto4z.ui16 value = 1;  
	} 
	class STATIC_EC_INVALID_PARAM //invalid param 
	{ 
		public static Proto4z.ui16 value = 1000;  
	} 
	class STATIC_EC_UNKNOWN_ID //unknwon id 
	{ 
		public static Proto4z.ui16 value = 1001;  
	} 
 
	class TestIntegerData: Proto4z.IProtoObject //测试 
	{	 
		public Proto4z.i8 _char;  
		public Proto4z.ui8 _uchar;  
		public Proto4z.i16 _short;  
		public Proto4z.ui16 _ushort;  
		public Proto4z.i32 _int;  
		public Proto4z.ui32 _uint;  
		public Proto4z.i64 _i64;  
		public Proto4z.ui64 _ui128;  
		public Proto4z.ui64 _ui64;  
		public System.Collections.Generic.List<byte> __encode() 
		{ 
			Proto4z.ui32 sttLen = 0; 
			 Proto4z.ui64 tag = 383; 
			 
			var data = new System.Collections.Generic.List<byte>(); 
			data.AddRange(_char.__encode()); 
			data.AddRange(_uchar.__encode()); 
			data.AddRange(_short.__encode()); 
			data.AddRange(_ushort.__encode()); 
			data.AddRange(_int.__encode()); 
			data.AddRange(_uint.__encode()); 
			data.AddRange(_i64.__encode()); 
//			data.AddRange(_ui128.__encode());//[already deleted] 
			data.AddRange(_ui64.__encode()); 
			sttLen = (System.UInt32)data.Count + 8;			var ret = new System.Collections.Generic.List<byte>(); 
			ret.AddRange(sttLen.__encode()); 
			ret.AddRange(tag.__encode()); 
			ret.AddRange(data); 
			return ret; 
		} 
		public int __decode(byte[] binData, ref int pos) 
		{ 
			Proto4z.ui32 offset = 0; 
			Proto4z.ui64 tag = 0; 
			offset.__decode(binData, ref pos); 
			offset.val += (System.UInt32)pos; 
			tag.__decode(binData, ref pos); 
			_char = new Proto4z.i8(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_char.__decode(binData, ref pos); 
			} 
			_uchar = new Proto4z.ui8(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_uchar.__decode(binData, ref pos); 
			} 
			_short = new Proto4z.i16(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_short.__decode(binData, ref pos); 
			} 
			_ushort = new Proto4z.ui16(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_ushort.__decode(binData, ref pos); 
			} 
			_int = new Proto4z.i32(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_int.__decode(binData, ref pos); 
			} 
			_uint = new Proto4z.ui32(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_uint.__decode(binData, ref pos); 
			} 
			_i64 = new Proto4z.i64(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_i64.__decode(binData, ref pos); 
			} 
			_ui128 = new Proto4z.ui64(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_ui128.__decode(binData, ref pos); 
			} 
			_ui64 = new Proto4z.ui64(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_ui64.__decode(binData, ref pos); 
			} 
			return (int)offset.val; 
		} 
	} 
 
	class TestFloatData: Proto4z.IProtoObject //测试 
	{	 
		public Proto4z.Float _float;  
		public Proto4z.Double _double;  
		public System.Collections.Generic.List<byte> __encode() 
		{ 
			Proto4z.ui32 sttLen = 0; 
			 Proto4z.ui64 tag = 3; 
			 
			var data = new System.Collections.Generic.List<byte>(); 
			data.AddRange(_float.__encode()); 
			data.AddRange(_double.__encode()); 
			sttLen = (System.UInt32)data.Count + 8;			var ret = new System.Collections.Generic.List<byte>(); 
			ret.AddRange(sttLen.__encode()); 
			ret.AddRange(tag.__encode()); 
			ret.AddRange(data); 
			return ret; 
		} 
		public int __decode(byte[] binData, ref int pos) 
		{ 
			Proto4z.ui32 offset = 0; 
			Proto4z.ui64 tag = 0; 
			offset.__decode(binData, ref pos); 
			offset.val += (System.UInt32)pos; 
			tag.__decode(binData, ref pos); 
			_float = new Proto4z.Float(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_float.__decode(binData, ref pos); 
			} 
			_double = new Proto4z.Double(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_double.__decode(binData, ref pos); 
			} 
			return (int)offset.val; 
		} 
	} 
 
	class TestStringData: Proto4z.IProtoObject //测试 
	{	 
		public Proto4z.String _string;  
		public System.Collections.Generic.List<byte> __encode() 
		{ 
			Proto4z.ui32 sttLen = 0; 
			 Proto4z.ui64 tag = 1; 
			 
			var data = new System.Collections.Generic.List<byte>(); 
			data.AddRange(_string.__encode()); 
			sttLen = (System.UInt32)data.Count + 8;			var ret = new System.Collections.Generic.List<byte>(); 
			ret.AddRange(sttLen.__encode()); 
			ret.AddRange(tag.__encode()); 
			ret.AddRange(data); 
			return ret; 
		} 
		public int __decode(byte[] binData, ref int pos) 
		{ 
			Proto4z.ui32 offset = 0; 
			Proto4z.ui64 tag = 0; 
			offset.__decode(binData, ref pos); 
			offset.val += (System.UInt32)pos; 
			tag.__decode(binData, ref pos); 
			_string = new Proto4z.String(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_string.__decode(binData, ref pos); 
			} 
			return (int)offset.val; 
		} 
	} 
 
	class IntArray : System.Collections.Generic.List<Proto4z.ui32>, Proto4z.IProtoObject  
	{ 
		public System.Collections.Generic.List<byte> __encode() 
		{ 
			var ret = new System.Collections.Generic.List<byte>(); 
			 var len = new Proto4z.ui32((System.UInt32)this.Count); 
			ret.AddRange(len.__encode()); 
			for (int i = 0; i < this.Count; i++ ) 
			{ 
				ret.AddRange(this[i].__encode()); 
			} 
			return ret; 
		} 
 
		public int __decode(byte[] binData, ref int pos) 
		{ 
			var len = new Proto4z.ui32(0); 
			len.__decode(binData, ref pos); 
			if(len.val > 0) 
			{ 
				for (int i=0; i<len.val; i++) 
				{ 
					var data = new Proto4z.ui32(); 
					 data.__decode(binData, ref pos); 
					this.Add(data); 
				} 
			} 
			return pos; 
		} 
	} 
 
	class TestIntegerDataArray : System.Collections.Generic.List<Proto4z.TestIntegerData>, Proto4z.IProtoObject  
	{ 
		public System.Collections.Generic.List<byte> __encode() 
		{ 
			var ret = new System.Collections.Generic.List<byte>(); 
			 var len = new Proto4z.ui32((System.UInt32)this.Count); 
			ret.AddRange(len.__encode()); 
			for (int i = 0; i < this.Count; i++ ) 
			{ 
				ret.AddRange(this[i].__encode()); 
			} 
			return ret; 
		} 
 
		public int __decode(byte[] binData, ref int pos) 
		{ 
			var len = new Proto4z.ui32(0); 
			len.__decode(binData, ref pos); 
			if(len.val > 0) 
			{ 
				for (int i=0; i<len.val; i++) 
				{ 
					var data = new Proto4z.TestIntegerData(); 
					 data.__decode(binData, ref pos); 
					this.Add(data); 
				} 
			} 
			return pos; 
		} 
	} 
 
	class TestFloatDataArray : System.Collections.Generic.List<Proto4z.TestFloatData>, Proto4z.IProtoObject  
	{ 
		public System.Collections.Generic.List<byte> __encode() 
		{ 
			var ret = new System.Collections.Generic.List<byte>(); 
			 var len = new Proto4z.ui32((System.UInt32)this.Count); 
			ret.AddRange(len.__encode()); 
			for (int i = 0; i < this.Count; i++ ) 
			{ 
				ret.AddRange(this[i].__encode()); 
			} 
			return ret; 
		} 
 
		public int __decode(byte[] binData, ref int pos) 
		{ 
			var len = new Proto4z.ui32(0); 
			len.__decode(binData, ref pos); 
			if(len.val > 0) 
			{ 
				for (int i=0; i<len.val; i++) 
				{ 
					var data = new Proto4z.TestFloatData(); 
					 data.__decode(binData, ref pos); 
					this.Add(data); 
				} 
			} 
			return pos; 
		} 
	} 
 
	class TestStringDataArray : System.Collections.Generic.List<Proto4z.TestStringData>, Proto4z.IProtoObject  
	{ 
		public System.Collections.Generic.List<byte> __encode() 
		{ 
			var ret = new System.Collections.Generic.List<byte>(); 
			 var len = new Proto4z.ui32((System.UInt32)this.Count); 
			ret.AddRange(len.__encode()); 
			for (int i = 0; i < this.Count; i++ ) 
			{ 
				ret.AddRange(this[i].__encode()); 
			} 
			return ret; 
		} 
 
		public int __decode(byte[] binData, ref int pos) 
		{ 
			var len = new Proto4z.ui32(0); 
			len.__decode(binData, ref pos); 
			if(len.val > 0) 
			{ 
				for (int i=0; i<len.val; i++) 
				{ 
					var data = new Proto4z.TestStringData(); 
					 data.__decode(binData, ref pos); 
					this.Add(data); 
				} 
			} 
			return pos; 
		} 
	} 
 
	class TestIntegerDataMap : System.Collections.Generic.Dictionary<Proto4z.String, Proto4z.TestIntegerData>, Proto4z.IProtoObject  
	{ 
		public System.Collections.Generic.List<byte> __encode() 
		{ 
			var ret = new System.Collections.Generic.List<byte>(); 
			 var len = new Proto4z.ui32((System.UInt32)this.Count); 
			ret.AddRange(len.__encode()); 
			foreach(var kv in this) 
			{ 
				ret.AddRange(kv.Key.__encode()); 
				ret.AddRange(kv.Value.__encode()); 
			} 
			return ret; 
		} 
 
		public int __decode(byte[] binData, ref int pos) 
		{ 
			var len = new Proto4z.ui16(0); 
			len.__decode(binData, ref pos); 
			if(len.val > 0) 
			{ 
				for (int i=0; i<len.val; i++) 
				{ 
					var key = new Proto4z.String(); 
					var val = new Proto4z.TestIntegerData(); 
					key.__decode(binData, ref pos); 
					val.__decode(binData, ref pos); 
					this.Add(key, val); 
				} 
			} 
			return pos; 
		} 
	} 
 
	class TestFloatDataMap : System.Collections.Generic.Dictionary<Proto4z.String, Proto4z.TestFloatData>, Proto4z.IProtoObject  
	{ 
		public System.Collections.Generic.List<byte> __encode() 
		{ 
			var ret = new System.Collections.Generic.List<byte>(); 
			 var len = new Proto4z.ui32((System.UInt32)this.Count); 
			ret.AddRange(len.__encode()); 
			foreach(var kv in this) 
			{ 
				ret.AddRange(kv.Key.__encode()); 
				ret.AddRange(kv.Value.__encode()); 
			} 
			return ret; 
		} 
 
		public int __decode(byte[] binData, ref int pos) 
		{ 
			var len = new Proto4z.ui16(0); 
			len.__decode(binData, ref pos); 
			if(len.val > 0) 
			{ 
				for (int i=0; i<len.val; i++) 
				{ 
					var key = new Proto4z.String(); 
					var val = new Proto4z.TestFloatData(); 
					key.__decode(binData, ref pos); 
					val.__decode(binData, ref pos); 
					this.Add(key, val); 
				} 
			} 
			return pos; 
		} 
	} 
 
	class TestStringDataMap : System.Collections.Generic.Dictionary<Proto4z.String, Proto4z.TestStringData>, Proto4z.IProtoObject  
	{ 
		public System.Collections.Generic.List<byte> __encode() 
		{ 
			var ret = new System.Collections.Generic.List<byte>(); 
			 var len = new Proto4z.ui32((System.UInt32)this.Count); 
			ret.AddRange(len.__encode()); 
			foreach(var kv in this) 
			{ 
				ret.AddRange(kv.Key.__encode()); 
				ret.AddRange(kv.Value.__encode()); 
			} 
			return ret; 
		} 
 
		public int __decode(byte[] binData, ref int pos) 
		{ 
			var len = new Proto4z.ui16(0); 
			len.__decode(binData, ref pos); 
			if(len.val > 0) 
			{ 
				for (int i=0; i<len.val; i++) 
				{ 
					var key = new Proto4z.String(); 
					var val = new Proto4z.TestStringData(); 
					key.__decode(binData, ref pos); 
					val.__decode(binData, ref pos); 
					this.Add(key, val); 
				} 
			} 
			return pos; 
		} 
	} 
 
	class P2P_EchoPack: Proto4z.IProtoObject 
	{	 
		 public Proto4z.ui16 getProtoID() { return new Proto4z.ui16(30000); } 
		  public string getProtoName() { return "P2P_EchoPack"; } 
		public Proto4z.TestIntegerDataArray _iarray;  
		public Proto4z.TestFloatDataArray _farray;  
		public Proto4z.TestStringDataArray _sarray;  
		public Proto4z.TestIntegerDataMap _imap;  
		public Proto4z.TestFloatDataMap _fmap;  
		public Proto4z.TestStringDataMap _smap;  
		public System.Collections.Generic.List<byte> __encode() 
		{ 
			Proto4z.ui32 sttLen = 0; 
			 Proto4z.ui64 tag = 63; 
			 
			var data = new System.Collections.Generic.List<byte>(); 
			data.AddRange(_iarray.__encode()); 
			data.AddRange(_farray.__encode()); 
			data.AddRange(_sarray.__encode()); 
			data.AddRange(_imap.__encode()); 
			data.AddRange(_fmap.__encode()); 
			data.AddRange(_smap.__encode()); 
			sttLen = (System.UInt32)data.Count + 8;			var ret = new System.Collections.Generic.List<byte>(); 
			ret.AddRange(sttLen.__encode()); 
			ret.AddRange(tag.__encode()); 
			ret.AddRange(data); 
			return ret; 
		} 
		public int __decode(byte[] binData, ref int pos) 
		{ 
			Proto4z.ui32 offset = 0; 
			Proto4z.ui64 tag = 0; 
			offset.__decode(binData, ref pos); 
			offset.val += (System.UInt32)pos; 
			tag.__decode(binData, ref pos); 
			_iarray = new Proto4z.TestIntegerDataArray(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_iarray.__decode(binData, ref pos); 
			} 
			_farray = new Proto4z.TestFloatDataArray(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_farray.__decode(binData, ref pos); 
			} 
			_sarray = new Proto4z.TestStringDataArray(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_sarray.__decode(binData, ref pos); 
			} 
			_imap = new Proto4z.TestIntegerDataMap(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_imap.__decode(binData, ref pos); 
			} 
			_fmap = new Proto4z.TestFloatDataMap(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_fmap.__decode(binData, ref pos); 
			} 
			_smap = new Proto4z.TestStringDataMap(); 
			if ((tag.val & ((System.UInt64)1 << 0)) != 0) 
			{ 
				_smap.__decode(binData, ref pos); 
			} 
			return (int)offset.val; 
		} 
	} 
 
	class C2S_Pulse: Proto4z.IProtoObject 
	{	 
		 public Proto4z.ui16 getProtoID() { return new Proto4z.ui16(30001); } 
		  public string getProtoName() { return "C2S_Pulse"; } 
		public System.Collections.Generic.List<byte> __encode() 
		{ 
			Proto4z.ui32 sttLen = 0; 
			 Proto4z.ui64 tag = 0; 
			 
			var data = new System.Collections.Generic.List<byte>(); 
			sttLen = (System.UInt32)data.Count + 8;			var ret = new System.Collections.Generic.List<byte>(); 
			ret.AddRange(sttLen.__encode()); 
			ret.AddRange(tag.__encode()); 
			ret.AddRange(data); 
			return ret; 
		} 
		public int __decode(byte[] binData, ref int pos) 
		{ 
			Proto4z.ui32 offset = 0; 
			Proto4z.ui64 tag = 0; 
			offset.__decode(binData, ref pos); 
			offset.val += (System.UInt32)pos; 
			tag.__decode(binData, ref pos); 
			return (int)offset.val; 
		} 
	} 
 
	class S2C_Pulse: Proto4z.IProtoObject 
	{	 
		 public Proto4z.ui16 getProtoID() { return new Proto4z.ui16(30002); } 
		  public string getProtoName() { return "S2C_Pulse"; } 
		public System.Collections.Generic.List<byte> __encode() 
		{ 
			Proto4z.ui32 sttLen = 0; 
			 Proto4z.ui64 tag = 0; 
			 
			var data = new System.Collections.Generic.List<byte>(); 
			sttLen = (System.UInt32)data.Count + 8;			var ret = new System.Collections.Generic.List<byte>(); 
			ret.AddRange(sttLen.__encode()); 
			ret.AddRange(tag.__encode()); 
			ret.AddRange(data); 
			return ret; 
		} 
		public int __decode(byte[] binData, ref int pos) 
		{ 
			Proto4z.ui32 offset = 0; 
			Proto4z.ui64 tag = 0; 
			offset.__decode(binData, ref pos); 
			offset.val += (System.UInt32)pos; 
			tag.__decode(binData, ref pos); 
			return (int)offset.val; 
		} 
	} 
 
} 
 
 
