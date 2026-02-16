#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex mtx;
condition_variable cv;
bool ready = false;

void worker(int id) {
    unique_lock<mutex> l(mtx);
    cv.wait(l, [] { return ready; });
    cout << "Worker " << id << " is running" << endl;
}

void controller() {
    {
        lock_guard<mutex> l(mtx);
        ready = true;
    }
    cv.notify_all();
    
    cout << "Controller: all workers notified" << endl;
}

int main() {
    thread t1(worker, 1);
    thread t2(worker, 2);
    thread t3(worker, 3);
    
    this_thread::sleep_for(chrono::milliseconds(100));
    
    thread t4(controller);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}