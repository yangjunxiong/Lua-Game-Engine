FactionWindow = {
    mActive = false
}
local self = FactionWindow

function FactionWindow:Init()
    G_UI.AddWindow(self.Window())
    Main.RegisterEvent("ChangePlanet", self, self.OnChangePlanet)
    self.mFaction = nil
end

function FactionWindow:OnChangePlanet(event)
    if (self.mActive == true) then
        local planet = event.newPlanet
        if (planet.mFaction) then
            FactionWindow.Open(planet.mFaction)
        else
            FactionWindow.Close()
        end
    end
end

function FactionWindow.Open(faction)
    self.mFaction = faction
    G_UI.SetWindowActive("Faction", true)
    self.mActive = true
end

function FactionWindow.Close()
    self.mFaction = nil
    G_UI.SetWindowActive("Faction", false)
    self.mActive = false
end

function FactionWindow.Window()
    return {
        Name = "Faction",
        Active = false,
        Layout = {
            {
                Type = "Text",
                Text = self.FactionName
            },
            {
                Type = "Text",
                Text = self.FactionPlanetCount
            },
            {
                Type = "Text",
                Text = self.FactionMineCount
            },
            {
                Type = "Text",
                Text = self.FactionArmyStrength
            }
        }
    }
end

function FactionWindow.FactionName()
    return "Faction: " .. self.mFaction.mName
end

function FactionWindow.FactionPlanetCount()
    return "Planets: " .. #self.mFaction.mPlanetList
end

function FactionWindow.FactionMineCount()
    return "Mine: " .. self.mFaction.mMineCount
end

function FactionWindow.FactionArmyStrength()
    return "Fleet Strength: " .. self.mFaction.mTotalArmyStrength
end
