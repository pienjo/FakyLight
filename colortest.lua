local nanojpeg = require 'nanojpeg'
local rects = require "rects"

local function do_test(input_fn)
  print(input_fn)
  local input = nanojpeg.readppm(input_fn)
  local output_fn = string.sub(input_fn, 1,-5) .."-out.ppm"
  local csvOutput = string.sub(input_fn, 1,-4) .. "csv"

  local output = input
  local hists = { }

  local header = { "color#" }
  for idx, rect in pairs(rects) do
    hists[idx] = nanojpeg.getHueHistogram(input, rect[1], rect[2], rect[3],rect[4])
    local b,g,r = nanojpeg.getDominantColor(input, rect[1], rect[2], rect[3], rect[4])

    if r then
      output = nanojpeg.setRect(output, rect[1], rect[2], rect[3], rect[4], r,g, b)
    end
    table.insert(header, "rect"..idx)
  end

  nanojpeg.writeppm(output, output_fn)
  local outfile = io.open(csvOutput, "w")
  outfile:write(table.concat(header, ",") .. "\n")

  for color = 1, 256 do
    local line = { tostring(color - 1)}
    for _, hist in ipairs(hists) do
      table.insert(line, hist[color])
    end
    outfile:write(table.concat(line, ",") .. "\n")
  end
  
end

do_test("TestImages/100nl.ppm")

do_test("TestImages/discovery.ppm")
do_test("TestImages/bamigo.ppm")

do_test("TestImages/startrek/input.ppm")
for i=1,5 do
  do_test("TestImages/startrek/input"..i..".ppm")
end
for i=1,5 do
  do_test("TestImages/americans1/input"..i..".ppm")
end 

