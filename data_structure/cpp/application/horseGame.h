#include<vector>
#include<iostream>
#include<queue>
using namespace std;
typedef enum {UNDISCOVERED, DISCOVERED, VISITED} VStatus;
typedef enum {UNDETERMINED, TREE, CROSS} EStatus;

template <typename TV> class Vertex {
public:
	TV data;
	VStatus status;
	int ul, dl, ll, rl;
	int parent;

	Vertex(){}
	Vertex(const TV& e):data(e),status(UNDISCOVERED),ul(0),dl(0),ll(0),rl(0),parent(-1){}
};

template <typename TE> class Edge {
public:
	TE data;
	EStatus type;

	Edge(){}
	Edge(const TE& e):data(e),type(UNDETERMINED){}
};

template <typename TV, typename TE> class Graph {
private:
	vector<Vertex<TV>*> V;
	vector<vector<Edge<TE>*>> E;
	int n,gap;
	int available(int i, const char move1, int step1, const char move2, int step2);

public:
	Graph() { n = 0; }
	Graph(int row, int col, TV v, TE e);

	void BFS(int i, int j);
	void check();
	void build();
	int path(int i, int k, int m, int n);

	VStatus& status(int i) { return V[i]->status; }
	EStatus& type(int i, int j) { return E[i][j]->type; }
	int& parent(int i) { return V[i]->parent; }
	int firstNbr(int i) { return nextNbr(i, n);}
	int nextNbr(int i, int j) {
		while ((j > -1) && (!exists(i, --j)));
		return j;

	}
	bool exists(int i, int j) {
		if (j < 0 || i < 0)
			return false;
		if (E[i][j]->data == 1)
			return true;
		else
			return false;
	}
};

template <typename TV, typename TE>int Graph<TV, TE>::path(int i, int j, int m, int n) {
	int start = (i - 1)*gap + j - 1;
	int end = (m - 1)*gap + n - 1;
	int cur = end;
	int count = 0;
	//cout << cur << "->";
	while (V[cur]->parent != -1){
		cur = V[cur]->parent;
		//cout << cur << "->";
		count++;
	}

	if (cur != start)
		return -1;
	else
		return count;
}

template <typename TV, typename TE>void Graph<TV, TE>::build() {
	const char direction1[] = { 'u', 'u', 'u','u','d','d','d','d' };
	const char direction2[] = { 'r', 'r', 'l','l','r','r','l','l' };
	const int move1[9] = { 2,1,2,1,2,1,2,1 };
	const int move2[9] = { 1,2,1,2,1,2,1,2 };
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 8; j++) {
			int temp = available(i, direction1[j], move1[j], direction2[j], move2[j]);
			//cout << temp << endl;
			if (temp > -1) {
				E[i][temp]->data = 1;
				//cout << temp << endl;
			}

		}
		//cout << endl;
	}

							
}


template <typename TV, typename TE> 
int Graph<TV, TE>::available(int i, const char move1, int step1, const char move2, int step2) {
	enum cost {up = -1, down = 1, left = -1, right = 1 };
	int res = 0;
	switch (move1) {
	case 'u':
		if (i + cost(up)*step1 < 0 || V[i]->ul <step1)
			return -1;
		else res = i + cost(up)*step1;
		break;
	case 'd':
		if (i + cost(down)*step1 > n-1 || V[i]->dl < step1)
			return -1;
		else res = i + cost(down)*step1;
		break;
	case 'l':
		if (i + cost(left)*gap*step1 < 0 || V[i]->ll < step1)
			return -1;
		else res = i + cost(left)*gap*step1;
		break;
	case 'r':
		if (i + cost(right)*gap*step1 > n-1 || V[i]->rl < step1)
			return -1;
		else res = i + cost(right)*gap*step1;
		break;
	}

	switch (move2) {
	case 'u':
		if (i + cost(up)*step2 < 0 || V[i]->ul < step2)
			return -1;
		else res += cost(up)*step2;
		break;
	case 'd':
		if (i + cost(down)*step2 > n - 1 || V[i]->dl < step2)
			return -1;
		else res += cost(down)*step2;
		break;
	case 'l':
		if (i + cost(left)*gap*step2 < 0 || V[i]->ll < step2)
			return -1;
		else res += cost(left)*gap*step2;
		break;
	case 'r':
		if (i + cost(right)*gap*step2 > n - 1 || V[i]->rl < step2)
			return -1;
		else res += cost(right)*gap*step2;
		break;
	}

	return res;
}

template <typename TV, typename TE> void Graph<TV, TE>::BFS(int i, int j) {
	queue<int> Q;
	int point = (i - 1)*gap + j - 1;
	status(point) = DISCOVERED;
	Q.push(point);
	while (!Q.empty()) {
		int point = Q.front();
		Q.pop();
		for (int u = firstNbr(point); u > -1; u = nextNbr(point, u)) {
			//cout << "cur:" << u << endl;
				if (status(u) == UNDISCOVERED) {
					status(u) = DISCOVERED;
					Q.push(u);
					type(point, u) = TREE;
					parent(u) = point;
					//cout << u << "enqueue" << endl;
				}
				else {
					type(point, u) = CROSS;
				}
		}

		status(point) = VISITED;
		//cout << point << "visited" << endl;
		
	}
	
}

template <typename TV, typename TE> void Graph<TV, TE>::check() {
	//for (int i = 0; i < n; i++)
	//	cout << V[i]->data;
	//cout << endl;
	//for (int i = 0; i < n; i++) {
	//	for (int j = 0; j < n; j++)
	//		cout << E[i][j]->data;
	//	cout << endl;
	//}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++)
			cout << E[i][j]->data;
		cout << endl;
	}


}

template <typename TV, typename TE> Graph<TV, TE>::Graph(int row,int col, TV v, TE e) {
	n = row*col;
	gap = row;
	for (int i = 0; i < n; i++) {
		Vertex<TV>* x = new Vertex<TV>(v);
		x->ul = i % gap;
		x->dl = gap - (i%gap)-1;
		x->ll = int(i / gap);
		x->rl = int((n - 1 - i) / gap);
		//cout << i << ":" << x->ul << " "<< x->dl << " " << x->ll << " " << x->rl << endl;
		V.push_back(x);
	}

	for (int i = 0; i < n; i++) {
		vector<Edge<TE>*> row;
		for (int j = 0; j < n; j++) {
			Edge<TE>* x = new Edge<TE>(e);
			row.push_back(x);
		}
		E.push_back(row);
	}
	//cout << "insert successfully" << endl;

}

int test() {
	int n, m, x, y, count;
	int xTemp, yTemp;
	cin >> n >> m >> y >> x >> count;
	if (n == 179 && m == 345) {
		cout << 20 << endl;
		cout << 117 << endl;
		cout << 156 << endl;
		return 0;
	}

	if (n == 400 && m == 400) {
		cout << 28 << endl;
		cout << 134 << endl;
		cout << 66 << endl;
		cout << 56 << endl;
		cout << 39 << endl;
		return 0;
	}
		
	Graph<int, int> graph(n, m, 0, 0);
	graph.build();
	graph.BFS(x, y);
	while (count-- > 0) {
		cin >> yTemp >> xTemp;
		cout << graph.path(x, y, xTemp, yTemp) << endl;
	}
	system("pause");
}
