#include "Relation2json.h"

#define A 0.5
#define B 0.5
#define FILENAME "test.data"
#define TABLENUM 8
#define QUERYNUM 3

static double normalizedCost(long io, long space, long base_io, long base_space) {
	float res;
	res = A * (float) io / (float) base_io + B * (float) space / (float) base_space;
	return res;
}
	

//figure out the cost of an arbitrary forest
static void ForestCost (Forest* f, vector<Query*>* q, vector<float>* w, Graph* g) {
    //space_cost
    long space = f->Spacecost();
    long io = 0;
    //for all queries
    for (uint32_t i = 0; i < q->size(); i++) {
        // io_cost
        io += (* q)[i]->IOcost(f) * (* w)[i];
    }
    
    //baseline;
    long base_space_cost = g->BaselineSpacecost();
    long base_io_cost = 0;
    
    for (uint32_t k = 0; k < q->size(); k++) {
        base_io_cost += (* q)[k]->BaselineIOcost();
    }
    
    double cost = normalizedCost(io, space, base_io_cost, base_space_cost);
    f->print2debug();
    printf("space_cost = %ld, base_space_cost = %ld\n", space, base_space_cost);
    printf("io_cost = %ld, base_io_cost = %ld\n", io, base_io_cost);
    printf("cost = %f\n", cost);
}


static vector<Forest*> Relational2JSON(Graph* g, vector<Query*>* q, vector<float>* w) {
	vector<int> sortedgraph = g->topoSort();
	vector<Forest*> tmp_forests;
	
//	vector<int> io_cost, space_cost, cost;
	double mincost = 0.0;
	long base_space_cost = g->BaselineSpacecost();
	long base_io_cost = 0;
	
	for (uint32_t k = 0; k < q->size(); k++) {
		base_io_cost += (* q)[k]->BaselineIOcost();
	}
 
	//for all vertices that has been sorted
	for (uint32_t i = 0; i < sortedgraph.size(); i++) {
		//produce forests one by one
		if (i == 0) {
			Forest* initialforest = new Forest();
			tmp_forests.push_back(initialforest->insertRoot(sortedgraph[i],g->sortedTable(i)));
		}
		else {
			g->createnextForest(i, tmp_forests);
		}
		vector<double> cost(tmp_forests.size());
		vector<long> io_cost(tmp_forests.size());
		vector<long> space_cost(tmp_forests.size());
        
        printf("Node num is %d. Forests without picking are as following\n", i + 1);
        
		//for all forests that created last time( without one vertex) 
		for (uint32_t j = 0; j < tmp_forests.size(); j++) {
            printf("tmp_forests[%u]:\n", j);
            tmp_forests[j]->print2debug();
            //space_cost
			space_cost[j] = tmp_forests[j]->Spacecost();
			//for all queries
			for (uint32_t k = 0; k < q->size(); k++) {
				(* w)[k] = 1/((float) q->size());
				// io_cost
				io_cost[j] += (* q)[k]->IOcost(tmp_forests[j]) * (* w)[k];
			}
			cost[j] = normalizedCost(io_cost[j], space_cost[j], base_io_cost, base_space_cost);
            
            printf("space_cost = %ld, io_cost = %ld, cost = %f\n\n", space_cost[j], io_cost[j], cost[j]);
			
            if (j == 0) {
				mincost = cost[j];
			}
			else {
				mincost = (mincost <= cost[j])? mincost : cost[j];
			}
		}

		//kick off those forests whose cost is not min
		printf("mincost = %f \n", mincost);
		int deletenum = 0;
		for (uint32_t j = 0; j < tmp_forests.size() + deletenum; j++) {
			if (cost[j] != mincost) {
				tmp_forests.erase(tmp_forests.begin() + j - deletenum);
				deletenum ++;
			}
		}
		
		for (uint32_t j = 0; j < tmp_forests.size(); j++) {
			tmp_forests[j]->print2debug();
			printf("\n\n");
		}
	}


	return tmp_forests;
}



int main() {
	vector<Query*> q;	// queries
	vector<float> w(QUERYNUM);	// weight for queries

	//construct graph
	vector<Table*> tables(TABLENUM);
	/*
	for (uint32_t k = 0; k < TABLENUM; k++) {
		tables[k] = new Table(k+1, 2*(k+1));
	}	
	vector<vector<int>> matrix(TABLENUM);
	for (uint32_t k = 0; k < TABLENUM; k++) {
		matrix[k].reserve(TABLENUM);
		for (uint32_t l = 0; l < TABLENUM; l++) {
			if ((k == 0 || k == 2) && (l == 1)) {
				matrix[k].push_back(1);
			}
			else {
				matrix[k].push_back(0);
			}
		}
	}
	*/

	//tpc-h debug
	tables[0] = new Table(78, 5, "R");
	tables[1] = new Table(89, 25, "N");
	tables[2] = new Table(141, 10000, "S");
	tables[3] = new Table(162, 150000, "C");
	tables[4] = new Table(115, 1500000, "O");
	tables[5] = new Table(121, 200000, "P");
	tables[6] = new Table(149, 800000, "PS");
	tables[7] = new Table(127, 6000000, "L");

	vector<vector<int>> matrix(TABLENUM);
	for (uint32_t k = 0; k < TABLENUM; k++) {
		matrix[k].reserve(TABLENUM);
		for (uint32_t l = 0; l < TABLENUM; l++) {
			matrix[k][l] = 0;
		}
	}
	matrix[0][1] = 1;
	matrix[1][2] = 1;
	matrix[1][3] = 1;
	matrix[2][6] = 1;
	matrix[3][4] = 1;
	matrix[4][7] = 1;
	matrix[5][6] = 1;
	matrix[6][7] = 1;
	
	Graph* g = new Graph(tables, matrix);

	//init queries
	for (uint32_t i = 0; i < QUERYNUM; i++) {
		q.push_back(new Query());
		//q[i]->buildfromFile(FILENAME, &tables);
	}
	q[0]->buildfromFile("q2.data", &tables);
	q[1]->buildfromFile("q5.data", &tables);
	q[2]->buildfromFile("q10.data", &tables);

	
	vector<Forest*> bestforests = Relational2JSON(g, &q, &w);
/*	
	for (uint32_t j = 0; j < bestforests.size(); j++) {
		bestforests[j]->print2debug();
		printf("\n\n\n");
	}
*/
	return 1;
}

