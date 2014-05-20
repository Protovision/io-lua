require 'code/title'
require 'code/start'

function main(event, arg1, arg2, arg3, arg4)

	if event == EVENT_INIT then

		title_init()
	
	elseif event == EVENT_MOUSEBUTTON then

		if callback_mousebutton ~= nil then
			callback_mousebutton(arg1, arg2, arg3, arg4)
		end

	elseif event == EVENT_SHUTDOWN then
		
		if callback_shutdown ~= nil then
			callback_shutdown()
		end

	elseif event == EVENT_KEYBOARD then

		if arg1 == KEY_Q then Quit() end

	end

end

