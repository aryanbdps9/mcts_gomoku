#ifndef Included_NameModel_Tree

#define Included_NameModel_Tree

#include<bits/stdc++.h>
#include "game.h"
#include "policies.h"
#include "utils.h"
#include "shuffle_set.h"

using namespace std;

class Node;
class Tree;
template<typename TreeType>
class BaseNode;
class BaseTree;

class Tree{
	public:
	Game * game;
	int num_rollouts;
	int T = 0;
	double C;
	int maxdepth;
	int timeout;
	Node * root;
	int num_workers;
	double gamma = 0.99;
	double alpha = 0.1; // reward_shaping
	double beta = 0.1; // prior_try0
	double beta1 = 0.1; 

	Tree(Game * game, int num_rollouts, double C, int max_depth, int timeout, int num_workers=4, double gamma=1.0, double alpha=0.1, double beta=0.2, double beta1=0.1);

	vector<vector<int> > play_one_move(vector<int> &mymove);
	vector<vector<int> > player_move(vector<int> place);
};

template <typename TreeType>
class BaseNode{
public:
	TreeType* tree;
	vector<vector<int> > board;
	BaseNode<TreeType>* parent;
	unordered_map<int, BaseNode<TreeType>*> children;
	double value = 0.0;
	uint32_t visits = 0, turn;
	int parent_action = -1;

	BaseNode();
	BaseNode(int parent_action, BaseNode<TreeType>* parent); // for non-root init
	BaseNode(TreeType* tree, vector<vector<int> > board, uint32_t turn=1); //for root init
	~BaseNode();
	virtual bool killAllExcept(int action);
	virtual int select();
	virtual int expand();
	virtual double getUCT();
	bool isEmptyLocn(int a);
	inline bool isLeaf();
	inline bool isRoot();
	inline void inc_visit();
	inline void set_value(int value){this->value = value;}
	inline uint32_t get_visits();
};

template <typename TreeType>
class MtNode: public BaseNode<TreeType>{
public:
	mutex lock;
};

template<typename TreeType>
class VanillaNode:public BaseNode<TreeType>{
public:
	int potential, total_child_potential=0;
	int gameover = 0;
	double depth;
	VanillaNode(); 
	VanillaNode(int parent_action, VanillaNode<TreeType>* parent);// for non-root init
	VanillaNode(TreeType* tree, vector<vector<int> > board, uint32_t turn=1); //for root init
	~VanillaNode();
	virtual double getUCT();
	void calcUCT(double& uct_opp, double& exploration_bonus, int total_child_potential=0);
	virtual int expand();
	virtual int updateTCP();
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
	BaseTree(double C);
	~BaseTree();
	virtual void playout() = 0;
	virtual int getMove() = 0;
	virtual bool changeRoot(int action) = 0;
	virtual int gameJudge(vector<vector<int> > &board, int last_action);
};

class VanillaTree: public BaseTree{
public:
	VanillaNode<VanillaTree>* root;
	const int32_t num_rollout_workers, num_rollouts, max_depth, timeout;
	double gamma = 0.99;
	double alpha = 0.1; // reward_shaping
	double beta = 0.1; // prior_try0
	double beta1 = 0.1; // PUCT
	int (*potential_fn) (vector<vector<int> > &board, int last_action, int &gameover);
	// TODO: init potential_fn in constructors

	VanillaTree();
	VanillaTree(int num_rollouts, double C, int max_depth, int timeout, int num_rollout_workers=4, double gamma=1.0, double alpha=0.1, double beta=0.2, double beta1=0.1);
	~VanillaTree();
	virtual void playout();
	virtual int getMove();
	virtual double getRolloutValue(VanillaNode<VanillaTree>* leaf);
};

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
	int turn;
	vector<int> parent_action;
	int gameover;
	int potential;

	Node(Tree * tree, vector< vector<int> > board, vector<int> parent_action={-1,-1}, Node * parent=NULL, int potential=0, int gameover = 0, int turn = -1);

	static void generate_children(Node * node, vector<Node*> &children, vector<vector<int> > &actions);

	void offset_depth(int offset = 0);

	void select();

	void print_value();
	
	void calcUCT(double& uct_opp, double& exploration_bonus, int total_child_potential=0);

	double calcExplornBonus(int total_child_potential);

	vector<vector<double> > get_ExpBon_mat();

	vector<vector<int> > get_visit_mat();

	vector<vector<double> > get_UCT_mat();

	vector<vector<double> > get_Val_mat();

	vector<vector<double> > get_ValToEBRatio_mat();
};

#endif