import argparse
import os

def main():
	parser = argparse.ArgumentParser()
	parser.add_argument("-n", "--num_games", type=int, default=1)
	parser.add_argument("-b", "--board_size", type=int, default=15)
	parser.add_argument("-l", "--line_size", type=int, default=5)
	parser.add_argument("-m", "--mode", type=str, default="00000000")
	parser.add_argument("-v", "--verbose", type=int, default=0)

	parser.add_argument("-r", "--num_rollouts_1", type=int, default=100)
	parser.add_argument("-d", "--max_depth_1", type=int, default=5)
	parser.add_argument("-t", "--timeout_1", type=int, default=1)
	parser.add_argument("-w", "--num_workers_1", type=int, default=4)
	parser.add_argument("-c", "--exploration_coeff_1", type=float, default=1.0)
	parser.add_argument("-g", "--gamma_1", type=float, default=0.99)
	parser.add_argument("-a", "--alpha_1", type=float, default=0.1)
	parser.add_argument("-z", "--beta_1", type=float, default=0.1)

		
	parser.add_argument("-R", "--num_rollouts_2", type=int, default=-1)
	parser.add_argument("-D", "--max_depth_2", type=int, default=-1)
	parser.add_argument("-T", "--timeout_2", type=int, default=-1)
	parser.add_argument("-W", "--num_workers_2", type=int, default=-1)
	parser.add_argument("-C", "--exploration_coeff_2", type=float, default=-1.0)
	parser.add_argument("-G", "--gamma_2", type=float, default=-1.0)
	parser.add_argument("-A", "--alpha_2", type=float, default=-1.0)
	parser.add_argument("-Z", "--beta_2", type=float, default=-1.0)

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

	num_games = args.num_games
	N, linesize = args.board_size, args.line_size
	num_rollouts_1, max_depth_1, timeout_1 = args.num_rollouts_1, args.max_depth_1, args.timeout_1
	num_rollouts_2, max_depth_2, timeout_2 = args.num_rollouts_2, args.max_depth_2, args.timeout_2
	mode = int(args.mode, 2)
	os.makedirs('objects',exist_ok=True)
	os.system('make')
	cmd_str = f"./mcts.out {N} {linesize} {mode} {args.verbose} {num_rollouts_1} {max_depth_1} {timeout_1} {args.num_workers_1} {args.exploration_coeff_1} {args.gamma_1} {args.alpha_1} {args.beta_1} {num_rollouts_2} {max_depth_2} {timeout_2} {args.num_workers_2} {args.exploration_coeff_2} {args.gamma_2} {args.alpha_2} {args.beta_2}"
	for i in range(num_games):
		try:
			result = subprocess.run(cmd_str, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=None)
			if result.returncode == 0:
				print(int(result.stdout.decode('utf-8')))
			else:
				print(result.stderr.decode('utf-8').strip().split('\n')[-1])
		except:
			print("Execution Error")
		# os.system(cmd_str)


if __name__ == '__main__':
	main()