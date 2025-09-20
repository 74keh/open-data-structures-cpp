#ifndef TESTS_TESTHARNESS_HPP
#define TESTS_TESTHARNESS_HPP

#include <chrono>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace test {

constexpr const char* kGreen = "\033[32m";
constexpr const char* kRed = "\033[31m";
constexpr const char* kReset = "\033[0m";

class TestFailure : public std::runtime_error {
   public:
    using std::runtime_error::runtime_error;
};

inline void check(bool condition, const char* expression, const char* file,
                  int line) {
    if (!condition) {
        std::ostringstream oss;
        oss << expression << " (" << file << ":" << line << ")";
        throw TestFailure(oss.str());
    }
}

template <typename Func>
void expect_out_of_range(Func&& func, const char* expression, const char* file,
                         int line) {
    bool caught = false;
    try {
        func();
    } catch (const std::out_of_range&) {
        caught = true;
    } catch (const TestFailure&) {
        throw;
    } catch (const std::exception& ex) {
        std::ostringstream oss;
        oss << "unexpected exception \"" << ex.what() << "\" for " << expression
            << " (" << file << ":" << line << ")";
        throw TestFailure(oss.str());
    } catch (...) {
        std::ostringstream oss;
        oss << "non-standard exception for " << expression << " (" << file
            << ":" << line << ")";
        throw TestFailure(oss.str());
    }

    if (!caught) {
        std::ostringstream oss;
        oss << "expected std::out_of_range for " << expression << " (" << file
            << ":" << line << ")";
        throw TestFailure(oss.str());
    }
}

inline std::string format_duration(std::chrono::steady_clock::duration delta) {
    using namespace std::chrono;
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3)
        << duration_cast<duration<double>>(delta).count() << "s";
    return oss.str();
}

struct TestCase {
    const char* name;
    void (*fn)();
};

inline int run_tests(std::initializer_list<TestCase> tests) {
    const auto suite_start = std::chrono::steady_clock::now();
    int passed = 0;

    for (const auto& test_case : tests) {
        const auto start = std::chrono::steady_clock::now();
        try {
            test_case.fn();
            ++passed;
            const auto elapsed = std::chrono::steady_clock::now() - start;
            std::cout << kGreen << "[PASS]" << kReset << " " << test_case.name
                      << " (" << format_duration(elapsed) << ")\n";
        } catch (const TestFailure& ex) {
            const auto elapsed = std::chrono::steady_clock::now() - start;
            std::cout << kRed << "[FAIL]" << kReset << " " << test_case.name
                      << " - " << ex.what() << " (" << format_duration(elapsed)
                      << ")\n";
        } catch (const std::exception& ex) {
            const auto elapsed = std::chrono::steady_clock::now() - start;
            std::cout << kRed << "[FAIL]" << kReset << " " << test_case.name
                      << " - unexpected exception: " << ex.what() << " ("
                      << format_duration(elapsed) << ")\n";
        } catch (...) {
            const auto elapsed = std::chrono::steady_clock::now() - start;
            std::cout << kRed << "[FAIL]" << kReset << " " << test_case.name
                      << " - unknown exception (" << format_duration(elapsed)
                      << ")\n";
        }
    }

    const auto total_elapsed = std::chrono::steady_clock::now() - suite_start;
    std::cout << ((passed == static_cast<int>(tests.size())) ? kGreen : kRed)
              << passed << "/" << tests.size() << " tests passed" << kReset
              << " (" << format_duration(total_elapsed) << ")\n";

    return passed == static_cast<int>(tests.size()) ? 0 : 1;
}

}  // namespace test

#define CHECK(condition) \
    ::test::check((condition), #condition, __FILE__, __LINE__)
#define EXPECT_OUT_OF_RANGE(expr) \
    ::test::expect_out_of_range([&]() { expr; }, #expr, __FILE__, __LINE__)

#endif  // TESTS_TESTHARNESS_HPP
