import argparse
import os

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
	parser.add_argument("-g", "--gamma", type=float, default=0.99)
	parser.add_argument("-a", "--alpha", type=float, default=0.1)
	parser.add_argument("-O", type=str, default="")
	args = parser.parse_args()

	N, linesize = args.board_size, args.line_size
	num_rollouts, max_depth, timeout = args.num_rollouts, args.max_depth, args.timeout
	C = args.exploration_coeff
	# os.chdir("cpp")
	compile_str = "g++ *.cpp --std=c++11 -lpthread"
	if (args.O != ""):
		compile_str += " -O"+args.O
	os.system(compile_str)
	os.system(f"./a.out {N} {linesize} {num_rollouts} {C} {max_depth} {timeout} {args.selfplay} {args.num_workers} {args.gamma} {args.alpha}")


if __name__ == '__main__':
	main()
# python main.py -b 7 -t 5 -r 1000
# python main.py -b 7 -d 3 -t 5 -r 15 -s 1
# python main_wrapper.py -b 11 -d 5 -t 5 -r 32 -w 64 -s 0 -C 0.1 -O 4 -a 0.0 -g 1 # best yet