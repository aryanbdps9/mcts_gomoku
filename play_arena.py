import argparse
import os
import subprocess

def main():
	parser = argparse.ArgumentParser()
	parser.add_argument("config", type=str)
	parser.add_argument("n", type=int, default=25)
	parser.add_argument("log")
	parser.add_argument("parsed")
	args = parser.parse_args()

	with open(args.config, 'r') as cf:
		config_str = cf.read()

	n = args.n
	configs = config_str.split("\n")
	logfile = open(args.log, 'a')
	parsedfile = open(args.parsed, 'w')
	for config in configs:
		config = config.strip()
		if (len(config) == 0):
			continue

		wc, lc, dc = [0]*3
		match_count = 0
		cmd_str = config
		for run_ in range(n):
			print("\t".join(map(str, [wc, lc, dc, match_count])) + "\n")
			try:
				result = subprocess.run(cmd_str, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=None)
				decoded = result.stdout.decode('utf-8')
				if (decoded[0] == '1'):
					wc += 1
				elif (decoded[0] == '2'):
					lc += 1
				else:
					dc += 1
				match_count += 1
				
			except Exception as e:
				decoded = str(e)
				pass
			logfile.write(cmd_str+'\n')
			logfile.write(decoded+'\n')
			logfile.write("\t".join(map(str, [wc, lc, dc, match_count])) + "\n")
			logfile.write('#'*40+'\n')
			logfile.flush()
		parsedfile.write("\t".join(map(str, [wc, lc, dc, match_count])) + "\n")
		parsedfile.flush()

if __name__ == '__main__':
	main()
