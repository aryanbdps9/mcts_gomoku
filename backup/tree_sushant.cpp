#include<bits/stdc++.h>

using namespace std;


class Tree{
	Game * game;
	int num_rollouts;
	int T = 0;
	int C;
	int maxdepth;
	int timeout;
	Node * root;
	int num_workers;

	Tree(Game * game, num_rollouts, C, max_depth, timeout, num_workers=4){
		this->game = game;
		this->num_rollouts = num_rollouts;
		this->C = C;
		this->max_depth = max_depth;
		this->timeout = timeout;
		this->num_workers = num_workers;
		int n = this->game->n;
		vector<vector<int> > board(n, vector<int> (n, 0));
		this->root = new Node(board, parent_action=-1, parent=NULL, gameover=0, turn=1);
	}

	vector<vector<int> > play_one_move(){
		int num_selects = 0;
		auto t_start = chrono::high_resolution_clock::now();
    	auto t_end = chrono::high_resolution_clock::now();
    	auto exec_time = chrono::duration_cast<chrono::duration<double>>(t_end - t_start) .count();
		while(exec_time < this->timeout){
    		t_start = chrono::high_resolution_clock::now();
    		exec_time = chrono::duration_cast<chrono::duration<double>>(t_end - t_start) .count();
    		root->select();
    		num_selects++;
		}
		vector<Node *> children = root->children;
		int num_child = children.size();
		int best_val = -1e9;
		int best_idx = -1;
		for(int i=0; i<num_child; i++){
			Node * child = children[i];
			int val = -child->val;
			if(best_idx == -1 or val >= best_val){
				best_val = val;
				best_idx = i;
			}
		}
		Node * best_child = children[best_idx];
		root = best_child;
		root->parent = NULL;
		root->parent_action = -1;
		root->offset_depth(-1);
		root->print_value();
		cout<<"performed "<<num_selects<<" iterations"<<endl;
		return root->board;
	}

	vector<vector<int> > player_move(vector<int> place){
		Node * newroot = NULL;
		vector<Node *> children = root->children;
		vector<int> actions = root->actions;
		int num_child = children.size();
		for(int i=0; i<num_child; i++){
			if(actions[i][0] == place[0] && actions[i][1] == place[1]){
				newroot = children[i];
			}
		}
		assert(newroot != NULL);
		root = newroot;
		root->parent = NULL;
		root->parent_action = -1;
		root->offset_depth(-1);
		return root->board;
	}
}



// class Tree:
// 	def __init__(self, game, num_rollouts, C, max_depth, timeout, num_workers=4):
// 		print("tree init")
// 		self.game = game
// 		self.num_rollouts = num_rollouts
// 		self.T = 0
// 		self.C = C
// 		self.maxdepth = max_depth
// 		self.timeout = timeout
// 		board = np.zeros((game.n, game.n))
// 		self.root = Node(self, board, parent_action=-1, parent=None, gameover=0, turn=1)
// 		self.num_workers = num_workers
	
// 	def play_one_move(self):
// 		print("tree.play_one_move")
// 		t_end = time.time() + self.timeout
// 		num_selects = 0
// 		# if True:
// 		while time.time() < t_end:
// 			self.root.select()
// 			num_selects += 1
// 		children = self.root.children
// 		# children = [child[0] for child in children]
// 		best_val = None
// 		best_idx = None
// 		for idx in range(len(children)):
// 			child = children[idx][0]
// 			val = -child.value
// 			# print(f"before:\tval:{val}\tbest_val:{best_val}\taction:{children[idx][1]}")
// 			if best_val is None or (val >= best_val):
// 				best_val = val
// 				best_idx = idx
// 			# print(f"after:\tval:{val}\tbest_val:{best_val}\taction:{children[idx][1]}")
// 		best_child = children[best_idx][0]
// 		self.root = best_child
// 		self.root.parent = None
// 		self.root.parent_action = -1
// 		self.root.offset_depth(-1)
// 		gc.collect()

// 		self.root.print_value()
// 		# self.root = Node(self, best_child.board, parent_action=-1, parent=None, turn=best_child.turn)
// 		print(f"performed {num_selects} iterations")
// 		return self.root.board

// 	def player_move(self, place):
// 		print("tree.player_move")

// 		place = np.array(place)
// 		newroot = None
// 		for child, action in self.root.children:
// 			if (np.array_equal(place, action)):
// 				newroot = child
// 				break
// 		assert newroot is not None
// 		self.root = newroot
// 		self.root.parent = None
// 		self.root.parent_action = -1
// 		self.root.offset_depth(-1)
// 		gc.collect()

// 		# board = self.root.board
// 		# turn = self.root.turn
// 		# board[place[0], place[1]] = turn
// 		# judgement = self.game.judge(board, place)
// 		# self.root = Node(self, board, parent_action=-1, parent=None, turn=((turn%2)+1))
// 		# if (judgement != 0):
// 		# 	self.root.gameover = judgement
// 		return self.root.board

// ###########################


class Node{
public:
	Tree * tree;
	vector< vector<int> > board;
	Node * parent;
	vector<Node*> children;
	vector<vector<int> > actions;
	double value = 0.0;
	int visits = 0;
	int depth;
	int self.turn;
	vector<int>parent_action(2);
	int gameover;

	Node(Tree tree, vector< vector<int> > board, int parent_action=-1, Node * parent=NULL, int gameover = 0, int turn = -1){
		this->tree = tree;
		this->board = board;
		this->parent = parent;
		this->depth = parent==NULL?1:parent->depth+1;
		this->turn = parent==NULL?turn:(parent->depth%2)+1;
		this->parent_action = parent_action;
		this->gameover = gameover;
	}

	static void generate_children(Node * node, vector<Node*> &children, vector<vector<int> > &actions){
		vector< vector<int> > board = node->board;
		int N = board.size();
		for(int i=0; i<N; i++){
			for(int j=0; j<N; j++){
				if(board[i][j] == 0){
					vector< vector<int> > newboard = board;
					newboard[i][j] = node->turn;
					vector<int> position(2) = {i,j};
					Node * child = new Node(node->tree, newboard, position, node);
					int judgement = child->tree->game->judge(newboard, position);
					if(judgement == node->turn){
						child->gameover = node->turn;
						children.clear();
						actions.clear();
						children.push_back(child);
						actions.push_back(position);
						return;
					}
					children.push_back(child);
					actions.push_back(position);
				}
			}
		}
	}

	void offset_depth(int offset = 0){
		this->depth += offset;
		int N = children.size();
		for(int i=0; i<N; i++){
			children[i]->offset_depth(offset);
		}
	}

	void print_value(){
		int N = children.size();
		for(int i=0; i<N; i++){
			int val = children[i]->value;
			if(val != 0.0){
				cout<<"["<<actions[0]<<", "<<actions[1]<<"]: "<<val<<"\t";
			}
		}	
	}
}

// class Node:
// 	def __init__(self, tree, board, parent_action=-1, parent=None, gameover=0, turn=-1):
// 		# print("node.init")
// 		self.tree = tree
// 		self.board = board
// 		self.parent = parent
// 		self.children = []
// 		self.value = 0.0
// 		self.visits = 0
// 		self.depth = 1 if parent is None else parent.depth + 1
// 		self.turn = turn if parent is None else ((parent.turn%2) + 1)
// 		self.parent_action = parent_action
// 		self.gameover = gameover
// 		# self._invboard = (board == 0).astype('uint8')

// 	@staticmethod
// 	def generate_children(node):
// 		# print("Node.generate_children")
// 		res = []
// 		free_locns = np.argwhere(node.board == 0) # array of nX2
// 		for position in free_locns:
// 			newboard = node.board.copy()
// 			newboard[position[0], position[1]] = node.turn
// 			child = Node(node.tree, newboard, position, node)
// 			judgement = child.tree.game.judge(newboard, position)
// 			if (judgement == node.turn):
// 				child.gameover = node.turn
// 				return [(child, position)]
// 			res.append((child, position))
// 		return res

// 	def offset_depth(self, offset = 0):
// 		self.depth += offset
// 		for child, action in self.children:
// 			child.offset_depth(offset=offset)

// 	def select(self):
// 		# print("select called")
// 		# it will return AFTER updating it's value function
// 		self.visits += 1
// 		self.tree.T += 1
// 		if (self.depth >= self.tree.maxdepth): # I'm a leaf
// 			# print("i'm a leaf and i'm calling rollouts")
// 			self.value = perform_rollouts(self.board.copy(), self.turn, self.parent_action, self.tree)
// 			return

// 		if (len(self.children) == 0):
// 			self.children = Node.generate_children(self)
// 		if (len(self.children) == 0): # no moves left
// 			self.value = 0.0
// 			return
// 			# raise NotImplementedError

// 		opp_move = ((self.turn%2)+1)
// 		bestUCT = None
// 		best_action = None
// 		best_child = None
// 		for child, action in self.children:
// 			if (child.gameover == self.turn):
// 				best_action = action
// 				best_child = child
// 				self.gameover = self.turn
// 				break
// 			elif (child.gameover == opp_move):
// 				uctval = (0-2)       + 0
// 			else:
// 				uct_opp, exploration_bonus = child.calcUCT()
// 				uctval = (0-uct_opp) + exploration_bonus 
// 			if (bestUCT is None or uctval >= bestUCT):
// 				bestUCT = uctval
// 				best_action = action
// 				best_child = child

// 		if (self.gameover == self.turn):
// 			self.value = 1
// 			return
		
// 		best_child.select()

// 		# SETTING VALUE 
// 		self.value = sum([self.children[idx][0].value * self.children[idx][0].visits for idx in range(len(self.children))]) / self.visits
// 		# self.value = sum([self.children[idx][0].value * self.children[idx][0].visits for idx in range(len(self.children))]) / sum([self.children[idx][0].visits for idx in range(len(self.children))])

// 	def print_value(self):
// 		actions = []
// 		values = []
// 		for child, action in self.children:
// 			val = child.value
// 			if (val != 0):
// 				values.append(val)
// 				actions.append(action)
// 		print(list(zip(actions, values)))

// 	def calcUCT(self):
// 		# print("calcUCT")
// 		uct_score = self.value, self.tree.C * np.sqrt(2*np.log(self.tree.T+1) / (self.visits+1))
// 		# print("calcUCT:", uct_score)
// 		return uct_score

// ##################################

// def rollout_workers(inqueue:mp.Queue, outqueue:mp.Queue, tree:Tree):
// 	# pull from inqueue and if game end, push to outqueue else push to inqueue
// 	# print("rollout_workers")
// 	while (True):
// 		job = inqueue.get()
// 		if job is None:
// 			break
// 		board = job[0]
// 		current_player = job[1]
// 		last_move_at = job[2]
// 		while(True):
// 			if (type(last_move_at) != int):
// 				judgement = tree.game.judge(board, last_move_at)
// 				if (judgement != 0):
// 					outqueue.put(judgement)
// 					break
// 					# continue
// 			if (np.count_nonzero(board) == board.size):
// 				outqueue.put(0)
// 				break
// 				# continue
// 			policy = tree.game.policies[current_player-1]
// 			next_move = policy(board, current_player)
// 			board[next_move[0], next_move[1]] = current_player

// 			current_player = ((current_player%2)+1)
// 			last_move_at = next_move

// 		# inqueue.put((board, ((current_player%2)+1), next_move))

// def rollout_workers_new(inqueue:mp.Queue, outqueue:mp.Queue, tree:Tree):
// 	# pull from inqueue and if game end, push to outqueue else push to inqueue
// 	# print("rollout_workers")
// 	while (True):
// 		job = inqueue.get()
// 		if job is None:
// 			break
// 		board_backup = job[0]
// 		val = 0.0
// 		num_itr = 1
// 		for itr in range(num_itr):
// 			board = board_backup.copy()
// 			current_player = job[1]
// 			last_move_at = job[2]
// 			while(True):
// 				if (type(last_move_at) != int):
// 					judgement = tree.game.judge(board, last_move_at)
// 					if (judgement != 0):
// 						val += judgement
// 						break
// 						# outqueue.put(judgement)
// 						# break
// 						# continue
// 				if (np.count_nonzero(board) == board.size):

// 					# outqueue.put(0)
// 					break
// 					# continue
// 				policy = tree.game.policies[current_player-1]
// 				next_move = policy(board, current_player)
// 				board[next_move[0], next_move[1]] = current_player

// 				current_player = ((current_player%2)+1)
// 				last_move_at = next_move
// 		outqueue.put(val / num_itr)

// 		# inqueue.put((board, ((current_player%2)+1), next_move))

// def perform_rollouts(board, current_player, last_move_at, tree:Tree):
// 	# print("perform_rollouts")
// 	num_rollouts = tree.num_rollouts
// 	inqueue = mp.Queue()
// 	outqueue = mp.Queue()
// 	pool = mp.Pool(processes=tree.num_workers, initializer=rollout_workers, initargs=(inqueue, outqueue, tree))
// 	for _ in range(num_rollouts):
// 		inqueue.put((board, current_player, last_move_at))
// 	temp_ret = []
// 	for _ in range(num_rollouts):
// 		temp_ret.append(outqueue.get())
// 	for _ in range(num_rollouts):
// 		inqueue.put(None)
// 	pool.close()
// 	pool.join()

// 	val = 0.
// 	opponent = ((current_player%2)+1)
// 	for ret in temp_ret:
// 		if ret == current_player:
// 			val += 1
// 		elif ret == opponent:
// 			val -= 1
// 		else:
// 			val += 0
// 	val /= num_rollouts
// 	return val
