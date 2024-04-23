module;
#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

export module zadanie3;

static int signum(int a) {
    if (a > 0) {
        return 1;
    }
    if (a < 0) {
        return -1;
    }
    return 0;
}

static int longestZigzag(const std::vector<std::atomic<int>>& sequence) {
    int n = sequence.size();

    if (n <= 1) return n;

    bool currentIsPositive = (sequence[1] > sequence[0]);

    int maxLength = 1;
    int currentLength = 2;

    for (int i = 2; i < n; ++i) {
        bool nextIsPositive = (sequence[i].load() > sequence[i - 1].load());

        if (nextIsPositive != currentIsPositive) {
            currentLength++;
            currentIsPositive = !currentIsPositive;
        }
        else {
            maxLength = std::max(maxLength, currentLength);
            currentLength = 2;
            currentIsPositive = nextIsPositive;
        }
    }

    maxLength = std::max(maxLength, currentLength);

    return maxLength;
}

static void calculateZigzag(const std::vector<int>& nums, std::vector<std::atomic<int>>& sequence,
    std::mutex& mtx, std::condition_variable& cv, bool& ready, bool& processed) {
    for (int i = 0; i < nums.size(); ++i) {
        sequence[i].store(nums[i]);
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }

    cv.notify_one();

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&] { return processed; });
}

export void zad3(std::vector<int> nums) {
    // sekwencja = 10
    std::vector<std::atomic<int>> sequence(nums.size());
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;
    bool processed = false;

    std::thread thrd(calculateZigzag, ref(nums), ref(sequence), ref(mtx), ref(cv), std::ref(ready), std::ref(processed));
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return ready; });
    }

    int result = longestZigzag(sequence);

    {
        std::lock_guard<std::mutex> lock(mtx);
        processed = true;
    }
    cv.notify_one();

    thrd.join();

    std::cout << "Najdluzsza sekwencja: " << result << '\n';
}