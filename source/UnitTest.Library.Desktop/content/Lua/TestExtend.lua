-- class ConstClass
-- class Dummy

Dummy.LuaA = 10
Dummy.LuaB = {
	1, 2, 3, 4, 5
}

function Dummy:LuaDo(a, b)
	if (b == nil or not type(b) == "table") then
		ConstClass.sDummy.mDoCount = ConstClass.sDummy.mDoCount + 1
	elseif (b.TypeName == nil) then
		ConstClass.sDummy.mDoCount = ConstClass.sDummy.mDoCount + 2
	elseif (b:TypeName() == "ConstClass") then
		ConstClass.sDummy.mDoCount = ConstClass.sDummy.mDoCount + 4
	else
		self.mDoCount = self.mDoCount + a
	end
end

function main()
	CheckNumber(10, Dummy.LuaA)
	Check("table", type(Dummy.LuaB))
	for i = 1, 5, 1 do
		CheckNumber(i, Dummy.LuaB[i])
	end

	local dummy = Dummy.New()

	CheckNumber(0, dummy.mDoCount);
	dummy:Do3(1, 2)
	CheckNumber(-1, dummy.mDoCount);

	dummy:LuaDo(1)
	CheckNumber(1, ConstClass.sDummy.mDoCount)

	dummy:LuaDo(1, {})
	CheckNumber(3, ConstClass.sDummy.mDoCount)

	dummy:LuaDo(1, ConstClass.New())
	CheckNumber(7, ConstClass.sDummy.mDoCount)

	dummy.mDoCount = 0
	dummy:LuaDo(100, dummy)
	CheckNumber(100, dummy.mDoCount)
end

main()