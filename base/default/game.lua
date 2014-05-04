function main(event, arg1, arg2, arg3, arg4)

	if event == EVENT_INIT then

		height, width = GetWindowSize()

		font = LoadFont("fonts/FreeMono.ttf", 60)
		doge = LoadImage("images/image.jpg")

		file = OpenFile("text/message.txt", "r")

		SetColor(0xffffffff)
		Clear()
		DrawText(10, 100, ReadFile(file), font, 0x000000ff)
	
		FreeFont(font)
		CloseFile(file)

	elseif event == EVENT_MOUSEBUTTON then

		if arg4 ~= 1 then return end
		if arg3 ~= BUTTON_LEFT then return end
                
                i = 0
		while i < 25 do
			x = math.random(height)
			y = math.random(width)
			DrawImage(x, y, doge)
			i = i + 1
		end 

	elseif event == EVENT_SHUTDOWN then

		FreeImage(doge)

	elseif event == EVENT_KEYBOARD then

		if arg1 == KEY_Q then Quit() end

	end

end

