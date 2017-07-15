local roap = require 'roap'
local socket = require "socket"
local posix = require "posix"
local fakyAddress = "192.168.64.27"

local fakySocket = socket.udp()
fakySocket:setpeername(fakyAddress, 5628)

local stripHeight = 14
local stripWidth = 25

local stripsize = 2 * stripHeight + stripWidth

local buffer = { }

for x = 1, stripsize * 3 do
  buffer[x] = 0
end

local function dim()
  for k,v in pairs(buffer) do
    buffer[k] = v / 2
  end
end

local testidx = 1
local output

repeat
  local img = roap.getImage( 320, 180);
  --local output = nanojpeg.testtransform(img)
  -- output=nanojpeg.setRect(img, 0, 0, 320, 180, 0,0,0);

  local rects = {
    { 240, 120, 320, 180 , 1,5}, 
    { 240, 60, 320, 120, 6, 10 },
    { 256, 0, 320, 60, 11, 19 },
    { 192, 0, 256, 60, 20, 24 },
    { 128, 0,  192, 60, 25, 29 },
    { 64,  0,  128, 60, 30, 34 },
    { 0, 0, 64, 60, 35, 42 },
    { 0, 60, 80, 120, 43,47 },
    { 0, 120, 80, 180, 48, 53 },
  }

  local data = { }
  for _, rect in pairs(rects) do
    local b,g,r= nanojpeg.getDominantColor(img, rect[1], rect[2], rect[3], rect[4]);

    if not r then
      r = 0
      g = 0
      b = 0
    end

    local snippet = string.char(g,r,b)
    for idx = rect[5],rect[6] do
      data[idx] = snippet
    end
    if output then
      output=nanojpeg.setRect(output, rect[1], rect[2], rect[3], rect[4], r,g,b)
    end
  end 

  data = table.concat(data)
  fakySocket:send(data)

  posix.nanosleep(0, 1E14)
  testidx = testidx + 1
  print(testidx)
until false
