Protoz.DBServer = 1 
 Protoz.GameServer = 2 
 Protoz.BattleServer = 3--BattleServer 
 Protoz.CenterServer = 4 
 Protoz.AgentServer = 5 
 Protoz.EC_SUCCESS = 0--success 
 Protoz.EC_ERROR = 1--error 
 Protoz.EC_INVALID_PARAM = 1000--invalid param 
 Protoz.EC_UNKNOWN_ID = 1001--unknwon id 
  
 Protoz.AuthInfo = {} --认证信息 
 Protoz.AuthInfo.__getName = "AuthInfo" 
 Protoz.AuthInfo[1] = {name="user", type="string"}  
 Protoz.AuthInfo[2] = {name="pwd", type="string"}  
 Protoz.AuthInfo[3] = {name="accID", type="ui64"} --帐号ID 
  
 Protoz.intVct = {} -- -- - -- 
 Protoz.intVct.__getName = "intVct" 
 Protoz.intVct.__getDesc = "array" 
 Protoz.intVct.__getTypeV = "ui32" 
  
 Protoz.AuthInfoVct = {}  
 Protoz.AuthInfoVct.__getName = "AuthInfoVct" 
 Protoz.AuthInfoVct.__getDesc = "array" 
 Protoz.AuthInfoVct.__getTypeV = "AuthInfo" 
  
 Protoz.MapUserAuth = {} --22323 
 Protoz.MapUserAuth.__getName = "MapUserAuth" 
 Protoz.MapUserAuth.__getDesc = "map" 
 Protoz.MapUserAuth.__getTypeK = "string" 
 Protoz.MapUserAuth.__getTypeV = "AuthInfoVct" 
  
 Protoz.register(30000,"XS2OS_ServerInitNotify")Protoz.XS2OS_ServerInitNotify = {} --服务初始化通知 
 Protoz.XS2OS_ServerInitNotify.__getID = 30000 
 Protoz.XS2OS_ServerInitNotify.__getName = "XS2OS_ServerInitNotify" 
 Protoz.XS2OS_ServerInitNotify[1] = {name="serverID", type="i16"}  
 Protoz.XS2OS_ServerInitNotify[2] = {name="info", type="AuthInfo"}  
  
 Protoz.register(30001,"XS2OS_StopServer")Protoz.XS2OS_StopServer = {} --服务关闭通知 
 Protoz.XS2OS_StopServer.__getID = 30001 
 Protoz.XS2OS_StopServer.__getName = "XS2OS_StopServer" 
  
 Protoz.register(30002,"XS2OS_ServerPulse")Protoz.XS2OS_ServerPulse = {} --服务心跳 
 Protoz.XS2OS_ServerPulse.__getID = 30002 
 Protoz.XS2OS_ServerPulse.__getName = "XS2OS_ServerPulse" 
 