#include"AsioIOServicePool.h"
#include <iostream>
using namespace std;
AsioIOServicePool::AsioIOServicePool(std::size_t size) :_ioServices(size),
_works(size), _nextIOService(0) {
	for (std::size_t i = 0; i < size; ++i) {
		_works[i] = std::make_unique<ExecutorWorkGuard>(boost::asio::make_work_guard(_ioServices[i]));
	}

	//遍历多个ioservice，创建多个线程，每个线程内部启动ioservice
	for (std::size_t i = 0; i < _ioServices.size(); ++i) {
		_threads.emplace_back([this, i]() {
			_ioServices[i].run();
			});
	}
}

AsioIOServicePool::~AsioIOServicePool() {
	std::cout << "AsioIOServicePool destruct" << endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
	auto& service = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size()) {
		_nextIOService = 0;
	}
	return service;
}

void AsioIOServicePool::Stop() {
   //因为仅仅执行work.reset并不能让iocontext从run的状态中退出
   //当iocontext已经绑定了读或写的监听事件后，还需要手动stop该服务。
   for (std::size_t i = 0; i < _works.size(); ++i) {
       //直接调用_ioServices的stop方法
	   _works[i].reset();
	   _ioServices[i].stop();
   }

   for (auto& t : _threads) {
       t.join();
   }
}