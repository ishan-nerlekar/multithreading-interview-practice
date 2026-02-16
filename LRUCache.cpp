#include<mutex>
#include<unordered_map>
#include<list>
using namespace std;

class LRUCache {
    private:
        int capacity;
        unordered_map<int,pair<int, list<pair<int, int>>::iterator>> cache;
        mutex mtx;
        list<pair<int, int>> lru;

    public:
        LRUCache(int capacity){
            this->capacity = capacity;
        }

        void put(int key, int value){
            unique_lock<mutex> l(mtx);
            if(cache.count(key)){
                lru.erase(cache[key].second);
                lru.push_front({key, value});
                cache[key] = {value, lru.begin()};
            }
            else {
                if(cache.size() == capacity) {
                    cache.erase(lru.back().first);
                    lru.pop_back();
                }
                lru.push_front({key, value});
                cache[key] = {value, lru.begin()};
            }
            
        }

        int get(int key) {
            unique_lock<mutex> l(mtx);
            if(cache.count(key) == 0) return -1;
            lru.erase(cache[key].second);
            lru.push_front({key, cache[key].first});
            cache[key] = {cache[key].first, lru.begin()};
            return cache[key].first;
        }
};