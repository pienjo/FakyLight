local socket = require "socket"
require "nanojpeg"

local host = "192.168.64.233"
local port = 8080
local pairingKey = 855905

local roap = { }

local function SendRequest(method, url, request, mimetype)

  local headers = { 
    method.." "..url.." HTTP/1.1",
    "Host: "..host..":"..port,
    "Content-Type: "..mimetype,
    "Connection: Close",
  }

  if request then
    table.insert(headers, "Content-Length: "..#request)
  end

  table.insert(headers, "")
  
    table.insert(headers,request or "")

  local data = table.concat(headers, "\r\n")

  local sock = socket.connect(host, port)
  
  if not sock then
    return 999, nil
  end
  sock:setoption( 'keepalive', true)
  sock:setoption( 'tcp-nodelay', true)

  sock:send(data)
  
  local httpResponse = sock:receive("*l")
  
  if not httpResponse then return 999, nil end

  local _, _, code =string.find(httpResponse, "HTTP/1.1 (%d+)")

  if code ~= "200" then
    return code, nil
  end

  -- sink remaining headers
  while true do 
    -- Read another line
    local h = sock:receive("*l")
    if #h == 0 then break end
  end    

  -- read remaining
  data = sock:receive("*a")
  sock:close()

  return 200, data
end

local function RoapPostRequest(url, request)
  return SendRequest("POST", url, request, "application/atom+xml")
end


roap.getImage = function( horizResolution, vertResolution)

	local status, data = RoapPostRequest( "/roap/api/auth", '<?xml version="1.0" encoding="utf-8"?><auth><type>AuthReq</type><value>'.. pairingKey ..'</value></auth>')

	status,data = SendRequest("GET", "/roap/api/data?target=screen_image&width="..horizResolution.."&height="..vertResolution.."&type=0", nil, "image/jpeg")

	if status ~= 200 then
	  return nil
	else
	  return nanojpeg.extract(data)
	end
end

return roap
