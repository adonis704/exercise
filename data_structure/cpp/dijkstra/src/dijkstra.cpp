#include"dijkstra.h"

Dijkstra::Dijkstra(int num) {
	Node* temp;
	for (int i = 0; i < num; i++) {
		temp = new Node(i);
		cities.push_back(temp);
	}
	generate();
}

//对每个城市，随机生成可达城市以及权重
void Dijkstra::generate() {
	vector<Node*> node;
	vector<int> cost;

	for (int i = 0; i < cities.size(); i++) {
		node = randNode(i);
		cost = randCost(node.size());
		cities[i]->available = node;
		cities[i]->cost = cost;
	}

	Node* cur;	//当前结点
	Node* target;	//当前结点的某个可达结点
	int pos;
	//检查每个城市，如果不在其可达城市的可达列表内，则增加相应项，否则，使权重相等
	for (int i = 0; i < cities.size(); i++) {
		cur = cities[i];
		for (int j = 0; j < cur->available.size(); j++) {
			target = cur->available[j];
			node = target->available;
			pos = find(cur, node);
			if ( pos == -1) {
				target->available.push_back(cur);
				target->cost.push_back(cur->cost[j]);
			}
			else 
				target->cost[pos] = cur->cost[j];
		}
	}
}
//输入城市位置，返回随机产生的其他可达城市的指针的数组
vector<Node*> Dijkstra::randNode(int pos) {
	vector<Node*> res;
	int temp;
	for (int i = 0; i < CITY_NUM; i++) {
		temp = rand() % cities.size();
		if(temp != pos && find(cities[temp],res) == -1) res.push_back(cities[temp]);
	}
	return res;
}
//输入城市数目，返回随机生成的权重数组
vector<int> Dijkstra::randCost(int num) {
	vector<int> res;
	for (int i = 0; i < num; i++)
		res.push_back(rand() % MAX_COST+1);
	return res;
}

//打印可达关系
void Dijkstra::print() {
	cout << "- - - - - - - - - - - - - - -" << endl;
	for (int i = 0; i < cities.size(); i++) {
		cout << "城市 " << i << " 可以到达: ";
		for (int j = 0; j < cities[i]->available.size(); j++)
			cout << "城市 " << cities[i]->available[j]->name << ":" << cities[i]->cost[j] << "  ";
		cout << endl;
	}	
	cout << "- - - - - - - - - - - - - - -" << endl;
}

//求解最短路径
void Dijkstra::solve(int start, int end) {
	//将起点加入堆中
	Heap<Node*> heap;	//待检查结点
	vector<Node*> done;	//已经检查完成的结点
	cities[start]->weight = 0;
	heap.insert(cities[start]);

	Node* cur;
	vector<Node*> available;
	vector<int> cost;
	int weight;
	int size;
	cout << "- - - - - - - - - - - - - - -" << endl;

	//dijkstra算法
	while (heap.size() > 0) {
		cur = heap.pop();
		available = cur->available;
		cost = cur->cost;
		size = available.size();

		for (int i = 0; i < size; i++) {
			if (find(available[i], done) != -1) continue;
			weight = cur->weight + cost[i];
			if (available[i]->weight == infinity) {
				heap.insert(available[i]);
				available[i]->weight = weight;
				available[i]->parent = cur;
			}
			else {
				if (available[i]->weight > weight) {
					cout << "改变城市" << available[i]->name << "的开销，由 "
						<< available[i]->weight << " 到 " << weight << endl;
					available[i]->weight = weight;
					available[i]->parent = cur;
					heap = update(heap);
				}
			}
				
		}
		done.push_back(cur);
		cout << "城市 "<< cur->name << " 检查完毕" << endl;
	}
	cout << "- - - - - - - - - - - - - - -" << endl;

	Node* target = cities[end];
	if (target->weight == infinity || target->parent == NULL) cout << "无解" << endl;
	else print(cities[start], target);
	//求解完成或无解后，将所有城市的开销，父结点初始化
	init();
}
//输入结点指针，结点指针数组，返回结点指针在数组中的位置，不在则返回-1
int Dijkstra::find(Node* target, vector<Node*> done) {
	for (int i = 0; i < done.size(); i++)
		if (target == done[i]) return i;
	return -1;
}
//更新堆，保持堆序性
Heap<Node*> Dijkstra::update(Heap<Node*> heap) {
	Heap<Node*> res;
	while (heap.size() > 0)
		res.insert(heap.pop());
	return res;
}
//输出路径
void Dijkstra::print(Node* start, Node* target) {
	Node* cur;
	stack<Node*> stack;
	cur = target;
	while (cur->parent != NULL) {
		stack.push(cur);
		cur = cur->parent;
	}

	cout << "路径:"<<start->name << "-";
	while (stack.size() > 0) {
		cur = stack.top();
		stack.pop();
		if (stack.size() > 0) cout << cur->name << "-";
		else cout << cur->name;
	}
	cout << " 开销:" << target->weight << endl;
}
//初始化所有城市
void Dijkstra::init() {
	for (int i = 0; i < cities.size(); i++) {
		cities[i]->weight = infinity;
		cities[i]->parent = NULL;
	}
}

int Dijkstra::size() {
	return cities.size();
}