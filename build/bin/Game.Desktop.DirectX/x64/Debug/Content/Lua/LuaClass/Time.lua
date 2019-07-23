Time =
{
    mTimeSinceStart = 0,
    mDeltaTime = 0,
    mFrameCount = 0
}
local self = Time

function Time.Update(deltaTime)
    Time.mDeltaTime = deltaTime
    Time.mTimeSinceStart = Time.mTimeSinceStart + deltaTime
    Time.mFrameCount = Time.mFrameCount + 1
end
