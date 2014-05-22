
platform = GetPlatform()

if platform == "Mac OS X" then
	Set("v_renderer", "1")
	Set("v_driver", "0")
elseif platform == "Windows" then
	Set("s_driver", "2")
end

Set("v_width", "640")
Set("v_height", "480")
Set("c_fps", "85")

