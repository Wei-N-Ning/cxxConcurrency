//
// Created by Wei on 24/03/2016.
//

#ifndef CXXUSAGE_THREADING_THREADMODEL_HPP
#define CXXUSAGE_THREADING_THREADMODEL_HPP


#include <future>

namespace threading_ {

int _compute(int value) {
  if (value > 0) {
    return 10;
  }
  return -10;
}


int execute(int value) {
  using TaskPromise = std::promise<int>;
  using TaskFuture = std::future<int>;
  TaskPromise taskPromise{};
  taskPromise.set_value(_compute(value));
  return taskPromise.get_future().get();
}

} // namespace threading_

#endif //CXXUSAGE_THREADING_THREADMODEL_HPP
