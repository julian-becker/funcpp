#include <iostream>
#include <typeclass/functor.h>
#include <typeclass/functor_list.h>

using namespace funcpp::typeclass;

void testFunctor() {
	static_assert(functor::is_instance_v<std::list>);
}

int main() {
	testFunctor();

	std::cout << "***************************************\n";
	std::cout << "**  ALL TESTS OK                     **\n";
	std::cout << "***************************************\n";

}