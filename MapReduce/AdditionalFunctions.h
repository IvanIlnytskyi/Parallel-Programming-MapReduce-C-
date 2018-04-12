#pragma once
#include <iostream>
#include <random>
#include <vector>
#include <utility>
#include <ctime>
using namespace std;

void print_result(const std::vector<pair<int, size_t>>& arr)
{
	for (auto iter = arr.begin(); iter != arr.end(); ++iter)
	{
		cout << "Value: " << iter->first << ", count: " << iter->second << ';' << endl;
	}
}
std::vector<int> create_test_array(size_t capacity)
{
	srand(time(NULL));
	std::vector<int> vec(capacity);
	for (int i = 0; i < capacity; ++i)
	{
		vec[i] = rand() % 10;
	}
	return vec;
}

