#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <vector>
using namespace std;

enum Party { NONE, DEMOCRAT, REPUBLICAN };

struct Person {
    int id;
    Party party;
    int duration;
};

class Bathroom {
private:
    int capacity = 3;
    int inside = 0;
    Party currentParty = NONE;
    
    queue<Person> waitingD;
    queue<Person> waitingR;
    
    mutex mtx;
    condition_variable cv;
    
public:
    void arrive(Person p) {
        unique_lock<mutex> lock(mtx);
        
        // Add to appropriate queue
        if (p.party == DEMOCRAT) {
            waitingD.push(p);
        } else {
            waitingR.push(p);
        }
        
        // Wait until we can enter
        cv.wait(lock, [&, p]() {
            // Can't enter if full
            if (inside >= capacity) return false;
            
            // If empty, anyone can start
            if (inside == 0) {
                currentParty = NONE;
            }
            
            // Check if our party can enter
            if (currentParty == NONE || currentParty == p.party) {
                // Are we at the front of our queue?
                auto& myQueue = (p.party == DEMOCRAT) ? waitingD : waitingR;
                if (!myQueue.empty() && myQueue.front().id == p.id) {
                    return true;
                }
            }
            
            return false;
        });
        
        // Admitted!
        auto& myQueue = (p.party == DEMOCRAT) ? waitingD : waitingR;
        myQueue.pop();
        inside++;
        currentParty = p.party;
        
        cout << "Person " << p.id << " (" << (p.party == DEMOCRAT ? "D" : "R") 
             << ") entered. Inside: " << inside << endl;
        
        lock.unlock();
        
        // Use bathroom
        this_thread::sleep_for(chrono::seconds(p.duration));
        
        // Exit
        lock.lock();
        inside--;
        cout << "Person " << p.id << " left. Inside: " << inside << endl;
        
        // If bathroom now empty, reset party
        if (inside == 0) {
            currentParty = NONE;
        }
        
        cv.notify_all();
        lock.unlock();
    }
};

int f(int id) {
    return 1 + (id % 5);
}

int main() {
    Bathroom bathroom;
    vector<thread> threads;
    
    Party parties[] = {DEMOCRAT, REPUBLICAN, DEMOCRAT, REPUBLICAN, DEMOCRAT, 
                       REPUBLICAN, DEMOCRAT, DEMOCRAT, REPUBLICAN, REPUBLICAN};
    
    for (int i = 0; i < 10; i++) {
        Person p{i, parties[i], f(i)};
        threads.emplace_back([&bathroom, p]() {
            this_thread::sleep_for(chrono::milliseconds(100 * p.id));
            bathroom.arrive(p);
        });
    }
    
    for (auto& t : threads) t.join();
    
    return 0;
}