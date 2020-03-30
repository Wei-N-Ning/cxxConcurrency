//
// Created by wein on 28/10/18.
//

// source
// master cxx mt: L1228 (The Scheduler)

// ideally, each worker thread would wait idly until it is
// needed again. To accomplish this, we have to approach the
// problem from the other side: not from the perspective of the
// worker threads, but from that of the queue.

// much like the scheduler of an operating system, it is the
// scheduler which is aware of both the tasks which require
// processing as well as the available worker threads.

// a central scheduler instance would accept new tasks and
// actively assign them to worker threads.

// the said scheduler instance may also manage these worker
// threads, such as their number and priority, depending on the
// number of incoming tasks and the type of task or other
// properties

// recall message queue

//////// READ EMC++ 265 (283/334) ////////
// consider using void* future for one shot comm

#include <memory>

//--------------------------------------------------------------
// simulating a separate compilation unit: request.hh/cc
namespace request {

class Request {
public:
    explicit Request(int value);

private:
    int m_value = -1;
};

Request::Request(int value)
    : m_value(value) {
    ;
}

}

//--------------------------------------------------------------
// simulating a separate compilation unit: worker.hh/cc
namespace worker {

class Worker {

};

}

//--------------------------------------------------------------
// simulating a separate compilation unit: dispatcher.hh/cc
namespace dispatcher {

using request_t = std::shared_ptr<request::Request>;

class Dispatcher {
public:
    void addRequest(request_t req);
};

void Dispatcher::addRequest(request_t req) {
    ;
}

}

//--------------------------------------------------------------

constexpr int NUM_REQUESTS = 50;

int main() {
    dispatcher::Dispatcher disp;
    for (int cycle = 0; cycle < NUM_REQUESTS; ++cycle) {
        auto req = std::make_shared<request::Request>(cycle);
        disp.addRequest(req);
    }

    return 0;
}
