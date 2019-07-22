PlanetDemo = Main.NewClass()
local self = PlanetDemo

function PlanetDemo:LabelCallback()
    local label = "Click Left/Right button: "
    if (self.mRotationDirection == 1) then
        label = label .. "Forward rotation"
    else
        label = label .. "Backward rotation"
    end
    return label
end

function PlanetDemo:ButtonTextCallback()
    if (self.mRotating == true) then
        return "Stop Rotation"
    else
        return "Start Rotation"
    end
end

function PlanetDemo:ButtonClick()
    if (self.mRotating == true) then
        self.mRotating = false
    else
        self.mRotating = true
    end
end

function PlanetDemo:DestroyButtonText()
    if (self.mObject == nil) then
        return "Create"
    else
        return "Destroy"
    end
end

function PlanetDemo:DestroyButtonCallback()
    if (self.mObject ~= nil) then
        self.mObject:Destroy()
        self.mObject = nil
        self.mTrans = nil
        self.mRender = nil
        self.mMaterial = nil
        self.mCollider = nil
        collectgarbage("collect")
    else
        self:Create()
    end
end

function PlanetDemo:Window()
    return
    {
        Name = "Planet Control",
        Active = true,
        Layout =
        {
            {
                Type = "Text",
                Text = "Lua UI demo"
            },
            {
                Type = "Text",
                Text = { self.LabelCallback, self }
            },
            {
                Type = "Button",
                Text = { self.ButtonTextCallback, self },
                Click = { self.ButtonClick, self }
            },
            {
                Type = "Button",
                Text = { self.DestroyButtonText, self },
                Click = { self.DestroyButtonCallback, self }
            },
            { 
                Type = "Text",
                Text = "Press middle button to hide this window"
            }
        },
    }
end

function PlanetDemo:Init()
    Main.AddUpdate(self, self.Update)
    Main.RegisterEvent("MouseEvent", self.MouseEventCallback, self)

    self.mRotationDirection = 1
    self.mRotationSpeed = 1
    self.mRotating = true
    self.mPassedTime = 0

    G_UI.AddWindow(self:Window())
    self:Create()
end

function PlanetDemo:Update(deltaTime)
    if (self.mObject ~= nil and self.mRotating) then
        local quat = self.mTrans:GetWorldRotation()
        local addQuat = Quaternion.RotationAroundAxis(self.mTrans:Up(), self.mRotationDirection * self.mRotationSpeed * deltaTime)
        local newQuat = Quaternion.AddRotation(quat, addQuat)
        self.mTrans:SetWorldRotation(newQuat)
    end
end

function PlanetDemo:MouseEventCallback(event)
    if (self.mObject ~= nil) then
        if (event.Type == MouseEventType.Click) then
            local collisions = G_Mouse:RayHitComponents()
            local size = collisions:Size() - 1
            for i = 0, size, 1 do
                local collider = collisions:Get(i)
                if (collider == self.mCollider) then
                    if (event.Button == MouseButtons.Left) then
                        self.mRotationDirection = 1
                    elseif (event.Button == MouseButtons.Right) then
                        self.mRotationDirection = -1
                    end
                    break
                end
            end
        elseif (event.Type == MouseEventType.Press) then
            if (event.Button == MouseButtons.Middle) then
                G_UI.SetWindowActive("Planet Control", false)
            end
        elseif (event.Type == MouseEventType.Release) then
            if (event.Button == MouseButtons.Middle) then
                G_UI.SetWindowActive("Planet Control", true)
            end
        end
    end
end

function PlanetDemo:Create()
    self.mObject = Main.Create(Entity)
    self.mTrans = self.mObject:GetTransform()
    self.mTrans:SetWorldRotation(Quaternion.RotationAroundAxis(Vector4.Forward, 0.5))

    -- Render
    self.mRender = Main.Create(StaticMeshRenderComponent, self.mObject)
    self.mRender:SetModel(String.New("Models/Sphere.obj.bin"))
    self.mMaterial = self.mRender:GetMaterial()
    self.mMaterial:SetColorMap(String.New("Textures/Earth.jpg"))

    -- Collision
    self.mCollider = Main.Create(SphereComponent, self.mObject, Vector3.Zero, 6)
end
