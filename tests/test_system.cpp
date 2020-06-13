#include "doctest.h"
#include "ccli_system.h"

TEST_CASE("Test CCLI System Class")
{
	ccli::System temp;

	bool test_flag = false;
	auto fn = [&](bool flag){ test_flag = flag;};

	temp.registerCommand("test", "Simple description", fn, ccli::Arg<bool>("Test_Argument"));
	temp.runCommand("test true");
	CHECK(test_flag);

	float time_variable = 0;
	int temp_var = 0;
	temp.registerVariable<float>("time", time_variable);
	temp.registerVariable<int>("temp_var", temp_var);

	temp.runCommand("set time 10");
	CHECK(time_variable == 10);
	temp.runCommand("set time 15");
	CHECK(time_variable == 15);
	temp.runCommand("set temp_var 30");
	CHECK(temp_var == 30);

	temp.unregisterVariable("time");
	temp.runCommand("set time 10");
	CHECK(time_variable == 15);

	temp.unregisterCommand("test");
	temp.runCommand("test false");
	CHECK(test_flag);
}