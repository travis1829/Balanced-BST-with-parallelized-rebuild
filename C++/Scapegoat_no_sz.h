// Scapgegoat_no_sz.h
// No "size" field in NODEs.
#ifndef SCAPEGOAT_H
#define SCAPEGOAT_H

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

template <typename T>
class Scapegoat {
public:
	Scapegoat() {
		root = NULL;
		alpha = 0.9846154; // 0.0111111 (2)
		size = 0;
		max_size = 0;
	}

	Scapegoat(double Alpha) {
		if ((Alpha <= 0.5) || (1 <= Alpha))
			throw invalid_argument("Alpha must be 0.5 < Alpha < 1");
		root = NULL;
		alpha = Alpha;
		size = 0;
		max_size = 0;
	}

	~Scapegoat() {
		_clear(root);
	}

	bool search(T v) {
		return _search(root, v) != NULL;
	}

	bool insert(T v) {
		int curr_size = 0;
		return _insert(root, v, 0, curr_size);
	}

	bool remove(T v) {
		bool result = _delete(root, v);
		if (size <= max_size / 2) {
			_rebuild(root);
			max_size = size;
		}
		return result;
	}

	void clear() {
		_clear(root);
	}

private:
	struct NODE {
		NODE* left, * right;
		T key;

		NODE(T v) {
			left = right = NULL;
			key = v;
		}
	};
	NODE* root;
	int size, max_size;
	double alpha;

	NODE* _search(NODE* t, T v) {
		if (t == NULL)
			return NULL;
		else if (v < t->key)
			return _search(t->left, v);
		else if (v > t->key)
			return _search(t->right, v);
		else
			return t;
	}

	/* Auxillary function used in _insert */
	int get_size(NODE* t) {
		if (t == NULL)
			return 0;
		return get_size(t->left) + get_size(t->right) + 1;
	}

	bool _insert(NODE*& t, T v, int depth, int& curr_size) {
		bool result, is_left;
		if (t == NULL) {
			t = new NODE(v);
			size++;
			if (max_size < size)
				max_size = size;
			if (depth > int(log(size) / log(1 / alpha)) + 1)
				curr_size = 1;
			return true;
		}
		else if (v < t->key) {
			result = _insert(t->left, v, depth + 1, curr_size);
			is_left = true;
		}
		else if (v > t->key) {
			result = _insert(t->right, v, depth + 1, curr_size);
			is_left = false;
		}
		else
			return false;

		if (curr_size > 0) {
			int tot, left, right;
			if (is_left) {
				left = curr_size;
				right = get_size(t->right);
			}
			else {
				left = get_size(t->left);
				right = curr_size;
			}
			tot = left + right + 1;
			if (left > alpha * tot || right > alpha * tot) {
				_rebuild(t);
				curr_size = 0;
			}
			else
				curr_size = tot;
		}
		return result;
	}

	/* Auxillary function used in _delete */
	NODE* _getRightMost(NODE* t) {
		while (t->right != NULL)
			t = t->right;
		return t;
	}

	bool _delete(NODE*& t, T v) {
		if (t == NULL)
			return false;
		else if (v < t->key)
			return _delete(t->left, v);
		else if (v > t->key)
			return _delete(t->right, v);
		else { //Node found
			if (t->left && t->right) {	//Both child nodes exist.
				t->key = _getRightMost(t->left)->key;	//Find the inorder predecessor of t and copy its key.
				_delete(t->left, t->key);				//Delete the inorder predecessor. Note that it always has 0 or 1 child nodes.
			}
			else { //0 or 1 child nodes exist
				NODE* successor = NULL;					//If t is a leaf, its successor is NULL. Otherwise, its successor is its sole child node.
				if (t->left)
					successor = t->left;
				else
					successor = t->right;
				delete t;
				t = successor;
				size--;
			}
			return true;
		}
	}

	/* Auxillary function used in _rebuild */
	void _getCopy(NODE* t, vector<NODE*> &nodeArr) {
		if (t->left != NULL)
			_getCopy(t->left, nodeArr);
		nodeArr.push_back(t);
		if (t->right != NULL)
			_getCopy(t->right, nodeArr);
	}

	/* Auxillary function used in _rebuild */
	NODE* _buildTree(vector<NODE*> &nodeArr, int s, int f) {
		if (s > f)
			return NULL;
		int m = (s + f + 1) / 2;
		NODE* t = nodeArr[m];
		t->left = _buildTree(nodeArr, s, m - 1);
		t->right = _buildTree(nodeArr, m + 1, f);
		return t;
	}

	void _rebuild(NODE*& t) {
		if (t == NULL)
			return;
		vector<NODE*> nodeArr;
		_getCopy(t, nodeArr);							// Make nodeArr store all nodes in increasing key order
		t = _buildTree(nodeArr, 0, nodeArr.size() - 1);	// Rebuild the tree using the array
	}

	void _clear(NODE*& t) {
		if (t == NULL)
			return;
		_clear(t->left);
		_clear(t->right);
		delete t;
		t = NULL;
		size--;
	}
};
#endif
