#include<iostream>
#include<thread>
#include<mutex>
#include<vector>
using namespace std;

class UnisexBathroom {
private:
    int counter;
    mutex mtx;
    condition_variable cv;
public:
    UnisexBathroom() {
        counter = 0;
    }

    void maleUseBathroom(string name) {
        unique_lock<mutex> l(mtx);
        cv.wait(l, [&] {return counter>=0 && counter <=2;});
        cout<<name<<" entered the bathroom"<<endl;
        counter++;
        l.unlock();

        cout<<name<<" exited the bathroom"<<endl;
        l.lock();
        counter--;
        l.unlock();

        cv.notify_all();
    }

    void femaleUseBathroom(string name) {
        unique_lock<mutex> l(mtx);
        cv.wait(l, [&] {return counter>=-2 && counter <=0;});
        cout<<name<<" entered the bathroom"<<endl;
        counter--;
        l.unlock();

        cout<<name<<" exited the bathroom"<<endl;
        l.lock();
        counter++;
        l.unlock();

        cv.notify_all();
    }
};

void maleUseBathroom(UnisexBathroom *ub, string name) {
    ub->maleUseBathroom(name);
}

void femaleUseBathroom(UnisexBathroom *ub, string name) {
    ub->femaleUseBathroom(name);
}

int main() {
    UnisexBathroom *ub = new UnisexBathroom();
    thread t1(maleUseBathroom, ub, "Neel");
    thread t2(femaleUseBathroom, ub, "Shreya");
    thread t4(femaleUseBathroom, ub, "Pratibha");
    thread t3(femaleUseBathroom, ub, "Simran");
    thread t5(femaleUseBathroom, ub, "Rani");
    thread t6(maleUseBathroom, ub, "Kartik");

    t1.join(), t2.join(), t3.join(), t4.join(), t5.join(), t6.join();

    return 0;
}