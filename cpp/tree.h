#ifndef Included_NameModel_Tree

#define Included_NameModel_Tree

#include<bits/stdc++.h>
#include "game.h"
#include "policies.h"

using namespace std;

class Node;
class Tree;

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

	Tree(Game * game, int num_rollouts, double C, int max_depth, int timeout, int num_workers=4);

	vector<vector<int> > play_one_move();

	vector<vector<int> > player_move(vector<int> place);
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

	Node(Tree * tree, vector< vector<int> > board, vector<int> parent_action={-1,-1}, Node * parent=NULL, int gameover = 0, int turn = -1);

	static void generate_children(Node * node, vector<Node*> &children, vector<vector<int> > &actions);

	void offset_depth(int offset = 0);

	void select();

	void print_value();
	
	void calcUCT(int& uct_opp, int& exploration_bonus);
};

#endif