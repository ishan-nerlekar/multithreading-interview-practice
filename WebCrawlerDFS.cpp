#include<iostream>
#include<thread>
#include<mutex>
#include<unordered_map>
#include<vector>

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

class MultithreadedWebCrawlerDFS {
    mutex mtx;
    unordered_map<string, int> visited;
    HtmlParser *hp;
    public:
        MultithreadedWebCrawlerDFS(HtmlParser *hp) {
            this->hp =  hp;
        }

        void executeDFS(string url) {
            unique_lock<mutex> l(mtx);
            if(visited.count(url)) return;

            visited[url]=1;
            cout<<"Visited: "<<url<<endl;
            l.unlock();

            vector<string> urls = hp->getUrls(url);
            for(auto url: urls) {
                executeDFS(url);
            }
        }
};

void runDFS(MultithreadedWebCrawlerDFS *crawler) {
    crawler->executeDFS("https://www.google.com");
}

int main() {
    HtmlParser *hp = new HtmlParser();
    MultithreadedWebCrawlerDFS *crawler = new MultithreadedWebCrawlerDFS(hp);
    vector<thread> threads;
    for(int i=0; i<10; i++) {
        threads.push_back(thread(runDFS, crawler));
    }
    for(auto &t: threads) {
        t.join();
    }
    return 0;
}