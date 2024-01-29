#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cassert>

class TestFramework {
public:
    using TestFunction = std::function<void()>;

    static TestFramework& GetInstance() {
        static TestFramework instance;
        return instance;
    }

    void RegisterTest(const std::string& name, const TestFunction& testFunction) {
        tests.push_back(std::make_pair(name, testFunction));
    }

    void RunTests() {
        for (const auto& test : tests) {
            std::cout << "Running test: " << test.first << " ... ";
            test.second();
            std::cout << "Passed!" << std::endl;
        }
    }

private:
    TestFramework() {}
    std::vector<std::pair<std::string, TestFunction>> tests;
};

class TestSuite {
public:
    TestSuite(const std::string& suiteName) : suiteName_(suiteName) {}

    void AddTest(const std::string& testName, const TestFramework::TestFunction& testFunction) {
        tests_.push_back(std::make_pair(testName, testFunction));
    }

    void RegisterTests() {
        for (const auto& test : tests_) {
            TestFramework::GetInstance().RegisterTest(suiteName_ + "::" + test.first, test.second);
        }
    }

private:
    std::string suiteName_;
    std::vector<std::pair<std::string, TestFramework::TestFunction>> tests_;
};

// Example usage
/* void TestAddition() {
    int result = 2 + 3;
    assert(result == 5);
}

void TestSubtraction() {
    int result = 7 - 4;
    assert(result == 3);
}

int main() {
    TestSuite mathSuite("MathSuite");
    mathSuite.AddTest("TestAddition", TestAddition);
    mathSuite.AddTest("TestSubtraction", TestSubtraction);

    mathSuite.RegisterTests();

    TestFramework::GetInstance().RunTests();

    return 0;
}
*/
