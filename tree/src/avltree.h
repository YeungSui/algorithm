/*
 * avltree.h
 *
 *  Created on: 2019年12月30日
 *      Author: Administrator
 */

#ifndef AVLTREE_H_
#define AVLTREE_H_
#include "binarytree.h"

template <typename K, typename V>
void printTreeNode(TreeNode<K, V>* node) {
	if(node == nullptr) {
		printf("node is null\r\n");
	} else {
		std::cout << "Factor is " << node->factor << ". " << node->key << ": " << node->value << std::endl;
	}
}

template <typename K, typename V>
void deleteAll(TreeNode<K, V>* node) {
	if(node != nullptr) {
		delete node;
	}
}

template <typename K, typename V>
class AvlTree {
public:
	AvlTree(TreeNode<K,V>* root);
	bool insert(TreeNode<K, V>* node);
	bool remove(K key);
	int getHeight();
	void preorderTraverse(void (*hdlr)(TreeNode<K,V>*));
	void postorderTraverse(void (*hdlr)(TreeNode<K, V>*));
	virtual ~AvlTree();
private:
	void rebalance(TreeNode<K, V>* node);
	void rebalance(ListNode<TreeNode<K, V>*>* trace, int op);
	void rrRotate(TreeNode<K, V>* node);
	void llRotate(TreeNode<K, V>* node);
	void lrRotate(TreeNode<K, V>* node);
	void rlRotate(TreeNode<K, V>* node);
	int getHeight(TreeNode<K, V>* node);
	void doRemove(TreeNode<K, V>* node, ListNode<TreeNode<K, V>*>* trace);
	void preorderTraverse(TreeNode<K,V>* node, void (*hdlr)(TreeNode<K,V>*));
	void postorderTraverse(TreeNode<K, V>* node, void (*hdlr)(TreeNode<K, V>*));
	void swapContent(TreeNode<K,V>* node1, TreeNode<K, V>* node2);
	TreeNode<K,V>* root;
};

template <typename K, typename V>
AvlTree<K, V>::AvlTree(TreeNode<K,V>* mroot):root(mroot) {}

template <typename K, typename V>
bool AvlTree<K,V>::insert(TreeNode<K, V>* node) {
	ListNode<TreeNode<K, V>*>* lNode = new ListNode<TreeNode<K, V>*>(root, nullptr);
	int op = 0;
	TreeNode<K,V>* tn = root;
	// 普通二叉查找树插入
	while(tn != nullptr) {
		TreeNode<K, V>* pa = tn;
		if(node->key <= tn->key){
			tn = pa->left;
			if(tn == nullptr) {
				pa->left = node;
				op = -1;
			} else {
				lNode = new ListNode<TreeNode<K, V>*>(tn, lNode);
			}
		} else {
			tn = pa->right;
			if(tn == nullptr) {
				pa->right = node;
				op = 1;
			} else {
				lNode = new ListNode<TreeNode<K, V>*>(tn, lNode);
			}
		}
	}
	int tmpFactor;
	while(lNode != nullptr) {
		tmpFactor = lNode->value->factor;
		lNode->value->factor += op;
		if(lNode->value->factor < -1 || lNode->value->factor > 1) {
			rebalance(lNode->value);
			break;
		}
		if(lNode->next != nullptr) {
			if(abs(tmpFactor) < abs(lNode->value->factor)) {
				op = 1;
			} else {
				break;
			}
			if(lNode->value == lNode->next->value->left) {
				op = -op;
			}
		}
		ListNode<TreeNode<K, V>*>* tempNode = lNode;
		lNode = lNode->next;
		delete tempNode;
	}
	while(lNode != nullptr) {
		ListNode<TreeNode<K,V>*>* tempNode = lNode;
		lNode = lNode->next;
		delete tempNode;
	}
	return true;
}

template <typename K, typename V>
void AvlTree<K,V>::rebalance(TreeNode<K, V>* node) {
	if(node->factor < 0) {
		if(node->left->factor < 0) {
			llRotate(node);
		} else {
			lrRotate(node);
		}
	} else {
		if(node->right->factor > 0) {
			rrRotate(node);
		} else {
			rlRotate(node);
		}
	}
}

template <typename K, typename V>
void AvlTree<K, V>::rebalance(ListNode<TreeNode<K, V>*>* trace, int op) {
	ListNode<TreeNode<K, V>*>* lNode = trace;
	int tempFactor;
	while(lNode != nullptr) {
		tempFactor = lNode->value->factor;
		lNode->value->factor += op;
		if(lNode->value->factor < -1 || lNode->value->factor > 1) {
			rebalance(lNode->value);
		}
		if(lNode->next != nullptr) {
			if(abs(tempFactor) > abs(lNode->value->factor)) {
				op = 1;
			} else {
				// 高度不变，已平衡
				break;
			}
			if(lNode->next->value->right == lNode->value) {
				op = -op;
			}
		}
		lNode = lNode->next;
	}
	while(lNode != nullptr) {
		ListNode<TreeNode<K, V>*>* tempNode = lNode;
		lNode = lNode->next;
		delete tempNode;
	}
}

template <typename K, typename V>
void AvlTree<K, V>::llRotate(TreeNode<K, V>* node) {
	TreeNode<K, V>* root = node->left;
	TreeNode<K, V>* right = new TreeNode<K,V>(root->right, node->right, node->key, node->value,
			node->factor+1-(root->factor < 0 ? root->factor:0));
	node->right = right;
	node->left = root->left;
	node->key = root->key;
	node->factor = root->factor+1+(right->factor > 0 ? right->factor : 0);
	node->value = root->value;
	delete root;
}

template <typename K, typename V>
void AvlTree<K, V>::lrRotate(TreeNode<K, V>* node) {
	rrRotate(node->left);
	llRotate(node);
}

template <typename K, typename V>
void AvlTree<K, V>::rrRotate(TreeNode<K, V>* node) {
	/* 一般思路按照llRotate操作，但是为了提高效率，于是写了如下不使用new、delete的代码，较为复杂
	* 参照llRotate，rrRotate最终删除的是node的右子节点，我们不删除该节点，而是将其内容（key，value，factor等，除left，right）与node交换
	* 交换以后，新的右节点（原来的node）的位置是不正确的，应当插入到左节点和根节点之间，右节点的左子树变右子树，右子树则接替父亲节点作为新的node的右子树
	*/
	TreeNode<K, V>* rlc = node->right->left;
	TreeNode<K, V>* rrc = node->right->right;
	node->right->left = node->left;
	node->right->right = rlc;
	K key = node->right->key;
	V value = node->right->value;
	int factor = node->right->factor;
	node->right->key = node->key;
	node->right->value = node->value;
	// 计算原根节点旋转之后的平衡因子，假设原根节点为A，右节点为B。nb(A)表示新的平衡因子，ob(A)表示原来的平很因子
	// 则有：nb(A) = ob(A) - 1 - max(ob(B), 0)
	node->right->factor = node->factor - 1 - (0 > factor ? 0 : factor);
	// 至此，右节点已经替换成原根节点
	node->left = node->right;
	node->key = key;
	node->value = value;
	// 解释参照前面的注释：nb(B) = ob(B) - 1 + min(nb(A), 0)
	node->factor = factor - 1 + (0 < node->left->factor ? 0: node->left->factor);
	node->right = rrc;
}

template <typename K, typename V>
void AvlTree<K, V>::rlRotate(TreeNode<K, V>* node) {
	llRotate(node->right);
	rrRotate(node);
}

template <typename K, typename V>
void AvlTree<K, V>::preorderTraverse(void (*hdlr)(TreeNode<K, V>*)) {
	preorderTraverse(root, hdlr);
}

template <typename K, typename V>
void AvlTree<K, V>::preorderTraverse(TreeNode<K, V>* node, void (*hdlr)(TreeNode<K, V>*)) {
	hdlr(node);
	if(node != nullptr) {
		preorderTraverse(node->left, hdlr);
		preorderTraverse(node->right, hdlr);
	}
}

template <typename K, typename V>
AvlTree<K,V>::~AvlTree() {
	void (*hdlr) (TreeNode<K, V>*) = deleteAll;
	postorderTraverse(hdlr);
}

template <typename K, typename V>
void AvlTree<K, V>::postorderTraverse(void (*hdlr)(TreeNode<K, V>*)) {
	postorderTraverse(root, hdlr);
}

template <typename K, typename V>
void AvlTree<K, V>::postorderTraverse(TreeNode<K, V>* node, void (*hdlr)(TreeNode<K,V>*)) {
	if(node != nullptr) {
		postorderTraverse(node->left, hdlr);
	}
	if(node != nullptr) {
		postorderTraverse(node->right, hdlr);
	}
	hdlr(node);
}

template <typename K, typename V>
bool AvlTree<K, V>::remove(K key) {
	TreeNode<K, V>* tn = root;
	ListNode<TreeNode<K, V>*>* lNode = new ListNode<TreeNode<K, V>*>(tn, nullptr);
	while(tn != nullptr) {
		if(tn->key == key) {
			doRemove(tn, lNode);
			break;
		} else if(tn->key < key) {
			lNode = new ListNode<TreeNode<K, V>*>(tn, lNode);
			tn = tn->right;
		} else {
			lNode = new ListNode<TreeNode<K, V>*>(tn, lNode);
			tn = tn->left;
		}
	}
	return true;
}

template <typename K, typename V>
void AvlTree<K, V>::doRemove(TreeNode<K, V>* node, ListNode<TreeNode<K, V>*>* trace) {
	int op = 0;
	// 叶节点，直接删除
	if(node->left == nullptr && node->right == nullptr) {
		if(trace->value->left == node) {
			op = 1;
			trace->value->left = nullptr;
		} else {
			op = -1;
			trace->value->right = nullptr;
		}
		delete node;
		rebalance(trace, op);
	}
	// 一边为空，父子节点交换后删除父节点
	else if((node->right == nullptr && node->left != nullptr)) {
		TreeNode<K, V>* tempNode = node->left;
		node->key = node->left->key;
		node->value = node->left->value;
		node->left = node->left->left;
		node->right = node->left->right;
		node->factor = node->left->factor;
		delete tempNode;
		op = 1;
		rebalance(trace, op);
	} else if((node->left == nullptr && node->right != nullptr)) {
		TreeNode<K, V>* tempNode = node->right;
		node->key = node->right->key;
		node->value = node->right->value;
		node->left = node->right->left;
		node->right = node->right->right;
		node->factor = node->right->factor;
		delete tempNode;
		op = -1;
		rebalance(trace, op);
	} else {
		TreeNode<K, V>* successor = node->right;
		trace = new ListNode<TreeNode<K, V>*>(node, trace);
		while(successor->left != nullptr) {
			trace = new ListNode<TreeNode<K, V>*>(successor, trace);
			successor = successor->left;
		}
		swapContent(node, successor);
		// 平衡因子不交换
		int noswap = node->factor;
		node->factor = successor->factor;
		successor->factor = noswap;
		doRemove(successor, trace);
	}
}

template <typename K, typename V>
void AvlTree<K, V>::swapContent(TreeNode<K,V>* node1, TreeNode<K, V>* node2) {
	assert(node1 != nullptr && node2 != nullptr);
	K key = node1->key;
	V value = node1->value;
	int factor = node1->factor;
	node1->key = node2->key;
	node1->value = node2->value;
	node1->factor = node2->factor;
	node2->key = key;
	node2->value = value;
	node2->factor = factor;
}

#endif /* AVLTREE_H_ */
