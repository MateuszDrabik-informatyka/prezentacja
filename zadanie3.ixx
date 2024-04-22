module;
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
export module zadanie3;

int signum(int a)
{
	if (a > 0)
	{
		return 1;
	}
	if (a < 0)
	{
		return -1;
	}
	return 0;
}

int longestZigzag(const std::vector<std::atomic<int>>& sequence) {
    int n = sequence.size();
    if (n <= 1) return n; // Jeœli sekwencja jest pusta lub ma tylko jeden element, zwracamy jej d³ugoœæ

    // Dwa stany: up (true) i down (false)
    bool currentIsPositive = (sequence[1] > sequence[0]); // Pocz¹tkowy stan

    int maxLength = 1; // Minimalna d³ugoœæ sekwencji "zigzag" to 1
    int currentLength = 2; // Minimalna d³ugoœæ sekwencji "zigzag" z dwóch pierwszych elementów

    for (int i = 2; i < n; ++i) {
        bool nextIsPositive = (sequence[i].load() > sequence[i - 1].load());

        if (nextIsPositive != currentIsPositive) {
            currentLength++;
            currentIsPositive = !currentIsPositive;
        }
        else {
            maxLength = std::max(maxLength, currentLength);
            currentLength = 2; // Zaczynamy now¹ sekwencjê
            currentIsPositive = nextIsPositive;
        }
    }

    // Sprawdzamy ostatni¹ sekwencjê
    maxLength = std::max(maxLength, currentLength);

    return maxLength;
}

export void zad3()
{
	std::vector<int> nu{ 1,5,2,4,0,10,3,20,0,1,2,0,4,2,6,10,2,0,2,4 };//10
    std::vector<std::atomic<int>> num(nu.size());
    for (int i =0;i<nu.size();++i)
    {
        num[i].store(nu[i]);
    }
    std::cout << longestZigzag(num)<<std::endl;
    std::cout << num.size() << std::endl;
}