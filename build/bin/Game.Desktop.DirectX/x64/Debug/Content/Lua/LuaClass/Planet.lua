---@class Planet
Planet = Main.NewClass()
Planet.PlanetList = {}
local self = Planet

function Planet:__gc()
    self:DestroyPlanet()
    Main.RemoveUpdate(self)
    LogicTick.RemoveTick(self)
end

function Planet:Init()
    table.insert(Planet.PlanetList, self)

    self.mRotationDirection = 1
    self.mRotationSpeed = math.random()
    self.mRotating = true

    self.mName = ""
    self.mFaction = nil
    self.mMineSpeed = 0
    self.mArmyStrength = 0

    self.mTraningCount = 0
    self.mCurrentTraningProgress = 0

    Main.AddUpdate(self, self.Update)
    Main.RegisterEvent("ChangePlanet", self, self.OnChangePlanet)
    Main.RegisterEvent("FleetArrive", self, self.OnFleetArrive)
    LogicTick.AddTick(self, self.LogicTick)
end

function Planet:Update(deltaTime)
    if (self.mObject ~= nil and self.mRotating) then
        local quat = self.mTrans:GetWorldRotation()
        local addQuat = Quaternion.RotationAroundAxis(self.mTrans:Up(), self.mRotationDirection * self.mRotationSpeed * deltaTime)
        local newQuat = Quaternion.AddRotation(quat, addQuat)
        self.mTrans:SetWorldRotation(newQuat)
    end
end

function Planet:LogicTick(turn)
    if (self.mTraningCount > 0) then
        self.mCurrentTraningProgress = self.mCurrentTraningProgress + 0.02 * turn
        if (self.mCurrentTraningProgress >= 1) then
            self:FinishTraining()
        end
    end
end

function Planet:CreateModel(position, rotation, scale, texture)
    self.mObject = Main.Create(Entity)
    self.mTrans = self.mObject:GetTransform()
    self.mTrans:SetWorldPosition(position)
    self.mTrans:SetWorldRotation(rotation)
    self.mTrans:SetWorldScale(scale)

    -- Render
    self.mRender = Main.Create(StaticMeshRenderComponent, self.mObject)
    self.mRender:SetModel(String.New("Models/Sphere.obj.bin"))
    self.mMaterial = self.mRender:GetMaterial()
    self.mMaterial:SetColorMap(String.New(texture))

    -- Collision
    self.mCollider = Main.Create(SphereComponent, self.mObject, Vector3.Zero, 6)
end

function Planet:DestroyPlanet()
    if (self.mObject ~= nil) then
        self.mObject:Destroy()
        self.mObject = nil
        self.mTrans = nil
        self.mRender = nil
        self.mMaterial = nil
        self.mCollider = nil
    end
end

function Planet:StartTraining()
    if (self.mFaction and self.mFaction.mMineCount >= 100) then
        self.mFaction.mMineCount = self.mFaction.mMineCount - 100
        self.mTraningCount = self.mTraningCount + 1
    end
end

function Planet:StopTraining()
    if (self.mTraningCount > 0) then
        self.mTraningCount = 0
        self.mCurrentTraningProgress = 0
    end
end

function Planet:FinishTraining()
    self.mCurrentTraningProgress = 0
    self.mTraningCount = self.mTraningCount - 1
    self.mArmyStrength = self.mArmyStrength + 1
    self.mFaction:UpdateArmyStrength()
end

function Planet:ChangeFaction(faction)
    self.mFaction = faction
    if (faction ~= nil) then
        self.mMaterial:SetCoverColor(faction.mColor)
    else
        self.mMaterial:SetCoverColor(Vector3.One)
    end
end

function Planet:OnChangePlanet(event)
    if (event.oldPlanet == self) then
        self.mMaterial:SetOutline(0)
    elseif (event.newPlanet == self) then
        self.mMaterial:SetOutline(1)
    end
end

function Planet:OnFleetArrive(event)
    local fleet = event.fleet
    if (fleet.mEndPlanet == self) then
        if (self.mFaction == nil) then  -- Colonize planet
            self:ChangeFaction(fleet.mStartPlanet.mFaction, fleet.mStrength)
        elseif (fleet.mStartPlanet.mFaction == self.mFaction) then  -- Reinforce
            self.mArmyStrength = self.mArmyStrength + fleet.mStrength
            self.mFaction:UpdateArmyStrength()
        else  -- Conquest
            if (self.mArmyStrength >= fleet.mStrength) then
                self.mArmyStrength = self.mArmyStrength - fleet.mStrength
            else
                fleet.mStrength = fleet.mStrength - self.mArmyStrength
                self:ChangeFaction(fleet.mStartPlanet.mFaction, fleet.mStrength)
            end
        end
    end
end

function Planet:ChangeFaction(faction, newStrength)
    if (faction == self.mFaction) then
        return
    end

    if (self.mFaction) then
        self.mFaction:RemovePlanet(self)
        self.mFaction:UpdateArmyStrength()
    end

    self.mFaction = faction
    self.mMineCount = 0
    self.mArmyStrength = newStrength or 0
    self.mFaction:AddPlanet(self)
    self.mFaction:UpdateArmyStrength()
    self.mMaterial:SetCoverColor(faction.mColor)
end
