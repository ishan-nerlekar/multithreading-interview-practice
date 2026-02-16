#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include<atomic>
using namespace std;

mutex mtx;
condition_variable cv;
queue<int> buffer;
const int MAX_SIZE = 5;
atomic<bool> done(false);

void producer() {
    for(int i=0; i<10; i++){
        unique_lock<mutex> l(mtx);
        cv.wait(l, [] { return buffer.size() < MAX_SIZE; });
        buffer.push(i);
        cout << "Produced: " << i << endl;
        cv.notify_all();
    }
    done.store(true);
    unique_lock<mutex> l(mtx);
    cv.notify_all();
}

void consumer() {
    while(1){
        unique_lock<mutex> l(mtx);
        cv.wait(l, [] { return !buffer.empty() || done.load(); });
        if(buffer.empty() && done.load()) break;
        int val = buffer.front();
        buffer.pop();
        cout << "Consumed: " << val << endl;
        cv.notify_all();       
    }
}

int main() {
    thread t1(producer);
    thread t2(consumer);

    t1.join();
    t2.join();
    return 0;
}