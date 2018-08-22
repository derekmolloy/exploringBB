srv=net.createServer(net.TCP)                  
gpio.mode(1,gpio.INPUT)                    -- the button
gpio.mode(7,gpio.OUTPUT)                   -- the LED
srv:listen(80,function(conn) 
    conn:on("receive",function(conn,payload) print(payload) 
        gpio.write(7, gpio.HIGH)
        conn:send("HTTP/1.1 200 OK\n\n")
        conn:send("<html><body><h1> Hello Beagle from the NodeMCU</h1>")
        conn:send("<div> The device ID is = ")
        conn:send(node.chipid())
        -- using integers only! float version uses more memory
        raw_voltage = adc.read(0) - 233    -- 233 is 25C
        diff_degC   = raw_voltage / 6      -- 6 steps is 1C
        temperature  = diff_degC + 25      -- add/sub from 25
        conn:send("<div> The temperature is ")
        conn:send(temperature)
        conn:send(" degrees Celsius</div>")
        conn:send("<div> The GPIO 1 Value = ")
        conn:send(gpio.read(1))
        conn:send("</div></body></html>")
        gpio.write(7, gpio.LOW)
        conn:on("sent",function(conn) conn:close() end)
    end) 
end)

