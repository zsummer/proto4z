 
 namespace Proto4z  
 { 
 	class STATIC_DBServer  
 	{ 
 		public static Proto4z.ui16 value = 1;  
 	} 
 	class STATIC_GameServer  
 	{ 
 		public static Proto4z.ui16 value = 2;  
 	} 
 	class STATIC_BattleServer //BattleServer 
 	{ 
 		public static Proto4z.ui16 value = 3;  
 	} 
 	class STATIC_CenterServer  
 	{ 
 		public static Proto4z.ui16 value = 4;  
 	} 
 	class STATIC_AgentServer  
 	{ 
 		public static Proto4z.ui16 value = 5;  
 	} 
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
  
 	class AuthInfo: Proto4z.IProtoObject //认证信息 
 	{	 
 		public Proto4z.String user;  
 		public Proto4z.String pwd;  
 		public Proto4z.ui64 accID; //帐号ID 
 		public System.Collections.Generic.List<byte> __encode() 
 		{ 
 			var ret = new System.Collections.Generic.List<byte>(); 
 			ret.AddRange(user.__encode()); 
 			ret.AddRange(pwd.__encode()); 
 			ret.AddRange(accID.__encode()); 
 			return ret; 
 		} 
 		public int __decode(byte[] binData, ref int pos) 
 		{ 
 			user = new Proto4z.String(); 
 			user.__decode(binData, ref pos); 
 			pwd = new Proto4z.String(); 
 			pwd.__decode(binData, ref pos); 
 			accID = new Proto4z.ui64(); 
 			accID.__decode(binData, ref pos); 
 			return pos; 
 		} 
 	} 
  
 	class intVct : System.Collections.Generic.List<Proto4z.ui32>, Proto4z.IProtoObject // -- - -- 
 	{ 
 		public System.Collections.Generic.List<byte> __encode() 
 		{ 
 			var ret = new System.Collections.Generic.List<byte>(); 
 			 var len = new Proto4z.ui16((System.UInt16)this.Count); 
 			ret.AddRange(len.__encode()); 
 			for (int i = 0; i < this.Count; i++ ) 
 			{ 
 				ret.AddRange(this[i].__encode()); 
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
 					var data = new Proto4z.ui32(); 
 					 data.__decode(binData, ref pos); 
 					this.Add(data); 
 				} 
 			} 
 			return pos; 
 		} 
 	} 
  
 	class AuthInfoVct : System.Collections.Generic.List<Proto4z.AuthInfo>, Proto4z.IProtoObject  
 	{ 
 		public System.Collections.Generic.List<byte> __encode() 
 		{ 
 			var ret = new System.Collections.Generic.List<byte>(); 
 			 var len = new Proto4z.ui16((System.UInt16)this.Count); 
 			ret.AddRange(len.__encode()); 
 			for (int i = 0; i < this.Count; i++ ) 
 			{ 
 				ret.AddRange(this[i].__encode()); 
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
 					var data = new Proto4z.AuthInfo(); 
 					 data.__decode(binData, ref pos); 
 					this.Add(data); 
 				} 
 			} 
 			return pos; 
 		} 
 	} 
  
 	class MapUserAuth : System.Collections.Generic.Dictionary<Proto4z.String, Proto4z.AuthInfoVct>, Proto4z.IProtoObject  
 	{ 
 		public System.Collections.Generic.List<byte> __encode() 
 		{ 
 			var ret = new System.Collections.Generic.List<byte>(); 
 			 var len = new Proto4z.ui16((System.UInt16)this.Count); 
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
 					var val = new Proto4z.AuthInfoVct(); 
 					key.__decode(binData, ref pos); 
 					val.__decode(binData, ref pos); 
 					this.Add(key, val); 
 				} 
 			} 
 			return pos; 
 		} 
 	} 
  
 	class XS2OS_ServerInitNotify: Proto4z.IProtoObject //服务初始化通知 
 	{	 
 		 public Proto4z.ui16 getProtoID() { return new Proto4z.ui16(30000); } 
 		  public string getProtoName() { return "XS2OS_ServerInitNotify"; } 
 		public Proto4z.i16 serverID;  
 		public Proto4z.AuthInfo info;  
 		public System.Collections.Generic.List<byte> __encode() 
 		{ 
 			var ret = new System.Collections.Generic.List<byte>(); 
 			ret.AddRange(serverID.__encode()); 
 			ret.AddRange(info.__encode()); 
 			return ret; 
 		} 
 		public int __decode(byte[] binData, ref int pos) 
 		{ 
 			serverID = new Proto4z.i16(); 
 			serverID.__decode(binData, ref pos); 
 			info = new Proto4z.AuthInfo(); 
 			info.__decode(binData, ref pos); 
 			return pos; 
 		} 
 	} 
  
 	class XS2OS_StopServer: Proto4z.IProtoObject //服务关闭通知 
 	{	 
 		 public Proto4z.ui16 getProtoID() { return new Proto4z.ui16(30001); } 
 		  public string getProtoName() { return "XS2OS_StopServer"; } 
 		public System.Collections.Generic.List<byte> __encode() 
 		{ 
 			var ret = new System.Collections.Generic.List<byte>(); 
 			return ret; 
 		} 
 		public int __decode(byte[] binData, ref int pos) 
 		{ 
 			return pos; 
 		} 
 	} 
  
 	class XS2OS_ServerPulse: Proto4z.IProtoObject //服务心跳 
 	{	 
 		 public Proto4z.ui16 getProtoID() { return new Proto4z.ui16(30002); } 
 		  public string getProtoName() { return "XS2OS_ServerPulse"; } 
 		public System.Collections.Generic.List<byte> __encode() 
 		{ 
 			var ret = new System.Collections.Generic.List<byte>(); 
 			return ret; 
 		} 
 		public int __decode(byte[] binData, ref int pos) 
 		{ 
 			return pos; 
 		} 
 	} 
  
 } 
  
  
 