// WBTree.h
#ifndef WBTREE_H
#define WBTREE_H

#include <iostream>
using namespace std;

template <typename T>
class WBTree {
public:
	WBTree() {
		root = NULL;
		alpha = 0.32;
	}

	WBTree(double Alpha) {
		if ((Alpha <= 0) || (0.5 <= Alpha))
			throw invalid_argument("Alpha must be 0 < Alpha < 0.5");
		root = NULL;
		alpha = Alpha;
	}

	~WBTree() {
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
		double thres = alpha * (t->size + 1);
		if ((t->left && t->left->size + 1 < thres) || (!t->left && 1 < thres))
			return true;
		else if ((t->right && t->right->size + 1 < thres) || (!t->right && 1 < thres))
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

	bool _insert(NODE*& t, T v, NODE**& rebuildLoc) {
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

	bool _delete(NODE*& t, T v, NODE**& rebuildLoc) {
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
				_delete(t->left, t->key, rebuildLoc);	//Delete the inorder predecessor. Note that it always has 0 or 1 child nodes.
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
	void _getCopy(NODE* t, NODE** nodeArr, int s) {
		int index = s;
		if (t->left != NULL) {
			index += t->left->size;
			_getCopy(t->left, nodeArr, s);
		}
		nodeArr[index] = t;
		if (t->right != NULL)
			_getCopy(t->right, nodeArr, index + 1);
	}

	/* Auxillary function used in _update for rebuilds */
	NODE* _buildTree(NODE** nodeArr, int s, int f) {
		if (s > f)
			return NULL;
		int m = (s + f + 1) / 2;
		NODE* t = nodeArr[m];
		t->left = _buildTree(nodeArr, s, m - 1);
		t->right = _buildTree(nodeArr, m + 1, f);
		t->size = f - s + 1;
		return t;
	}

	void _rebuild(NODE*& t) {
		// if (t == NULL)
		// 	return;
		int length = t->size;
		NODE** nodeArr = new NODE * [length]();
		_getCopy(t, nodeArr, 0);				// Make nodeArr store all nodes in increasing key order
		t = _buildTree(nodeArr, 0, length - 1);	// Rebuild the tree using the array
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
