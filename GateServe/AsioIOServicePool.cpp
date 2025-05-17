#include "AsioIOServicePool.h"
#include <iostream>

AsioIOServicePool::AsioIOServicePool(std::size_t size) : _IOService(size), _works(size), _nextIOService(0)
{
    for (std::size_t i = 0; i < size; i++)
    {
        // Updated to use the correct executor type for WorkGuard
        _works[i] = std::unique_ptr<WorkGuard>(new WorkGuard(boost::asio::make_work_guard(_IOService[i])));
    }
    for (size_t i = 0; i < _IOService.size(); i++)
    {
        _thread.emplace_back([this, i]()
                             { _IOService[i].run(); });
    }
}

AsioIOServicePool::~AsioIOServicePool()
{
    Stop();
    std::cout << "AsioIOServicePool destruct" << std::endl;
}

boost::asio::io_context &AsioIOServicePool::GetIOService()
{
    auto &service = _IOService[_nextIOService++];
    if (_nextIOService == _IOService.size())
    {
        _nextIOService = 0;
    }
    return service;
}

void AsioIOServicePool::Stop()
{
    for (auto &io : _IOService)
    {
        io.stop();
    }
    for (auto &WorkGuard : _works)
    {
        WorkGuard.reset();
    }
    for (auto &t : _thread)
    {
        t.join();
    }
}
