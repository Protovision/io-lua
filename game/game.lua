
x = 5
y = 30
text = 'Type stuff: '

SetColor(COLOR_WHITE)

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
	DrawText(0, 0, text)
end

