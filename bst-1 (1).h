#pragma once
#include <vector>
#include <stack>
#include <exception>

namespace cop3530 {

template<typename Key, typename T>
class bst {
    struct Node {
        Key k;// key
        T value;// value
        Node* left;// left child
        Node* right;// right child
		int balance;

        Node() {
			k = NULL;
			value = NULL;
			left = nullptr;
			right = nullptr;
			balance = 0;
		}
        Node(Key k, T t) {
			this->k = k;
			this->value = t;
			left = nullptr;
			right = nullptr;
			balance = 0;
		}
		~Node() {
			left = nullptr;
			right = nullptr;
		}
		Node(const Node& copyNode) {
			this->left = copyNode.left;
			this->right = copyNode.right;
			this->value = copyNode.value;
			this->balance = copyNode.balance;
		}
		Node& operator=(const Node& copyNode) {
			this->left = copyNode.left;
			this->right = copyNode.right;
			this->value = copyNode.value;
			this->balance = copyNode.balance;
			return *this;
		}
    };

    Node* root;// root of tree

    public:
        bst();
        ~bst();
		bst(const bst& copybst);
		bst& operator=(const bst& copybst);
        void insert(const Key &key, const T &t);
		Node* insertNode(Node* &localRoot, const Key &key, const T &t, Node* &parent);
        void erase(const Key &key);
		Node*  removeNode(Node* &localRoot, const Key &key);
		Node*  leftMostNode(Node* &localRoot);
        T& at(const Key &key);
		T& findKey(Node* &localRoot, const Key &key);
        bool contains(const Key &key) const;
		bool recursiveContains(const Key &key, Node* localRoot) const;
        bool empty() const;
        size_t size() const;
		int getHeight(Node* localRoot);
		Node* correctBalance(Node* localRoot);
		Node* rotateRight(Node* localRoot);
		Node* rotateLeft(Node* localRoot);
		int countNodes(Node* localRoot) const;
        std::vector<std::pair<Key, T>> inorder_contents();
		void storeInOrder(std::vector<std::pair<Key, T>> &contents, Node* curr);
        std::vector<std::pair<Key, T>> preorder_contents();
		void storePreOrder(std::vector<std::pair<Key, T>> &contents, Node* curr);
};

template<typename Key, typename T>
bst<Key, T>::bst() {
	root = nullptr;
}

template<typename Key, typename T>
bst<Key, T>::~bst() {
	delete root;
}

template <typename Key, typename T>
bst<Key, T>::bst(const bst& copybst){
	this->root = copybst.root;
}

template <typename Key, typename T>
bst<Key, T>& bst<Key, T>::operator=(const bst& copybst){
	this->root = copybst.root;
	return *this;
}

template<typename Key, typename T>
void bst<Key, T>::insert(const Key &key, const T &t) {	
	if (root == nullptr){
		root = new Node(key, t);;
	}
	else {
		if (root->k > key) {
			root->left = insertNode(root->left, key, t, root);
			root->balance = getHeight(root->right) - getHeight(root->left);
			root = correctBalance(root);
		}
		else if (root->k < key) {
			root->right = insertNode(root->right, key, t, root);
			root->balance = getHeight(root->right) - getHeight(root->left);
			root = correctBalance(root);
		}
		else {
			root->value = t;
		}
	}
}

template<typename Key, typename T>
typename bst<Key, T>::Node* bst<Key, T>::insertNode(Node* &localRoot, const Key &key, const T &t, Node* &parent) {
	if (localRoot == nullptr) {
		localRoot = new Node(key, t);
		if (localRoot->k < parent->k) {
			parent->left = localRoot;
		}
		else {
			parent->right = localRoot;
		}
		return localRoot;
	}
	else {
		if (localRoot->k > key) {
			insertNode(localRoot->left, key, t, localRoot);
			localRoot->balance = getHeight(localRoot->right) - getHeight(localRoot->left);
			return correctBalance(localRoot);
		}
		else if (localRoot->k < key) {
			insertNode(localRoot->right, key, t, localRoot);
			localRoot->balance = getHeight(localRoot->right) - getHeight(localRoot->left);
			return correctBalance(localRoot);
		}
		else {
			localRoot->value = t;
		}
	}
	return correctBalance(localRoot);
}

template<typename Key, typename T>
void bst<Key, T>::erase(const Key &key) {
	if (root == nullptr) {
		throw std::out_of_range("Tree is empty.");
	}
	else {
		if (root->k == key) { //key is the root. 
			if (root->right != nullptr) { //has right child
				Node* inOrderSuccessor = leftMostNode(root->right);
				root->k = inOrderSuccessor->k;
				root->value = inOrderSuccessor->value;
				root->right = removeNode(root->right, inOrderSuccessor->k);
			}
			else if (root->left != nullptr){ //has left child, but no right child. Just reassigns root
				root = root->left;
			}
			else { //has neither left or right. Just empty the tree
				root = nullptr;
			}
		}
		else { //key is not the root. Call recursive remove algorithm
			if (key < root->k) {
				root->left = removeNode(root->left, key);
			}
			else {
				root->right = removeNode(root->right, key);
			}
			root->balance = getHeight(root->right) - getHeight(root->left);
			root = correctBalance(root);
		}
	}
}

template<typename Key, typename T>
typename bst<Key, T>::Node*  bst<Key, T>::removeNode(Node* &localRoot, const Key &key) {
	if (localRoot == nullptr) {
		throw std::out_of_range("Cannot find node with key.");
		return localRoot; 
	}
	if (key < localRoot->k) {
		localRoot->left = removeNode(localRoot->left, key);
	}
	else if (key > localRoot->k) {
		localRoot->right = removeNode(localRoot->right, key);		
	}
	else { 
		if (localRoot->left == nullptr || localRoot->right == nullptr) { //if has at most one child...
			Node* temp = nullptr;
			if (localRoot->left != nullptr) { //if it has only left child...
				temp = localRoot->left;
			}
			else { //if it has only right child...
				temp = localRoot->right;
			}
			if (temp == nullptr) { //if it has neither child...
				temp = localRoot;
				localRoot = nullptr;
			}
			else { //if it has only one child, temp is that child
				localRoot->k = temp->k;
				localRoot->value = temp->value;
				if (localRoot->left == temp) {
					localRoot->left = nullptr;
				}
				else {
					localRoot->right = nullptr;
				}
			}
		}		
		else {
			Node* inOrderSuccessor = leftMostNode(localRoot->right);
			localRoot->k = inOrderSuccessor->k;
			localRoot->value = inOrderSuccessor->value;
			localRoot->right = removeNode(localRoot->right, inOrderSuccessor->k);
		}
	}
	if (localRoot == nullptr) {
		return localRoot;
	}
	localRoot->balance = getHeight(localRoot->right) - getHeight(localRoot->left);
	return correctBalance(localRoot);
}

template<typename Key, typename T>
typename bst<Key, T>::Node* bst<Key, T>::correctBalance(Node* localRoot) {
	if (localRoot->balance > 1) {
		if (localRoot->right->balance > 0) {
			return rotateLeft(localRoot);
		}
		else if (localRoot->right->balance < 0) {
			localRoot->right = rotateRight(localRoot->right);
			Node* newLocalRoot = rotateLeft(localRoot);
			return newLocalRoot;
		}
	}
	
	else if (localRoot->balance < -1) {
		if (localRoot->left->balance > 0) {
			localRoot->left = rotateLeft(localRoot->left);
			Node* newLocalRoot = rotateRight(localRoot);
			return newLocalRoot;
		}
		else if (localRoot->left->balance < 0) {
			return rotateRight(localRoot);
		}		
	}
	return localRoot;
}


template<typename Key, typename T>
int bst<Key, T>::getHeight(Node* localRoot) { //returns the height of the tree starting from *this;
	if (localRoot != nullptr) {
		int leftHeight = getHeight(localRoot->left);
		int rightHeight = getHeight(localRoot->right);
		if (leftHeight > rightHeight) {
			return leftHeight+1;
		}
		else {
			return rightHeight+1;
		}
	}
	return 0;
}

template<typename Key, typename T>
typename bst<Key, T>::Node* bst<Key, T>::rotateRight(Node* localRoot) {
	Node* grandchild = localRoot->left->right;
	Node* newParent = localRoot->left;
	localRoot->left = grandchild;
	newParent->right = localRoot;
	return newParent;
}

template<typename Key, typename T> 
typename bst<Key, T>::Node* bst<Key, T>::rotateLeft(Node* localRoot) {
	Node* grandchild = localRoot->right->left;
	Node* newParent = localRoot->right;
	localRoot->right = grandchild;
	newParent->left = localRoot;
	return newParent;
}

template<typename Key, typename T> 
T& bst<Key, T>::at(const Key &key) {
    if (root == nullptr) { //tree is empty, nothing to find. 
		throw std::out_of_range("");
	}
	else {
		return findKey(root, key);
	}
}

template<typename Key, typename T>
typename bst<Key, T>::Node* bst<Key, T>::leftMostNode(Node* &localRoot) {
	Node* curr = localRoot;
	while (curr->left != nullptr) {
		curr = curr->left;
	}
	return curr;
}

template<typename Key, typename T> 
T& bst<Key, T>::findKey(Node* &localRoot, const Key &key) {
	if (localRoot == nullptr) {
		throw std::out_of_range("Given key is not in the tree");
	}
	else {
		if (key == localRoot->k) {
			return localRoot->value;
		}
		else if (key < localRoot->k) {
			return findKey(localRoot->left, key);
		}
		else {
			return findKey(localRoot->right, key);
		}
	}
}

template<typename Key, typename T>
bool bst<Key, T>::contains(const Key &key) const { 
		return recursiveContains(key, root);
}

template<typename Key, typename T>
bool bst<Key, T>::recursiveContains(const Key &key, Node* localRoot) const {
	if (localRoot == nullptr) {
		return false;
	}
	if (key == localRoot->k) {
		return true;
	} 
	else {
		if(key < localRoot->k) {
			return recursiveContains(key, localRoot->left);
		}
		else {
			return recursiveContains(key, localRoot->right);
		}
	}	
}


template<typename Key, typename T>
bool bst<Key, T>::empty() const {
    return (root == nullptr);
}

template<typename Key, typename T>
size_t bst<Key, T>::size() const {
	if (empty()) {
		return 0;
	}
	else {
		return countNodes(this->root);
	}
}

template<typename Key, typename T>
int bst<Key, T>::countNodes(Node* localRoot) const {
	 if (localRoot == nullptr) {
		 return 0;
	 }
	 else {
		 return (countNodes(localRoot->left) + countNodes(localRoot->right) + 1);
	 }
}

template<typename Key, typename T>
std::vector<std::pair<Key, T>> bst<Key, T>::inorder_contents() {
    std::vector<std::pair<Key, T>> contents;
	storeInOrder(contents, root);
    return contents;
}

template<typename Key, typename T>
void bst<Key, T>::storeInOrder(std::vector<std::pair<Key, T>> &contents, Node* curr) {
		if (curr == nullptr) {
			return;
		}
		else {
			storeInOrder(contents, curr->left);
			contents.push_back(std::pair <Key, T> (curr->k, curr->value) );
			storeInOrder(contents, curr->right);
		}
}

template<typename Key, typename T>
std::vector<std::pair<Key, T>> bst<Key, T>::preorder_contents() {
    std::vector<std::pair<Key, T>> contents;
	storePreOrder(contents, root);
    return contents;
}

template<typename Key, typename T>
void bst<Key, T>::storePreOrder(std::vector<std::pair<Key, T>> &contents, Node* curr) {
		if (curr == nullptr) {
			return;
		}
		else {
			contents.push_back(std::pair <Key, T> (curr->k, curr->value) );
			storePreOrder(contents, curr->left);
			storePreOrder(contents, curr->right);
		}
}


}