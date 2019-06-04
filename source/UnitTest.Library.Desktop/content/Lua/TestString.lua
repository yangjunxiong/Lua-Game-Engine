local str = String("123")
check("123", tostring(str))
check("hello", tostring(Temp))
check("lua", tostring(Temp2))
check("!", tostring(Temp3))
Temp4:Set(tostring(Temp4) .. "3")
check("String", str.TypeName())

function f1()
	local int = int32()
	check("0", tostring(int))
	int:Set(100)
	check("100", tostring(int:Get()))
	local int2 = int32(123)
	check("123", tostring(int2))
end

function f2()
	local string = String("abc")
	check("abc", tostring(string))
	string:Set("cbd")
	check("cbd", string:Get())
end

function f3()
	local int = int64()
	checkNumber(0, int:Get())
	int:Set(100)
	checkNumber(100, int:Get())
	local int2 = int64(123)
	checkNumber(123, int2:Get())
end

function f4()
	local f = float()
	check(0.0, f:Get())
	f:Set(100.125)
	check(100.125, f:Get())
	local float2 = float(1.25)
	check(1.25, float2:Get())
end

function f5()
	local d = double()
	check(0.0, d:Get())
	d:Set(100.125)
	check(100.125, d:Get())
	local d2 = double(1.25)
	check(1.25, d2:Get())
end

function f6()
	local b = bool()
	check("true", tostring(b))
	b:Set(false)
	check("false", tostring(b:Get()))
	local b2 = bool(false)
	check("false", tostring(b2:Get()))
end

function f7()
	local c = char()
	checkNumber(0, c:Get())
	c:Set(127)
	checkNumber(127, c:Get())
	local c2 = char(130)
	check("-126", tostring(c2:Get()))
end

f1()
f2()
f3()
f4()
f5()
f6()
f7()