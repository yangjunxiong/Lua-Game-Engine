CameraController = Main.NewClass()
local self = CameraController

-- Control whether to move the camera XY position when move mouse to the edge of screen
self.mXBoundary = 0.75
self.mYBoundary = 0.75
self.mScrollSpeed = 500

-- Control scale mouse in and out
self.mNearDistance = 200
self.mFarDistance = 1500
self.mDefaultDistance = 200
self.mCurrentDistance = self.mDefaultDistance
self.mScaleSpeed = 10

function CameraController:Print(flag)
    local mousePos = G_Mouse:ViewportPosition()
    local x = mousePos:GetX()
    local y = mousePos:GetY()
    local str =  "Viewport pos: (" .. x .. ", " .. y .. ")\n"
    if (flag) then
        str = str .. "Scroll delta: (" .. self.scrollDelta:GetX() .. ", " .. self.scrollDelta:GetY() .. ")\n"
        str = str .. "Mouse wheel delta: " .. G_Mouse:WheelDelta() .. "\n"
        str = str .. "Camera distance: " .. self.mCurrentDistance .. "\n"
    end

    return str
end

function CameraController:Window()
    return {
        Name = "DEBUG",
        Active = true,
        Layout = {
            {
                Type = "Text",
                Text = { self.Print, self, true }
            }
        }
    }
end

function CameraController:Init()
    Main.AddUpdate(self, self.Update)
    self.mCameraTrans = G_Camera:GetTransform()
    self.mCameraTrans:SetWorldPosition(Vector3.New(0, 0, self.mDefaultDistance))
    --G_UI.AddWindow(self:Window())
end

function CameraController:Update(deltaTime)
    self:UpdateScale(deltaTime)
    self:UpdatePosition(deltaTime)
end

function CameraController:UpdateScale(deltaTime)
    local wheelDelta = G_Mouse:WheelDelta()
    if (wheelDelta ~= 0) then
        local scaleDelta = -wheelDelta * self.mScaleSpeed * deltaTime
        local newDistance = self.mCurrentDistance + scaleDelta
        local clampedDistance = math.max(math.min(newDistance, self.mFarDistance), self.mNearDistance)
        local cameraPos = self.mCameraTrans:GetWorldPosition()
        self.mCurrentDistance = clampedDistance
        self.mCameraTrans:SetWorldPosition(Vector3.New(cameraPos:GetX(), cameraPos:GetY(), clampedDistance))
    end
end

function CameraController:UpdatePosition(deltaTime)
    local mousePos = G_Mouse:ViewportPosition()
    local x = mousePos:GetX()
    local y = mousePos:GetY()
    local scrollDelta = Vector3.New(0, 0, 0)
    local xRatio = 1 - self.mXBoundary
    local yRatio = 1 - self.mYBoundary
    local scaledScrollSpeed = self.mScrollSpeed * (self.mCurrentDistance / self.mFarDistance)
    if (x > self.mXBoundary) then
        scrollDelta:SetX(scaledScrollSpeed * (x - self.mXBoundary) / xRatio * deltaTime)
    elseif (x < -self.mXBoundary) then
        scrollDelta:SetX(-scaledScrollSpeed * (-self.mXBoundary - x) / xRatio * deltaTime)
    end
    if (y > self.mYBoundary) then
        scrollDelta:SetY(-scaledScrollSpeed * (y - self.mYBoundary) / yRatio * deltaTime)
    elseif (y < -self.mYBoundary) then
        scrollDelta:SetY(scaledScrollSpeed * (-self.mYBoundary - y) / yRatio * deltaTime)
    end
    self.scrollDelta = scrollDelta

    local currentPos = self.mCameraTrans:GetWorldPosition()
    local newPos = Vector3.New(currentPos:GetX() + scrollDelta:GetX(), currentPos:GetY() + scrollDelta:GetY(), currentPos:GetZ() + scrollDelta:GetZ())
    self.mCameraTrans:SetWorldPosition(newPos)
end
