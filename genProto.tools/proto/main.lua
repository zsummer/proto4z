-- proto4z test file
-- Auther: zhangyawei
-- mail:yawei_zhang@foxmail.com
-- date: 2015-01-12


require("proto4z")

Protoz.InvalidUserID = 0xffffffff

Protoz.EC_SUCCESS = 0
Protoz.EC_ERROR = 1

Protoz.Hero = {}
Protoz.Hero.__getName = "Hero"
Protoz.Hero[1] = {name = "_name", type = "normal", vtype = "string"}

Protoz.UserInfo = {}
Protoz.UserInfo.__getName = "UserInfo"
Protoz.UserInfo[1] = {name = "_uID", type = "normal", vtype = "ui32"}
Protoz.UserInfo[2] = {name = "_user", type = "normal", vtype = "string"}
Protoz.UserInfo[3] = {name = "_items", type = "array", vtype = "ui32"}
Protoz.UserInfo[4] = {name = "_heros", type = "array", vtype = "Hero"}


Protoz.LoginResult = {}

Protoz.LoginResult.__getName = "LoginResult"
Protoz.LoginResult.__getID = 100
Protoz.register(Protoz.LoginResult.__getID, Protoz.LoginResult.__getName)

Protoz.LoginResult[1] = {name = "_retCode", type = "normal", vtype = "ui32"}
Protoz.LoginResult[2] = {name = "_info", type = "dict", ktype = "ui32", vtype = "UserInfo"}


local result = {_retCode = 100,
					_info = { {k=200, v = {_uID = 200, _user = "username", _items = {1,2,3}, _heros = {{_name = "hero1"},{_name="hero2"}} } },
							{k=300, v = {_uID = 300, _user = "username2", _items = {4,5,6}, _heros = {{_name = "hero3"},{_name="hero4"}} }}
					 } 
				}

--Protoz.dump(result)

local now = os.time()
for i=1, 100000 do

	local data = Protoz.encode(result, "LoginResult")
	local dr = Protoz.decode(data, 100)
end
now = os.time() - now
print(now)
--Protoz.putbin(data)


--Protoz.dump(dr)


