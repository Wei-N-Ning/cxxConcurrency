//
// Created by weining on 24/6/20.
//

// see pro tbb P/160
// how to implement atomic multiply3() function using CAS idiom

// given v is std::atomic<uint32_t> v;
// calling v.compare_and_swap(new_v, old_v) atomically does:
// ov = v;
// if (ov == old_v) {
//   v = new_v;
// }
// return ov;

// now the trick to implement multiply3() is to code what is dubbed CAS loop
void fetch_and_triple(std::atomic<uint32_t> &v) {
    uint32_t old_v;
    do {
        // take a snapshot of the shared variable, which is key to later compare
        // if other thread has managed to modify it
        // P161
        // after taking the snapshot, it is possible that other thread updates v,
        // in this case v != old_v which implies that:
        // - we do not update v
        // - we stay in the do-while loop hoping that next time we are lucky
        old_v = v;
        while (v.compare_and_swap(old_v * 3, old_v) ! = old_v);
    }
}
