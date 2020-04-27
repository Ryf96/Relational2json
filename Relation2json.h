#include <stdio.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>
#include <utility>
#include <string.h>

#pragma once
#define FACTOR 1
#define LINESIZE 100
#define THRESHOLD 100
using namespace std;

class Table {
private:
	int record_size; 	//size of a record
	long record_num;  	//num of records
    string table_name;

public:
	Table() {
		record_size = 0;
		record_num = 0;
        table_name = "";
	}
	Table(int size, int num, string name) {
		record_size = size;
		record_num = num;
        table_name = name;
	}
	~Table(void) = default;

public:
	long TableSize() {
		long table_size = record_size * record_num;
		return table_size;
	}

	long RecordNum() {
		return record_num;
	}
    
    string TableName() {
        return table_name;
    }
/*	
	void set(int size, int num) {
		record_size = size;
		record_num = num;
	}
*/
};

class TreeNode {
public:
	int nid;
	Table* t;
	TreeNode* firstchild;
	TreeNode* nextsibling;
public:
	
	TreeNode() {
		nid = 0;
		t = new Table();
		firstchild = nullptr;
		nextsibling = nullptr;
	}
	
	TreeNode(int id) {
		nid = id;
		t = new Table();
		firstchild = nullptr;
		nextsibling = nullptr;
	}
	
	TreeNode(TreeNode* node) {
		nid = node->nid;
		t = node->t;
		firstchild = nullptr;
		nextsibling = nullptr;
	}

	~TreeNode() = default;
public:
	void setTable(Table* table) {
		t = table;
	}

};


class Forest {
private:
	TreeNode* firstroot;

public:
	Forest() {
		firstroot = nullptr;
	}
	Forest(Forest* f) {
		firstroot = f->firstroot;
	}
	void build(TreeNode* rootNode) {
		firstroot = rootNode;
	}
	
	bool contain(TreeNode* in, TreeNode* out) {
		TreeNode* par = firstroot;
		TreeNode* chi; 
		stack<TreeNode*> s;
		
		while (par) {
			s.push(par);
			if (par->nid == in->nid) {
				chi = par->firstchild;
				while (chi) {
					if (chi->nid == out->nid) {
						return true;
					}
					else {
						chi = chi->nextsibling;
					}
				}
			}
			if (!par->firstchild) {
				while(!s.top()->nextsibling) {
					s.pop();
					if (s.empty()) {
						return false;
					}
				}
				par = s.top()->nextsibling;
				s.pop();
			}
			else {
				par = par->firstchild;
			}
		}
		return false;
					
	}
	/*	
	Forest* insertLeaf(int anchor_id, int new_id, Table* t) {
		Forest* res = this->copyForest();
		TreeNode* newNode = new TreeNode(new_id);
		newNode->setTable(t);
		stack<TreeNode*> s;
		TreeNode* cur = res->firstroot;
		
		while (cur) {
			if (cur->nid == anchor_id) {
				newNode->nextsibling = cur->firstchild;
				cur->firstchild = newNode;
				return res;
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
			else {
				cur = cur->firstchild;
			}
		}
		printf("insert leaf error!, no %d found!\n", anchor_id);
		exit(0);
	}

*/
	Forest* copyForest() {
		stack<TreeNode*> s, t;
		TreeNode* base = firstroot;
		TreeNode* cur = nullptr;
		TreeNode* tail = nullptr;
		TreeNode* newroot = nullptr;
		Forest* res = new Forest();

		while (base) {
			if (cur == nullptr) {
				cur = new TreeNode(base);
				if (tail) {
					tail->nextsibling = cur;
				}
			}
			else {
				cur->firstchild = new TreeNode(base);
				cur = cur->firstchild;
			}

			if (cur->nid == firstroot->nid) {
				newroot = cur;
			}
			s.push(base);
			t.push(cur);

			if (!base->firstchild) {
				while (!s.top()->nextsibling) {
					s.pop();
					t.pop();
					if (s.empty()) {
						res->build(newroot);
						return res;
					}
				}
				base = s.top()->nextsibling;
				s.pop();
				cur = t.top()->nextsibling;
				tail = t.top();
				t.pop();
			}
			else {		
				base = base->firstchild;
			}
		}
		return res;
	}


	Forest* insertLeaf(int anchor_id, int new_id, Table* t) {
		Forest* res = this->copyForest();
		TreeNode* newNode = new TreeNode(new_id);
		newNode->setTable(t);
		stack<TreeNode*> s;
		TreeNode* cur = res->firstroot;
		
		while (cur) {
			if (cur->nid == anchor_id) {
				newNode->nextsibling = cur->firstchild;
				cur->firstchild = newNode;
				return res;
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
			else {
				cur = cur->firstchild;
			}
		}
		printf("insert leaf error!, no %d found!\n", anchor_id);
		exit(0);
	}

	Forest* insertRoot(int new_id, Table* t) {
		Forest* res = this->copyForest();;
		TreeNode* newNode = new TreeNode(new_id);
		newNode->setTable(t);
		if (res->firstroot == nullptr) {
			res->firstroot = newNode;
			return res;
		}

		TreeNode* cur = res->firstroot;
		
		while (cur->nextsibling) {
			cur = cur->nextsibling;
		}
		cur->nextsibling = newNode;
		return res;
	}
		
	long Spacecost() {
		stack<TreeNode*> s;
		TreeNode* cur = firstroot;
		long res = 0;
		
		while (cur) {
			res += cur->t->TableSize();
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
		return res;
	}

//to do
/*
	void print2debug() {	
		queue<TreeNode*> q1, q2;
		queue<TreeNode*> emptyqueue;
		TreeNode* cur = firstroot;

		while (cur) {
			printf("%d	", cur->nid);
			q2.push(cur);
			if (!cur->nextsibling) {
				printf("|	");
				if (q1.empty()) {
					printf("\n");
					cur = q2.front()->firstchild;
					q2.pop();
					q1.swap(q2);
					q2.swap(emptyqueue);
				}
				else {
					while(!q1.front()->firstchild) {
						q1.pop();
						if (q1.empty()) {
							return ;
						}
					}
					cur = q1.front()->firstchild;
					q1.pop();
				}
			}
			else {
				cur = cur->nextsibling;
			}
		}
	}
*/
    void printstack(stack<TreeNode*> s) {
        stack<string> t;
        
        while (!s.empty()) {
            t.push(s.top()->t->TableName());
            s.pop();
        }
        
        while (!t.empty()) {
            printf(" %s ", t.top().c_str());
            t.pop();
            if (!t.empty()) {
                printf("-->");
            }
        }
        printf("\n");
    }
    
	void print2debug() {
        stack<TreeNode*> s;
        TreeNode* cur = firstroot;
        
        while (cur) {
            s.push(cur);
            if (!cur->firstchild) {
                printstack(s);
                while (!s.empty()) {
                    cur = s.top()->nextsibling;
                    if (cur) {
                        s.pop();
                        break;
                    }
                    s.pop();
                }
            }
            else {
                cur = cur->firstchild;
            }
        }
	}
			
};


class Join {
public:
	TreeNode* in;
	TreeNode* out;

public:
	Join(TreeNode* c_in, TreeNode* c_out) {
		in = c_in;
		out = c_out;
	}
};


class Query {
private:
	vector<Join*> joinpairs;
	vector<int> cost_in;
	vector<int> cost_out;

public:
	void buildfromFile(string filename, vector<Table* >* tables) {
		FILE* fp;
		char* line = new char[LINESIZE];
		char* token;

		TreeNode* in_tmp;
		TreeNode*  out_tmp;
		fp = fopen(filename.c_str(), "r");
		if (fp == NULL) {
			printf("read file '%s' failed!\n", filename.c_str());
			return ;
		}

		// read a line from file
		while ((fgets(line, LINESIZE, fp) != NULL)) {
			//split the line with " "
			token = strtok(line, " ");
			if (token != NULL) {
				in_tmp = new TreeNode(atoi(token));
				in_tmp->setTable((* tables)[atoi(token)]);
			}
			token = strtok(NULL, " ");
			if (token != NULL) {
				out_tmp = new TreeNode(atoi(token));
				out_tmp->setTable((* tables)[atoi(token)]);
			}
			//construct joinpairs
            joinpairs.push_back(new Join(in_tmp, out_tmp));
            //construct cost_in and cost_out
            token = strtok(NULL, " ");
            if (token != NULL) {
                cost_in.push_back(atoi(token));
            }
            token = strtok(NULL, " ");
            if (token != NULL) {
                cost_out.push_back(atoi(token));
            }
		}
		fclose(fp);
	}

	long IOcost(Forest* f) {
		long cost = 0;
		for (uint32_t i = 0; i < joinpairs.size(); i++) {
			if (!f->contain(joinpairs[i]->in, joinpairs[i]->out)) {
				cost += cost_in[i] * joinpairs[i]->in->t->RecordNum() * FACTOR;
				cost += cost_out[i] * joinpairs[i]->out->t->RecordNum() * FACTOR;
			}
		}
		return cost;
	}
	
	long BaselineIOcost() {
		long cost = 0;
		for (uint32_t i = 0; i < joinpairs.size(); i++) {
			cost += cost_in[i] * joinpairs[i]->in->t->RecordNum() * FACTOR;
			cost += cost_out[i] * joinpairs[i]->out->t->RecordNum() * FACTOR;
		}
		return cost;
	}
};	 


class Graph {
private:
	int num;  					//num of vertices
	vector<int> id; 			//id of all vertices;
	vector<Table*> tables; 		//all tables (vertices);
	list<int>* joinmap;  		//joinmap of the graph
	vector<int> in_degree; 		//in degree of all vertices
	vector<int> sortedID;      //ID after toposort
    list<int>* joinmap_valid;   // joinmap after picking off unvalid pairs

public:
	//construct Graph with Tables[] and their connection matrix
	Graph(vector<Table*> &t, vector<vector<int>>& matrix) {
		num = t.size();
		vector<int> init(num);
		in_degree = init;
		id.reserve(num);
		tables = t;
		joinmap = new list<int>[num];	
		joinmap_valid = new list<int>[num];
        
		for (int i = 0; i < num; i++) {
			id.push_back(i);
			for (int j = 0; j < num; j++) {
				if (matrix[i][j]) {
					joinmap[i].push_back(j);
					in_degree[j] ++;
                    //set joinmap_valid
                    if (tables[j]->RecordNum() / tables[i]->RecordNum() < THRESHOLD) {
                        joinmap_valid[i].push_back(j);
                    }
				}
			}
		}
        
        
/*
        for (int k = 0; k < num; k++) {
            list<int>::iterator iter = joinmap[k].begin();
            while (iter != joinmap[k].end()) {
                if (tables[*iter]->RecordNum() / tables[k]->RecordNum() > THRESHOLD) {
                    joinmap_valid[k].push_back(*iter);
                }
            }
        }
 */
	} 
	
    
	Table* sortedTable(int i) {
		for (int j = 0; j < num; j++) {
			if (sortedID[i] == id[j]) {
				return tables[j];
			}
		}
		printf("sorted id not found!\n");
		exit(0);
	}

	long BaselineSpacecost() {
		long cost = 0;
		for (int i = 0; i < num; i++) {
			cost += tables[i]->TableSize();
		}
		return cost;
	}
	
	vector<int> topoSort() {
		queue<int> q;       // queue for sort
		for (int i = 0; i < num; i++) {
			if (in_degree[i] == 0) {
				q.push(i);
			}
		}
		int count = 0;
		while (!q.empty()) {
			int head = q.front();
			sortedID.push_back(id[head]);
			q.pop();
			list<int>::iterator it = joinmap[head].begin();
			for (; it != joinmap[head].end(); it++) {
				if (--in_degree[*it] == 0)	{	q.push(*it);	}
			}
			count++;
		}
		if (count < num) {
			printf("sort num error!\n");
			exit(1);
		}
		return sortedID;
	}

	void createnextForest(int index, vector<Forest*>& oldforests) {
		//vector<vector<Forest*>> res_tmp;	// result
		//res_tmp.resize(index);
        vector<Forest*> res_tmp;
		unordered_map<int, int> sorted2chaos;	// map from index of sortedid[] to id[]
		
	
		// construct the map from index of sorted_id[] to id[];
		for (uint32_t i = 0; i < id.size(); i++) {
			for (uint32_t m = 0; m < sortedID.size(); m++) {
				if (id[i] == sortedID[m]) {
					sorted2chaos.insert(make_pair<int, int>(m, i));
					break;
				}
			}
		}
/*
		if (new_id == id[i]) {
				index = i;
				break;
			}
		if (index < 0) {
			printf("new_id not found!\n");
			exit(0);
		}
*/

		int chaos_id;
/*
		int added = 0;
		//init, deal with the first element in sorted_id[]
		//for all old forests
		for (uint32_t k = 0; k < oldforests.size(); k++) {
			//find index in id[], whose index in sorted_id[] is 0  
			unordered_map<int, int>::const_iterator got = sorted2chaos.find(0);
			if (got == sorted2chaos.end()) {
				printf("find 0 in sorted2chaos error!\n");
				exit(0);
			}
			else {
				chaos_id = got->second;	//assign chaos_id
			}
			

			for (list<int>::iterator it = joinmap[chaos_id].begin(); it != joinmap[chaos_id].end(); it++) {
				if (*it == sorted2chaos[index]) {
					res_tmp[0].push_back(oldforests[k]->insertLeaf(chaos_id, sortedID[index], tables[sortedID[index]]));
					added = 1;
				}
				else {
					res_tmp[0] = oldforests;
				}
			}
        }
 */
		/*
			if (res_tmp[0].empty()) {
				printf("empty!\n");
			}
		*/
		//	res_tmp[0].push_back(oldforests[k]->insertRoot(sortedID[index], tables[sortedID[index]]));
		

        
        /*
		//for all vertices that have been exsited
		for (int j = 1; j < index; j++) {
			//find index in id[], whose index in sorted_id[] is j  
			unordered_map<int, int>::const_iterator got = sorted2chaos.find(j);
			if (got == sorted2chaos.end()) {
				printf("find j in sorted2chaos error!\n");
				exit(0);
			}
			else {
				chaos_id = got->second;
			}

			//for all old forests
			for (uint32_t k = 0; k < res_tmp[j-1].size(); k++) {
				if (added == 1) {
					res_tmp[j].push_back(res_tmp[j-1][k]);
				}
			}

			for (uint32_t k = 0; k < res_tmp[j-1].size(); k++) {
				for (list<int>::iterator it = joinmap[chaos_id].begin(); it != joinmap[chaos_id].end(); it++) {
					if (*it == sorted2chaos[index]) {
						res_tmp[j].push_back(res_tmp[j-1][k]->insertLeaf(chaos_id, sortedID[index], tables[sortedID[index]]));
						added = 1;
					}	
				}
				if (added == 0) {
					res_tmp[j].push_back(res_tmp[j-1][k]);
				}
			}
		}
		
		vector<Forest*> oldtmp;
		oldtmp = oldforests;

		if (added == 1) {
			oldforests = res_tmp[index-1];
		}
		else {
			oldforests.clear();
		}
		int msize = oldtmp.size();
		for (uint32_t m = 0; m < msize; m++) {
				//res_tmp[index-1].push_back(oldforests[m]->insertRoot(sortedID[index], tables[sortedID[index]]));
			oldforests.push_back(oldtmp[m]->insertRoot(sortedID[index], tables[sortedID[index]]));
		}
        */

        for (int j = 0; j < index; j++) {
            unordered_map<int, int>::const_iterator got = sorted2chaos.find(j);
            if (got == sorted2chaos.end()) {
                printf("find j in sorted2chaos error!\n");
                exit(0);
            }
            else {
                chaos_id = got->second;
            }
            
            for (uint32_t k = 0; k < oldforests.size(); k++) {
                for (list<int>::iterator it = joinmap_valid[chaos_id].begin(); it != joinmap_valid[chaos_id].end(); it++) {
                    if (*it == sorted2chaos[index]) {
                        res_tmp.push_back(oldforests[k]->insertLeaf(chaos_id, sortedID[index], tables[sortedID[index]]));
                    }
                }
            }
        }
        for (uint32_t k = 0; k < oldforests.size(); k++) {
            res_tmp.push_back(oldforests[k]->insertRoot(sortedID[index], tables[sortedID[index]]));
        }
        oldforests = res_tmp;
	}
         
        
        

};
