def print_board(board):
	gap = "\t"
	N = len(board)
	print("", end=gap)

	for i in range(N):
		print(i, end=gap)
	print("\n")
	for i in range(N):
		print(i, end=gap)
		for j in range(N):
			char_ = 'X' if board[i,j] == 1 else ('O' if board[i,j] == 2 else '-')
			print(char_, end=gap)
		print("\n")
