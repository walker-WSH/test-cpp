#include "test_gdip.h"
#include <thread>
#include <Windows.h>   // must be inluded before gdiplus.h, otherwise compile error 
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

void thread1()
{
	ULONG_PTR m_GdiplusToken;
	Gdiplus::GdiplusStartupInput startupInput;
	GdiplusStartup(&m_GdiplusToken, &startupInput, NULL);
	printf("%u inited GDIP \n", GetCurrentThreadId());

	Sleep(5000);

	Gdiplus::FontFamily fm(L"Arial");
	if (fm.GetLastStatus() != Gdiplus::Ok) {
		printf("%u error \n", GetCurrentThreadId());
	}

	Gdiplus::Image *m_pImage = Gdiplus::Image::FromFile(L"test.jpg");
	auto err = m_pImage->GetLastStatus();

	Gdiplus::GdiplusShutdown(m_GdiplusToken);
	printf("%u uninited GDIP \n", GetCurrentThreadId());
}

void thread2() 
{
	ULONG_PTR m_GdiplusToken;
	Gdiplus::GdiplusStartupInput startupInput;
	GdiplusStartup(&m_GdiplusToken, &startupInput, NULL);
	printf("%u inited GDIP \n", GetCurrentThreadId());

	
	Gdiplus::GdiplusShutdown(m_GdiplusToken);
	printf("%u uninited GDIP \n", GetCurrentThreadId());
}

void test_gdip()
{
	std::thread(thread1).detach();

	Sleep(3000);

	std::thread(thread2).detach();

	getchar();
}