
function DrawGrid()
	DrawLine(width/3, 0, width/3, height)
	DrawLine(width/3*2, 0, width/3*2, height)
	DrawLine(0, height/3, width, height/3)
	DrawLine(0, height/3*2, width, height/3*2)
end

function ResetGame()
	player = "x"
	
	SetColor(0xffffffff)
	Clear()
	
	SetColor(0x000000ff)
	DrawGrid(width, height)
end

function GetCell(x, y)
	if x <= width/3 then
		cx = width/3/2
	elseif x <= width/3*2 then
		cx = width/3*2 - width/3/2
	else
		cx = width - width/3/2
	end

	if y <= height/3 then
		cy = height/3/2
	elseif y <= height/3*2 then
		cy = height/3*2 - height/3/2
	else
		cy = height - height/3/2
	end

	return cx, cy
end

function DrawX(x, y)
	DrawLine(x-width/3/4, y-height/3/4, x+width/3/4, y+height/3/4)
	DrawLine(x+width/3/4, y-height/3/4, x-width/3/4, y+height/3/4)
end

function DrawO(x, y)
	DrawRect(x-width/3/4, y-height/3/4, width/3/2, height/3/2)
end

function ClearCell(x, y)
	cx, cy = GetCell(x, y)
	SetColor(0xffffffff)
	FillRect(cx-width/3/4, cy-height/3/4, width/3/2, height/3/2)
	SetColor(0x000000ff)	
	if player == "x" then player = "o"
	else player = "x" end
end

function main(event, arg1, arg2, arg3, arg4)

	if event == EVENT_INIT then

		width, height = GetWindowSize()
		ResetGame()

	elseif event == EVENT_MOUSEBUTTON then

		if arg4 ~= 1 then return end

		if arg3 == BUTTON_RIGHT then
			ClearCell(arg1, arg2)
			return
		end

		x, y = GetCell(arg1, arg2)
		if player == "x" then
			DrawX(x, y)
			player = "o"
		elseif player == "o" then
			DrawO(x, y)
			player = "x"
		end
		
	elseif event == EVENT_KEYBOARD then
	
		if arg2 ~= 1 then return end	
		if arg1 == KEY_Q then Quit()
		elseif arg1 == KEY_N then ResetGame() end

	end

end

