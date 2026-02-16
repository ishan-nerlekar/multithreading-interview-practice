# Multithreading and Concurrency Practice

A collection of C++ implementations demonstrating various multithreading and concurrency patterns, synchronization primitives, and thread-safe data structures.

## 📋 Contents

### Thread-Safe Data Structures
- **LRUCache.cpp** - Thread-safe LRU (Least Recently Used) Cache implementation using mutex locks, hash map, and doubly linked list
- **RateLimiter.cpp** - Rate limiting implementation for controlling request throughput

### Synchronization Patterns
- **ProducerConsumer.cpp** - Classic producer-consumer problem using condition variables
- **UnisexBathroom.cpp** - Synchronization problem demonstrating resource access control
- **WorkerController.cpp** - Worker thread management and control

### Concurrent Algorithms
- **WebCrawlerBFS.cpp** - Breadth-First Search web crawler with concurrent processing
- **WebCrawlerDFS.cpp** - Depth-First Search web crawler with concurrent processing
- **NodeTaskHandler.cpp** - Task handling system with thread coordination

### System Programming
- **MemoryMapping.cpp** - Memory-mapped file operations
- **IPRequests.cpp** - IP-based request handling with concurrency

## 🔧 Technologies Used
- **Language**: C++
- **Concurrency Primitives**: 
  - `std::mutex`
  - `std::condition_variable`
  - `std::thread`
  - `std::unique_lock`
- **Data Structures**: 
  - `std::unordered_map`
  - `std::list`
  - `std::queue`
  - `std::vector`

## 🎯 Learning Objectives
- Understanding thread synchronization mechanisms
- Implementing thread-safe data structures
- Avoiding race conditions and deadlocks
- Managing shared resources in concurrent environments
- Implementing common concurrency patterns

## 🚀 Compilation
Most files can be compiled using:
```bash
g++ -std=c++11 -pthread <filename>.cpp -o <output>
```

Example:
```bash
g++ -std=c++11 -pthread LRUCache.cpp -o lru_cache
```

## 📚 Key Concepts Demonstrated
- **Mutual Exclusion**: Using mutexes to protect critical sections
- **Condition Variables**: Thread coordination and signaling
- **Lock Guards**: RAII-style lock management
- **Thread Pools**: Efficient worker thread management
- **Cache Coherence**: Thread-safe caching mechanisms
- **Resource Scheduling**: Fair resource allocation among threads

## 📝 Notes
This repository is for educational purposes and practice. The implementations focus on demonstrating concurrency concepts and may not be production-ready.

## 🤝 Contributing
Feel free to suggest improvements or add new concurrency patterns!

## 📄 License
MIT License - Feel free to use for learning and practice.

