
start_button_new_game = { x = 20, y = 40, text = "New Game" }
start_button_load_game = { x = 20, y = 100, text = "Load Game" }
start_button_credits = { x = 20, y = 160, text = "Credits" }
start_button_exit = { x = 20, y = 220, text = "Exit" }

start_buttons = {
	start_button_new_game,
	start_button_load_game,
	start_button_credits,
	start_button_exit
}

function start_init()
	start_font = LoadFont("fonts/helvetica-neue-lt-com-25-ultra-light.ttf", 64)
	callback_mousebutton = start_mousebutton
	callback_shutdown = start_shutdown

	SetColor(0xffffffff)
	Clear()
	for key, value in pairs(start_buttons) do
		DrawText(value.x, value.y, value.text, start_font, 0x000000ff)
	end
end

function start_shutdown()
	FreeFont(start_font)
end


function start_mousebutton(x, y, button, pressed)
	if pressed ~= 1 then return end
	
	if x > start_button_exit.x and x < start_button_exit.x + 80 and
		y > start_button_exit.y and y < start_button_exit.y + 50 then
		Quit()
	end
end

