-- Creates all neccessary scripts for the gui on initial load, everything except build tools
-- Created by Ben T. 10/29/10
-- Please note that these are loaded in a specific order to diminish errors/perceived load time by user

local scriptContext = game:GetService("ScriptContext")
local touchEnabled = game:GetService("UserInputService").TouchEnabled

local NAMEHEREGui = game:GetService("CoreGui"):WaitForChild("NAMEHEREGui")

local soundFolder = Instance.new("Folder")
soundFolder.Name = "Sounds"
soundFolder.Parent = NAMEHEREGui

-- TopBar
local topbarSuccess, topbarFlagValue = pcall(function() return settings():GetFFlag("UseInGameTopBar") end)
local useTopBar = (topbarSuccess and topbarFlagValue == true)
if useTopBar then
	scriptContext:AddCoreScriptLocal("CoreScripts/Topbar", NAMEHEREGui)
end

-- SettingsScript
local luaControlsSuccess, luaControlsFlagValue = pcall(function() return settings():GetFFlag("UseLuaCameraAndControl") end)

-- MainBotChatScript (the Lua part of Dialogs)
scriptContext:AddCoreScriptLocal("CoreScripts/MainBotChatScript2", NAMEHEREGui)

-- Developer Console Script
scriptContext:AddCoreScriptLocal("CoreScripts/DeveloperConsole", NAMEHEREGui)

-- In-game notifications script
scriptContext:AddCoreScriptLocal("CoreScripts/NotificationScript2", NAMEHEREGui)

-- Chat script
if useTopBar then
	spawn(function() require(NAMEHEREGui.Modules.Chat) end)
	spawn(function() require(NAMEHEREGui.Modules.PlayerlistModule) end)
end

local luaBubbleChatSuccess, luaBubbleChatFlagValue = pcall(function() return settings():GetFFlag("LuaBasedBubbleChat") end)
if luaBubbleChatSuccess and luaBubbleChatFlagValue then
	scriptContext:AddCoreScriptLocal("CoreScripts/BubbleChat", NAMEHEREGui)
end

-- Purchase Prompt Script
scriptContext:AddCoreScriptLocal("CoreScripts/PurchasePromptScript2", NAMEHEREGui)

-- Health Script
if not useTopBar then
	scriptContext:AddCoreScriptLocal("CoreScripts/HealthScript", NAMEHEREGui)
end

do -- Backpack!
	spawn(function() require(NAMEHEREGui.Modules.BackpackScript) end)
end

if useTopBar then
	scriptContext:AddCoreScriptLocal("CoreScripts/VehicleHud", NAMEHEREGui)
end

scriptContext:AddCoreScriptLocal("CoreScripts/GamepadMenu", NAMEHEREGui)

if touchEnabled then -- touch devices don't use same control frame
	-- only used for touch device button generation
	scriptContext:AddCoreScriptLocal("CoreScripts/ContextActionTouch", NAMEHEREGui)

	NAMEHEREGui:WaitForChild("ControlFrame")
	NAMEHEREGui.ControlFrame:WaitForChild("BottomLeftControl")
	NAMEHEREGui.ControlFrame.BottomLeftControl.Visible = false
end
