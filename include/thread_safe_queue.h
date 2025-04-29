#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue
{
private:
  std::queue<T> queue;
  mutable std::mutex mutex;
  std::condition_variable cv;

public:
  ThreadSafeQueue() = default;
  ~ThreadSafeQueue() = default;

  // Add an item to the queue
  void push(const T &item)
  {
    {
      std::lock_guard<std::mutex> lock(mutex);
      queue.push(item);
    }
    cv.notify_one(); // Wake up one waiting thread
  }

  // Remove and return the front item (wait if empty)
  T wait_and_pop()
  {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [this]
            { return !queue.empty(); }); // Wait until queue is not empty
    T item = queue.front();
    queue.pop();
    return item;
  }

  bool empty() const
  {
    std::lock_guard<std::mutex> lock(mutex);
    return queue.empty();
  }

  // Try to remove and return the front item (return false if empty)
  bool try_pop(T &item)
  {
    std::lock_guard<std::mutex> lock(mutex);
    if (queue.empty())
    {
      return false;
    }
    item = queue.front();
    queue.pop();
    return true;
  }
};
