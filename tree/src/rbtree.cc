/*
 * rbtree.cc
 *
 *  Created on: 2020年1月7日
 *      Author: Administrator
 */
#ifndef _RBTREE_H
#define _RBTREE_H
#include "binarytree.h"

template <typename K, typename V>
struct RBTreeNode {
	RBTreeNode<K, V>* left;
	RBTreeNode<K, V>* right;
	K key;
	V value;
	bool black;
	RBTreeNode(K key, V value) {
		this->key = key;
		this->value = value;
		this->left = nullptr;
		this->right = nullptr;
		this->black = true;
	}
};
template <typename K, typename V>
class RBTree {
public:
	RBTree(RBTreeNode<K, V>* node);
	bool insert(K key, V value);
	bool insert(RBTreeNode<K, V>* node);
	bool remove(K key);
	void print();
private:
	RBTreeNode<K, V>* root;
	void balance(RBTreeNode<K,V>* node, ListNode<RBTreeNode<K,V>*>* lNode);
	void lRotate(RBTreeNode<K, V>* node);
	void rRotate(RBTreeNode<K, V>* node);
	void preorderTraverse(RBTreeNode<K, V>* node, void (*handler)(RBTreeNode<K, V>*));
	void print(RBTreeNode<K, V>* node);
};

template <typename K, typename V>
RBTree<K, V>::RBTree(RBTreeNode<K, V>* node):root(node) {

}

template <typename K, typename V>
bool RBTree<K, V>::insert(K key, V value) {
	RBTreeNode<K, V>* node = new RBTreeNode<K, V>(key, value);
	return this->insert(node);
}

template <typename K, typename V>
bool RBTree<K,V>::insert(RBTreeNode<K, V>* node) {
	bool result = true;
	ListNode<RBTreeNode<K, V>*>* lNode = new ListNode<RBTreeNode<K, V>*>(root, nullptr);
	if(node != nullptr) {
		node->black = false;
	}
	if(root == nullptr) {
		node->black = true;
		root = node;
	}
	RBTreeNode<K, V>* tn = root;
	while(tn != nullptr) {
		if(node->key == tn->key) {
			result = false;
			break;
		}
		lNode = new ListNode<RBTreeNode<K,V>*>(tn, lNode);
		if(node->key < tn->key) {
			if(tn->left == nullptr) {
				tn->left = node;
				if(!tn->black) {
					balance(node, lNode);
				}
				break;
			}
			tn = tn->left;
		} else {
			if(tn->right == nullptr) {
				tn->right = node;
				if(!tn->black) {
					balance(node, lNode);
				}
				break;
			}
			tn = tn->right;
		}
	}
	while(lNode != nullptr) {
		ListNode<RBTreeNode<K, V>*>* ln = lNode;
		lNode = lNode->next;
		delete ln;
	}
	return result;
}

template <typename K, typename V>
void RBTree<K, V>::balance(RBTreeNode<K, V>* node, ListNode<RBTreeNode<K, V>*>* lNode) {
	// lNode为空，结束
	// lNode不为空，检查node是否为黑色，是则结束，不是则检查lNode是否为黑色，是则结束，不是则继续调整

	if(lNode != nullptr) {
		if(!node->black && !lNode->black) {
			if(lNode->next != nullptr){
				// 父节点肯定不为空，但祖父节点可能为空
				RBTreeNode<K,V> *parent = lNode->value, *grandParent = lNode->next->value, *uncle=nullptr;
				if(grandParent->left  == parent) {
					uncle = grandParent->right;;
				} else {
					uncle = grandParent->left;
				}
				if(!uncle->black) {
					parent->black = true;
					uncle->black = true;
					grandParent->black = false;
					balance(grandParent, lNode->next->next);
				} else if(parent->right == node){
					lRotate(parent);
					lNode->value = node;
					balance(parent, lNode);
				} else {
					grandParent->black = false;
					parent->black = true;
					rRotate(grandParent);
					lNode->next->value = parent;
					lNode->value = grandParent;
					balance(uncle, lNode);
				}
			} else {
				// 第二层插入，由于根节点为黑色，不会调用该方法
				/* 可能出现的情况只有：叔红，lNode跳到第二层，显然不需要调整
				 * 叔黑也不可能出现，左旋右旋之后lNode的长度均不变，
				 */
				std::cout << "嘿，自大狂，lNode跳到第二层了" << std::endl;
			}
		}
	}
}

template <typename K, typename V>
void RBTree<K, V>::rRotate(RBTreeNode<K, V>* node) {
	K key = node->right->key;
	V value = node->right->value;
	RBTreeNode<K, V>* rrNode = node->right->right;
	bool black = node->black;

	node->right->key = node->key;
	node->right->value = node->value;
	node->right->black = node->black;
	node->right->right = node->right->left;
	node->right->left = node->left;

	node->left = node->right;

	node->right = rrNode;
	node->key = key;
	node->value = value;
	node->black = black;
}

template <typename K, typename V>
void RBTree<K, V>::lRotate(RBTreeNode<K, V>* node) {
	K key = node->left->key;
	V value = node->left->value;
	RBTreeNode<K, V>* llNode = node->left->left;
	bool black = node->left->black;

	node->left->key = node->key;
	node->left->value = node->value;
	node->left->black = node->black;
	node->left->left = node->left->right;
	node->left->right = node->right;

	node->right = node->left;

	node->left = llNode;
	node->key = key;
	node->value = value;
	node->black = black;
}

template <typename K, typename V>
void RBTree<K, V>::print() {
	void (*hdlr)(RBTreeNode<K, V>* node) = print;
	preorderTraverse(root, print);
}

template <typename K, typename V>
void RBTree<K, V>::print(RBTreeNode<K, V>* node) {
	if(node == nullptr) {
		std::cout << "empty node" << std::endl;
	} else {
		std::cout << (node->black ? "black" : "red") << " node: " << node->key << "=" << node->value << std::endl;
	}
}

template <typename K, typename V>
void RBTree<K, V>::preorderTraverse(RBTreeNode<K, V>* node, void (*handler)(RBTreeNode<K,V>*)) {
	handler(node);
	if(node != nullptr) {
		preorderTraverse(node->left, handler);
		preorderTraverse(node->right, handler);
	}
}
#endif
