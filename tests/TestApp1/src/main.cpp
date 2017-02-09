#include <iostream>
#include <typeclass/functor.h>
#include <typeclass/functor_list.h>
#include <typeclass/functor_vector.h>
#include <typeclass/functor_optional.h>
#include <typeclass/eq_primitive.h>
#include <typeclass/eq_list.h>
#include <typeclass/eq.h>
#include <typeclass/monoid.h>
#include <typeclass/monoid_primitive.h>
#include <typeclass/monoid_list.h>
#include <typeclass/monad.h>
#include <typeclass/monad_list.h>
#include <typeclass/monad_free.h>
#include <boost/variant.hpp>



#define ASSERT(...) if(!(__VA_ARGS__)) throw  "Assertion that " #__VA_ARGS__ " failed"
#define ASSERT_NOT(...) if(__VA_ARGS__) throw "Assertion that not " #__VA_ARGS__ " failed"

using namespace funcpp::typeclass;

void testEqPrimitive() {
	using namespace funcpp::typeclass::eq;
	ASSERT(equal(1,1));
	ASSERT_NOT(equal(2,1));
	ASSERT(equal(1.0,1.0));
	ASSERT_NOT(equal(1.1,1.0));
}

void testEqList() {
	using namespace funcpp::typeclass::eq;
	ASSERT(equal(std::list<int>{1,2,3},std::list<int>{1,2,3}));
	ASSERT_NOT(equal(std::list<int>{1,2,3},std::list<int>{1,0,3}));
	ASSERT_NOT(equal(std::list<int>{0,2,3},std::list<int>{1,0,3}));
}

void testFunctorList() {
	std::list<int> in {1,2,3};
	std::list<double> out = functor::fmap([](int x) { return 2.0*x; }, in);
}

void testMonoidPrimitive() {
	using namespace monoid;
	ASSERT(42+100 == mappend(42,100));
	ASSERT(42+100 == mappend<int,std::plus<int>>(42,100));
	ASSERT(42*100 == mappend<int,std::multiplies<int>>(42,100));
}

void testMonoidList() {
	using namespace monoid;
	std::list<int> a{1,2,3,4}, b{1,2}, c{3,4};
	ASSERT(a == b + c);
}

void testMonadList() {
	using namespace monad;
	std::list<int> a{42}, b{1,2}, c{3,4};
	ASSERT(a == mreturn<std::list>(42));
	ASSERT(std::list<int>{84} == (instance<std::list>::mreturn(42) >>= [](int x){ return instance<std::list>::mreturn(2*x); }));
	ASSERT(a >> std::list<int>{72} == std::list<int>{72});
}


using unit = std::tuple<>;

template <typename Next>
struct Prog {
	struct Read {
		std::function<Next(int)> next;
	};

	template <typename F>
	static Prog<std::result_of_t<F(int)>> read(F next) {
	  return { Read{next} };
	}

	struct Write {
		int x;
		std::function<Next(unit)> next;
	};

	template <typename F>
	static Prog<std::result_of_t<F(unit)>> write(int x, F next) {
		return { Write{x, next} };
	}
	boost::variant<Read, Write> v;
};


template <typename Fun, typename Next>
Prog<std::result_of_t<Fun(Next)>> 
fmap(Fun&& fun, const Prog<Next>& prog) {
	using Res = std::result_of_t<Fun(Next)>;
	struct Visitor {
		Fun& fun;
		// fmap f (Read n) = Read (f . n)
		Prog<Res> operator()(const typename Prog<Next>::Read& read) const {
			return Prog<Res>::read(compose(fun, read.next));
		}
		// fmap f (Write x n) = Write x (f . n)
		Prog<Res> operator()(const typename Prog<Next>::Write& write) const {
			return Prog<Res>::write(write.x, compose(fun, write.next));
		}
	};
	return boost::apply_visitor(Visitor{fun}, prog.v);
}











void testMonadFree() {
	using namespace monad;
	using namespace funcpp::free;
}

int main() {
	try {
		testEqPrimitive();
		testEqList();
		testMonoidPrimitive();
		testMonoidList();
		testMonadList();
		testMonadFree();


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