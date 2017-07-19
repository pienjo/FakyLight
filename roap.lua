local socket = require "socket"
require "nanojpeg"

local host = "192.168.64.233"
local port = 8080
local pairingKey = 855905

local roap = { }

local roapState = 0 -- Not connected
local roapSocket = nil
local requests = { }


local function SendRequest(method, url, request, mimetype)
  
  local done = false
  
  local headers = { 
    method.." "..url.." HTTP/1.1",
    "Host: "..host..":"..port,
    "Content-Type: "..mimetype,
    "Connection: keep-alive",
  }

  local errorSentinel = 2

  if request then
    table.insert(headers, "Content-Length: "..#request)
  end

  table.insert(headers, "")
  
  table.insert(headers,request or "")

  local data = table.concat(headers, "\r\n")
  local nextToSend = 1

  local errorString
  local contentLength 
  local content
  local contentCode

  repeat
    if (roapState == 0) then
      -- open socket
      roapSocket = socket.tcp()
      roapSocket:settimeout(10)

      if 1 ~= roapSocket:connect(host, port) then
	contentCode = 999
	done = true
	roapSocket:close()
	roapSocket = nil
      else
	print ("Connected")
	roapSocket:setoption('keepalive', true)
	roapSocket:settimeout(5)
	roapState = 1;
      end
    elseif roapState == 1 then
      -- Connected, sending header
      nextToSend, errorString = roapSocket:send(data, nextToSend )
      if not nextToSend then
	-- Error sending data
	print ("Error sending request: ", errorString)
	roapState = 99 -- error, retry.
      else
	if nextToSend == #data then
	  -- header sent. 
	  roapState = 2
	end
      end
    elseif roapState == 2 then
      -- receiving response
      local httpResponse
      httpResponse, errorString = roapSocket:receive("*l")
      if not httpResponse then
	print ("Error receiving HTTP response", errorString)
	roapState = 99 -- error, retry.
      else
	local _
	_, _, contentCode =string.find(httpResponse, "HTTP/1.1 (%d+)")
	contentCode = tonumber(contentCode)
        roapState = 3
      end
    elseif roapState == 3 then
      -- Parse header line
      local headerLine
      headerLine, errorString = roapSocket:receive("*l")
      if not headerLine then
	print ("Error retrieving header", errorString)
	roapState = 99 -- error, retry.
      elseif #headerLine == 0 then
	-- Header is done
	roapState = 4
      else
	local contentLengthStr = "Content-Length: "
	if string.sub(headerLine, 1, #contentLengthStr) == contentLengthStr then
	  contentLength = tonumber(string.sub(headerLine, #contentLengthStr + 1))
	end
      end
    elseif roapState == 4 then
      -- Retrieve data
      if contentLength then
	-- content length is known
	content,errorString = roapSocket:receive(contentLength)
	if content then
	  roapState = 1 -- allow to reuse connection
	end
      else
	-- Content length not known
	content, errorString = roapSocket:receive("*a")
	if content then
	  roapState = 0
	  roapSocket:close()
	  roapSocket = nil
	end
      end

      if not content then
        print ("Error retrieving content : ", errorString)
        roapState = 99
      else
	done = true
      end
    else
      -- error
      nextToSend = 1
      if (roapSocket) then roapSocket:close() end
      roapSocket = nil
      roapState = 0

      if errorSentinel > 0 then
	errorSentinel = errorSentinel - 1
      else
	contentCode = 999
	done = true
      end
    end
  until done

  return contentCode, content
end
local function RoapPostRequest(url, request)
  return SendRequest("POST", url, request, "application/atom+xml")
end


roap.getImage = function( horizResolution, vertResolution)
  
  local try_auth = true

  while true do
    local status, data = SendRequest("GET", "/roap/api/data?target=screen_image&width="..horizResolution.."&height="..vertResolution.."&type=0", nil, "image/jpeg")

    if status == 200 then
      return nanojpeg.extract(data)
    elseif try_auth then
      try_auth = true
      RoapPostRequest( "/roap/api/auth", '<?xml version="1.0" encoding="utf-8"?><auth><type>AuthReq</type><value>'.. pairingKey ..'</value></auth>')
    else
       return nil
    end
  end
end

return roap
