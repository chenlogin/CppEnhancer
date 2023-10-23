#include<stdio.h>
#include<iostream>
#include<windows.h>
#include<string>
#include<tchar.h>
#include "circle.h"

/**
* 标准库引用
cout << "Area=" << c.Area() << endl;
如果不加的话引用要写全
std::cout << "Area=" << c.Area() << std::endl;
*/
using namespace std;
#include <napi.h>

//所选磁盘空间
long getLeftSpace(string path) {
	string str_disk_name = path;
	DWORD64 qwFreeBytesToCaller = 0;
	DWORD64 qwTotalBytes = 0;
	DWORD64 qwFreeBytes = 0;
	///使用GetDiskFreeSpaceEx获取磁盘信息并打印结果
	BOOL bResult = GetDiskFreeSpaceExA(str_disk_name.c_str(),
		(PULARGE_INTEGER)&qwFreeBytesToCaller,
		(PULARGE_INTEGER)&qwTotalBytes,
		(PULARGE_INTEGER)&qwFreeBytes);
	/// 读取成功
	if (bResult)
	{
		printf("使用GetDiskFreeSpaceEx获取磁盘空间信息\n");

		cout << "Free space on the selected disk:" + to_string(qwFreeBytesToCaller / (1024 * 1024 * 1024)) + "G" << endl;
		cout << "Free space on the selected disk:" + to_string(qwFreeBytes / (1024 * 1024 * 1024)) + "G" << endl;
		cout << "Total space of the selected disk:" + to_string(qwTotalBytes / (1024 * 1024 * 1024)) + "G" << endl;
	}
	else {
		cout << "read fail" << endl;
	}
}

//电脑磁盘总空间
std::string getSystemStorageUsed()
{
	long long all_Free = 0;
	long long all_Total = 0;
	long long used = 0;
	DWORD dwSize = MAX_PATH;
	TCHAR szLogicalDrives[MAX_PATH] = { 0 };

	DWORD dwResult = GetLogicalDriveStrings(dwSize, szLogicalDrives);

	if (dwResult > 0 && dwResult <= MAX_PATH)
	{
		TCHAR* szSingleDrive = szLogicalDrives;

		while (*szSingleDrive)
		{
			uint64_t available, total, free;
			if (GetDiskFreeSpaceEx(szSingleDrive, (ULARGE_INTEGER*)&available, (ULARGE_INTEGER*)&total, (ULARGE_INTEGER*)&free))
			{
				uint64_t Total, Available, Free;
				Total = total >> 20;
				Available = available >> 20;
				Free = free >> 20;

				all_Total += Total;   //总
				all_Free += Free;   //剩余
			}
			// 获取下一个驱动器号起始地址
			szSingleDrive += _tcslen(szSingleDrive) + 1;
		}
	}
	used = (all_Total - all_Free) * 1024 * 1024;
	cout << "Total space:" + to_string(all_Total/1024) + "G" << endl;
	cout << "Free space:" + to_string(all_Free/1024) + "G" << endl;
	return std::to_string(used);
}

//电池电量
std::string getBatteryPower() {
	LPSYSTEM_POWER_STATUS power = new _SYSTEM_POWER_STATUS();
	int ret = GetSystemPowerStatus(power);
	int percent = (int)power->BatteryLifePercent;
	delete power;
	if (ret == 0){ //表示获取失败
    return "0%";
  } 
	cout << "电池电量:" + to_string(percent) + "%" << endl;
	return std::to_string(percent);
}

/**
* Method 函数 ：它被定义为接收一个 Napi::CallbackInfo 对象
* 返回一个 Napi::String。返回一个字符串 "world"
*/
Napi::String Method(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  printf("stdio: printf end \n");
  return Napi::String::New(env, "Napi::string, hello world");
}

/**
* Init 函数是一个初始化函数，接收一个环境对象 env 和一个导出对象 exports。

* 在N-API中，env（环境）是一个代表特定执行上下文的对象。当一个原生插件的函数被Node.js调用时，该插件可以通过这个环境对象来访问JavaScript运行环境和各种N-API功能。
* 也就是说，Napi::Env对象是JavaScript运行环境的表示，它提供了创建和操纵JavaScript值的能力，以及调用JavaScript函数和处理异常等操作。这让C++代码可以在Node.js中运行。

* Napi::String::New(env, "hello")：这是在当前环境下，使用 "hello" 创建一个新的 JavaScript 字符串。
* Napi::Function::New(env, Method)：在当前环境下，创建一个新的 JavaScript 函数，该函数当被调用时，会执行 C++ 中的 Method 函数。
* exports.Set()：它是 N-API 提供的一个添加属性至导出对象的方法。在 exports 对象上设置一个属性，这个属性的名称是 "hello"，值是一个JavaScript函数。这个函数在被调用时会执行 Method 函数并返回 "world"。
* 所以，这段代码的作用是将一个名为 "hello" 的函数添加到模块的 exports 对象中，因此当其他文件导入(即require)此模块时，它们可以访问到 "hello" 函数。
*/
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  	Circle c(3);
  	cout << "Area=" << c.Area() << endl;
  	printf("stdio: printf start \n");
    getLeftSpace("C:\\");
    getSystemStorageUsed();
    getBatteryPower();
  exports.Set(Napi::String::New(env, "hello"),
              Napi::Function::New(env, Method));
  return exports;
}

/** 
* NODE_API_MODULE：这是一个宏，它告诉 Node.js 这个文件是一个插件，并且 Init 函数应该在加载插件时被调用
* 在 Node.js 中使用 require 引入这个插件将得到对象titlebar，对象有一个 "hello" 方法，调用时返回 "world"
*/
NODE_API_MODULE(titlebar, Init)