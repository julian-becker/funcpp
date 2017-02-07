#include <iostream>
#include <typeclass/functor.h>
#include <typeclass/functor_list.h>
#include <typeclass/functor_vector.h>
#include <typeclass/functor_optional.h>
#include <typeclass/eq.h>
#include <typeclass/eq_primitive.h>
#include <typeclass/eq_list.h>
#include <typeclass/monoid.h>
#include <typeclass/monoid_primitive.h>
#include <typeclass/monoid_list.h>



#define ASSERT(...) if(!(__VA_ARGS__)) throw  "Assertion that " #__VA_ARGS__ " failed"
#define ASSERT_NOT(...) if(__VA_ARGS__) throw "Assertion that not " #__VA_ARGS__ " failed"

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
	using namespace functor;
	static_assert(is_instance_v<std::list>);
	static_assert(is_instance_v<std::vector>);
	static_assert(is_instance_v<std::experimental::optional>);
}

void testFunctorList() {
	std::list<int> in {1,2,3};
	std::list<double> out = functor::fmap([](int x) { return 2.0*x; }, in);
}

void testMonoidPrimitive() {
	using namespace monoid;
	static_assert(is_instance_v<int>);
	static_assert(is_instance_v<int,std::plus<int>>);
	static_assert(is_instance_v<int,std::multiplies<int>>);
	static_assert(is_instance_v<unsigned char>);
	static_assert(is_instance_v<unsigned char,std::plus<unsigned char>>);
	static_assert(is_instance_v<unsigned char,std::multiplies<unsigned char>>);
	ASSERT(42+100 == mappend(42,100));
	ASSERT(42+100 == mappend<int,std::plus<int>>(42,100));
	ASSERT(42*100 == mappend<int,std::multiplies<int>>(42,100));
}

void testMonoidList() {
	using namespace monoid;
	static_assert(is_instance_v<std::list<int>,std::plus<void>>);
	std::list<int> a{1,2,3,4}, b{1,2}, c{3,4};
	ASSERT(a == b + c);
}

int main() {
	try {
		testEqPrimitive();
		testEqList();
		testFunctorInstance();
		testMonoidPrimitive();
		testMonoidList();

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