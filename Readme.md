# GOMOKU AGENT USING MCTS ALGORITHM

## Acknowledgement
Most optimization tricks have been heavily inspired by [this repo](https://github.com/lygztq/gomoku) including:
- Board is not a part of each node
- Making playouts iterative instead of recursive. This removes the overhead of calling a function multiple times (This is also useful if someone wants to run playouts in parallel)
- Limiting maximum length of a rollout and decreasing total number of rollouts.


## Getting Started
Run `main_wrapper.py` using **python 3** to play. If you type `python` to run `python 3`, then you may type `python main_wrapper.py -m 10` to play against AI as player 2.

Run main_wrapper.py without any arguments to play against AI. Enter your move in the following format:
row_number \<space\> column number. e.g: `0 5`

You can modify the parameters in `main_wrapper.py` as per your liking.
`-m` is 2-bit binary string where left bit is for first player and right bit is for second player. A bit is 1 means the corresponding player is AI. Otherwise it's a human.

With -v = 1 the play state of the board is displayed after each move.
With -v = 2 will show the debug output with details of visits and UCT matrices of each position

Some other useful arguments are:
- `-b`: Board size
- `-l`: Line size. A player will have to make a line of this size to win.

## Next step
Implement RAVE
