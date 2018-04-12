#pragma once
#include <iostream>
#include <thread>
#include <random>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>

#include "AdditionalFunctions.h"
using namespace std;

const int THREADS_NUMBER = 4;
const int MAX_VALUE = 10;


vector<pair<int, size_t>> simpleSeqMapReduce(const std::vector<int>& arr)
{
	map<int, size_t> m;
	for (auto iter = arr.begin(); iter != arr.end(); ++iter)
	{
		m[*iter] += 1;
	}
	return vector<pair<int, size_t> >(m.begin(), m.end());
}

//Parallel mapreduce functions
void partial_map (const vector<int> & elems,size_t start, size_t end, vector<pair<int, size_t>> & pairs)
{
	for (int index = start; index < end; ++index)
	{
		pairs[index] = { elems[index], 1};
	}
}

size_t element_hash(int value)
{
	return value % THREADS_NUMBER;
}

vector<pair<int, size_t>> reduce(const vector<pair<int, size_t>> & pairs)
{
	map<int, size_t> m;
	for (auto iter = pairs.begin(); iter != pairs.end(); ++iter)
	{
		m[iter->first] += iter->second;
	}	
	return vector<pair<int, size_t> > (m.begin(), m.end());
}

void parReduce(vector<pair<int, size_t>> & pairs)
{
	pairs = reduce(pairs);
}

vector<pair<int,size_t>> parMapReduce(const std::vector<int>& arr)
{
	const int size = arr.size();
	vector<pair<int, size_t>> pairs(size);
	std::thread * t = new thread[THREADS_NUMBER];
	//parallel mapping
	cout << "\nParallel mapping, number of mappers: " << THREADS_NUMBER << "\n\n";
	for (int i = 0; i < THREADS_NUMBER; i++)
	{
		t[i] = std::thread(partial_map,std::ref(arr),(size*i)/THREADS_NUMBER , (size*(i+1))/ THREADS_NUMBER, std::ref(pairs));
	}
	for (int i = 0; i < THREADS_NUMBER; ++i) {
		t[i].join();
	}

	//shuffling
	vector<pair<int, size_t>> reduce_data [THREADS_NUMBER];
	for (auto iter = pairs.begin(); iter != pairs.end(); ++iter)
	{
		reduce_data[element_hash(iter->first)].push_back(*iter);
	}
	cout << "Number of elements on each reducer:"  << '\n';
	//parallel reducing
	for (int i = 0; i < THREADS_NUMBER; i++)
	{
		cout << "Reducer num:" << i + 1 << ", count of elements: " << reduce_data[i].size() << ";\n";
	}
	cout << "\nParallel reducing, number of reducers: " << THREADS_NUMBER << "\n\n";
	for (int i = 0; i < THREADS_NUMBER; i++)
	{
		t[i] = std::thread(parReduce, std::ref(reduce_data[i]));
	}
	for (int i = 0; i < THREADS_NUMBER; ++i) {
		t[i].join();
	}
	cout << "Result of reduce on each reducer:" << '\n';
	//parallel reducing
	for (int i = 0; i < THREADS_NUMBER; i++)
	{
		cout << "\nReducer num:" << i + 1 << "\n";
		print_result(reduce_data[i]);
	}

	//merging the vectors from all reducers
	vector<pair<int, size_t>> result_vector;
	for (int i = 0; i < THREADS_NUMBER; ++i)
	{
		result_vector.insert(result_vector.end(), reduce_data[i].begin(), reduce_data[i].end());
	}
	//sorting the data for clearer output
	sort(result_vector.begin(), result_vector.end(),
		[](const pair<int,size_t> & left, const pair<int, size_t> & right) -> bool
	{
		return left.first < right.first;
	});
	return result_vector;	
}

#pragma once
