local roap = require 'roap'
local socket = require "socket"
local posix = require "posix"
local fakyAddress = "192.168.64.27"
local rects = require 'rects'

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

repeat
  local img = roap.getImage( 320, 180);
  if not img then
    -- Unable to retrieve
    posix.sleep(1)
  else
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
    end 

    data = table.concat(data)
    fakySocket:send(data)

    posix.nanosleep(0, 1E14)
  end
until false
