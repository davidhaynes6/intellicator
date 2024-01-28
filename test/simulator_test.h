//#include "simulator.h"
#include <cassert>

class SimulatorTests {
public:
	static void TestAddition() {
		auto result = 5;
		auto expected = 5;

		assert(result == expected);
	}
};