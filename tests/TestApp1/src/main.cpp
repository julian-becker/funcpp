#include <iostream>
#include <typeclass/functor.h>
#include <typeclass/functor_list.h>
#include <typeclass/functor_vector.h>
#include <typeclass/functor_optional.h>
#include <typeclass/eq.h>
#include <typeclass/eq_primitive.h>
#include <typeclass/eq_list.h>


#define ASSERT(cond) if(!(cond)) throw  "Assertion that " #cond " failed"
#define ASSERT_NOT(cond) if(cond) throw "Assertion that not " #cond " failed"

using namespace funcpp::typeclass;

void testEqPrimitive() {
	using namespace funcpp::typeclass::eq;
	static_assert(is_instance_v<char>);
	static_assert(is_instance_v<signed char>);
	static_assert(is_instance_v<unsigned char>);
	static_assert(is_instance_v<short>);
	static_assert(is_instance_v<signed short>);
	static_assert(is_instance_v<unsigned short>);
	static_assert(is_instance_v<int>);
	static_assert(is_instance_v<signed int>);
	static_assert(is_instance_v<unsigned int>);
	ASSERT(instance<int>::equal(1,1));
	ASSERT_NOT(instance<int>::equal(2,1));
	ASSERT(instance<double>::equal(1.0,1.0));
	ASSERT_NOT(instance<double>::equal(1.1,1.0));
}

void testEqList() {
	using namespace funcpp::typeclass::eq;
	static_assert(is_instance_v<std::list<int>>);
	ASSERT(instance<std::list<int>>::equal({1,2,3},{1,2,3}));
	ASSERT_NOT(instance<std::list<int>>::equal({1,2,3},{1,0,3}));
	ASSERT_NOT(instance<std::list<int>>::equal({0,2,3},{1,0,3}));
}

void testFunctorInstance() {
	static_assert(functor::is_instance_v<std::list>);
	static_assert(functor::is_instance_v<std::vector>);
	static_assert(functor::is_instance_v<std::experimental::optional>);
}

void testFunctorList() {
	std::list<int> in {1,2,3};
	std::list<double> out = functor::fmap([](int x) { return 2.0*x; }, in);


}

int main() {
	try {
		testEqPrimitive();
		testEqList();
		testFunctorInstance();

		std::cout << "***************************************\n";
		std::cout << "**  ALL TESTS OK                     **\n";
		std::cout << "***************************************\n";
	}
	catch(const char* ex) {
		std::cout << "***************************************\n";
		std::cout << "**  TESTS FAILED:                    **\n";
		std::cout << "***************************************\n";
		std::cout << ex << std::endl;
	}

}