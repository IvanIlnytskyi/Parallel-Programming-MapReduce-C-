#include "Implementation.h"
#include <fstream>
using namespace std;
void main()
{
	vector<int> arr = create_test_array(1024);
	auto seq = simpleSeqMapReduce(arr);
	auto par = parMapReduce(arr);
	cout << "\t\t MapReduce results (numbers count)" << endl;
	cout << "\t\t\t Sequantial stupid MapReduce:" << endl;
	print_result(seq);
	cout << "\t\t\t Parallel MapReduce:" << endl;
	print_result(seq);
	system("Pause");
}
