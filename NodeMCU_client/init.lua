countdown = 5
tmr.alarm(0,1000, 1, function()
  countdown = countdown - 1
  if countdown == 0 then
    tmr.stop(0)
    countdown = nil
    local s,err = pcall(function() dofile("user.lua") end)
    if not s then print(err) end
  end
end)

