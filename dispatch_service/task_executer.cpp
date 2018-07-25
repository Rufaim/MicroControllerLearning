#include "task_executer.h"


TaskExecuter::~TaskExecuter() {
    if(!interrupt_requested_)
        Stop();
}

void TaskExecuter::Start() {
    if (task_query_thread_ == nullptr) {
        alive_ = true;
        interrupt_requested_ = false;
        task_query_thread_ = new std::thread(&TaskExecuter::TaskQueryThreadProc, this);
    }
}

void TaskExecuter::Stop() {
    interrupt_requested_ = true;

    if(task_query_thread_ != nullptr) {
        alive_ = false;
        task_query_thread_->join();
        delete task_query_thread_;
    }
}
