#include "test.h"
#include "simulator_test.h"

int main() {
    TestFramework::GetInstance().RegisterTest("SimulatorTests::TestAddition", SimulatorTests::TestAddition);
    TestFramework::GetInstance().RunTests();

    return 0;
}