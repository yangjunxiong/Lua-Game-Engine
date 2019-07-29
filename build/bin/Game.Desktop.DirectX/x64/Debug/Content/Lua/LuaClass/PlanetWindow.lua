PlanetWindow = {}
local self = PlanetWindow

function PlanetWindow.Init()
    G_UI.AddWindow(self.Window())
    Main.RegisterEvent("MouseEvent", self, self.OnMouseClick)
    self.mPlanet = nil
end

function PlanetWindow.PlanetName()
    if (self.mPlanet) then
        return "Name: " .. self.mPlanet.mName
    else
        return "Name: None"
    end
end

function PlanetWindow.FactionName()
    if (self.mPlanet and self.mPlanet.mFaction) then
        return "Faction: " .. self.mPlanet.mFaction.mName
    else
        return "Faction: None"
    end
end

function PlanetWindow.MineSpeed()
    if (self.mPlanet) then
        return "Mine Production: " .. self.mPlanet.mMineSpeed
    else
        return "Mine Production: None"
    end
end

function PlanetWindow.ArmyStrength()
    if (self.mPlanet) then
        return "Fleet Strength: " .. self.mPlanet.mArmyStrength
    else
        return "Fleet Strength: None"
    end
end

function PlanetWindow.Window()
    return
    {
        Name = "Planet",
        Active = true,
        Layout =
        {
            {
                Type = "Text",
                Text = self.PlanetName
            },
            {
                Type = "Button",
                Text = "Go to",
                Click = self.FocusPlanet
            },
            {
                Type = "Text",
                Text = self.FactionName
            },
            {
                Type = "Button",
                Text = "Detail",
                Click = self.ClickFactionDetail
            },
            {
                Type = "Text",
                Text = self.MineSpeed
            },
            {
                Type = "Text",
                Text = self.ArmyStrength
            },
            {
                Type = "Button",
                Text = "Build new ship",
                Click = self.TrainButton
            },
            {
                Type = "Text",
                Text = self.TrainProgress
            }
        }
    }
end

---@param event MouseEvent
function PlanetWindow:OnMouseClick(event)
    if (event.Type == MouseEventType.Click) then
        if (event.Button == MouseButtons.Left or event.Button == MouseButtons.Right) then
            -- Find the planet under selection
            local collisions = G_Mouse:RayHitComponents()
            local size = collisions:Size() - 1
            local selectPlanet = nil
            for i = 0, size, 1 do
                local collider = collisions:Get(i)
                for i, planet in pairs(Planet.PlanetList) do
                    if (collider == planet.mCollider) then
                        selectPlanet = planet
                        break
                    end
                end
                if (selectPlanet) then
                    break
                end
            end

            -- Differentiate right click and left click
            if (selectPlanet ~= nil) then
                if (event.Button == MouseButtons.Left) then
                    self.ChangePlanet(selectPlanet)
                elseif (self.mPlanet ~= nil and self.mPlanet ~= selectPlanet) then
                    SelectFleetWindow.Open(self.mPlanet, selectPlanet)
                end
            end
        else
            self.mPlanet = nil
        end
    end
end

function PlanetWindow.ChangePlanet(planet)
    if (self.mPlanet ~= planet) then
        Main.EventNotify({oldPlanet = self.mPlanet, newPlanet = planet}, "ChangePlanet")
        self.mPlanet = planet
    end
end

function PlanetWindow.ClickFactionDetail()
    if (FactionWindow.mActive == true) then
        FactionWindow.Close()
    elseif (self.mPlanet and self.mPlanet.mFaction) then
        FactionWindow.Open(self.mPlanet.mFaction)
    end
end

function PlanetWindow.TrainButton()
    if (self.mPlanet and self.mPlanet.mFaction) then
        self.mPlanet:StartTraining()
    end
end

function PlanetWindow.TrainProgress()
    if (self.mPlanet and self.mPlanet.mFaction and self.mPlanet.mTraningCount > 0) then
        return string.format("Training progress: %.2f %%%% (%d)", self.mPlanet.mCurrentTraningProgress * 100, self.mPlanet.mTraningCount)
    end
    return "No current building"
end

function PlanetWindow.FocusPlanet()
    if (self.mPlanet) then
        local planetPos = self.mPlanet.mTrans:GetWorldPosition()
        local cameraPos = G_Camera:GetTransform():GetWorldPosition()
        local newPos = Vector3.New(planetPos:GetX(), planetPos:GetY(), cameraPos:GetZ())
        G_Camera:GetTransform():SetWorldPosition(newPos)
    end
end
