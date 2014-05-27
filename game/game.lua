
text = 'Type stuff: '

mono = LoadFont("fonts/FreeMono.ttf", 32)

SetColor(COLOR_BLACK)

function keyboard(key, state)
	if state ~= PRESSED then return end
	if (key == KEY_BACKSPACE) then
		text = string.sub(text, 0, -2)
	else
		text = text .. string.char(key)
	end
end

function update()
	Clear()
	DrawText(0, 0, text, COLOR_LIME, mono)
end

function shutdown()
	FreeFont(mono)
end

