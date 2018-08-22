-- a simple http server
srv=net.createServer(net.TCP) 
gpio.mode(1,gpio.INPUT)
srv:listen(80,function(conn) 
    conn:on("receive",function(conn,payload) print(payload) 
       conn:send("HTTP/1.1 200 OK\n\n")
       conn:send("<html><body><h1> Hello from the NodeMCU.</h1>")
       conn:send("<h2> GPIO 1 = ")
       conn:send(gpio.read(1))
       conn:send("</h2></body></html>")
       conn:on("sent",function(conn) conn:close() end)
    end) 
end)

