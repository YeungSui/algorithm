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
using namespace std;

typedef TreeNode<int, std::string> IntNode;

int main() {
//	TreeNode child3 = {nullptr, nullptr, "this is child3", "child3"};
//	TreeNode child5 = {nullptr, nullptr, "this is child5", "child5"};
//	TreeNode child4 = {child5, nullptr, "this is child4", "child4"};
//	TreeNode child1 = {child3, child4, "this is child1", "child1"};
//	TreeNode child7 = {nullptr, nullptr, "this is child7", "child7"};
//	TreeNode child8 = {nullptr, nullptr, "this is child8", "child8"};
//	TreeNode child6 = {&child7, &child8, "this is child6", "child6"};
//	TreeNode child2 = {nullptr, child6, "this is child2", "child2"};
//	TreeNode root = {child1, child2, std::string("this is the root of tree"), std::string("root")};
	IntNode* root = new IntNode(nullptr, nullptr, 80, "root", 0);
	IntNode* child1 = new IntNode(nullptr, nullptr, 70, "child1", 0);
	IntNode* child2 = new IntNode(nullptr, nullptr, 85, "child2", 0);
	IntNode* child3 = new IntNode(nullptr, nullptr, 60, "child3", 0);
	IntNode* child4 = new IntNode(nullptr, nullptr, 50, "child4", 0);
	IntNode* child5 = new IntNode(nullptr, nullptr, 90, "child5", 0);
	IntNode* child6 = new IntNode(nullptr, nullptr, 77, "child6", 0);
	IntNode* child7 = new IntNode(nullptr, nullptr, 75, "child7", 0);
	IntNode* child8 = new IntNode(nullptr, nullptr, 71, "child8", 0);
	IntNode* child9 = new IntNode(nullptr, nullptr, 95, "child9", 0);
	IntNode* child10 = new IntNode(nullptr, nullptr, 83, "child10", 0);
	AvlTree<int, std::string> at(root);
	at.insert(child1);
	at.insert(child2);
	at.insert(child3);
	at.insert(child4);
	at.insert(child5);
	at.insert(child6);
	at.insert(child7);
	at.insert(child8);
	at.insert(child9);
	at.insert(child10);
	at.remove(90);
	void (*hdlr)(IntNode* node) = printTreeNode;
	at.preorderTraverse(hdlr);
	return 0;
}
