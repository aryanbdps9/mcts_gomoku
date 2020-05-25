#ifndef Included_NameModel_Tree

#define Included_NameModel_Tree

#include<bits/stdc++.h>
#include "utils.h"
#include "shuffle_set.h"
#include "judge_and_potential.h"
#include "argdict.h"


using namespace std;

template<typename TreeType>
class BaseNode;
class BaseTree;

template <typename TreeType>
class BaseNode{
private:
	BaseNode();
public:
	TreeType* tree;
	vector<vector<int> > board;
	BaseNode<TreeType>* parent;
	int parent_action = -1;
	unordered_map<int, BaseNode<TreeType>*> children;
	double value = 0.0;
	uint32_t visits = 0, turn;

	// BaseNode() = 0;
	BaseNode(int parent_action, BaseNode<TreeType>* parent); // for non-root init
	BaseNode(TreeType* tree, vector<vector<int> > board, uint32_t turn=1); // for root init
	~BaseNode();
	virtual bool killAllExcept(int action);
	virtual int select();
	virtual int expand();
	virtual double getUCT();
	bool isEmptyLocn(int a);
	inline bool isLeaf();
	inline bool isRoot();
	inline void inc_visit();
	inline void set_value(double value){this->value = value;}
	inline uint32_t get_visits();
};

template <typename TreeType>
class MtNode: public BaseNode<TreeType>{
public:
	mutex lock;
};

template<typename TreeType>
class VanillaNode:public BaseNode<TreeType>{
private:
	VanillaNode();

public:
	int potential, total_child_potential=0;
	int gameover = 0;
	double depth;
	int gameoverCount=0;
	VanillaNode(int parent_action, VanillaNode<TreeType>* parent);// for non-root init
	VanillaNode(TreeType* tree, vector<vector<int> > board, uint32_t turn=1); //for root init
	~VanillaNode();
	virtual double getUCT();
	void calcUCT(double& uct_opp, double& exploration_bonus, int total_child_potential=0);
	virtual int select();
	virtual int expand();
	inline virtual void childGameOver(int newgameover);
	virtual void offsetDepth(int offset);
	inline virtual int getPotential(){return potential;}
	inline virtual int getTCP(){return total_child_potential;}
};

class BaseTree{
public:
	// BaseNode<BaseTree>* root;
	double C;
	BaseTree();
	// BaseTree(BaseNode<BaseTree>* root);
	BaseTree(double C): C(C){}
	~BaseTree(){}
	virtual void playout() = 0;
	virtual int getMove() = 0;
	virtual void opponentMove(int pos) = 0;
	// virtual bool changeRoot(int action) = 0;
	// int  (*gameJudge)(const vector<vector<int> > &, int, int);
	function<int(const vector<vector<int> > &, int, int)> gameJudge = judge;
	// virtual int gameJudge(vector<vector<int> > &board, int last_action);
};

class VanillaTree: public BaseTree{
private:
	VanillaTree();
public:
	VanillaNode<VanillaTree>* root;
	const int32_t num_rollout_workers, num_rollouts, max_depth, timeout, linesize;
	int max_rollout_len=50;
	double gamma = 0.99;
	double alpha = 0.1; // reward_shaping
	double beta = 0.1; // prior_try0
	double beta1 = 0.1; // PUCT
	int verbose = 0;
	int (*potential_fn) (const vector<vector<int> > &board, int last_action, int &gameover, int linesize);

	// VanillaTree() = 0; // Was giving some kind of error.
	VanillaTree(int linesize, int nr, int nc, int turn, int num_rollouts, double C, int max_depth, int timeout, int num_rollout_workers=4, double gamma=1.0, double alpha=0.1, double beta=0.2, double beta1=0.1, int potfn_v=1);
	VanillaTree(argdict vanillaTreeArgDict);
	~VanillaTree();
	virtual void playout();
	virtual int getMove();
	virtual void opponentMove(int pos);
	virtual double getRolloutValue(VanillaNode<VanillaTree>* leaf);
	inline virtual vector<vector<int> > getBoard(){return root->board;}
	inline virtual void setVerbosity(int v){
		this->verbose = v;
	}
};

// template <typename TreeType>
// class GameOverPropagatorNode:public VanillaNode<TreeType>{
// private:
// 	GameOverPropagatorNode();
// public:
// 	int gameoverCount = 0;
// 	GameOverPropagatorNode(int parent_action, GameOverPropagatorNode<TreeType>* parent); // for non-root init
// 	GameOverPropagatorNode(TreeType* tree, vector<vector<int> > board, uint32_t turn=1); //for root init
// 	~GameOverPropagatorNode();
// 	virtual void childGameOver(int newgameover);
// };

// class GameOverPropagatorTree: public VanillaTree{
// private:
// 	GameOverPropagatorTree();
// public:
// 	GameOverPropagatorTree(int linesize, int nr, int nc, int turn, int num_rollouts, double C, int max_depth, int timeout, int num_rollout_workers=4, double gamma=1.0, double alpha=0.1, double beta=0.2, double beta1=0.1, int potfn_v=1);
// 	GameOverPropagatorTree(argdict TreeArgDict);
// 	~GameOverPropagatorTree();
// };





















template <typename TreeType>
vector<vector<double> > get_Val_mat(BaseNode<TreeType>* node){
	int nr = node->board.size(), nc = node->board[0].size();
	vector<vector<double> > res(nr, vector<double> (nc, 0.0));
	for (auto [action, child]: node->children){
		res[action/nc][action%nc] = -child->value;
	}
	return res;
}

template <typename TreeType>
vector<vector<double> > get_UCT_mat(BaseNode<TreeType>* node){
	int nr = node->board.size(), nc = node->board[0].size();
	vector<vector<double> > res(nr, vector<double> (nc, 0.0));
	for (auto [action, child]: node->children){
		res[action/nc][action%nc] = child->getUCT();
	}
	return res;
}

template <typename TreeType>
vector<vector<int> > get_visit_mat(BaseNode<TreeType>* node){
	int nr = node->board.size(), nc = node->board[0].size();
	vector<vector<int> > res(nr, vector<int> (nc, 0));
	for (auto [action, child]: node->children){
		res[action/nc][action%nc] = child->get_visits();
	}
	return res;
}

template <typename TreeType>
vector<vector<int> > get_gameover_mat(BaseNode<TreeType>* node){
	int nr = node->board.size(), nc = node->board[0].size();
	vector<vector<int> > res(nr, vector<int> (nc, -2));
	for (auto [action, child]: node->children){
		res[action/nc][action%nc] = static_cast<VanillaNode<TreeType>*>(child)->gameover;
	}
	return res;
}



#endif