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
	virtual ~RBTree();
private:
	RBTreeNode<K, V>* root;
	bool doRemove(RBTreeNode<K, V>* node, ListNode<RBTreeNode<K, V>*>* lNode);
	void delFixup(RBTreeNode<K, V>* node, ListNode<RBTreeNode<K, V>*>* lNode);
	void balance(RBTreeNode<K,V>* node, ListNode<RBTreeNode<K, V>*>* lNode);
	void lRotate(RBTreeNode<K, V>* node);
	void rRotate(RBTreeNode<K, V>* node);
	void preorderTraverse(RBTreeNode<K, V>* node, void (RBTree<K,V>::*handler)(RBTreeNode<K, V>*));
	void postOrderTraverse(RBTreeNode<K, V>* node, void (RBTree<K, V>::*handler)(RBTreeNode<K, V>*));
	void printNode(RBTreeNode<K, V>* node);
	void destroyNode(RBTreeNode<K, V>* node);
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
	ListNode<RBTreeNode<K, V>*>* lNode = nullptr;
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

/**
 * 插入后的平衡调整，仅针对父节点为红色
 * 分两两大类：
 * 1. 叔叔节点为红色，叔叔和父亲染黑，祖父染红，祖父为node递归调整
 * 2. 叔叔节点为黑色，各种旋转，结束
 * node 调整的节点，不一定是插入的，可能是递归过程中需要调整的节点
 * lNode 回溯路径（node的所有祖先节点）
 */
template <typename K, typename V>
void RBTree<K, V>::balance(RBTreeNode<K, V>* node, ListNode<RBTreeNode<K, V>*>* lNode) {
	// lNode为空，结束
	// lNode不为空，检查node是否为黑色，是则结束，不是则检查lNode是否为黑色，是则结束，不是则继续调整

	if(lNode != nullptr) {
		if(node != nullptr && !node->black && !lNode->value->black) {
			if(lNode->next != nullptr){
				// 父节点肯定不为空，但祖父节点可能为空
				RBTreeNode<K,V> *parent = lNode->value, *grandParent = lNode->next->value, *uncle=nullptr;
				if(grandParent->left  == parent) {
					uncle = grandParent->right;;
				} else {
					uncle = grandParent->left;
				}
				if(uncle != nullptr && !uncle->black) {
					parent->black = true;
					uncle->black = true;
					grandParent->black = grandParent->key == root->key ? true:false;
					balance(grandParent, lNode->next->next);
				} else {
					if(parent->right == node && grandParent->right == parent){
						grandParent->black = false;
						parent->black = true;
						lRotate(grandParent);
					} else if(parent->left == node && grandParent->left == parent){
						grandParent->black = false;
						parent->black = true;
						rRotate(grandParent);
					} else if(parent->right == node && grandParent->left == parent) {
						lRotate(parent);
						lNode->value = parent;
						balance(node, lNode);
					} else {
						rRotate(parent);
						lNode->value = parent;
						balance(node, lNode);
					}
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
/**
 * 左旋，父节点和右节点颜色交换
 */
template <typename K, typename V>
void RBTree<K, V>::lRotate(RBTreeNode<K, V>* node) {
	K key = node->right->key;
	V value = node->right->value;
	RBTreeNode<K, V>* rrNode = node->right->right;
	bool black = node->right->black;

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
/**
 * 右旋，父节点和左节点的颜色互换
 */
template <typename K, typename V>
void RBTree<K, V>::rRotate(RBTreeNode<K, V>* node) {
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
	void (RBTree<K, V>::*hdlr)(RBTreeNode<K, V>*) = RBTree<K,V>::printNode;
	preorderTraverse(root, hdlr);
}

template <typename K, typename V>
void RBTree<K, V>::printNode(RBTreeNode<K, V>* node) {
	if(node == nullptr) {
		std::cout << "empty node" << std::endl;
	} else {
		std::cout << (node->black ? "black" : "red") << " node: " << node->key << "=" << node->value << std::endl;
	}
}

template <typename K, typename V>
void RBTree<K, V>::preorderTraverse(RBTreeNode<K, V>* node, void (RBTree<K, V>::*handler)(RBTreeNode<K,V>*)) {
	(this->*handler)(node);
	if(node != nullptr) {
		preorderTraverse(node->left, handler);
		preorderTraverse(node->right, handler);
	}
}

template <typename K, typename V>
void RBTree<K, V>::postOrderTraverse(RBTreeNode<K, V>* node, void (RBTree<K, V>::*handler)(RBTreeNode<K, V>*)) {
	if(node != nullptr) {
		postOrderTraverse(node->left, handler);
		postOrderTraverse(node->right, handler);
	}
	(this->*handler)(node);
}

template <typename K, typename V>
bool RBTree<K, V>::remove(K key) {
	ListNode<RBTreeNode<K, V>*>* lNode;
	RBTreeNode<K ,V>* tn = root;
	while (tn != nullptr) {
		if(key == tn->key) {
			doRemove(tn, lNode);
			break;
		} else if(key < tn->key){
			lNode = new ListNode<RBTreeNode<K, V>*>(tn, lNode);
			tn = tn->left;
		} else {
			lNode = new ListNode<RBTreeNode<K, V>*>(tn, lNode);
			tn = tn->right;
		}
	}
	return true;
}

template <typename K, typename V>
bool RBTree<K, V>::doRemove(RBTreeNode<K, V>* node, ListNode<RBTreeNode<K, V>*>* lNode) {
	bool delBlack = node->black;
	if(node->left != nullptr && node->right == nullptr) {
		RBTreeNode<K, V>* delNode = node->left;
		node->key = node->left->key;
		node->value = node->left->value;
		node->black = node->left->black;
		node->left = node->left->left;
		delete delNode;
		if(delBlack) {
			delFixup(node, lNode);
		}
	} else if(node->right != nullptr && node->left == nullptr) {
		RBTreeNode<K, V>* delNode = node->right;
		node->key = node->right->key;
		node->value = node->right->value;
		node->black = node->right->black;
		node->right = node->right->right;
		delete delNode;
		if(delBlack) {
			delFixup(node, lNode);
		}
	} else if(node->right == nullptr && node->left == nullptr){
		if(lNode != nullptr) {
			if(lNode->value->right == node) {
				lNode->value->right = nullptr;
			} else {
				lNode->value->left = nullptr;
			}
		}
		delete node;
		if(delBlack) {
			delFixup(nullptr, lNode);
		}
	} else {
		RBTreeNode<K, V>* tn = node->right, *successor = node->right;
		while(tn->left != nullptr) {
			lNode = new ListNode<RBTreeNode<K, V>*>(tn, lNode);
			tn = tn->left;
			successor = tn;
		}
		K key = node->key;
		V value = node->value;
		bool black = node->black;
		node->key = successor->key;
		node->value = successor->value;
		node->black = successor->black;
		successor->key = key;
		successor->value = value;
		successor->black = black;
		doRemove(successor, lNode);
	}
	return true;
}

template <typename K, typename V>
void RBTree<K, V>::delFixup(RBTreeNode<K, V>* node, ListNode<RBTreeNode<K, V>*>* lNode) {
	if(node != nullptr && node->black == false) { // 红色节点直接设置为黑色
		node->black = true;
	} else if(lNode != nullptr){
		RBTreeNode<K, V> *parent = lNode->value, *sibling = lNode->value->right == node ? lNode->value->left : lNode->value->right;
		if(sibling == nullptr) {
			// 删除节点为黑色才会执行调整函数，红黑树任意节点到其所有叶子结点的路径中都有相同数目的黑色节点，所以必然存在一个兄弟节点，否则不满足红黑树性质
			std::cout << "嘿，自大狂，删除调整出现了兄弟节点不存在的情况，赶紧处理吧！" << std::endl;
		} else if(!sibling->black) {
			// 兄红，旋转，转到兄黑的情况
			sibling->black = parent->black;
			parent->black = false;
			if(sibling == parent->left) {
				rRotate(parent);
			} else {
				lRotate(parent);
			}
			lNode = new ListNode<RBTreeNode<K, V>*>(sibling, lNode);
			delFixup(node, lNode);
		} else {
			// 兄黑，分为子全黑，非全黑两种情况
			if((sibling->left == nullptr || sibling->left->black) && (sibling->right == nullptr ||sibling->right->black)) {
				if(!parent->black) {
					parent->black = true;
					sibling->black = false;
				} else {
					sibling->black = false;
					delFixup(parent, lNode->next);
				}
			} else {
				if(parent->left == sibling) {
					if(sibling->left != nullptr && !sibling->left->black) {
						sibling->black = parent->black;
						sibling->left->black = true;
						parent->black = true;
						rRotate(parent);
					} else {
						lRotate(sibling);
						delFixup(node, lNode);
					}
				} else {
					if(sibling->right != nullptr && !sibling->right->black) {
						sibling->black = parent->black;
						sibling->right->black = true;
						parent->black = true;
						lRotate(parent);
					} else {
						rRotate(sibling);
						delFixup(node, lNode);
					}
				}
			}
		}
	} else { // 不可能出现
		// lNode为空表示删除的节点为根节点，对于根节点
		// 1.如果只有一个子节点，那么该子节点必然为红色，而且子节点的左右节点都是空节点，此时满足第一个if判断，执行后调整结束
		// 2.如果有两个子节点，那么会去找后继节点，后继节点必然有父节点，lNode一定不会为空
		std::cout << "嘿，自大狂，删除调整出现了lNode为空的情况，赶紧处理吧！" << std::endl;
	}
}

template <typename K, typename V>
void RBTree<K, V>::destroyNode(RBTreeNode<K, V>* node) {
	if(node != nullptr) {
		delete node;
	}
}

template <typename K, typename V>
RBTree<K, V>::~RBTree() {
	void (RBTree<K, V>::*hdlr)(RBTreeNode<K, V>*) = RBTree<K, V>::destroyNode;
	postOrderTraverse(root, hdlr);
}
#endif
