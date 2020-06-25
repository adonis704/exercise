#include"heap.h"

template <typename T> Heap<T>::Heap() {
	
}

//²åÈë
template <typename T> void Heap<T>::insert(T e) {
	heap.push_back(e);
	up();
}
//ÉÏÂË
template <typename T> void Heap<T>::up() {
	int pos = heap.size() - 1;
	int parentPos;
	T temp = heap[pos];

	while (true) {
		parentPos = parent(pos);
		if (pos == 0) break;
		if (temp->weight >= heap[parentPos]->weight) break;
		heap[pos] = heap[parentPos];
		pos = parentPos;
	}
	heap[pos] = temp;
}

//É¾³ı
template <typename T> T Heap<T>::pop() {
	T res = heap.front();
	heap[0] = heap.back();
	heap.pop_back();
	if (heap.size() == 0) return res;
	down();
	return res;
}
//ÏÂÉø
template <typename T> void Heap<T>::down() {
	int pos = 0;
	int lcPos, rcPos;
	bool flagL = false, flagR = false;
	int smallerPos;
	T temp = heap[pos];

	while (true) {
		lcPos = lchild(pos);
		rcPos = rchild(pos);
		if (lcPos >= heap.size() || (lcPos < heap.size() && temp->weight <= heap[lcPos]->weight)) flagL = true;
		if (rcPos >= heap.size() || (rcPos < heap.size() && temp->weight <= heap[rcPos]->weight)) flagR = true;
		if (flagL && flagR) break;
		else flagL = flagR = false;
		if (rcPos >= heap.size()) smallerPos = lcPos;
		else smallerPos = heap[lcPos]->weight <= heap[rcPos]->weight ? lcPos : rcPos;
		heap[pos] = heap[smallerPos];
		pos = smallerPos;
	}
	heap[pos] = temp;
}

template <typename T> int Heap<T>::size() {
	return heap.size();
}

template <typename T>T Heap<T>::front() {
	return heap[0];
}