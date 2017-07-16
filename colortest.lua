local nanojpeg = require 'nanojpeg'

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

local function do_test(input_fn)
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

do_test("TestImages/startrek/input.ppm")
for i=1,17 do
  do_test("TestImages/startrek/input"..i..".ppm")
end
for i=1,29 do
  do_test("TestImages/americans1/input"..i..".ppm")
end
