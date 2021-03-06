---@type World
G_World = G_World
---@type MouseEntity
G_Mouse = G_Mouse
---@type KeyboardEntity
G_Keyboard = G_Keyboard
---@type Camera
G_Camera = G_Camera
G_UI = G_UI

Main = {}
Main.UpdateList = {}
Main.EventList = {}

function Main.NewClass(parent)
    local table
    if (parent) then
        table = parent:New()
    else
        table = {}
        table._New = function(self, ...)
            local obj = {}
            setmetatable(obj, self)
            self.__index = self
            obj:New(...)
            return obj
        end
        table.New = function(self, ...) end
    end

    return table
end

-- Add a new function to the update list
function Main.AddUpdate(obj, func)
    -- Argument check
    if (obj == nil or (not type(obj) == "table" and not type(obj) == "userdata")) then
        error("AddUpdate expects a table or user object");
    end
    if (func == nil or not type(func) == "function")then
        error("AddUpdate expects a function");
    end

    -- Add to update list
    Main.UpdateList[obj] = func
end

-- Remove the update function for an object
function Main.RemoveUpdate(obj)
    Main.UpdateList[obj] = nil
end

function Main.RegisterEvent(eventName, obj, func)
    if (Main.EventList[eventName] == nil) then
        Main.EventList[eventName] = {}
    end
    assert(obj ~= nil)
    assert(func ~= nil)
    Main.EventList[eventName][obj] = func
end

function Main.UnregisterEvent(eventName, obj)
    if (not Main.EventList[eventName] == nil) then
        Main.EventList[eventName][obj] = nil
    end
end

-- Called from C++ update frame, will dispatch update event to all registered functions
function Main.Update(deltaTime)
    Time.Update(deltaTime)
    for obj, func in pairs(Main.UpdateList) do
        func(obj, deltaTime)
    end
    collectgarbage("collect")
end

-- Called when Lua scripts is loaded
function Main.Start()
    -- To cope with existing game framework, just make everything in one sector
    assert(World:TypeName() == "World")
    Main.ActiveSector = Main.Create(Sector)
    Entry.Load()
end

-- Factory method to create a C++ class that is bound to Lua
-- @param class Either the name of class in string or the global table for that class
-- @return the created object
function Main.Create(class, ...)
    -- Convert from string to type
    if (type(class) == "string")then
        class = _G[class]
    end

    -- Check it's valid C++ class type
    if (class == nil or class.New == nil or not type(class.New) == "function") then
        error("Try to create a bad class");
    end

    -- Create the object and call potential init function
    local obj
    if (type(class.TypeName) == "function") then
        obj = class.New(...)
    else
        obj = class:_New(...)
    end
    if (type(obj.Init) == "function") then
        obj:Init()
    end

    return obj
end

function Main.EventNotify(event, eventName)
    local list = Main.EventList[eventName]
    if (list) then
        for obj, func in pairs(list) do
            func(obj, event)
        end
    end
end

require("Content/Lua/Class/__Include__")
require("Content/Lua/Include")
