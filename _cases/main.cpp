
#include <iostream>
#include <fstream>
#include <string>
#include <future>

#include <ctime>

void process(const char* i_path) {
    using namespace std;

    // condition
    promise<bool> isPublished;
    future<bool> isPublishedFut = isPublished.get_future();

    // workers
    auto copier = [&](const char* i_path) {
        if (! isPublishedFut.get()) {
            return;  // 1) block, 2) should never get here
        }
        ifstream ifs;
        ofstream ofs;
        ifs.open(i_path);
        ofs.open((string(i_path) + ".cpy").c_str());
        ofs << ifs.rdbuf();
        ofs.close();
        ifs.close();
    };
    auto publisher = [&](const char* i_path) {
        fstream iofs;
        iofs.open(i_path, ios::in | ios::out);
        iofs << iofs.rdbuf()
             << ".published at: "
             << time(nullptr) << endl
             << endl;
        iofs.close();
        isPublished.set_value(true);
    };

    // launch
    // changing the order will cause deadlock
    async(launch::async, publisher, i_path).get();
    async(launch::async, copier, i_path).get();
}

// argv[1]: file path
int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }
    process(argv[1]);
    return 0;
}