#include <string>
#include <iostream>

void printKey(std::string& s) {
	std::cout << s << std::endl;
}
int main() {
	std::string key = "123";
	printKey(key);
	return 0;
}