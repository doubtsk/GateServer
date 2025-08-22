#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include "Singleton.h"

class AsioIOServicePool : public Singleton<AsioIOServicePool>
{
	friend Singleton<AsioIOServicePool>;

public:
	using IOService = boost::asio::io_context;
	// ������ʹ��auto�Ƶ�make_work_guard���صľ�ȷ����
	using ExecutorWorkGuard = decltype(boost::asio::make_work_guard(std::declval<IOService&>()));
	using WorkPtr = std::unique_ptr<ExecutorWorkGuard>;

	~AsioIOServicePool();
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;

	// ��ѯ��ʽ��ȡio_service
	boost::asio::io_context& GetIOService();
	void Stop();

private:
	AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
	std::vector<IOService> _ioServices;
	std::vector<WorkPtr> _works;
	std::vector<std::thread> _threads;
	std::size_t _nextIOService;
	std::mutex _mutex;
};
