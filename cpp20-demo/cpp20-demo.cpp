﻿#include "thread_test.h"

#include <Windows.h>
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

inline size_t StringToHash(std::string_view str)
{
	static std::hash<std::string_view> hash;
	return hash(str);
}

#include <random>
int64_t generateRandomNumber(int64_t min, int64_t max)
{
	std::default_random_engine engine((unsigned int)std::chrono::steady_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int64_t> dist(min, max);

	auto num = dist(engine);
	return num;
}

#include "test_gdip.h"
int main()
{
	test_gdip();

	test_thread();


	test_condition();

	for (int i = 0; i < 50; i++) {
		printf("%d : %lld \n", i, generateRandomNumber(0, 50));
	}

	/*
	int stoi
	long stol
	unsigned long stoul
	long long stoll
	unsigned long long stoull
	float stof
	double stod
	*/
	auto t1 = stoi("123");
	auto t2 = stof("3.1415926"); // 有误差

	auto str1 = std::to_string(123987);
	auto str2 = std::to_string(3.1415926f); // 有误差

	auto id = StringToHash("123");
	auto id2 = StringToHash("123"); // 和系统时间无关 只要输入的字符串一样 生成的hash id也会一样

	test_format();
	test_location(); 

	return 0;
}
