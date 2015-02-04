-- proto4z test file
-- Auther: zhangyawei
-- mail:yawei_zhang@foxmail.com
-- date: 2015-01-12

package.path =  "../../?.lua;" .. package.path
--require
require("proto4z")
require("lua.ControlProto")
--const value
Protoz.InvalidUserID = 0xffffffff

Protoz.EC_SUCCESS = 0
Protoz.EC_ERROR = 1

--array
Protoz.Items = {}
Protoz.Items.__getID = nil
Protoz.Items.__getName = "Items"
Protoz.Items.__getDesc = "array"
Protoz.Items.__getTypeK = nil
Protoz.Items.__getTypeV = "ui32"

--map
Protoz.Heros = {}
Protoz.Heros.__getID = nil
Protoz.Heros.__getName = "Heros"
Protoz.Heros.__getDesc = "map"
Protoz.Heros.__getTypeK = "ui32"
Protoz.Heros.__getTypeV = "string"

--struct
Protoz.UserInfo = {}
Protoz.UserInfo.__getID = nil
Protoz.UserInfo.__getName = "UserInfo"
Protoz.UserInfo.__getDesc = nil
Protoz.UserInfo.__getTypeK = nil
Protoz.UserInfo.__getTypeV = nil

Protoz.UserInfo[1] = {name = "_uID", type = "ui32"}
Protoz.UserInfo[2] = {name = "_user", type = "string"}
Protoz.UserInfo[3] = {name = "_items", type = "Items"}
Protoz.UserInfo[4] = {name = "_heros", type = "Heros", del=true}


--proto
Protoz.LoginResult = {}
Protoz.LoginResult.__getID = 100
Protoz.LoginResult.__getName = "LoginResult"
Protoz.LoginResult.__getDesc = nil
Protoz.LoginResult.__getTypeK = nil
Protoz.LoginResult.__getTypeV = nil

Protoz.register(100, "LoginResult")

Protoz.LoginResult[1] = {name = "_retCode", type = "ui32"}
Protoz.LoginResult[2] = {name = "_info", type = "UserInfo"}


local result = {_retCode = 100,
					_info = {_uID = 200, _user = "username", _items = {1,2,3}, _heros = {{k=100, v="hero1"},{k=200, v="hero2"}} 
					 } 
				}

Protoz.dump(result)

local now = os.time()
--for i=1, 100000 do

	local data = Protoz.encode(result, "LoginResult")
	local dr = Protoz.decode(data, Protoz.getName(Protoz.LoginResult.__getID)) -- "LoginResult"
--end
now = os.time() - now
--print(now)
Protoz.putbin(data)


Protoz.dump(dr)




-------------

local notify = {serverID = 100, info = {user="user", pwd="pwd", accID="12345678"}}
local data = Protoz.encode(notify, "XS2OS_ServerInitNotify")
print(Protoz.XS2OS_ServerInitNotify.__getID)
local dr = Protoz.decode(data, Protoz.getName(Protoz.XS2OS_ServerInitNotify.__getID))
Protoz.putbin(data)
Protoz.dump(dr)