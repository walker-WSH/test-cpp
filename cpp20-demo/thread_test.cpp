#include "thread_test.h"


std::mutex cv_m;
std::condition_variable cv;


uint64_t os_gettime_ns()
{
	auto tm = std::chrono::steady_clock::now().time_since_epoch().count();
	return tm;
}


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

		std::chrono::steady_clock::time_point pos1 = std::chrono::steady_clock::now();
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		std::chrono::steady_clock::time_point pos2 = std::chrono::steady_clock::now();

		std::chrono::steady_clock::duration dur = pos2 - pos1;
		auto ms = duration_cast<std::chrono::milliseconds>(dur).count();
		printf("take %llu ms to init \n", ms);
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
	{ 
		/*
		yield()函数可以用来将调用者线程跳出运行状态，重新交给操作系统进行调度，
		即当前线程放弃执行，操作系统调度另一线程继续执行
		*/
		std::this_thread::yield();

		printf("in class mem: %d %d  GetCurrentThreadId() :%lu \n", a, input, std::this_thread::get_id());
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

	//-------------------------------------------------------------
	{
		/*
		// 推荐使用C++20的 jthread
		它拥有同 std::thread 的行为外，主要增加了以下两个功能：
		(1) std::jthread 对象被 destruct 时，会自动调用 join，等待其所表示的执行流结束。
		(2) 支持外部请求中止（通过 get_stop_source、get_stop_token 和 request_stop ） ?????
		*/

		std::jthread my_thread([]() {
			std::cout << "start sleep" << std::endl;
			Sleep(5000);
			std::cout << "finish sleep" << std::endl;
		});

		printf("to end \n"); // jthread析构时会自动等待join， 而std::thread则不会等待 此处就会发生错误
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

		/*
		joinable：检查线程是否可被join。检查thread对象是否标识一个活动(active)的可行性线程。
		缺省构造的thread对象、已经完成join的thread对象、已经detach的thread对象都不是joinable。
		*/
		assert(my_thread.joinable() == true);

		/*
		detach：将当前线程对象所代表的执行实例与该线程对象分离，使得线程的执行可以单独进行。
		一旦线程执行完毕，它所分配的资源将会被释放。
		*/
		my_thread.detach(); // 在后台运行线程

		assert(my_thread.joinable() == false);
	}

	
	//-------------------------------------------------------------
	printf("------------------- \n");
	getchar();
}
