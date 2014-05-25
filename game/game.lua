require 'code/title'
require 'code/start'

function main(event, arg1, arg2, arg3, arg4)

	if event == EVENT_INIT then

		files = ReadDirectory(Gamepath("."))
		for k, v in pairs(files) do
			print(k .. ": " .. v)
		end

		door = LoadSound("waves/door.wav")
		sound = LoadSound("AgoraCastleIntro.wav")
		channel = LoopSound(sound)

		title_init()
	
	elseif event == EVENT_MOUSEBUTTON then

		if callback_mousebutton ~= nil then
			callback_mousebutton(arg1, arg2, arg3, arg4)
		end

	elseif event == EVENT_SHUTDOWN then
		
		FreeSound(sound)	
		FreeSound(door)
		if callback_shutdown ~= nil then
			callback_shutdown()
		end

	elseif event == EVENT_KEYBOARD then

		if arg2 ~= PRESSED then return end

		if arg1 == KEY_Q then Quit() 
		elseif arg1 == KEY_P then PauseAudio() 
		elseif arg1 == KEY_R then ResumeAudio()
		elseif arg1 == KEY_UP then SetVolume(GetVolume() + 20)
		elseif arg1 == KEY_DOWN then SetVolume(GetVolume() - 20)
		elseif arg1 == KEY_M then MuteAudio()
		elseif arg1 == KEY_U then UnmuteAudio()
		elseif arg1 == KEY_D then PlaySound(door)
		end
	end

end

