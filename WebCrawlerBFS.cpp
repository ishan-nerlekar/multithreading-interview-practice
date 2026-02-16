/**
 * // This is the HtmlParser's API interface.
 * // You should not implement it, or speculate about its implementation
 * class HtmlParser {
 *  public:
 *   vector<string> getUrls(string url);
 * };
 */

// class Solution {
//  public:
//   vector<string> crawl(string startUrl, HtmlParser htmlParser) {
//     queue<string> q{ {startUrl} };
//     unordered_set<string> seen{ {startUrl} };
//     const string& hostname = getHostname(startUrl);

//     // Threading
//     const int nThreads = std::thread::hardware_concurrency();
//     vector<thread> threads;
//     std::mutex mtx;
//     std::condition_variable cv;

//     auto t = [&]() {
//       while (true) {
//         unique_lock<mutex> lock(mtx);
//         cv.wait_for(lock, 30ms, [&]() { return q.size(); });

//         if (q.empty())
//           return;

//         auto cur = q.front();
//         q.pop();

//         lock.unlock();

//         const vector<string> urls = htmlParser.getUrls(cur);

//         lock.lock();
//         for (const string& url : urls) {
//           if (seen.count(url))
//             continue;
//           if (url.find(hostname) != string::npos) {
//             q.push(url);
//             seen.insert(url);
//           }
//         }
//         lock.unlock();
//         cv.notify_all();
//       }
//     };

//     for (int i = 0; i < nThreads; ++i)
//       threads.emplace_back(t);

//     for (std::thread& t : threads)
//       t.join();

//     return {begin(seen), end(seen)};
//   }

//  private:
//   string getHostname(const string& url) {
//     const int firstSlash = url.find_first_of('/');
//     const int thirdSlash = url.find_first_of('/', firstSlash + 2);
//     return url.substr(firstSlash + 2, thirdSlash - firstSlash - 2);
//   }
// };

#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<unordered_map>
#include<vector>
#include<queue>

using namespace std;

class HtmlParser {
    public:
        vector<string> getUrls(string url) {
            if(url == "https://www.google.com") {
                return {"https://www.facebook.com", "https://www.twitter.com"};
            }
            else {
                return {"https://www.google.com", "https://www.github.com", "https://www.youtube.com"};
            }
        }

};

class MultithreadedWebCrawlerBFS {
    private:
        condition_variable cv;
        mutex mtx;
        unordered_map<string, int> visited;
        queue<string> q;
        HtmlParser *hp;

    public:
        MultithreadedWebCrawlerBFS(HtmlParser *hp) {
            this->hp =  hp;
            q.push("https://www.google.com");
            visited["https://www.google.com"] = 1;
        }
        
        void executeBFS() {
            while(1) {
                unique_lock<mutex> l(mtx);
                cv.wait_for(l, chrono::seconds(1), [&] {return !q.empty();});
                if(q.empty()){
                    break;
                }
                string cur = q.front();
                cout<<"Visited: "<<cur<<endl;
                q.pop();
                l.unlock();

                vector<string> urls = hp->getUrls(cur);
                for(auto url: urls) {
                    if(visited[url] == 0) {
                        visited[url] = 1;
                        q.push(url);
                    }
                }
                cv.notify_all();
            }
        }
};

void runBFS(MultithreadedWebCrawlerBFS *crawler) {
    crawler->executeBFS();
}

int main() {
    HtmlParser *hp = new HtmlParser();
    MultithreadedWebCrawlerBFS *crawler = new MultithreadedWebCrawlerBFS(hp);
    vector<thread> threads;
    for(int i=0; i<10; i++) {
        threads.push_back(thread(runBFS, crawler));
    }
    for(auto &t: threads) {
        t.join();
    }
    return 0;
}