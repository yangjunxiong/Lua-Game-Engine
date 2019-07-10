PlanetDemo = {
    mPassedTime = 0
}
local self = PlanetDemo

function PlanetDemo:Init()
    Main.AddUpdate(PlanetDemo, self.Update)
end

function PlanetDemo:Update(deltaTime)
    self.mPassedTime = self.mPassedTime + deltaTime
    Earth.mSpinSpeed = 10 * math.sin(self.mPassedTime)
end

Main.ClassList[PlanetDemo] = true
