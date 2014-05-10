function main(event, arg1, arg2, arg3, arg4)

	if event == EVENT_INIT then

		height, width = GetWindowSize()

		font = LoadFont("fonts/FreeMono.ttf", 60)
		doge = LoadImage("images/image.jpg", 0, 0)

		SetColor(0xffffffff)
		Clear()
		DrawText(10, 100, "wow", font, 0x000000ff)
	
		FreeFont(font)

	elseif event == EVENT_MOUSEBUTTON then

		if arg4 ~= 1 then return end
		if arg3 ~= BUTTON_LEFT then return end
               
		DrawImage(arg1, arg2, doge) 

	elseif event == EVENT_SHUTDOWN then

		FreeImage(doge)

	elseif event == EVENT_KEYBOARD then

		if arg1 == KEY_Q then Quit() end

	end

end

