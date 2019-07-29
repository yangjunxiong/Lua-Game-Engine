Faction = Main.NewClass()
Faction.FactionList = {}
local self = Faction

function Faction:Init()
    table.insert(Faction.FactionList, self)

    self.mId = 0
    self.mName = ""
    self.mPlanetList = {}
    self.mMineCount = 0
    self.mTotalArmyStrength = 0

    LogicTick.AddTick(self, self.LogicTick)
end

function Faction:LogicTick(turn)
    for i, planet in pairs(self.mPlanetList) do
        self.mMineCount = self.mMineCount + planet.mMineSpeed * turn
    end
end

function Faction:AddPlanet(planet)
    for i = 1, #self.mPlanetList do
        if (self.mPlanetList[i] == planet) then
            return
        end
    end
    table.insert(self.mPlanetList, planet)
end

function Faction:RemovePlanet(planet)
    for i = 1, #self.mPlanetList do
        if (self.mPlanetList[i] == planet) then
            table.remove(self.mPlanetList, i)
        end
    end
end

function Faction:UpdateArmyStrength()
    local total = 0
    for i, planet in pairs(self.mPlanetList) do
        total = total + planet.mArmyStrength
    end
    self.mTotalArmyStrength = total
end
