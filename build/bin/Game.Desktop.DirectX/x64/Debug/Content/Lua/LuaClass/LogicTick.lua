LogicTick = {
    mTickSpeed = 1,
    mTickInterval = { 0, 2, 1, 0.5, 0.05 },
    mCurrentIntervalLevel = 1,
    mAccumulatedTime = 0,
    mTickTimes = 0,
    mStartDate = os.time({year = 2200, month = 1, day = 1}),
    mTickList = {}
}
local self = LogicTick

function LogicTick:Init()
    Main.AddUpdate(self, self.Tick)
    G_UI.AddWindow(self.Window())
end

function LogicTick:Tick(deltaTime)
    self.mAccumulatedTime = self.mAccumulatedTime + deltaTime
    local interval = self.mTickInterval[self.mCurrentIntervalLevel]
    if (interval > 0 and self.mAccumulatedTime >= interval) then
        self.DoTick(1)
        self.mAccumulatedTime = 0
        self.mTickTimes = self.mTickTimes + 1
    end
end

function LogicTick.AddTick(obj, func)
    self.mTickList[obj] = func
end

function LogicTick.RemoveTick(obj)
    self.mTickList[obj] = nil
end

function LogicTick.DoTick(turn)
    for obj, func in pairs(self.mTickList) do
        func(obj, turn)
    end
end

function LogicTick.SetTickIntervalLevel(level)
    level = math.max(1, math.min(5, level))
    self.mCurrentIntervalLevel = level
end

function LogicTick.GetTickIntervalLevel()
    return self.mCurrentIntervalLevel
end

function LogicTick.WindowString()
    local newDate = self.mStartDate + self.mTickTimes * 60 * 60* 24
    local date = os.date("*t", newDate)
    local str = "ANNO " .. date.year .. "-" .. date.month .. "-" .. date.day .. "\n"
    str = str .. "Game speed: " .. tostring(self.mCurrentIntervalLevel - 1)
    return str
end

function LogicTick.SlowInterval()
    self.SetTickIntervalLevel(self.mCurrentIntervalLevel - 1)
end

function LogicTick.FastInterval()
    self.SetTickIntervalLevel(self.mCurrentIntervalLevel + 1)
end

function LogicTick.Window()
    return
    {
        Name = "Game Speed",
        Active = true,
        Layout =
        {
            {
                Type = "Text",
                Text = self.WindowString
            },
            {
                Type = "Button",
                Text = "<<",
                Click = self.SlowInterval
            },
            {
                Type = "Button",
                Text = ">>",
                Click = self.FastInterval
            }
        }
    }
end
