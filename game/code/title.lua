function title_mousebutton(x, y, button, pressed)
	if pressed ~= 1 then return end

	title_shutdown()
	start_init()

end

function title_init()
	title_font = LoadFont("fonts/helvetica-neue-lt-com-25-ultra-light.ttf", 64)
	callback_mousebutton = title_mousebutton
	callback_shutdown = title_shutdown

	SetColor(0xffffffff)
	Clear()
	DrawText(10, 100, "University Enigma", title_font, 0x000000ff)
end

function title_shutdown()
	FreeFont(title_font)
end


