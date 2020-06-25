template<typename T> class node {
public:
	T value; node<T>* pred; node<T>* succ;

	node(){}
	node(T e, node<T>* p = NULL, node<T>* s =NULL):value(e),pred(p),succ(s){}
	void set_value(T e) { value = e; }
	T get_value() { return value; }
	node<T>* get_prev() { return pred; }
	node<T>* get_next() { return succ; }
	void append(node<T>* x);

};

template<typename T> void node<T>::append(node<T>* x) {
	x->pred = this;
	this->succ = x;
}
