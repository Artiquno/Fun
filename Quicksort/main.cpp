#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include <mutex>
#include <random>
#include <vector>
#include <ctime>

std::mutex mtx;
int maxThreads = 100;
void quicksort(std::vector<int>* arr, int lo, int hi);
int partition(std::vector<int>* arr, int lo, int hi);

void printArray(std::vector<int>* arr, int size);
void printArray(std::vector<int>* arr, int size)
{
	for (int i = 0; i < size; ++i)
	{
		std::cout << std::setw(4) << (*arr)[i];
		if (i != size - 1)
		{
			std::cout << ", ";
		}
		if ((i + 1) % 8 == 0)
		{
			std::cout << "\n";
		}
	}
	std::cout << std::endl;
}

std::vector<int>* makeRandom(int size);

std::vector<int>* makeRandom(int size)
{
	std::vector<int>* arr = new std::vector<int>(size);
	for (int i = 0; i < size; ++i)
	{
		(*arr)[i] = std::rand() % 1000;
	}
	return arr;
}

int usedThreads = 0;
void quicksort(std::vector<int>* arr, int lo, int hi)
{
	if (lo < hi)
	{
		int p = partition(arr, lo, hi);

		if (usedThreads < maxThreads)
		{
			std::thread th1;
			std::thread th2;
			try
			{
				th1 = std::thread(quicksort, arr, lo, p);
				th2 = std::thread(quicksort, arr, p + 1, hi);
			}
			catch(std::exception ex)
			{
				std::cerr << "Maximum threads reached, sorry. Try lowering the maximum allowed number of threads with --threads\n";
				exit(1);
			}

			usedThreads += 2;

			th1.join();
			th2.join();

			usedThreads -= 2;
		}
		else
		{
			quicksort(arr, lo, p);
			quicksort(arr, p + 1, hi);
		}

		/*quicksort(arr, lo, p);
		quicksort(arr, p + 1, hi);*/
	}
}
int partition(std::vector<int>* arr, int lo, int hi)
{
	int pivot = (*arr)[lo];
	int i = lo - 1;
	int j = hi + 1;

	while (true)
	{
		do
		{
			i += 1;
		} while ((*arr)[i] < pivot);

		do
		{
			j -= 1;
		} while ((*arr)[j] > pivot);

		if (i >= j)
		{
			return j;
		}

		int temp = (*arr)[j];
		(*arr)[j] = (*arr)[i];
		(*arr)[i] = temp;
	}
}

int parseInt(const char* str)
{
	int res = 0;
	for (int i = strlen(str) - 1, j = 0; i >= 0; --i, ++j)
	{
		res += (str[j] - '0') * pow(10, i);
	}
	return res;
}

int main(int argc, char** argv)
{
	std::srand(std::time(0));

	int i = parseInt("13482");

	// Stupid, I know, but too lazy for a parser right now
	// Maybe I'll add a parser later and fix this monstrosity
	if (strcmp(argv[1], "--threads") == 0)
	{
		maxThreads = parseInt(argv[2]);
	}

	std::cout << "Using " << maxThreads << " threads\n";

	int size = 10000000;
	std::vector<int>* arr = makeRandom(size);

	quicksort(arr, 0, size - 1);

	mtx.lock();
	std::cout << "\nSorted (hopefully) array:\n";
	printArray(arr, size);
	mtx.unlock();

	delete arr;
	return 0;
}