function Main(int1, int2, string, dummy1, dummy2)
	local count = int1 + int2
	local result = ""
	result = result .. tostring(count) .. string .. tostring(dummy1.mDoCount) .. tostring(dummy2.mDoCount)
	Check("3Hello World1020", result)
	return count
end

function MainNoReturn(int1, int2, string, dummy1, dummy2)
	local count = int1 + int2
	local result = ""
	result = result .. tostring(count) .. string .. tostring(dummy1.mDoCount) .. tostring(dummy2.mDoCount)
	Check("3Hello World1020", result)
end