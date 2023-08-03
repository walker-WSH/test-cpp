
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

#include <thread>
#include <iostream>
#include <condition_variable>
#include <mutex>
std::mutex cv_m;
std::condition_variable cv;
void consume()
{
	while (true) {
		std::unique_lock<std::mutex> lk(cv_m);

		auto ret = cv.wait_for(lk, std::chrono::milliseconds(1000));
		if (ret == std::cv_status::timeout) { // 此处判断应该是错误的：wait_for的返回值  并不是表达是否超时
			printf("timeout %u \n", GetCurrentThreadId());
		} else {
			printf("wait done %u \n", GetCurrentThreadId());
		}
	}
}

void produce()
{
	while (1) {
		//cv.notify_one();
		//cv.notify_all();

		
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
}

void test_thread()
{
	std::thread t1(produce);

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::thread t3(consume);
	//std::thread t2(consume); // 为什么启动两个线程  即使没调用notify  wait_for也返回了 no_timeout ？

	t1.join();
	//t2.join();
	t3.join();
}

int main()
{
	test_thread();

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
