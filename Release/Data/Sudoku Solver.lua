function FindUnassignedSpot(board)

	local i, j

	-- Check array for next valid (0) spot
	for i = 1, 9 do
		for j = 1, 9 do
			if board[i][j] == 0 then
				return true, i, j
			end
		end
	end

	return false, 0, 0
end


function FindValidNumbers(x, y, board)

	local bValid = {}
	local iNumbers = {}
	local i, j

	-- Initialise
	for i = 0, 9 do
		bValid[i] = true;
	end

	-- Check row / column for which numbers are taken
	for i = 1, 9 do
		--print("i = ", i)
		--print("setting to false: ", board[i][y], board[x][i])
		bValid[board[i][y]] = false
		bValid[board[x][i]] = false
	end

	local x = x - 1
	local y = y - 1

	-- Find smaller square location
	local smallX = x - (x % 3)
	local smallY = y - (y % 3)

	--print("top left corner: ", smallX + 1, smallY + 1)

	-- Check smaller square for which numbers are taken
	for i = 1, 3 do
		for j = 1, 3 do
			bValid[board[smallX + i][smallY + j]] = false
		end
	end

	-- Index (Counter)
	local index = 1

	-- Add all remaining numbers (valid numbers) to an array
	for i = 1, 9 do
		if bValid[i] then
			iNumbers[index] = i
			index = index + 1
		end
	end

	-- Return the valid number
	return iNumbers
end


function Sudoku(iBoard2D)

	-- find any empty spot
	local b, x, y = FindUnassignedSpot(iBoard2D)

	--print("unassigned spot: ", x, y)

	if b == false then
		return true
	end

	-- Find possible numbers
	local validSpots = FindValidNumbers(x, y, iBoard2D)

	--print("valid spots # = ", table.getn(validSpots))

	if #validSpots == 0 then
		return false
	end

	local i

	-- Attempt a solution from each spot
	for i = 1, #validSpots do

		-- Add number to the board ("try it")
		iBoard2D[x][y] = validSpots[i]

		-- Recursive function call, if true the board has been solved
		if Sudoku(iBoard2D) then
		--if true then
			return true
		end

		-- Otherwise if no solution on this pathway, remove it and continue with other valid numbers
		iBoard2D[x][y] = 0
	end

	-- If no solution is found for this pathway, return false
	return false

end


function SudokuSolver(strBoard)

	iBoard2D = {}

	local index = 1
	local i, j

	-- convert to 2D array
	for i = 1, 9 do
		iBoard2D[i] = {}

		for j = 1, 9 do
			s = string.sub(strBoard, index, index)
			iBoard2D[i][j] = tonumber(s, 10)
			index = index + 1
		end
	end

	strResult = "FAIL"

	-- run algorithm
	local b = Sudoku(iBoard2D)

	-- if solution found, convert back to string
	if b then

		strResult = ""

		for i = 1, 9 do
			for j = 1, 9 do
				strResult = strResult .. tostring(iBoard2D[i][j])
			end
		end
	end

	return strResult
end

--print(SudokuSolver("930001704000067023000005010102000060000000000060000207050200000610480000204100076"))
