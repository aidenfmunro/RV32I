#include <gtest/gtest.h>

#include <array>
#include <cstdio>
#include <string>

#ifndef E2E_BIN_DIR
#error "E2E_BIN_DIR not defined – check tests/CMakeLists.txt"
#endif

#ifndef INTERP_PATH
#error "INTERP_PATH not defined – check tests/CMakeLists.txt"
#endif

// Run: echo <input> | INTERP_PATH <rv32_bin>, capture stdout.
static int run_program(const std::string& rv32_bin,
                       const std::string& input,
                       std::string& output)
{
    std::string cmd = "echo '" + input + "' | " INTERP_PATH " " + rv32_bin;

    std::array<char, 4096> buf{};
    output.clear();

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return -1;

    while (fgets(buf.data(), buf.size(), pipe) != nullptr)
    {
        output.append(buf.data());
    }

    int status = pclose(pipe);
#ifdef WEXITSTATUS
    return WEXITSTATUS(status);
#else
    return status;
#endif
}

struct E2ETestCase
{
    const char* name;            // program name, e.g. "sqrt"
    std::string input;           // stdin
    std::string expected_output; // stdout
    int expected_exit = 0;
};

class InterpreterE2E : public ::testing::TestWithParam<E2ETestCase> {};

TEST_P(InterpreterE2E, RunsCorrectly)
{
    const auto& tc = GetParam();

    std::string bin_path = std::string(E2E_BIN_DIR) + "/" +
                           tc.name + std::string(".rv32");

    std::string output;
    int exit_code = run_program(bin_path, tc.input, output);

    ASSERT_EQ(exit_code, tc.expected_exit) << "exit code mismatch for " << tc.name;
    EXPECT_EQ(output, tc.expected_output)  << "stdout mismatch for " << tc.name;
}

INSTANTIATE_TEST_SUITE_P(
    Programs,
    InterpreterE2E,
    ::testing::Values(
        E2ETestCase{"echo",       "123",      "123\n", 1},
        E2ETestCase{"isqrt",      "9\n",        "3\n"},
        E2ETestCase{"bubblesort", "3 3 1 2\n",  "1 2 3 \n"}
        // add more here
    )
);
