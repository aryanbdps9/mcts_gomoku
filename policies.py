import numpy as np

def policy_random(board, current_player):
	free_locns = np.argwhere(board == 0)
	choice = np.random.choice(free_locns.shape[0])
	return free_locns[choice, :].flatten()

