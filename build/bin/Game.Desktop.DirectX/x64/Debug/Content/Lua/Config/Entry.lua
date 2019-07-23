Entry = {
    mTextureList =
    {
        "Textures/Earth.jpg",
        "Textures/Jupiter.jpg",
        "Textures/Mars.jpg",
        "Textures/Mercury.jpg",
        "Textures/Moon.jpg",
        "Textures/Neptune.jpg",
        "Textures/Pluto.jpg",
        "Textures/Saturn.jpg",
        "Textures/Sun.jpg",
        "Textures/Uranus.jpg",
        "Textures/Venus.jpg"
    },
    mFactionList =
    {
        {
            Name = "Desert Beagel",
            Color = Vector3.New(1, 0, 0)
        },
        {
            Name = "Big Duck Energy",
            Color = Vector3.New(0, 1, 0)
        },
        {
            Name = "Trust the process",
            Color = Vector3.New(0, 0, 1)
        },
        {
            Name = "Snowmad or whatever",
            Color = Vector3.New(0.5, 0.5, 0.5)
        }
    }
}
local self = Entry

function Entry.Load()
    -- Setup basics
    Main.Create(CameraController)
    LogicTick:Init()

    -- Create UI
    PlanetWindow:Init()
    FactionWindow:Init()
    SelectFleetWindow:Init()

    -- Create data
    self.CreateFactions()
    self.CreatePlanets()
end

function Entry.CreatePlanets()
    for i = 1, 8, 1 do
        for j = 1, 5, 1 do
            local position = Vector3.New(i * 100, j * 100, 0)
            local rotation = Quaternion.RotationAroundAxis(Vector4.Forward, math.rad(math.random() * 90 - 45))
            local scale = math.random(2, 5)
            local texture = self.mTextureList[math.random(9)]
            local planet = Main.Create(Planet)
            planet:CreateModel(position, rotation, Vector3.New(scale, scale, scale), texture)
            local faction = Faction.FactionList[math.random(1, 10)]
            if (faction) then
                faction:AddPlanet(planet)
                planet:ChangeFaction(faction)
            end
            planet.mName = "Planet " .. i .. "-" .. j
            planet.mMineSpeed = math.random(1, 10)
            planet.mArmyStrength = 0
        end
    end
end

function Entry.CreateFactions()
    for i, config in pairs(self.mFactionList) do
        local faction = Main.Create(Faction)
        faction.mId = i
        faction.mName = config.Name
        faction.mColor = config.Color
    end
end
