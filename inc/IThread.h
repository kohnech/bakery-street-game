#pragma once


#include <chrono>
#include <thread>
/*!
 * Thread method
 */

class IThread
{
public:
    IThread();

    void start();
    void join();
    static void sleep(int milliseconds);

    virtual bool ThreadMethod() = 0;
    bool mIsRunning;

    bool getRunningState();

private:
    std::thread mThread;
    static int ThreadProxy(void* ptr);
};