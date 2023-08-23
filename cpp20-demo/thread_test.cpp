#include "thread_test.h"


std::mutex cv_m;
std::condition_variable cv;

static void consume()
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

static void produce()
{
	while (1) {
		//cv.notify_one();
		//cv.notify_all();

		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
}

void test_condition()
{
	std::thread t1(produce);

	std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // 相当于Sleep

	std::thread t3(consume);
	//std::thread t2(consume); // 为什么启动两个线程  即使没调用notify  wait_for也返回了 no_timeout ？

	t1.join();
	t3.join();

	getchar();
}

//----------------------------------------------------------------------------------------------------------------
#include <thread>

void hello()
{
	std::cout << "Hello 111 std::thread!" << std::endl;
}

void hello2(int a, std::string str) // 此处的string  不能使用引用传参 用了引用也编译不过
{
	printf("in thread: %p \n", str.c_str()); // 传进来的地址 和外部参数不一样 做了拷贝
	printf("a: %d  str: %s \n", a, str.c_str());
}

class CTestThread
{
public:
	CTestThread() { 
		// 成员函数作为多线程函数
		t = new std::thread(&CTestThread::hello, this, 789);
	}

	~CTestThread()
	{
		t->join();
		delete t;
	}

private:
	void hello(int input)
	{ printf("in class mem: %d %d  GetCurrentThreadId() :%lu \n", a, input, std::this_thread::get_id());
	}

	int a = 123;
	std::thread *t = nullptr;
};

void test_thread()
{
	/*
	同时跑几个线程合适？推荐使用std::thread::hardware_concurrency(), 
	在多核架构的运行环境上，这个返回值一般对应核的颗数。
	*/
	auto count = std::thread::hardware_concurrency();

	//-------------------------------------------------------------
	{
		std::thread t(hello);
		t.join(); //等待线程直到执行完成
	}

	//-------------------------------------------------------------
	{
		CTestThread t;
	}

	printf("------------------- \n");

	//-------------------------------------------------------------
	{
		std::string strP = "wangTest";
		printf("origin: %p \n", strP.c_str());

		std::thread t(hello2, 789, strP);
		t.join(); //等待线程直到执行完成
	}

	//-------------------------------------------------------------
	{
		std::thread my_thread([]() {
			Sleep(3000);
			std::cout << "Hello 2 std::thread!" << std::endl;
			Sleep(3000);
			std::cout << "Hello 3 std::thread!" << std::endl;
		});
		assert(my_thread.joinable() == true);
		my_thread.detach(); // 在后台运行线程
		assert(my_thread.joinable() == false);
	}

	
	//-------------------------------------------------------------
	printf("------------------- \n");
	getchar();
}