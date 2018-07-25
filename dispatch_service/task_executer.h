#ifndef TASK_EXECUTER_H
#define TASK_EXECUTER_H

#include <chrono>
#include <thread>
#include <memory>
#include <mutex>
#include <atomic>

class TaskExecuter {
public:
    TaskExecuter() { task_query_thread_ = nullptr; }
    virtual ~TaskExecuter();

    virtual void Start();
    virtual void Stop();

    bool isActive() const { return alive_; }

    bool exit_on_fail() const { return exit_on_fail_; }
    void set_exit_on_fail(bool exit_on_fail) { exit_on_fail_ = exit_on_fail; }

protected:
    std::thread* task_query_thread_;
    std::mutex task_mutex_;
    std::atomic<bool> interrupt_requested_;

    bool alive_ = false;

    bool exit_on_fail_;

    virtual void TaskQueryThreadProc() = 0;

};

#endif // TASK_EXECUTER_H
