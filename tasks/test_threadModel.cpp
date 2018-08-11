
#include <cassert>
#include "threadModel.hpp"

int main(int argc, char **argv) {
    int result = threading_::execute(-1);
    assert(result == -10);
}
