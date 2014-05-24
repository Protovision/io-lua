require 'code/title'
require 'code/start'

function main(event, arg1, arg2, arg3, arg4)

	if event == EVENT_INIT then

		sound = LoadSound("AgoraCastleIntro.wav")
		channel = LoopSound(sound)

		title_init()
	
	elseif event == EVENT_MOUSEBUTTON then

		if callback_mousebutton ~= nil then
			callback_mousebutton(arg1, arg2, arg3, arg4)
		end

	elseif event == EVENT_SHUTDOWN then
		
		FreeSound(sound)	
		if callback_shutdown ~= nil then
			callback_shutdown()
		end

	elseif event == EVENT_KEYBOARD then

		if arg1 == KEY_Q then Quit() 
		elseif arg1 == KEY_P then PauseAudio() 
		elseif arg1 == KEY_R then ResumeAudio() end
	end

end

