#include <cstdio>
#include <ctime>
#include <chrono>
#include <random>
#include <algorithm>
#include "Scapegoat.h"
//#include "Scapegoat_no_sz.h"
#include "ScapegoatP.h"
#include "WBTree.h"
#include "WBTreeP.h"

#define K	(N/2)
#define N	10000000

using namespace std;

int arr[N + 10] = { 0 };

int benchRebuildS(int n, bool shuffle) {
	Scapegoat<int> s_tree;
	ScapegoatP<int> sp_tree;
	chrono::system_clock::time_point wcts;
	chrono::duration<double> wt1, wt2;
	int i;
	for (i = 0; i < n; i++)
		arr[i] = i;
	if (shuffle)
		random_shuffle(&arr[0], &arr[n - 1] + 1);

	for (i = 0; i < n; i++)
		if (!s_tree.insert(arr[i]))
			return -1;
	wcts = chrono::system_clock::now();
	s_tree.rebuild();
	wt1 = (chrono::system_clock::now() - wcts);
	s_tree.clear();

	for (i = 0; i < n; i++)
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

int benchRebuildW(int n, bool shuffle) {
	WBTree<int> wb_tree;
	WBTreeP<int> wbp_tree;
	chrono::system_clock::time_point wcts;
	chrono::duration<double> wt1, wt2;
	int i;
	for (i = 0; i < n; i++)
		arr[i] = i;
	if (shuffle)
		random_shuffle(&arr[0], &arr[n - 1] + 1);

	for (i = 0; i < n; i++)
		if (!wb_tree.insert(arr[i]))
			return -1;
	wcts = chrono::system_clock::now();
	wb_tree.rebuild();
	wt1 = (chrono::system_clock::now() - wcts);
	wb_tree.clear();

	for (i = 0; i < n; i++)
		if (!wbp_tree.insert(arr[i]))
			return -1;
	wcts = chrono::system_clock::now();
	wbp_tree.rebuild();
	wt2 = (chrono::system_clock::now() - wcts);
	wbp_tree.clear();

	cout << "WBTree  " << wt1.count() << " seconds (Wall Clock)" << endl;
	cout << "WBTreeP " << wt2.count() << " seconds (Wall Clock)" << endl;
	return 0;
}

int benchAllS(int n, bool shuffle) {
	Scapegoat<int> s_tree;
	ScapegoatP<int> sp_tree;
	chrono::system_clock::time_point wcts;
	chrono::duration<double> wt1, wt2, wt3, wt4;
	int i;
	for (i = 0; i < n; i++)
		arr[i] = i;
	if (shuffle)
		random_shuffle(&arr[0], &arr[n - 1] + 1);

	wcts = chrono::system_clock::now();
	for (i = 0; i < n; i++)
		if (!s_tree.insert(arr[i]))
			return -1;
	wt1 = (chrono::system_clock::now() - wcts);
	
	wcts = chrono::system_clock::now();
	for (i = 0; i < n; i++)
		if (!s_tree.remove(arr[i]))
			return -1;
	wt2 = (chrono::system_clock::now() - wcts);

	wcts = chrono::system_clock::now();
	for (i = 0; i < n; i++)
		if (!sp_tree.insert(arr[i]))
			return -1;
	wt3 = (chrono::system_clock::now() - wcts);

	wcts = chrono::system_clock::now();
	for (i = 0; i < n; i++)
		if (!sp_tree.remove(arr[i]))
			return -1;
	wt4 = (chrono::system_clock::now() - wcts);

	cout << "Scapegoat  (insert) " << wt1.count() << " seconds (Wall Clock)" << endl;
	cout << "Scapegoat  (remove) " << wt2.count() << " seconds (Wall Clock)" << endl;
	cout << "ScapegoatP (insert) " << wt3.count() << " seconds (Wall Clock)" << endl;
	cout << "ScapegoatP (remove) " << wt4.count() << " seconds (Wall Clock)" << endl;
	return 0;
}

int benchAllW(int n, bool shuffle) {
	WBTree<int> wb_tree;
	WBTreeP<int> wbp_tree;
	chrono::system_clock::time_point wcts;
	chrono::duration<double> wt1, wt2, wt3, wt4;
	int i;
	for (i = 0; i < n; i++)
		arr[i] = i;
	if (shuffle)
		random_shuffle(&arr[0], &arr[n - 1] + 1);

	wcts = chrono::system_clock::now();
	for (i = 0; i < n; i++)
		if (!wb_tree.insert(arr[i]))
			return -1;
	wt1 = (chrono::system_clock::now() - wcts);

	wcts = chrono::system_clock::now();
	for (i = 0; i < n; i++)
		if (!wb_tree.remove(arr[i]))
			return -1;
	wt2 = (chrono::system_clock::now() - wcts);

	wcts = chrono::system_clock::now();
	for (i = 0; i < n; i++)
		if (!wbp_tree.insert(arr[i]))
			return -1;
	wt3 = (chrono::system_clock::now() - wcts);

	wcts = chrono::system_clock::now();
	for (i = 0; i < n; i++)
		if (!wbp_tree.remove(arr[i]))
			return -1;
	wt4 = (chrono::system_clock::now() - wcts);

	cout << "WBTree  (insert) " << wt1.count() << " seconds (Wall Clock)" << endl;
	cout << "WBTree  (remove) " << wt2.count() << " seconds (Wall Clock)" << endl;
	cout << "WBTreeP (insert) " << wt3.count() << " seconds (Wall Clock)" << endl;
	cout << "WBTreeP (remove) " << wt4.count() << " seconds (Wall Clock)" << endl;
	return 0;
}

int main() {
	benchRebuildW(N, true);
	//benchAllW(N, true);
	return 0;
}
