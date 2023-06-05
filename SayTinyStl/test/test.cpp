#ifdef _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#endif


#if defined(_MSC_VER) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h>
#include <crtdbg.h>
#endif // check memory leaks

#include "algorithm_performance_test.h"
#include "algorithm_test.h"
#include "../test/vector_test.h"
#include "deque_test.h"


int main()
{
	using namespace saystl::test;

	std::cout.sync_with_stdio(false);

	RUN_ALL_TESTS();
	algorithm_performance_test::algorithm_performance_test();

	//using namespace vector_test;
	vector_say_test::vector_test();
	deque_test::deque_test();


#if defined(_MSC_VER) && defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // check memory leaks

}
