//============================================================================
// Name        : tree.cpp
// Author      : Boer
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "binarytree.h"
#include "avltree.h"
#include "rbtree.cc"
using namespace std;

typedef TreeNode<int, std::string> IntNode;
typedef RBTreeNode<int, std::string> RBIntNode;

void testRBTree();

int main() {
	testRBTree();
//	TreeNode child3 = {nullptr, nullptr, "this is child3", "child3"};
//	TreeNode child5 = {nullptr, nullptr, "this is child5", "child5"};
//	TreeNode child4 = {child5, nullptr, "this is child4", "child4"};
//	TreeNode child1 = {child3, child4, "this is child1", "child1"};
//	TreeNode child7 = {nullptr, nullptr, "this is child7", "child7"};
//	TreeNode child8 = {nullptr, nullptr, "this is child8", "child8"};
//	TreeNode child6 = {&child7, &child8, "this is child6", "child6"};
//	TreeNode child2 = {nullptr, child6, "this is child2", "child2"};
//	TreeNode root = {child1, child2, std::string("this is the root of tree"), std::string("root")};
//	IntNode* root = new IntNode(nullptr, nullptr, 80, "root", 0);
//	IntNode* child1 = new IntNode(nullptr, nullptr, 70, "child1", 0);
//	IntNode* child2 = new IntNode(nullptr, nullptr, 85, "child2", 0);
//	IntNode* child3 = new IntNode(nullptr, nullptr, 60, "child3", 0);
//	IntNode* child4 = new IntNode(nullptr, nullptr, 50, "child4", 0);
//	IntNode* child5 = new IntNode(nullptr, nullptr, 90, "child5", 0);
//	IntNode* child6 = new IntNode(nullptr, nullptr, 77, "child6", 0);
//	IntNode* child7 = new IntNode(nullptr, nullptr, 75, "child7", 0);
//	IntNode* child8 = new IntNode(nullptr, nullptr, 71, "child8", 0);
//	IntNode* child9 = new IntNode(nullptr, nullptr, 95, "child9", 0);
//	IntNode* child10 = new IntNode(nullptr, nullptr, 83, "child10", 0);
//	AvlTree<int, std::string> at(root);
//	at.insert(child1);
//	at.insert(child2);
//	at.insert(child3);
//	at.insert(child4);
//	at.insert(child5);
//	at.insert(child6);
//	at.insert(child7);
//	at.insert(child8);
//	at.insert(child9);
//	at.insert(child10);
//	at.remove(90);
//	at.remove(95);
//	void (*hdlr)(IntNode* node) = printTreeNode;
//	at.preorderTraverse(hdlr);
	return 0;
}

void testRBTree() {
	RBIntNode* root = new RBIntNode(80, "root");
	RBIntNode* child1 = new RBIntNode(70, "child1");
	RBIntNode* child2 = new RBIntNode(85, "child2");
	RBIntNode* child3 = new RBIntNode(60, "child3");
	RBIntNode* child4 = new RBIntNode(50, "child4");
	RBIntNode* child5 = new RBIntNode(90, "child5");
	RBIntNode* child6 = new RBIntNode(77, "child6");
	RBIntNode* child7 = new RBIntNode(75, "child7");
	RBIntNode* child8 = new RBIntNode(71, "child8");
	RBIntNode* child9 = new RBIntNode(95, "child9");
	RBIntNode* child10 = new RBIntNode(83, "child10");
	RBIntNode* child11 = new RBIntNode(72, "child11");
	RBIntNode* child12 = new RBIntNode(82, "child12");
	RBTree<int, std::string> rbt(root);
	rbt.insert(child1);
	rbt.insert(child2);
	rbt.insert(child3);
	rbt.insert(child4);
	rbt.insert(child5);
	rbt.insert(child6);
	rbt.insert(child7);
	rbt.insert(child8);
	rbt.insert(child9);
	rbt.insert(child10);
	rbt.insert(child11);
	rbt.insert(child12);
	rbt.print();
}
