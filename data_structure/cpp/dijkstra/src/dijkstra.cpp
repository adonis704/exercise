#include"dijkstra.h"

Dijkstra::Dijkstra(int num) {
	Node* temp;
	for (int i = 0; i < num; i++) {
		temp = new Node(i);
		cities.push_back(temp);
	}
	generate();
}

//��ÿ�����У�������ɿɴ�����Լ�Ȩ��
void Dijkstra::generate() {
	vector<Node*> node;
	vector<int> cost;

	for (int i = 0; i < cities.size(); i++) {
		node = randNode(i);
		cost = randCost(node.size());
		cities[i]->available = node;
		cities[i]->cost = cost;
	}

	Node* cur;	//��ǰ���
	Node* target;	//��ǰ����ĳ���ɴ���
	int pos;
	//���ÿ�����У����������ɴ���еĿɴ��б��ڣ���������Ӧ�����ʹȨ�����
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
//�������λ�ã�������������������ɴ���е�ָ�������
vector<Node*> Dijkstra::randNode(int pos) {
	vector<Node*> res;
	int temp;
	for (int i = 0; i < CITY_NUM; i++) {
		temp = rand() % cities.size();
		if(temp != pos && find(cities[temp],res) == -1) res.push_back(cities[temp]);
	}
	return res;
}
//���������Ŀ������������ɵ�Ȩ������
vector<int> Dijkstra::randCost(int num) {
	vector<int> res;
	for (int i = 0; i < num; i++)
		res.push_back(rand() % MAX_COST+1);
	return res;
}

//��ӡ�ɴ��ϵ
void Dijkstra::print() {
	cout << "- - - - - - - - - - - - - - -" << endl;
	for (int i = 0; i < cities.size(); i++) {
		cout << "���� " << i << " ���Ե���: ";
		for (int j = 0; j < cities[i]->available.size(); j++)
			cout << "���� " << cities[i]->available[j]->name << ":" << cities[i]->cost[j] << "  ";
		cout << endl;
	}	
	cout << "- - - - - - - - - - - - - - -" << endl;
}

//������·��
void Dijkstra::solve(int start, int end) {
	//�����������
	Heap<Node*> heap;	//�������
	vector<Node*> done;	//�Ѿ������ɵĽ��
	cities[start]->weight = 0;
	heap.insert(cities[start]);

	Node* cur;
	vector<Node*> available;
	vector<int> cost;
	int weight;
	int size;
	cout << "- - - - - - - - - - - - - - -" << endl;

	//dijkstra�㷨
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
					cout << "�ı����" << available[i]->name << "�Ŀ������� "
						<< available[i]->weight << " �� " << weight << endl;
					available[i]->weight = weight;
					available[i]->parent = cur;
					heap = update(heap);
				}
			}
				
		}
		done.push_back(cur);
		cout << "���� "<< cur->name << " ������" << endl;
	}
	cout << "- - - - - - - - - - - - - - -" << endl;

	Node* target = cities[end];
	if (target->weight == infinity || target->parent == NULL) cout << "�޽�" << endl;
	else print(cities[start], target);
	//�����ɻ��޽�󣬽����г��еĿ�����������ʼ��
	init();
}
//������ָ�룬���ָ�����飬���ؽ��ָ���������е�λ�ã������򷵻�-1
int Dijkstra::find(Node* target, vector<Node*> done) {
	for (int i = 0; i < done.size(); i++)
		if (target == done[i]) return i;
	return -1;
}
//���¶ѣ����ֶ�����
Heap<Node*> Dijkstra::update(Heap<Node*> heap) {
	Heap<Node*> res;
	while (heap.size() > 0)
		res.insert(heap.pop());
	return res;
}
//���·��
void Dijkstra::print(Node* start, Node* target) {
	Node* cur;
	stack<Node*> stack;
	cur = target;
	while (cur->parent != NULL) {
		stack.push(cur);
		cur = cur->parent;
	}

	cout << "·��:"<<start->name << "-";
	while (stack.size() > 0) {
		cur = stack.top();
		stack.pop();
		if (stack.size() > 0) cout << cur->name << "-";
		else cout << cur->name;
	}
	cout << " ����:" << target->weight << endl;
}
//��ʼ�����г���
void Dijkstra::init() {
	for (int i = 0; i < cities.size(); i++) {
		cities[i]->weight = infinity;
		cities[i]->parent = NULL;
	}
}

int Dijkstra::size() {
	return cities.size();
}