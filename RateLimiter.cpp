#include<mutex>
#include<queue>
#include<unordered_map>
#include<chrono>
#include<vector>
using namespace std;

long long timeNow() {
    return chrono::steady_clock::now().time_since_epoch().count();
}

class RateLimiter{
    private:
        long long w;
        int maxRequests;
        unordered_map<string, queue<long long>> ipMap;
        static int const NUM_STRIPES = 16;
        vector<mutex> mutexes;  // array of 16 mutexes
        mutex& getMutex(const string& ip){
            size_t h = hash<string>{}(ip);
            return mutexes[h%NUM_STRIPES];
        }

    public:

        RateLimiter(int maxRequests, int windowSeconds) : mutexes(NUM_STRIPES), w(windowSeconds), maxRequests(maxRequests) {}

        bool allowRequest(const string& ip){
            unique_lock<mutex> l(getMutex(ip));
            if(maxRequests == 0) return false;
            long long cutoff = timeNow() - w;
            while(!ipMap[ip].empty() && ipMap[ip].front() < cutoff) {
                ipMap[ip].pop();
            }
            if(ipMap[ip].size() >= maxRequests) return false;
            ipMap[ip].push(timeNow());
            return true;
        }
};