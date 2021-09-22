// WBTreeP.h
#ifndef WBTREEP_H
#define WBTREEP_H

#include <iostream>
#include <thread>
#include <future>
using namespace std;

template <typename T>
class WBTreeP {
public:
	WBTreeP() {
		root = NULL;
		alpha = 0.4;
	}

	WBTreeP(double Alpha) {
		if ((Alpha <= 0) || (0.5 <= Alpha))
			throw invalid_argument("Alpha must be 0 < Alpha < 0.5");
		root = NULL;
		alpha = Alpha;
	}

	~WBTreeP() {
		_clear(root);
	}

	bool search(T v) {
		return _search(root, v) != NULL;
	}

	bool insert(T v) {
		NODE** rebuildLoc = NULL;
		bool result = _insert(root, v, rebuildLoc);
		if (rebuildLoc)
			_rebuild(*rebuildLoc);
		return result;
	}

	bool remove(T v) {
		NODE** rebuildLoc = NULL;
		bool result = _delete(root, v, rebuildLoc);
		if (rebuildLoc)
			_rebuild(*rebuildLoc);
		return result;
	}

	void rebuild() {
		_rebuild(root);
	}

	void clear() {
		_clear(root);
	}

private:
	struct NODE {
		NODE* left, * right;
		T key;
		int size;

		NODE(T v) {
			left = right = NULL;
			key = v;
			size = 1;
		}
	};
	NODE* root;
	double alpha;

	bool _isUnbalanced(NODE* t) {
		if (t->left && t->left->size + 1 < alpha * (t->size + 1))
			return true;
		else if (t->right && t->right->size + 1 < alpha * (t->size + 1))
			return true;
		return false;
	}

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

	bool _insert(NODE*& t, T v, NODE** rebuildLoc) {
		bool result;
		if (t == NULL) {
			t = new NODE(v);
			return true;
		}
		else if (v < t->key)
			result = _insert(t->left, v, rebuildLoc);
		else if (v > t->key)
			result = _insert(t->right, v, rebuildLoc);
		else
			return false;

		if (result) {
			t->size++;
			if (_isUnbalanced(t))
				rebuildLoc = &t;
		}
		return result;
	}

	/* Auxillary function used in _delete */
	NODE* _getRightMost(NODE* t) {
		while (t->right != NULL)
			t = t->right;
		return t;
	}

	bool _delete(NODE*& t, T v, NODE** rebuildLoc) {
		bool result;
		if (t == NULL)
			return false;
		else if (v < t->key)
			result = _delete(t->left, v, rebuildLoc);
		else if (v > t->key)
			result = _delete(t->right, v, rebuildLoc);
		else { //Node found
			if (t->left && t->right) {	//Both child nodes exist.
				t->key = _getRightMost(t->left)->key;	//Find the inorder predecessor of t and copy its key.
				_delete(t->left, t->key, rebuildLoc);				//Delete the inorder predecessor. Note that it always has 0 or 1 child nodes.
				result = true;
			}
			else { //0 or 1 child nodes exist
				NODE* successor = NULL;					//If t is a leaf, its successor is NULL. Otherwise, its successor is its sole child node.
				if (t->left)
					successor = t->left;
				else
					successor = t->right;
				delete t;
				t = successor;
				return true;
			}
		}

		if (result) {
			t->size--;
			if (_isUnbalanced(t))
				rebuildLoc = &t;
		}
		return result;
	}

	/* Auxillary function used in _update for rebuilds */
	void _getCopy(NODE* t, NODE** nodeArr, int s, int depth) {
		bool need_wait = false;
		future<void> ft;

		int index = s;
		if (t->left != NULL) {
			index += t->left->size;
			if (t->left->size > CONCUR_SIZE && depth < CONCUR_DEPTH) {
				need_wait = true;
				ft = async(std::launch::async, &WBTreeP<T>::_getCopy, this, t->left, ref(nodeArr), s, depth + 1);
			}
			else
				_getCopy(t->left, nodeArr, s, depth + 1);
		}
		nodeArr[index] = t;
		if (t->right != NULL)
			_getCopy(t->right, nodeArr, index + 1, depth + 1);

		if (need_wait)
			ft.wait();
	}

	/* Auxillary function used in _update for rebuilds */
	NODE* _buildTree(NODE** nodeArr, int s, int f, int depth) {
		if (s > f)
			return NULL;
		int m = (s + f + 1) / 2;
		NODE* t = nodeArr[m];
		if (m - s > CONCUR_SIZE && depth < CONCUR_DEPTH) {
			auto handler = async(std::launch::async, &WBTreeP<T>::_buildTree, this, ref(nodeArr), s, m - 1, depth + 1);
			t->right = _buildTree(nodeArr, m + 1, f, depth + 1);
			t->left = handler.get();
		}
		else {
			t->left = _buildTree(nodeArr, s, m - 1, depth + 1);
			t->right = _buildTree(nodeArr, m + 1, f, depth + 1);
		}
		t->size = f - s + 1;
		return t;
	}

	void _rebuild(NODE*& t) {
		if (t == NULL)
			return;
		int length = t->size;
		NODE** nodeArr = new NODE * [length]();
		_getCopy(t, nodeArr, 0, 0);					// Make nodeArr store all nodes in increasing key order
		t = _buildTree(nodeArr, 0, length - 1, 0);	// Rebuild the tree using the array
		delete[] nodeArr;
	}

	// Note : Does not update parent node's size field.
	void _clear(NODE*& t) {
		if (t == NULL)
			return;
		_clear(t->left);
		_clear(t->right);
		delete t;
		t = NULL;
	}
};
#endif
