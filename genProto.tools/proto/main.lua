-- proto4z test file
-- Auther: zhangyawei
-- mail:yawei_zhang@foxmail.com
-- date: 2015-01-12

package.path =  "../../?.lua;" .. package.path
--require
require("proto4z")
require("lua.ControlProto")


local echo = {  _iarray = {{_char=1,_uchar=2,_short=3,_ushort=4,_int=5,_uint=6,_i64="12345678",_ui64="12345678"},{_char=1,_uchar=2,_short=3,_ushort=4,_int=5,_uint=6,_i64="12345678",_ui64="12345678"}},
				_farray = {{_float=2.235,_double=235.111},{_float=2.235,_double=235.111},},
				_sarray = {{_string="abcdefg"},{_string="abcdefg"},{_string="abcdefg"}},
				_imap = {{k="123", v={_char=1,_uchar=2,_short=3,_ushort=4,_int=5,_uint=6,_i64="12345678",_ui64="12345678"}}, {k="223", v={_char=1,_uchar=2,_short=3,_ushort=4,_int=5,_uint=6,_i64="12345678",_ui64="12345678"}}},
				_fmap = {{k="523", v={_float=2.235,_double=235.111}},{k="623", v={_float=2.235,_double=235.111}}},
				_smap = {{k="523", v={_string="abcdefg"}},{k="523", v={_string="abcdefg"}}},
				}

Protoz.dump(echo)

local now = os.time()
--for i=1, 100000 do
	local data = Protoz.encode(echo, "P2P_EchoPack")
	Protoz.putbin(data)
	local dr = Protoz.decode(data, Protoz.getName(Protoz.P2P_EchoPack.__getID)) -- "P2P_EchoPack"
	Protoz.dump(dr)

--end
now = os.time() - now
--print(now)





