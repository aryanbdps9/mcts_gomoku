import numpy as np
from game import Game
from tree import Tree
import argparse
from utils import print_board

def player_move(board):
	N = board.shape[1]
	while(True):
		input_pos = input("Enter your new mark position(pair of space separated integers):")
		try:
			pos = [int(i) for i in input_pos.split()]
		except ValueError:
			print("ERROR: Not an integer")
		if(pos[0] < 0 or pos[0] >= N or pos[1] < 0 or pos[1] >= N):
			print("ERROR: Enter a pair of integers within board range")
		else:
			if(board[pos[0]][pos[1]] != 0):
				print("ERROR: Enter a position that is empty")
			else:	
				return pos

def main():
	parser = argparse.ArgumentParser()
	parser.add_argument("-b", "--board_size", type=int, default=15)
	parser.add_argument("-l", "--line_size", type=int, default=5)
	parser.add_argument("-r", "--num_rollouts", type=int, default=100)
	parser.add_argument("-C", "--exploration_coeff", type=float, default=1.0)
	parser.add_argument("-d", "--max_depth", type=int, default=5)
	parser.add_argument("-t", "--timeout", type=int, default=1)
	parser.add_argument("-s", "--selfplay", type=int, default=0)
	parser.add_argument("-w", "--num_workers", type=int, default=4)
	args = parser.parse_args()

	N, linesize = args.board_size, args.line_size
	num_rollouts, max_depth, timeout = args.num_rollouts, args.max_depth, args.timeout
	C = args.exploration_coeff

	game = Game(N, linesize)
	tree = Tree(game, num_rollouts, C, max_depth, timeout, num_workers=args.num_workers)

	turn = 0
	print_board(tree.root.board)
	while(True):
		board = tree.root.board
		if(turn == 1):
			print("-"*(8*N), "")
			print("Your Turn")
			if (args.selfplay == 0):
				pos = player_move(board)
				board = tree.player_move(pos)
			else:
				board = tree.play_one_move()
		else:
			print("\nComputer's Turn")
			board = tree.play_one_move()

		print_board(board)
		if(tree.root.gameover != 0):
			if(turn == 1):
				print("Congrats!! You Won...")
			else:
				print("Sorry!! You Lost... Better Luck Next Time")
			break

		turn = 1 - turn

if __name__ == '__main__':
	main()
# python main.py -b 7 -t 5 -r 1000
# python main.py -b 7 -d 3 -t 5 -r 15 -s 1
