
function keyboard(key, status)
	if status == PRESSED then
		MessageBox(string.char(key))
	end
end
