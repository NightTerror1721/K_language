#include <rawmem.h>

class A {};
class B : public A {};


int main(int argc, char** argv)
{
	klang::mem::SharedResource<A> s = new B{};

	auto view = klang::mem::view(s);

	auto view2 = klang::mem::as<B>(view);

	return 0;
}
