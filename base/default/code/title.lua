function title_event_click(x, y, button, pressed)
	if pressed ~= 1 then return end

	title_destroy()
	start_init()
	callback_mousebutton = start_show()
end

function title_init()
	title_font = LoadFont("fonts/helvetica-neue-lt-com-25-ultra-light.ttf", 64)
end

function title_destroy()
	FreeFont(title_font)
end

function title_show()

	SetColor(0xffffffff)
	Clear()

	DrawText(10, 100, "University Enigma", title_font, 0x000000ff)
	
	return title_event_click
end

