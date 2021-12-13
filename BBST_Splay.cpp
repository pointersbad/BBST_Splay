#include <iostream>
using namespace std;
class Node {
public:
	Node() {}
	Node(const int& x) {
		data = x;
	}
	Node* left = NULL;
	Node* right = NULL;
	Node* parent = NULL;
	int data = 0;
	int value = 1;
	int descendants = 0;
};

class BBST {
public:
	int countRank(Node*& node);
	void splay(Node* node);
	void insert(const int& x);
	void erase(const int& x);
	void find(Node*& node, const int& x, int& count);
	void findxsmallest(Node*& node, const int& x, int& count);
	void precursor(Node*& node, const int& x, Node*& value);
	void successor(Node*& node, const int& x, Node*& value);
	void rotateL(Node* node);
	void rotateR(Node* node);
	Node* root = NULL;
};

int BBST::countRank(Node*& node) {
	if (node != NULL) return node->descendants + node->value;
	return 0;
}

void BBST::rotateL(Node* node) {
	if (node == NULL) return;
	else {
		Node* lson = node->left;
		node->descendants = countRank(node->right) + countRank(lson->right);
		lson->descendants = countRank(node) + countRank(lson->left);
		node->left = lson->right;
		if (lson->right) lson->right->parent = node;
		lson->parent = node->parent;
		if (node->parent == NULL) this->root = lson;
		else if (node == node->parent->left) node->parent->left = lson;
		else if (node == node->parent->right) node->parent->right = lson;
		lson->right = node;
		node->parent = lson;
	}
}

void BBST::rotateR(Node* node) {
	if (node == NULL) return;
	else {
		Node* rson = node->right;
		node->descendants = countRank(node->left) + countRank(rson->left);
		rson->descendants = countRank(node) + countRank(rson->right);
		node->right = rson->left;
		if (rson->left) rson->left->parent = node;
		rson->parent = node->parent;
		if (node->parent == NULL) this->root = rson;
		else if (node == node->parent->left) node->parent->left = rson;
		else if (node == node->parent->right) node->parent->right = rson;
		rson->left = node;
		node->parent = rson;
	}
}

void BBST::splay(Node* node) {
	while (node->parent != NULL) {
		if (node->parent->parent == NULL) {
			if (node == node->parent->left) rotateL(node->parent);
			else if (node == node->parent->right) rotateR(node->parent);
		}
		else if (node == node->parent->left && node->parent == node->parent->parent->left) { 
			rotateL(node->parent->parent);
			rotateL(node->parent);
		}
		else if (node == node->parent->right && node->parent == node->parent->parent->right) {
			rotateR(node->parent->parent);
			rotateR(node->parent);
		}
		else if (node == node->parent->left && node->parent == node->parent->parent->right) {
			rotateL(node->parent);
			rotateR(node->parent);
		}
		else if (node == node->parent->right && node->parent == node->parent->parent->left) {
			rotateR(node->parent);
			rotateL(node->parent);
		}
	}
}

void BBST::insert(const int& x) {
	Node* node = new Node(x);
	Node* ascendant = NULL;
	Node* root = this->root;
	while (root != NULL) {
		ascendant = root;
		if (node->data < root->data) {
			root->descendants++;
			root = root->left;
		}
		else if (node->data > root->data) {
			root->descendants++;
			root = root->right;
		}
		else {
			root->value++;
			splay(root);
			return;
		}
	}
	node->parent = ascendant;
	if (ascendant == NULL) this->root = node;
	else if (node->data < ascendant->data) ascendant->left = node;
	else ascendant->right = node;
	splay(node);
}

void BBST::erase(const int& x) {
	Node* node = this->root;
	while (node != NULL) {
		
		if (x < node->data) node = node->left;
		else if (x > node->data) node = node->right;
		else break;
	}
	if (node == NULL) return;
	splay(node);
	if (node->value == 1) {
		BBST lt, rt; Node* pre;
		precursor(this->root, this->root->data, pre);
		lt.root = this->root->left;
		rt.root = this->root->right;
		if (lt.root != NULL) {
			lt.splay(pre);
			lt.root->right = rt.root;
			lt.root->descendants = countRank(lt.root->left) + countRank(lt.root->right);
			node = NULL; delete node;
			this->root = lt.root;
			root->parent = NULL;
			if(root->left != NULL) root->left->parent = root;
			if (root->right != NULL) root->right->parent = root;
		}
		else {
			node = NULL; delete node;
			this->root = rt.root;
			if (root != NULL) {
				root->parent = NULL;
			}
		}
	}
	else node->value--;
}

void BBST::find(Node*& node, const int& x, int& count) {
	
	if (node != NULL) {
		if (count > 0) {
			if (x > node->data) {
				count += (countRank(node->left) + node->value);
				find(node->right, x, count);
			}
			else if (x == node->data) {
				count += countRank(node->left);
				cout << count << endl;
				splay(node);
				count = -1;
			}
			else {
				find(node->left, x, count);
			}
		}
	}
}

void BBST::findxsmallest(Node*& node, const int& x, int& count) {
	
	if (count >= 0) {
		if (node != NULL) {
			if (count + countRank(node->left) + 1 <= x &&
				count + countRank(node->left) + node->value >= x) {
				cout << node->data << endl;
				splay(node);
				count = -1;
			}
			else if (count + countRank(node->left) + 1 < x) {
				count += (countRank(node->left) + node->value);
				findxsmallest(node->right, x, count);
			}
			else findxsmallest(node->left, x, count);
		}
	}
}

void BBST::precursor(Node*& node, const int& x, Node*& value) {
	
	if (node != NULL) {
		if (x > node->data) {
			value = node;
			precursor(node->right, x, value);
		}
		else precursor(node->left, x, value);
	}
}

void BBST::successor(Node*& node, const int& x, Node*& value) {
	
	if (node != NULL) {
		if (x < node->data) {
			value = node;
			successor(node->left, x, value);
		}
		else successor(node->right, x, value);
	}
}

int main() {
	BBST b;
	int n; cin >> n;
	for (int i = 0; i < n; i++) {
		int opt, x; cin >> opt >> x;
		if (opt == 1) {
			b.insert(x);
		}
		else if (opt == 2) {
			b.erase(x);
		}
		else if (opt == 3) {
			int count = 1;
			b.find(b.root, x, count);
		}
		else if (opt == 4) {
			int count = 0;
			b.findxsmallest(b.root, x, count);
		}
		else if (opt == 5) {
			Node* value = NULL;
			b.precursor(b.root, x, value);
			if (value != NULL) {
				cout << value->data << endl;
				b.splay(value);
			}
		}
		else if (opt == 6) {
			Node* value = NULL;
			b.successor(b.root, x, value);
			if (value != NULL) {
				cout << value->data << endl;
				b.splay(value);
			}
		}
	}
	return 0;
}
