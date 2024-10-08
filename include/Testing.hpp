#ifndef TESTING_HPP
#define TESTING_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <format>
#include <unordered_map>

#define IS_EQUAL(a,b)                                                                                                   \
    if ((a) != (b))                                                                                                     \
    {                                                                                                                   \
        std::cout << "\033[91m" << "Failure at " << __FILE__ << " " << __LINE__ << "\n";                                \
        std::cout << #a << " != " << #b << "\033[39m" << "\n";                                                            \
        this->failures++;                                                                                               \
    }                                                                                                                   \
    else                                                                                                                \
    {                                                                                                                   \
    }

#define IS_NOT_EQUAL(a,b)                                                                                               \
    if ((a) == (b))                                                                                                     \
    {                                                                                                                   \
        std::cout << "\033[91m" << "Failure at " << __FILE__ << " " << __LINE__ << "\n";                                \
        std::cout << #a << " == " << #b << "\033[39m" << "\n";                                                            \
        this->failures++;                                                                                               \
    }                                                                                                                   \
    else                                                                                                                \
    {                                                                                                                   \
    }

#define RUN_TESTS()                                                                                                     \
    int main(int argc, char** argv)                                                                                     \
    {                                                                                                                   \
        if (argc != 2)                                                                                                  \
        {                                                                                                               \
            std::cout << "ARGUMENT COUNT WRONG\n";                                                                      \
            return EXIT_FAILURE;                                                                                        \
        }                                                                                                               \
        return TestRegistry::s_get_instance().run_test(argv[1]);                                                        \
    }

#define TEST(TestName)                                                                                                  \
    class TestName : public BaseTest                                                                                    \
    {                                                                                                                   \
    public:                                                                                                             \
        auto run() -> void override;                                                                                    \
        TestName()                                                                                                      \
        {                                                                                                               \
            TestRegistry::s_get_instance().add_test(#TestName, this);                                                   \
        }                                                                                                               \
    };                                                                                                                  \
                                                                                                                        \
    static TestName s_##TestName##_instance;                                                                            \
                                                                                                                        \
    auto TestName::run() -> void

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS DEFINITIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class BaseTest
{
public:
    std::uint8_t failures;

    BaseTest() : failures{0} {}
    virtual auto run() -> void = 0;
    virtual ~BaseTest() {}
    auto report_errors() const -> int
    {
        return failures > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
    }
};

class TestRegistry
{
public:
    std::unordered_map<std::string, BaseTest*> test_map;

    TestRegistry() = default;

    static auto s_get_instance() -> TestRegistry&
    {
        static TestRegistry instance;
        return instance;
    }

    auto add_test(const std::string& test_name, BaseTest* test) -> void
    {
        if (!test_map.contains(test_name))
        {
            test_map[test_name] = test;
        }
    }

    auto run_test(const std::string& test_name) -> int
    {
        if (test_map.contains(test_name))
        {
            test_map[test_name]->run();
            return test_map[test_name]->report_errors();
        }
        return EXIT_FAILURE;
    }
};

#endif
