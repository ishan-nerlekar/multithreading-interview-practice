#include <mutex>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <vector>
using namespace std;

long long timeNow() {
    return chrono::steady_clock::now().time_since_epoch().count();
}

class IPRequests {
    private:
    unordered_map<string, queue<long long>> ipMap;
    
    static const int NUM_STRIPES = 16;
    vector<mutex> mutexes;  // array of 16 mutexes
    long long w;

    mutex& getMutex(const string& ip) {
        size_t h = hash<string>{}(ip);
        return mutexes[h % NUM_STRIPES];
    }

    public:
    IPRequests(long long windowSeconds) 
        : mutexes(NUM_STRIPES), w(windowSeconds) {}

    void addRequest(const string& ip) {
        unique_lock<mutex> l(getMutex(ip));
        ipMap[ip].push(timeNow());
    }

    int getCount(const string& ip) {
        unique_lock<mutex> l(getMutex(ip));
        if (ipMap.count(ip) == 0) return 0;
        long long cutoff = timeNow() - w;
        while (!ipMap[ip].empty() && ipMap[ip].front() < cutoff) {
            ipMap[ip].pop();
        }
        return ipMap[ip].size();
    }
};