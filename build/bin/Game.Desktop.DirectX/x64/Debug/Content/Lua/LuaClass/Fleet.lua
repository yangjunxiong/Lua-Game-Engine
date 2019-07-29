Fleet = Main.NewClass()
local self = Fleet

function Fleet:New(startPlanet, endPlanet, strength)
    self.mStartPlanet = startPlanet
    self.mEndPlanet = endPlanet
    self.mStrength = strength
    self.mStartPos = self.mStartPlanet.mTrans:GetWorldPosition()
    self.mEndPos = self.mEndPlanet.mTrans:GetWorldPosition()
    self.mDirection = Vector3.New(self.mEndPos:GetX() - self.mStartPos:GetX(), self.mEndPos:GetY() - self.mStartPos:GetY(), self.mEndPos:GetZ() - self.mStartPos:GetZ())
    self.mDistance = self.mDirection:Length()
    self.mSpeed = 2 / self.mDistance
    self.mProgress = 0

    LogicTick.AddTick(self, self.LogicTick)

    self.mObject = Main.Create(Entity)
    self.mTrans = self.mObject:GetTransform()
    self.mTrans:SetWorldScale(Vector3.New(0.01, 0.01, 0.01))
    self.mTrans:SetWorldRotation(Quaternion.FromUnitVector(self.mDirection))

    -- Render
    self.mRender = Main.Create(StaticMeshRenderComponent, self.mObject)
    self.mRender:SetModel(String.New("Models/Spaceship.obj.bin"))
    self.mMaterial = self.mRender:GetMaterial()
    self.mMaterial:SetColorMap(String.New("Textures/Spaceship.jpg"))

    self:LogicTick(0)
end

function Fleet:LogicTick(turn)
    self.mProgress = self.mProgress + self.mSpeed * turn
    if (self.mProgress >= 1) then
        Main.EventNotify({ fleet = self }, "FleetArrive")
        self.mObject:Destroy()
        LogicTick.RemoveTick(self)
    else
        local newPos = Vector3.New(self.mStartPos:GetX() + self.mDirection:GetX() * self.mProgress,
            self.mStartPos:GetY() + self.mDirection:GetY() * self.mProgress,
            self.mStartPos:GetZ() + self.mDirection:GetZ() * self.mProgress)
        self.mTrans:SetWorldPosition(newPos)
    end
end
