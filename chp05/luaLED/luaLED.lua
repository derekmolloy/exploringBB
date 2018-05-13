#!/usr/bin/lua5.3
local LED3_PATH = "/sys/class/leds/beaglebone:green:usr3/" 

-- Example function to write a value to the GPIO
function writeLED(directory, filename, value)
   file = io.open(directory..filename, "w") -- append dir and file names
   file:write(value)                        -- write the value to the file
   file:close()
end

print("Starting the Lua LED Program")
if arg[1]==nil then                         -- no argument provided?
   print("This program requires a command")
   print("   usage is: ./luaLED.lua command")
   print("where command is one of on, off, or status")
   do return end
end
if arg[1]=="on" then
   print("Turning the LED on")
   writeLED(LED3_PATH, "trigger", "none")
   os.execute("sleep 0.1")
   writeLED(LED3_PATH, "brightness", "1")
elseif arg[1]=="off" then
   print("Turning the LED off")
   writeLED(LED3_PATH, "trigger", "none")
   os.execute("sleep 0.1")
   writeLED(LED3_PATH, "brightness", "0")
elseif arg[1]=="status" then
   print("Getting the LED status")
   file = io.open(LED3_PATH.."brightness", "r")
   print(string.format("The LED state is %s.", file:read()))
   file:close()
else
   print("Invalid command!")
end
print("End of the Lua LED Program")

