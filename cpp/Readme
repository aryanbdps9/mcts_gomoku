Before running, first compile the code by running: make

Run the following to run experiments mentioned in this report:
python3 play_arena.py <file_name>.config number_of_games <file_name>.log <file_name>.parsed 

Run main_wrapper.py without any arguments to play against AI. Enter your move in the following format:
row_number<space>column number. e.g: 0 5
You can modify the following parameters as per your liking:
"-b", "--board_size", type=int, default=11
	 "-l", "--line_size", type=int, default=5
	 "-m", "--mode", type=str, default="01000001"
	 "-v", "--verbose", type=int, default=1
	 "-r", "--num_rollouts_1", type=int, default=100
	 "-d", "--max_depth", type=int, default=5
	 "-t", "--timeout", type=int, default=1
	 "-w", "--num_workers", type=int, default=4
	 "-c", "--exploration_coeff", type=float, default=1.0
	 "-g", "--gamma", type=float, default=1.0
 "-z", "--beta", type=float, default=0.1
	 "-a", "--alpha", type=float, default=0.1
-m is 8 bit string where rightmost 4 bits is for first player and leftmost 4 bits is for second player
-m abcdefgh abcd = 0001 => first player is human; efgh = 0100 => second player is agent 

With -v = 1 the play state of the board is displayed after each move.
With -v = 2 will show the debug output with details of visits and UCT matrices of each position
