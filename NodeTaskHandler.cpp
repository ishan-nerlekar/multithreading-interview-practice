#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include <deque>

using namespace std;

class NodeTaskHandler {
    private:
        unordered_map<int, deque<int>> nodes;
        vector<int> nodeList;
        mutex mtx;

    public:
        void addTask(int nodeId, int taskId) {
            lock_guard<mutex> l(mtx);
            if(nodes.count(nodeId) == 0) {
                nodes[nodeId].push_back(nodeId);
            }
            nodes[nodeId].push_back(taskId);
        }

        void printTaskQueue(int nodeId) {
            lock_guard<mutex> l(mtx);
            if(nodes.count(nodeId) == 0) {
                cout<<"No tasks for node: "<<nodeId<<endl;
                return;
            }
            for(auto task: nodes[nodeId]) {
                cout<<task<<" ";
            }
            cout<<endl;
        }

        void runTask(){
            vector<thread> threads;
            for (int node: nodeList){
                threads.emplace_back([this, node]() {
                    while(1) {
                        int task;
                        lock_guard<mutex> l(mtx);
                        if(nodes[node].empty()) {
                            break;
                        }
                        task=nodes[node].front();
                        nodes[node].pop_front();

                        cout<<"Running task: "<<task<<" on node: "<<node<<endl;
                    }
                });
            }

            for(auto &t: threads) {
                t.join();
            }
        }

        void reassemble() {
            lock_guard<mutex> l(mtx);
            vector<int> taskList;
            for(auto node: nodes) {
                while(!node.second.empty()) {
                    taskList.push_back(node.second.front());
                    node.second.pop_front();
                }
            }

            int idx=0;
            for(auto task: taskList) {
                int node = nodeList[idx%nodeList.size()];
                nodes[node].push_back(task);
                idx++;
            }
        }
        
};