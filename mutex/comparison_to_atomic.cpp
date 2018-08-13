
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

ostream& operator<< (ostream& os, const vector<int>& store) {
    for (const auto& elem : store) {
        os << elem << " ";
    }
    os << endl;
    return os;
}

void use_mutex() {
    mutex m;
    vector<int> store;

    auto worker = [&](int v) {
        m.lock();
        store.emplace_back(v);
        m.unlock();
    };

    vector<thread> threads;
    for (int i = 1; i < 5; ++i) {
        threads.push_back(std::move(thread(worker, i)));
    }
    for (auto& th : threads) {
        th.join();
    }

    cout << store << endl;
}

// TODO: finish this
// see: https://stackoverflow.com/questions/7007834/what-is-the-use-case-for-the-atomic-exchange-read-write-operation
// read: https://www.arangodb.com/2015/02/comparing-atomic-mutex-rwlocks/
// atomic and custom type: https://baptiste-wicht.com/posts/2012/07/c11-concurrency-tutorial-part-4-atomic-type.html
void use_atomic_spinlock() {
    ;
}


int main() {
    use_mutex();
    return 0;
}
