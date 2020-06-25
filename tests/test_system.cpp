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
    temp.RegisterCommand("test", "Simple description", fn, csys::Arg<bool>("Test_Argument"));
    temp.RunCommand("test true");
    CHECK(test_flag);

    // Registration.
    temp.RegisterVariable<float>("time", time_variable);
    temp.RegisterVariable<int>("temp_var", temp_var);

    // Modification.
    temp.RunCommand("set time 10");
    CHECK(time_variable == 10);
    temp.RunCommand("set time 15");
    CHECK(time_variable == 15);
    temp.RunCommand("set temp_var 30");
    CHECK(temp_var == 30);

    // Test system variables.
    temp.UnregisterVariable("time");
    temp.RunCommand("set time 10");
    CHECK(time_variable == 15);

    // Test system commands.
    temp.UnregisterCommand("test");
    temp.RunCommand("test false");
    CHECK(test_flag);
}