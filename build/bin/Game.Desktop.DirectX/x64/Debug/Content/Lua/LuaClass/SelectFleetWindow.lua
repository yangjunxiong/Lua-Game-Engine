SelectFleetWindow = {
    mActive = false
}
local self = SelectFleetWindow

function SelectFleetWindow.Init()
    G_UI.AddWindow(self.Window())
    Main.RegisterEvent("ChangePlanet", self, self.OnChangePlanet)
    self.mStartPlanet = nil
    self.mEndPlanet = nil
    self.mFleetValue = 0
end

function SelectFleetWindow:OnChangePlanet(event)
    if (self.mActive == true) then
        self.Close()
    end
end

function SelectFleetWindow.Window()
    return {
        Name = "SelectFleet",
        Active = false,
        Layout = {
            {
                Type = "Text",
                Text = "Select fleet"
            },
            {
                Type = "Slider",
                Text = "Slider",
                Min = 0,
                Max = self.MaxFleet,
                Value = self.FleetValue,
                ValueChange = self.FleetValueChange
            },
            {
                Type = "Button",
                Text = "Send",
                Click = self.SendFleet
            },
            {
                Type = "Button",
                Text = "Cancel",
                Click = self.Close
            }
        }
    }
end

---@param startPlanet Planet
---@param endPlanet Planet
function SelectFleetWindow.Open(startPlanet, endPlanet)
    self.mActive = true
    self.mFleetValue = 0
    self.mStartPlanet = startPlanet
    self.mEndPlanet = endPlanet
    G_UI.SetWindowActive("SelectFleet", true)
end

function SelectFleetWindow.Close()
    self.mActive = false
    G_UI.SetWindowActive("SelectFleet", false)
end

function SelectFleetWindow.MaxFleet()
    return self.mStartPlanet.mArmyStrength
end

function SelectFleetWindow.FleetValue()
    return self.mFleetValue
end

function SelectFleetWindow.FleetValueChange(value)
    self.mFleetValue = math.floor(value)
end

function SelectFleetWindow.SendFleet()
    if (self.mFleetValue > 0) then
        Main.Create(Fleet, self.mStartPlanet, self.mEndPlanet, self.mFleetValue)
        self.mStartPlanet.mArmyStrength = self.mStartPlanet.mArmyStrength - self.mFleetValue
        self.mStartPlanet.mFaction:UpdateArmyStrength()
        self.Close()
    end
end
