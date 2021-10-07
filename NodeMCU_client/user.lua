
local stripSize = 106 --53
local stripBuffer = ws2812.newBuffer(stripSize,3)
local targetBuffer = ws2812.newBuffer(stripSize, 3)

-- Configure the GPIO output
gpio.mode(0,gpio.OUTPUT, gpio.PULLUP)
gpio.write(0, gpio.LOW)
local poweredOn = false

-- Configure led strip
ws2812.init(ws2812.MODE_SINGLE)
stripBuffer:fill(0,0,0)
ws2812.write(stripBuffer)

-- Create a timer that will fade between current and target
local fadeTimer = tmr.create()
local function fadeNow(reset)
  stripBuffer:mix(192, stripBuffer, 64,targetBuffer)
  ws2812.write(stripBuffer)
  fadeTimer:start() -- Restart fade timer.
end

fadeTimer:register(50, tmr.ALARM_SEMI, fadeNow)

-- Create a dead-mans switch that will turn off the LEDs after some time
local timeout = 0
local watchdogTimer = tmr.create()

watchdogTimer:register(1000, tmr.ALARM_AUTO, function()
  if timeout > -60 then
    timeout = timeout - 1
    if timeout == 0 then
      print("Timeout")
      targetBuffer:fill(0,0,0)
    end

    if timeout == -60 then
      print ("Power off")
      gpio.write(0, gpio.LOW)
      poweredOn = false
      fadeTimer:stop()
    end
  end
end)
watchdogTimer:start()


-- Received UDP packet
local function onPacketReceived(sock, data, port, ip)
  local maxPacketLen = stripSize * 3
  if #data > maxPacketLen then
    -- excess data -- abort
    --data = string.sub(data, 1, maxPacketLen)
    return
  end
  if (not poweredOn) then
    -- Power on the strip
    gpio.write(0, gpio.HIGH)
  end
  timeout = 5
  targetBuffer:replace(data)
  fadeNow()
end

-- Received IP address from access point
local udpSocket = nil

local function OnIPReceived(event)
  print("Address received: ", event.IP)
  if not udpSocket then
    udpSocket = net.createUDPSocket()
    udpSocket:listen(5628)
    udpSocket:on("receive", onPacketReceived)
  end
end

-- Configure wifi
wifi.setmode(wifi.STATION, false)
wifi.sta.config({ ssid="pienjo2", pwd="Appelstroop76"})
wifi.eventmon.register( wifi.eventmon.STA_GOT_IP, OnIPReceived)

