
#include <cassert>
#include <future>

namespace threading_ {

int _compute(int value) {
    if (value > 0) {
        return 10;
    }
    return -10;
}

int execute(int value) {
    std::future<int> fut = std::async(
        std::launch::async,
        _compute, std::ref(value));
    return fut.get();
}

} // namespace threading_

int main(int argc, char **argv) {
    int result = threading_::execute(-1);
    assert(result == -10);
}
