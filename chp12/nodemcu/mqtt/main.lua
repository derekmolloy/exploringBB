-- a simple NodeMCU MQTT publish example for AdaFruit IO
BROKER  = "io.adafruit.com"
BRPORT  = 1883
BRUSER  = "molloyd"
BRPWD   = "feb8ea6179c64b638b35b93ef87d4b42"
DEVID   = "NodeMCU1"
TOPIC   = "molloyd/feeds/weather.temperature"
count   = 0             -- used to count the number of samples sent

gpio.mode(7, gpio.OUTPUT)
gpio.write(7, gpio.HIGH)
print("Starting the NodeMCU MQTT client test")
print("Current heap is: " .. node.heap())   -- .. appends strings
m = mqtt.Client(DEVID, 120, BRUSER, BRPWD)  -- keep alive time 120s
m:connect(BROKER, BRPORT, 0, function(conn) -- secure off
   print("Connected to MQTT Broker: " .. BROKER)
   tmr.alarm(0, 10000, 1, function()        -- repeat is on
      publish_sample()
      print("Time for another sample")
      count = count + 1
   end)
end)

function publish_sample()
   raw_voltage = adc.read(0) - 233    -- 233 is 25C
   diff_degC   = raw_voltage / 6      -- 6 steps is 1C
   temp        = diff_degC + 25       -- add/sub from 25
   msg = string.format("%d", temp)
   m:publish(TOPIC, msg, 0, 0, function(conn)
      print("Published a message: " .. msg)
      print("Value of count is: " .. count)
      if count>=10 then 
         close()
         timer.cancel(0)
      end
   end)
end

function close()
   m:close()
   print("End of the NodeMCU MQTT Example")
   gpio.write(7, gpio.LOW)
end

