-- proto4z core file
-- Auther: zhangyawei
-- mail:yawei_zhang@foxmail.com
-- date: 2015-01-12


Protoz = {}

--[[--
pack base type to binary stream
@param __pack val.  base type value
@param __pack tp.  type
@return no result
]]
function Protoz.__pack(val, tp)
	if val == nil or tp == nil then
		error(debug.traceback())
	end
	--print(tp .. "[" .. val .. "]")
	-- integer type
	if     tp == "i8" or tp == "char" then
		return string.pack("<i1", val)
	elseif tp == "ui8" or tp == "unsigned char" then
		return string.pack("<I1", val)
	elseif tp == "i16" or tp == "short" then
		return string.pack("<i2", val)
	elseif tp == "ui16" or tp == "unsigned short" then
		return string.pack("<I2", val)	
	elseif tp == "i32" or tp == "int" then
		return string.pack("<i4", val)
	elseif tp == "ui32" or tp == "unsigned int" then
		return string.pack("<I4", val)	
	elseif tp == "i64" or tp == "long long" then
		return string.pack("<i8", val)
	elseif tp == "ui64" or tp == "unsigned long long" then
		return string.pack("<I8", val)	

	-- string type
	elseif tp == "string" then
		return string.pack("<s2", val)

	-- float type
	elseif tp == "float" then
		return string.pack("<f", val)
	elseif tp == "double" then
		return string.pack("<d", val)

	-- error
	else
		error("unknown base data type when __pack it. val type=" .. type(val) .. "trace: " .. debug.traceback())
	end
end

--[[--
unpack base type from binary stream
@param __unpack binData.  binary data stream
@param __unpack pos. binary current begin unpack position
@param __unpack tp.  type
@return value, next pos
]]
function Protoz.__unpack(binData, pos, tp)
	if binData == nil or tp == nil or pos == nil then
		error("can't unpack binData. " .. debug.traceback())
	end
	-- integer type
	if     tp == "i8" or tp == "char" then
		return string.unpack("<i1", binData, pos)
	elseif tp == "ui8" or tp == "unsigned char" then
		return string.unpack("<I1", binData, pos)
	elseif tp == "i16" or tp == "short" then
		return string.unpack("<i2", binData, pos)
	elseif tp == "ui16" or tp == "unsigned short" then
		return string.unpack("<I2", binData, pos)
	elseif tp == "i32" or tp == "int" then
		return string.unpack("<i4", binData, pos)
	elseif tp == "ui32" or tp == "unsigned int" then
		return string.unpack("<I4", binData, pos)
	elseif tp == "i64" or tp == "long long" then
		return string.unpack("<i8", binData, pos)
	elseif tp == "ui64" or tp == "unsigned long long" then
		return string.unpack("<I8", binData, pos)

	-- string type
	elseif tp == "string" then
		return string.unpack("<s2", binData, pos)

	-- float type
	elseif tp == "float" then
		return string.unpack("<f", binData, pos)
	elseif tp == "double" then
		return string.unpack("<d", binData, pos)

	else
		--error("unknown binData to unpack . pos=" .. pos .. " tp=" .. tp .." trace: " .. debug.traceback())
		return nil, pos
	end

end


--[[--
encode protocol table to binary stream
@param __encode obj.  protocol table 
@param __encode name. protocol name
@param __encode data.  output binary stream
@return no result
]]
function Protoz.__encode(obj, name, data)
	if name == nil then
		error("name is nil. " .. debug.traceback())
	end
	local proto = Protoz[name]
	if proto == nil then
		error("unknown name[" .. name .. "] " .. debug.traceback())
	end

	for i=1, #proto do
		local desc = proto[i]
		if type(desc) ~= "table" then
			error("parse Proto error. name[" .. name .. "] " .. debug.traceback())
		end
		if type(desc.name) ~= "string" or type(desc.type) ~= "string" or type(desc.vtype) ~= "string" then
			error("parse Proto error. proto[" .. desc.name .. ":" .. desc.type ..  "] " .. debug.traceback())
		end
		local val = obj[desc.name]
		if val == nil then
			error("not found the dest object. name[" .. desc.name .. "] " .. debug.traceback())
		end

		if desc.type == "normal" then
			if type(val) ~= "table" then
				data.data = data.data .. Protoz.__pack(val, desc.vtype)
			else
				Protoz.__encode(val, desc.vtype, data)
			end
		elseif desc.type == "array" then
			local arrayLen = #val
			data.data = data.data .. string.pack("<I2", arrayLen)
			for j =1, #val do
				if type(val[j]) ~= "table" then
					data.data = data.data .. Protoz.__pack(val[j], desc.vtype)
				else
					Protoz.__encode(val[j], desc.vtype, data)
				end
			end
		elseif desc.type == "dict" then
			local dictLen = #val
			data.data = data.data .. string.pack("<I2", dictLen)
			for j =1, #val do
				if type(val[j].k) ~= "table" then
					data.data = data.data .. Protoz.__pack(val[j].k, desc.ktype)
				else
					Protoz.__encode(val[j].k, desc.ktype, data)
				end
				if type(val[j].v) ~= "table" then
					data.data = data.data .. Protoz.__pack(val[j].v, desc.vtype)
				else
					Protoz.__encode(val[j].v, desc.vtype, data)
				end
			end
		end
	end
end

--[[--
encode protocol table to binary stream
@param encode obj.  protocol table 
@param encode name. protocol name
@return binary stream
]]
function Protoz.encode(obj, name)
	local data = {data=""}
	Protoz.__encode(obj, name, data)
	return data.data
end


--[[--
make dict [protocol id = protocol name]
@param register id.  protocol id 
@param register name. protocol name
@return  no result
]]
function Protoz.register(id, name)
	if Protoz.__protos == nil then
		Protoz.__protos = {}
	end
	if Protoz.__protos[id] ~= nil then
		error("id already register. id=" .. id .. " registrered name=" .. Protoz.__protos[id]  .. ", new name=" .. protoName .. ": " .. debug.traceback())
	end
	if type(name) ~= "string" then
		error("name ~= string " .. debug.traceback())
	end
	Protoz.__protos[id] = name
end

--[[--
get the protocol name from protoco id.
@param register id.  protocol id 
@return  protocol name
]]
function Protoz.getName(id)
	if Protoz.__protos == nil then
		return nil
	end
	if Protoz.__protos[id] == nil then
		return nil
	end
	return Protoz.__protos[id]
end

--[[--
decode binary stream to protocol table
@param __decode binData.  binary stream
@param __decode pos.  current binary begin index
@param __decode name.  dest protocol name
@param __decode result. output protocol table.
@return next begin index
]]
function Protoz.__decode(binData, pos, name, result)
	local proto = Protoz[name]
	if proto == nil then
		error("parse error. name=" .. name ..  " pos=" .. pos .. ". trace=" .. debug.traceback())
	end
	local v, p
	p = pos
	for i = 1, #proto do
		local desc = proto[i]
		--print("[" .. #binData .. ":" .. p .. "] " .. desc.name .. ":" ..  desc.type .. ":" .. ":" .. desc.vtype)
		if desc.type == "normal" then
			v, p = Protoz.__unpack(binData, p, desc.vtype)
			if v ~= nil then
				result[desc.name] = v
			else
				result[desc.name] = {}
				p = Protoz.__decode(binData, p, desc.vtype, result[desc.name])
			end
		elseif desc.type == "array" then
			local len
			local ret = {}
			len, p = Protoz.__unpack(binData, p, "ui16")
			for j=1, len do
				v, p = Protoz.__unpack(binData, p, desc.vtype)
				if v ~= nil then
					ret[j] = v
				else
					ret[j] = {}
					p = Protoz.__decode(binData, p, desc.vtype, ret[j])
				end
			end
			result[desc.name] = ret
		elseif desc.type == "dict" then
			local len
			local k
			local ret = {}
			len, p = Protoz.__unpack(binData, p, "ui16")
			for j=1, len do
				k, p = Protoz.__unpack(binData, p, desc.ktype)
				v, p = Protoz.__unpack(binData, p, desc.vtype)
				if v ~= nil then
					ret[k] = v
				else
					ret[k] = {}
					p = Protoz.__decode(binData, p, desc.vtype, ret[k])
				end
			end
			result[desc.name] = ret
		end
	end
	return p
end

--[[--
decode binary stream to protocol table
@param decode binData.  binary stream
@param decode id.  dest protocol id
@return protocol table
]]
function Protoz.decode(binData, id)
	--print("decode id = " .. id)
	local result = {}
	protoName = Protoz.getName(id)
	if  type(protoName) ~= "string" then
		print("unknown id. id=" .. id)
		return nil
	end
	Protoz.__decode(binData, 1, Protoz.__protos[id], result)
	return result
end





--------------------------------------------------------------------------
-----  assist
--------------------------------------------------------------------------

--[[--
print binary stream with hexadecimal string
@param decode binData.  binary stream
@return no result
]]
function Protoz.putbin(binData)
	local str = ""
	for i = 1, #binData do
		str = str .. string.format("%02x ",string.byte(binData, i))
	end
	print("[len:" .. #binData .. "]" .. str)
end




--[[--
trim string
]]
function Protoz.trim(input)
    input = string.gsub(input, "^[ \t\n\r]+", "")
    return string.gsub(input, "[ \t\n\r]+$", "")
end



--[[--
split string
]]
function Protoz.split(input, delimiter)
    input = tostring(input)
    delimiter = tostring(delimiter)
    if (delimiter=='') then return false end
    local pos,arr = 0, {}
    -- for each divider found
    for st,sp in function() return string.find(input, delimiter, pos, true) end do
        table.insert(arr, string.sub(input, pos, st - 1))
        pos = sp + 1
    end
    table.insert(arr, string.sub(input, pos))
    return arr
end

--[[--
dump table with nesting
]]
function Protoz.dump(value, desciption, nesting)
    if type(nesting) ~= "number" then nesting = 5 end

    local lookupTable = {}
    local result = {}

    local function _v(v)
        if type(v) == "string" then
            v = "\"" .. v .. "\""
        end
        return tostring(v)
    end

    local traceback = Protoz.split(debug.traceback("", 2), "\n")
    print("dump from: " .. Protoz.trim(traceback[3]))

    local function _dump(value, desciption, indent, nest, keylen)
        desciption = desciption or "<var>"
        spc = ""
        if type(keylen) == "number" then
            spc = string.rep(" ", keylen - string.len(_v(desciption)))
        end
        if type(value) ~= "table" then
            result[#result +1 ] = string.format("%s%s%s = %s", indent, _v(desciption), spc, _v(value))
        elseif lookupTable[value] then
            result[#result +1 ] = string.format("%s%s%s = *REF*", indent, desciption, spc)
        else
            lookupTable[value] = true
            if nest > nesting then
                result[#result +1 ] = string.format("%s%s = *MAX NESTING*", indent, desciption)
            else
                result[#result +1 ] = string.format("%s%s = {", indent, _v(desciption))
                local indent2 = indent.."    "
                local keys = {}
                local keylen = 0
                local values = {}
                for k, v in pairs(value) do
                    keys[#keys + 1] = k
                    local vk = _v(k)
                    local vkl = string.len(vk)
                    if vkl > keylen then keylen = vkl end
                    values[k] = v
                end
                table.sort(keys, function(a, b)
                    if type(a) == "number" and type(b) == "number" then
                        return a < b
                    else
                        return tostring(a) < tostring(b)
                    end
                end)
                for i, k in ipairs(keys) do
                    _dump(values[k], k, indent2, nest + 1, keylen)
                end
                result[#result +1] = string.format("%s}", indent)
            end
        end
    end
    _dump(value, desciption, "- ", 1)

    for i, line in ipairs(result) do
        print(line)
    end
end

