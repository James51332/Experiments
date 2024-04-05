#include <type_traits>
#include <iostream>
#include <string>

template <typename T> requires std::is_integral<T>::value 
void log(const T& val)
{
	std::cout << val << std::endl;
}

int main()
{
	log(5);
	
	log(std::string("Error!"));
}
