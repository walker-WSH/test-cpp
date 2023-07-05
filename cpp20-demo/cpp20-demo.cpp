
using namespace std;

#include <format>
void test_format()
{
	string who = "everyone";
	int ival = 15;
	double pi = 3.1415926;

	try {
		auto str = format("Hello, {}!\n", who);
		printf("%s", str.c_str());

		// 不能再用C语言的格式化符号了 最终输出的字符串就是"%d"
		str = format("??? ??? integer: %d\n", ival);
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

		//----------------------------------------------
		// format for wstring
		auto wstr = format(L"double:  {} {} {}\n", pi, L"teststr", L"hello");

		// format for {}
		str = format("double: {{}} {}\n", who);
		printf("%s", str.c_str());

		// 数字占多少位 不足的用0填充
		str = format("int: {:06d}\n", ival);
		printf("%s", str.c_str());

		// 16进制显示数字
		str = format("int: {:06X}  {:06x} \n", ival, ival);
		printf("%s", str.c_str());

		//----------------------------------------------
		// test exception 不允许混合使用手动和自动索引，下列写法是非法的
		str = format("read {0} bytes from {}", 100, "file.txt");
		printf("%s", str.c_str());
		
		// test exception: params error
		str = format("double: {3} {1}\n", pi, who);
		printf("%s", str.c_str());


	} catch (const std::exception &e) {
		printf("exception: %s \n", e.what());
	}
}

#include <source_location>
void test_location(std::source_location location = std::source_location::current())
{
	printf("called from: %s, %s \n", location.function_name(), location.file_name());
}

inline size_t StringToID(std::string_view str)
{
	static std::hash<std::string_view> hash;
	return hash(str);
}

int main()
{
	auto id = StringToID("123");
	auto id2 = StringToID("123");

	test_format();
	test_location(); 

	return 0;
}
