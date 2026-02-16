#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>

using namespace std;
class MetadataStore {
private:
    unordered_map<string, vector<string>> store;
    mutex mtx;

public:
    // Assign data to logical block
    void assign(const string& logical_id, const string& data) {
        lock_guard<mutex> lock(mtx);
        store[logical_id].push_back(data);
    }

    // Retrieve latest data for logical block
    string retrieve_latest(const string& logical_id) {
        lock_guard<mutex> lock(mtx);

        auto it = store.find(logical_id);
        if (it == store.end() || it->second.empty()) {
            return "";
        }
        return it->second.back();
    }

    // Print all versions in insertion order
    void print_all(const string& logical_id) {
        lock_guard<mutex> lock(mtx);

        auto it = store.find(logical_id);
        if (it == store.end()) {
            cout << "No data for logical_id\n";
            return;
        }

        for (const auto& entry : it->second) {
            cout << entry << " ";
        }
        cout << endl;
    }
};
