#include <sidlib.h>
#include <vector>
#include <stack>

#include "Relation2json.h"
#pragma once

using namespace std;

class TreeNode {
public:
	static int nid = 0;
	Table* t;
	TreeNode* firstchild;
	TreeNode* nextsibling;
public:
	TreeNode() {
		nid++;
		firstchild = nullptr;
		nexrsibling = nullptr;
	}
	TreeNode(int id) {
		nid = id;
		firstchild = nullptr;
		nextsibling = nullptr;
	}
	~TreeNode() = default;
};


class Forest {
private:
	TreeNode* firstroot;

public:
	void build(TreeNode* rootNode) {
		firstroot = rootNode;
	}
	
	bool contain(TreeNode* in, TreeNode* out) {
		TreeNode* par = firstroot;
		TreeNode* chi; 
		Forest* subforest;
		int halfcertain = 0;
		while (par) {
			if (par->nid == in->id) {
				par = cur->firstchild;
				while (chi) {
					if (chi->nid == out->id) {
						return true;
					}
					else {
						subforest->build(chi);
						if (subforest->contain(in, out)) {
							return true;
						}
						else {
							continue;
						}
					}
					chi = chi->nextsibling;
				}
				par = par->nextsibling;
		}
		return false;
	}
	
	Forest* insertLeaf(int anchor_id, int new_id) {
		TreeNode* newNode = new TreeNode(new_id);
		stack<TreeNode*> s;
		TreeNode* cur = firstroot;
		
		while (cur) {
			if (cur->nid == anchor_id) {
				newNode->nextsibling = cur->firstchild;
				cur->firstchild = newNode;
				return this;
			}
			else {
				s.push(cur);
			}
			if (!cur->firstchild) {
				while(!s.top()->nextsibling){
					s.pop();
					if (s.empty()) {
						printf("insert leaf error!, no %d found!\n", anchor_id);
					}
				}
				cur = s.top()->nextsibling;
				s.pop();
			}
		}
		printf("insert leaf error!, no %d found!\n", anchor_id);
	}

	Forest* insertRoot(int new_id) {
		TreeNode* newNode = new TreeNode(new_id);
		TreeNode* cur = firstroot;
		
		if (
		while (cur->nextsibling) {
			cur = cur->nextsibling;
		}
		cur->nextsibling = newNode;
		return this;
	}
	
	int Spacecost() {
		stack<TreeNode*> s;
		TreeNode* cur = firstroot;
		int res = 0;
		
		while (cur) {
			res += cur->t->table_size();
			s.push(cur);
			if (!cur->firstchild) {
				while(!s.top()->nextsibling) {
					s.pop();
					if (s.empty()) {
						return res;
					}
				}
				cur = s.top()->nextsibling;
				s.pop();
			}
			else {
				cur = cur->firstchild;
			}
		}
	}

//to do
	void print2debug() {	
		queue<TreeNode*> q;
		TreeNode* cur = firstroot;

		while (cur) {
			printf("%d	", cur->nid);
			q.push(cur);
			if (!cur->nextsibling) {
				printf(" | ");
				while(!q.front()->firstchild) {
					q.pop();
					if (q.empty()) {
						return ;
					}
				}
				printf("\n");
				cur = q.front()->firstchild;
				q.pop();
			}
			else {
				cur = cur->nextsibling;
			}
		}
	}
	
};
	
/*	
			if (halfcertain == 0) {
				if (cur->nid != in->id) {
					cur = cur->nextsibling;
				}
				else {
					halfcertain = 1;
					cur = cur->firstchild;
				}
			else {
				if (cur->nid == out->id) {
					return true;
				}
				else {
*/					
			
