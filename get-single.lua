local roap = require 'roap'

local img = roap.getImage( 320, 180);
nanojpeg.writeppm(img, "screenshot.ppm")

