#include"rcbNode.h"

//初始化
RCBNode::RCBNode(string n, int t) {
	name = n;	//资源名称
	total = t;	//资源总量
	available = t;	//可用资源
	wl = new PCBList();	//阻塞队列
}
RCBNode::~RCBNode() {

}