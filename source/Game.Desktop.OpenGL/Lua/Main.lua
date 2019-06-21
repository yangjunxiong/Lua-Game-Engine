require("Class/__Include__")

Main = {}
Main.UpdateList = {}
Main.EntityList = {}

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

-- Called from C++ update frame, will dispatch update event to all registered functions
function Main.Update()
    for obj, func in pairs(Main.UpdateList) do
        func(obj)
    end
end

-- Called when Lua scripts is loaded
function Main.Start()
    table.insert(Main.EntityList, Main.Create("Entity"))
    table.insert(Main.EntityList, Main.Create("Entity"))
    table.insert(Main.EntityList, Main.Create("Entity"))
    table.insert(Main.EntityList, Main.Create("Entity"))
    table.insert(Main.EntityList, Main.Create("Entity"))
end

-- Called when game loop ends
function Main.End()
    Main.EntityList = {}
end

-- Factory method to create a C++ class that is bound to Lua
-- @param class Either the name of class in string or the global table for that class
-- @return the created object
function Main.Create(class)
    -- Convert from string to type
    if (type(class) == "string")then
        class = _G[class]
    end

    -- Check it's valid C++ class type
    if (class == nil or not type(class) == "userdata" or class.New == nil or not type(class.New) == "function") then
        error("Try to create a bad C++ class");
    end

    -- Create the object and call potential init function
    local obj = class.New()
    if (type(obj.Init) == "function") then
        obj:Init()
    end

    Log("Create object " .. obj.TypeName())
    return obj
end

Main.Start()
