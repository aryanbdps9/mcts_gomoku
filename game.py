from policies import policy_random
import numpy as np


class Game:
	def __init__(self, n, linesize):
		self.n = n
		self.linesize = linesize
		self.policies = [policy_random, policy_random]

	def judge(self, board, currentpos):
		n, linesize = self.n, self.linesize
		r, c = currentpos[0], currentpos[1]
		currentmove = board[r, c]
		for row in range(max(0, r-linesize+1), min(r+1, n-linesize+1)):
			candidates = board[row:row+linesize, c]
			win = (candidates == currentmove).all()
			if (win):
				return currentmove
		for col in range(max(0, c-linesize+1), min(c+1, n-linesize+1)):
			candidates = board[r, col:col+linesize]
			win = (candidates == currentmove).all()
			if (win):
				return currentmove
		for t in range(0, linesize):
			if(r-t < 0 or r-t+linesize > n or c-t < 0 or c-t+linesize > n):
				continue
			candidates = np.diag(board[r-t:r-t+linesize, c-t:c-t+linesize])
			win = (candidates == currentmove).all()
			if (win):
				return currentmove
		for t in range(0, linesize):
			if(r+t >= n or r+t-linesize+1 < 0 or c-t < 0 or c-t+linesize > n):
				continue
			candidates = np.diag(np.fliplr(board[r+t-linesize+1:r+t+1, c-t:c-t+linesize]))
			win = (candidates == currentmove).all()
			if (win):
				return currentmove
		return 0
