#pragma once

#include <thread>
#include"EventLoop.h"

namespace net
{
    class EventLoopThread
    {
    public:
        EventLoopThread()noexcept;

        ~EventLoopThread()noexcept;

        EventLoopThread(const EventLoopThread &) = delete;
        EventLoopThread &operator=(const EventLoopThread &)= delete;
        EventLoopThread( EventLoopThread &&) = delete;
        EventLoopThread &operator=( EventLoopThread &&)= delete;

        void run();

        void stop();

        void join();

        void stop_and_join();

        EventLoop *loop();

    private:

        void thread_fn();

        std::thread _th;
        EventLoop _loop;
    };
}
