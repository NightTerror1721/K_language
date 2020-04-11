#include "rawmem.h"
#include "types.h"
#include "ref.h"

#include <iostream>

using namespace klang::type;
using klang::Ref;

 
int main(int argc, char** argv)
{
	Ref val;
	
	Ref a = 15, b = -7;

	Ref res = a + b;
	Ref res2 = !a;
	
	std::cout << (true == !res2) << std::endl;
	std::cout << ((a + b) == 8) << std::endl;
	std::cout << ++(res * 8) << std::endl;

	return 0;
}
