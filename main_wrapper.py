import argparse
import os

def main():
	parser = argparse.ArgumentParser()
	parser.add_argument("-b", "--board_size", type=int, default=11)
	parser.add_argument("-l", "--line_size", type=int, default=5)
	parser.add_argument("-m", "--mode", type=str, default="10")
	parser.add_argument("-v", "--verbose", type=int, default=1)
	parser.add_argument('--make', type=str, default="yes")

	parser.add_argument("-r", "--num_rollouts_1", type=int, default=96)
	parser.add_argument("-d", "--max_depth_1", type=int, default=5)
	parser.add_argument("-t", "--timeout_1", type=int, default=8)
	parser.add_argument("-w", "--num_workers_1", type=int, default=4)
	parser.add_argument("-c", "--exploration_coeff_1", type=float, default=0.22)
	parser.add_argument("-g", "--gamma_1", type=float, default=1.0)
	parser.add_argument("-a", "--alpha_1", type=float, default=0.0)
	parser.add_argument("-z", "--beta_1", type=float, default=0.1)
	parser.add_argument("-z1", "--beta1_1", type=float, default=0.1)
	parser.add_argument("-p", "--pf_version_1", type=int, default=1)

	parser.add_argument("-R", "--num_rollouts_2", type=int, default=-1)
	parser.add_argument("-D", "--max_depth_2", type=int, default=-1)
	parser.add_argument("-T", "--timeout_2", type=int, default=-1)
	parser.add_argument("-W", "--num_workers_2", type=int, default=-1)
	parser.add_argument("-C", "--exploration_coeff_2", type=float, default=-1.0)
	parser.add_argument("-G", "--gamma_2", type=float, default=-1.0)
	parser.add_argument("-A", "--alpha_2", type=float, default=-1.0)
	parser.add_argument("-Z", "--beta_2", type=float, default=-1.0)
	parser.add_argument("-Z1", "--beta1_2", type=float, default=-0.1)
	parser.add_argument("-P", "--pf_version_2", type=int, default=-1)

	args = parser.parse_args()

	if (args.num_rollouts_2 < 0):
		args.num_rollouts_2 = args.num_rollouts_1
	if (args.max_depth_2 < 0):
		args.max_depth_2 = args.max_depth_1
	if (args.timeout_2 < 0):
		args.timeout_2 = args.timeout_1
	if (args.num_workers_2 < 0):
		args.num_workers_2 = args.num_workers_1
	if (args.exploration_coeff_2 < 0):
		args.exploration_coeff_2 = args.exploration_coeff_1
	if (args.gamma_2 < 0):
		args.gamma_2 = args.gamma_1
	if (args.alpha_2 < 0):
		args.alpha_2 = args.alpha_1
	if (args.beta_2 < 0):
		args.beta_2 = args.beta_1
	if (args.beta1_2 < 0):
		args.beta1_2 = args.beta1_1
	if (args.pf_version_2 < 0):
		args.pf_version_2 = args.pf_version_1

	N, linesize = args.board_size, args.line_size
	num_rollouts_1, max_depth_1, timeout_1 = args.num_rollouts_1, args.max_depth_1, args.timeout_1
	num_rollouts_2, max_depth_2, timeout_2 = args.num_rollouts_2, args.max_depth_2, args.timeout_2
	mode = int(args.mode[-2:], 2)
	# os.makedirs('objects',exist_ok=True)
	if (args.make == 'yes'):
		os.system('make')

	cmd_str = f"./mcts.out {N} {linesize} {mode} {args.verbose} {num_rollouts_1} {max_depth_1} {timeout_1} {args.num_workers_1} {args.exploration_coeff_1} {args.gamma_1} {args.alpha_1} {args.beta_1} {num_rollouts_2} {max_depth_2} {timeout_2} {args.num_workers_2} {args.exploration_coeff_2} {args.gamma_2} {args.alpha_2} {args.beta_2}  {args.beta1_1} {args.beta1_2} {args.pf_version_1} {args.pf_version_2}"
	os.system(cmd_str)

if __name__ == '__main__':
	main()
# python main.py -b 7 -t 5 -r 1000
# python main.py -b 7 -d 3 -t 5 -r 15 -s 1
# python main_wrapper.py -b 11 -d 5 -t 5 -r 32 -w 64 -s 0 -C 0.15 -O 4 -a 0.0 -g 1 # best yet
# python main_wrapper.py -b 11 -m 10000010 -v 1 -r 96 -d 5 -t 5 -w 64 -c 0.22 -g 1 -a 0.0 -z 0.1

"""
strategies
P-V V-R R-P; d = 5 // 3.75 hr

timeout among the best one
1 4 8: d = 5 // 5.4 hr

depth on best one
d = 3 5 8 10: t = 4 // 6.66 hr

exploration coefficient on the best one
# 0.05 0.1 0.25 0.35: t = 4 // 6.66 hr

rollout on best one
32 64 96: t = 4 // 5 hrs

board size (11, 19)
num_workers -> CONSTANT
alpha -> among the best one
beta  -> among the best one
gamma -> among the best one (high depth)
"""
