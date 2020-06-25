#include "doctest.h"
#include "csys/system.h"

TEST_CASE ("Test CSYS System Class")
{
    csys::System temp;

    bool test_flag = false;
    auto fn = [&](bool flag)
    { test_flag = flag; };
    float time_variable = 0;
    int temp_var = 0;

    // Test Command registration.
    temp.registerCommand("test", "Simple description", fn, csys::Arg<bool>("Test_Argument"));
    temp.runCommand("test true");
    CHECK(test_flag);

    // Registration.
    temp.registerVariable<float>("time", time_variable);
    temp.registerVariable<int>("temp_var", temp_var);

    // Modification.
    temp.runCommand("set time 10");
    CHECK(time_variable == 10);
    temp.runCommand("set time 15");
    CHECK(time_variable == 15);
    temp.runCommand("set temp_var 30");
    CHECK(temp_var == 30);

    // Test system variables.
    temp.unregisterVariable("time");
    temp.runCommand("set time 10");
    CHECK(time_variable == 15);

    // Test system commands.
    temp.unregisterCommand("test");
    temp.runCommand("test false");
    CHECK(test_flag);
}