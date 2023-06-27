
#include <format>
#include <source_location>

using namespace std;

void test_format()
{
	string who = "everyone";
	int ival = 789;
	double pi = 3.1415926;

	try {
		auto str = format("Hello, {}!\n", who);
		printf("%s", str.c_str());

		str = format("integer: {}\n", ival);
		printf("%s", str.c_str());

		str = format("double: {}\n", pi);
		printf("%s", str.c_str());

		//----------------------------------------------
		str = format("double: {0} {1}\n", pi, who);
		printf("%s", str.c_str());

		str = format("double: {1} {0}\n", pi, who);
		printf("%s", str.c_str());

		str = format("double: {0} {0}\n", pi, who);
		printf("%s", str.c_str());

		str = format("double: {3} {1}\n", pi, who);
		printf("%s", str.c_str());
	} catch (const std::exception &e) {
		printf("exception: %s \n", e.what());
	}
}

void test_location(std::source_location location = std::source_location::current())
{
	printf("called from: %s, %s \n", location.function_name(), location.file_name());
}

int main()
{
	test_format();
	test_location(); 

	return 0;
}
