 
 #ifndef _CONTROLPROTO_H_ 
 #define _CONTROLPROTO_H_ 
  
 const unsigned short DBServer = 1;  
 const unsigned short GameServer = 2;  
 const unsigned short BattleServer = 3; //BattleServer 
 const unsigned short CenterServer = 4;  
 const unsigned short AgentServer = 5;  
 const unsigned short EC_SUCCESS = 0; //success 
 const unsigned short EC_ERROR = 1; //error 
 const unsigned short EC_INVALID_PARAM = 1000; //invalid param 
 const unsigned short EC_UNKNOWN_ID = 1001; //unknwon id 
  
 struct AuthInfo //认证信息 
 { 
 	std::string user;  
 	std::string pwd;  
 	unsigned long long accID; //帐号ID 
 	AuthInfo() 
 	{ 
 		accID = 0; 
 	} 
 }; 
 template<class T> 
 T & operator << (T & t, const AuthInfo & data) 
 { 
 	t << data.user; 
 	t << data.pwd; 
 	t << data.accID; 
 	return t; 
 } 
 template<class T> 
 T & operator >> (T & t, AuthInfo & data) 
 { 
 	t >> data.user; 
 	t >> data.pwd; 
 	t >> data.accID; 
 	return t; 
 } 
  
 typedef std::vector<unsigned int> intVct; // -- - -- 
  
 typedef std::vector<AuthInfo> AuthInfoVct;  
  
 typedef std::map<std::string, AuthInfoVct> MapUserAuth; //22323 
  
 const unsigned short ID_XS2OS_ServerInitNotify = 30000; //服务初始化通知 
 struct XS2OS_ServerInitNotify //服务初始化通知 
 { 
 	short serverID;  
 	AuthInfo info;  
 	XS2OS_ServerInitNotify() 
 	{ 
 		serverID = 0; 
 	} 
 	inline unsigned short GetProtoID() { return 30000;} 
 	inline std::string GetProtoName() { return "ID_XS2OS_ServerInitNotify";} 
 }; 
 template<class T> 
 T & operator << (T & t, const XS2OS_ServerInitNotify & data) 
 { 
 	t << data.serverID; 
 	t << data.info; 
 	return t; 
 } 
 template<class T> 
 T & operator >> (T & t, XS2OS_ServerInitNotify & data) 
 { 
 	t >> data.serverID; 
 	t >> data.info; 
 	return t; 
 } 
  
 const unsigned short ID_XS2OS_StopServer = 30001; //服务关闭通知 
 struct XS2OS_StopServer //服务关闭通知 
 { 
 	inline unsigned short GetProtoID() { return 30001;} 
 	inline std::string GetProtoName() { return "ID_XS2OS_StopServer";} 
 }; 
 template<class T> 
 T & operator << (T & t, const XS2OS_StopServer & data) 
 { 
 	return t; 
 } 
 template<class T> 
 T & operator >> (T & t, XS2OS_StopServer & data) 
 { 
 	return t; 
 } 
  
 const unsigned short ID_XS2OS_ServerPulse = 30002; //服务心跳 
 struct XS2OS_ServerPulse //服务心跳 
 { 
 	inline unsigned short GetProtoID() { return 30002;} 
 	inline std::string GetProtoName() { return "ID_XS2OS_ServerPulse";} 
 }; 
 template<class T> 
 T & operator << (T & t, const XS2OS_ServerPulse & data) 
 { 
 	return t; 
 } 
 template<class T> 
 T & operator >> (T & t, XS2OS_ServerPulse & data) 
 { 
 	return t; 
 } 
  
 #endif 
 