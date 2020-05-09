#include <iostream>
#include<vector>
#include <unistd.h>
#include "thread.h"

void fun1() {
	std::cout << "test fun1" << std::endl;
}

int main() {
	std::vector<sylar::thread::ptr> vec;
	for (int i = 0;i < 10;i++) {
		sylar::thread::ptr lthread(new sylar::thread("first", &fun1));
		vec.push_back(lthread);
	}
	sleep(10);
	for (int i = 0;i < vec.size();i++)
		vec[i]->join();
	return 0;
}