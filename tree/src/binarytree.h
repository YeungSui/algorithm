/*
 * binarytree.h
 *
 *  Created on: 2019年12月28日
 *      Author: Administrator
 */

#ifndef TREENODE_H_
#define TREENODE_H_
template <typename K, typename V>
struct TreeNode {
	TreeNode<K, V>* left;
	TreeNode<K, V>* right;
	K key;
	V value;
	uint16_t height;
	int factor;
	TreeNode(TreeNode<K, V>* left, TreeNode<K,V>* right, K key, V value, int factor) {
		this->left = left;
		this->right = right;
		this->key = key;
		this->value = value;
		this->height = 0;
		this->factor = factor;
	}
};
#endif

#ifndef LIST_TREENODE_H_
#define LIST_TREENODE_H_
template<typename T>
struct ListNode {
	T value;
	ListNode<T>* next;
	ListNode(T mvalue, ListNode<T>* mnext) {
		value = mvalue;
		next = mnext;
	}
};
#endif

#ifndef BINARYTREE_H_
#define BINARYTREE_H_
#include <string>
#include <cstdint>
#include <iostream>
#include <stack>
#include <cassert>
template <typename K, typename V>
class BinaryTree {
public:
	BinaryTree(TreeNode<K, V>* tn);
	bool insert(K pKey, TreeNode<K, V>* child, uint8_t pos);
	bool insert(K pKey, TreeNode<K, V>* child, uint8_t posP, uint8_t posC);
	bool remove(K key);
	TreeNode<K, V>* build(std::string track);
	void traverse(void (*handler)(TreeNode<K, V>*));
	int getHeight();
	static void printHandler(TreeNode<K, V>* node);
private:
	void traverse(TreeNode<K, V>* node, void (*handler)(TreeNode<K, V>*));
	int getHeight(TreeNode<K, V>* node);
	void updateHeight();
	int updateHeight(TreeNode<K, V>* node);
	TreeNode<K, V>* root;
};

template <class K, class V>
BinaryTree<K, V>::BinaryTree(TreeNode<K, V>* tn):root(tn) {}

template <class K, class V>
bool BinaryTree<K, V>::insert(K pKey, TreeNode<K, V>* child, uint8_t pos) {
	bool inserted = false;
	if(root == nullptr) {
		root = child;
		inserted = true;
	} else {
		TreeNode<K, V>* tn = root;
		std::stack<TreeNode<K, V>*> tnStack;
		bool found = false;
		while(true) {
			if(tn == nullptr) {
				if(tnStack.empty()) {
					break;
				} else {
					tn = tnStack.top();
					tnStack.pop();
					tn = tn->right;
				}
			} else {
				if(tn->key == pKey) {
					found = true;
					break;
				}
				tnStack.push(tn);
				tn = tn->left;
			}
		}
		if(found && (pos == 0 || pos == 1)) {
			inserted = true;
			if(pos == 0) {
				child->left = tn->left;
				tn->left = child;
			} else {
				child->right = tn->right;
				tn->right = child;
			}
		}
	}
	updateHeight();
	return inserted;
}

template <typename K, typename V>
void BinaryTree<K,V>::updateHeight() {
	updateHeight(root);
}

template <typename K, typename V>
int BinaryTree<K, V>::updateHeight(TreeNode<K, V>* node) {
	if(node == nullptr) {
		return 0;
	}
	int lHeight = updateHeight(node->left);
	int rHeight = updateHeight(node->right);
	node->height = (lHeight >= rHeight ? lHeight:rHeight) + 1;
	return node->height;
}

template <class K, class V>
void BinaryTree<K,V>::printHandler(TreeNode<K,V>* node) {
	if(node == nullptr) {
		std::cout << "null" << std::endl;
	} else {
		std::cout << "level- " << node->height << " | "<< node->key << ": " << node->value << std::endl;
	}
}

template <class K, class V>
void BinaryTree<K, V>::traverse(void (*handler)(TreeNode<K, V>* node)) {
	traverse(this->root, handler);
}

template <class K, class V>
void BinaryTree<K, V>::traverse(TreeNode<K, V>* node, void (*handler)(TreeNode<K, V>* node)) {
	handler(node);
	if(node == nullptr) {
		return;
	}
	traverse(node->left, handler);
	traverse(node->right, handler);
}

template <class K, class V>
int BinaryTree<K,V>::getHeight() {
	return getHeight(this->root);
}

template <class K, class V>
int BinaryTree<K,V>::getHeight(TreeNode<K,V>* node) {
	if(node == nullptr) {
		return 0;
	}
	int lHeight = getHeight(node->left);
	int rHeight = getHeight(node->right);
	return (lHeight > rHeight ? lHeight : rHeight) + 1;
}

template <class K, class V>
TreeNode<K,V>* BinaryTree<K, V>::build(std::string track) {
	return nullptr;
}
#endif /* BINARYTREE_H_ */
