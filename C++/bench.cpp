#include <cstdio>
#include <ctime>
#include <chrono>
#include <random>
#include <algorithm>
#include "Scapegoat.h"
//#include "Scapegoat_no_sz.h"
#include "ScapegoatP.h"

#define N	10000000

using namespace std;

int arr[N + 10] = { 0 };

int main() {
	Scapegoat<int> s_tree;
	ScapegoatP<int> sp_tree;
	chrono::system_clock::time_point wcts;
	chrono::duration<double> wt1, wt2;
	int i;
	for (i = 0; i < N; i++)
		arr[i] = i;
	random_shuffle(&arr[0], &arr[N - 1] + 1);	

	for (i = 0; i < N; i++)
		if (!s_tree.insert(arr[i]))
			return -1;
	wcts = chrono::system_clock::now();
	s_tree.rebuild();
	wt1 = (chrono::system_clock::now() - wcts);
	s_tree.clear();

	for (i = 0; i < N; i++)
		if (!sp_tree.insert(arr[i]))
			return -1;
	wcts = chrono::system_clock::now();
	sp_tree.rebuild();
	wt2 = (chrono::system_clock::now() - wcts);
	sp_tree.clear();

	cout << "Scapegoat  " << wt1.count() << " seconds (Wall Clock)" << endl;
	cout << "ScapegoatP " << wt2.count() << " seconds (Wall Clock)" << endl;
	return 0;
}
