#include <iostream>
using namespace std;

int main() {
	const char* literal = R"(This is a 
	string literal and should be preservered)";
	
	cout << literal;

	return 0;
}
