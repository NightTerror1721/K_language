#include "rawmem.h"
#include "types.h"
#include "ref.h"

#include "stacks.h"

#include <iostream>

using namespace klang::type;
using klang::Ref;

 
int main(int argc, char** argv)
{
	Ref val;
	
	Ref a = 15, b = -7;

	Ref res = a + b;
	Ref res2 = !a;

	Ref str = static_cast<std::wstring>(res);

	std::wcout << str[0] << std::endl;
	std::wcout << (true == !res2) << std::endl;
	std::wcout << ((a + b) == 8) << std::endl;
	std::wcout << ++(res * 8) << std::endl;


	klang::stack::RegisterStack stack{ 32 };
	stack.push(50);
	stack.push(a);


	std::cout << "Klang heap used: " << klang::heap::used() << " bytes." << std::endl;

	return 0;
}
