#include "thread_test.h"


std::mutex cv_m;
std::condition_variable cv;

static void consume()
{
	while (true) {
		std::unique_lock<std::mutex> lk(cv_m);

		auto ret = cv.wait_for(lk, std::chrono::milliseconds(1000));
		if (ret == std::cv_status::timeout) { // �˴��ж�Ӧ���Ǵ���ģ�wait_for�ķ���ֵ  �����Ǳ���Ƿ�ʱ
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

	std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // �൱��Sleep

	std::thread t3(consume);
	//std::thread t2(consume); // Ϊʲô���������߳�  ��ʹû����notify  wait_forҲ������ no_timeout ��

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

void hello2(int a, std::string str) // �˴���string  ����ʹ�����ô��� ��������Ҳ���벻��
{
	printf("in thread: %p \n", str.c_str()); // �������ĵ�ַ ���ⲿ������һ�� ���˿���
	printf("a: %d  str: %s \n", a, str.c_str());
}

class CTestThread
{
public:
	CTestThread() { 
		// ��Ա������Ϊ���̺߳���
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
	ͬʱ�ܼ����̺߳��ʣ��Ƽ�ʹ��std::thread::hardware_concurrency(), 
	�ڶ�˼ܹ������л����ϣ��������ֵһ���Ӧ�˵Ŀ�����
	*/
	auto count = std::thread::hardware_concurrency();

	//-------------------------------------------------------------
	{
		std::thread t(hello);
		t.join(); //�ȴ��߳�ֱ��ִ�����
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
		t.join(); //�ȴ��߳�ֱ��ִ�����
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
		my_thread.detach(); // �ں�̨�����߳�
		assert(my_thread.joinable() == false);
	}

	
	//-------------------------------------------------------------
	printf("------------------- \n");
	getchar();
}