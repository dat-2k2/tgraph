#include"config.h"
#include<iostream>
//TODO: Delete function
template <class T> struct Node
{
	using NType = Node<T>;
	//T class is an ordered set (derived <) 
	T _item;
	bool _color = 1;
	NType* _left = nullptr;
	NType* _right = nullptr;
	NType* _parent = nullptr;
	// 1 Red, 0 Black
};

template <class T>  void _swap_colors(Node<T>* x1, Node<T>* x2) {
	bool temp;
	temp = x1->_color;
	x1->_color = x2->_color;
	x2->_color = temp;
}

template <class T>  void _swap_values(Node<T>* u, Node<T>* v) {
	T temp;
	temp = u->_item;
	u->_item = v->_item;
	v->_item = temp;
}

template <class T>  bool _has_red_child(Node<T>* x) {
	if (x->_right)
		if (x->_right->_color == 1)
			return true;
	if (x->_left)
		if (x->_left->_color == 1)
			return true;
	return false;
}




//RedBlackTree to control the root node
// -------------------------------------
// 5 rules of RBT:
// 1. Each node is either red, either black 
// 2. ROOT is always black
// 3. 2 red nodes cannot be adjacent
// 4. Every paths from a node to each of its leaves must be equal in number of black nodes. 
// 5. Every leaves are black
// -------------------------------------
//max heap
template <class T>  struct RedBlackTree
{
	using NType = Node<T>;
	NType* Root = nullptr;
	// control the balance
private:
	bool ll = false;
	bool rr = false;
	bool lr = false;
	bool rl = false;
public: 
	~RedBlackTree() {
		if (Root) delete Root;
	}

private:

	NType* _rotate_right(NType* root) {
		NType* x = root->_left;
		root->_left = x->_right;
		if (x->_right != nullptr)
			x->_right->_parent = root;

		x->_parent = root->_parent;
		if (root->_parent == nullptr)
			Root = x;
		else if (root->_parent->_left == root)
			root->_parent->_left = x;
		else
			root->_parent->_right = x;
		x->_right = root;
		root->_parent = x;

		return x;
	}

	//parent -> left child of right child
	NType* _rotate_left(NType* root) {
		NType* y = root->_right;
		
		root->_right = y->_left;
		if (y->_left != nullptr)
			y->_left->_parent = root;

		y->_parent = root->_parent;
		if (root->_parent == nullptr)
			Root = y;
		else if (root->_parent->_left == root)
			root->_parent->_left = y;
		else
			root->_parent->_right = y;

		
		y->_left = root;
		root->_parent = y;

		
		return y;
	}

	NType* insertHelp(NType* root, T key) {
		//red red
		bool f = false;

		if (root == nullptr) {
			return new NType{ key, 1}; // RED Node
		}
		else if (key < root->_item) {
			root->_left = insertHelp(root->_left, key);
			root->_left->_parent = root;
			// root->_left = Node X
			// root       = X->_parent
			if (Root != root) {
				if (root->_color == root->_left->_color == 1)
					f = true;
			}
		}
		else if(key > root->_item) {
			root->_right = insertHelp(root->_right, key);
			root->_right->_parent = root;
			// root->_right = Node X
			// root        = X->_parent
			if (Root != root) {
				if (root->_color == root->_right->_color == 1)
					f = true;
			}
		}
		else
		{
#ifdef  APPEND
		root->_item += key;
#endif //  APPEND
		}

		// Case 1 : Left left
		if (ll) {
			root = _rotate_right(root);
			root->_color = 0;
			root->_right->_color = 1;
			ll = false;
		}
		// Case 2 : Right right
		else if (rr) {
			root = _rotate_left(root);
			root->_color = 0;
			root->_left->_color = 1;
			rr = false;
		}
		// Case 3 : Left right 
		else if (lr) {
			root->_left = _rotate_left(root->_left);
			root->_left->_parent = root;

			root = _rotate_right(root);
			root->_color = 0;
			root->_right->_color = 1;
			lr = false;
		}
		// Case 4 : Right left 
		else if (rl) {
			root->_right = _rotate_right(root->_right);
			root->_right->_parent = root;

			root = _rotate_left(root);
			root->_color = 0;
			root->_left->_color = 1;
			rl = false;
		}

		// RED RED
		if (f) {
			if (root->_parent->_right == root) {
				if (root->_parent->_left == nullptr || root->_parent->_left->_color == 0) {
					if (root->_left != nullptr && root->_left->_color == 1)
						rl = true;
					if (root->_right != nullptr && root->_right->_color == 1)
						rr = true;
				}
				else {
					root->_color = root->_parent->_left->_color = 0;
					if (root != Root)
						root->_parent->_color = 1;
				}
			}
			else {
				if (root->_parent->_right == nullptr || root->_parent->_right->_color == 0) {
					if (root->_left != nullptr && root->_left->_color == 1)
						ll = true;
					if (root->_right != nullptr && root->_right->_color == 1)
						lr = true;
				}
				else {
					root->_color = root->_parent->_right->_color = 0;
					if (root != Root)
						root->_parent->_color = 1;
				}
			}
			f = false;
		}
		return root;
	}

	NType* _max_value_node(NType* root) {
		NType* query = root;
		while (query->_right) {
			query = query->_right;
		}
		return query;
	}

	NType* sibling(NType* root) {
		if (!(root->_parent))
			return nullptr;

		if (root->_parent->_left == root)
			return root->_parent->_right;
		else 
			return root->_parent->_left;
	}

	void fixDoubleBlack(NType* root) {
		if (root == Root)
			return;
		NType* sib = sibling(root);
		NType* par = root->_parent;

		if (sib == nullptr)
			fixDoubleBlack(par);
		else {
			if (sib->_color == 1) {
				par->_color = 1;
				sib->_color = 0;

				if (sib->_parent->_left == sib)
					par = _rotate_right(par);
				else
					par = _rotate_left(par);

				fixDoubleBlack(root);
			}
			else {
				if (_has_red_child(sib)) {
					if (sib->_parent->_left == sib) {

						if (sib->_left != nullptr && sib->_left->_color) {
							sib->_left->_color = sib->_color;
							sib->_color = par->_color;
							par->_color = 0;

							par = _rotate_right(par);
						}
						else {

							sib->_right->_color = par->_color;
							par->_color = 0;
							sib = _rotate_left(sib);
							par = _rotate_right(par);
						}
					}
					else {

						if (sib->_right != nullptr && sib->_right->_color) {
							sib->_right->_color = sib->_color;
							sib->_color = par->_color;
							par->_color = 0;

							par = _rotate_left(par);
						}
						else {

							sib->_left->_color = par->_color;
							par->_color = 0;
							sib = _rotate_right(sib);
							par = _rotate_left(par);
						}
					}
				}
				else {
					sib->_color = 1;
					if (par->_color == 0)
						fixDoubleBlack(par);
					else
						par->_color = 0;
				}
			}
		}

	}

	void _delete(NType* vDelete) {
		NType* uReplace;

		if ((vDelete->_left != nullptr) && (vDelete->_right != nullptr))
			uReplace = _max_value_node(vDelete->_left);
		else if (vDelete->_left != nullptr)
			uReplace = vDelete->_left;
		else if (vDelete->_right != nullptr)
			uReplace = vDelete->_right;
		else
			uReplace = nullptr;

		bool uvBlack = ((uReplace == nullptr) || (uReplace->_color == 0)) && (vDelete->_color == 0);
		NType* par = vDelete->_parent;
		NType* sib = sibling(vDelete);
		if (uReplace == nullptr) {
			if (vDelete == Root) {
				Root = nullptr;
			}
			else {
				if (uvBlack)
					fixDoubleBlack(vDelete);
				else if (sib != nullptr)
					sib->_color = 1;

				if (vDelete->_parent->_left == vDelete)
					par->_left = nullptr;
				else
					par->_right = nullptr;
			}
			delete vDelete;
			return;
		}

		if (vDelete->_left == nullptr || vDelete->_right == nullptr) {
			if (vDelete == Root) {
				vDelete->_item = uReplace->_item;
				vDelete->_left = vDelete->_right = nullptr;
				delete uReplace;
			}
			else {
				if (vDelete->_parent->_left == vDelete)
					par->_left = uReplace;
				else
					par->_right = uReplace;
				delete vDelete;

				uReplace->_parent = par;
				if (uvBlack)
					fixDoubleBlack(uReplace);
				else
					uReplace->_color = 0;
			}
			return;
		}
		_swap_values(uReplace, vDelete);
		_delete(uReplace);
	}

public:
	//insert new items
	void Insert(T key) {
		if (Root == nullptr) {
			Root = new NType{ key, 0 };
		}
		else {
			Root = insertHelp(Root, key);
			if (Root->_color == 1)
				Root->_color = 0;
		}
	}

	//search for item
	NType* Search(T item) {
		NType *temp = Root;
		while (temp) {
			if (item < temp->_item) {
				if (! temp->_left)
					return nullptr;
				else
					temp = temp->_left;
			}
			else if (item == temp->_item) {
				break;
			}
			else {
				if (! temp->_right)
					return nullptr;
				else
					temp = temp->_right;
			}
		}
		return temp;
	}

	//delete item
	int Delete(T item) {
		NType* vDelete = Search(item);
		if (!vDelete) {
			return -1;
		}
		else {
			_delete(vDelete);
		}
		return 0;
	}

	void Clear() {
		if (Root) delete Root;
		Root = nullptr;
		ll = false;
		rr = false;
		lr = false;
		rl = false;
	}
private:
	void postorder(NType* root, int space) {
		if (!root) {
			space += 10;
			std::cout << '\n';
			for (int i = 10; i < space; i++)
				std::cout << " ";
			std::cout << "NIL" << " (Ч)"<< "\n";
			return;
		}
		space += 10;

		postorder(root->_right, space);
		std::cout << '\n';

		for (int i = 10; i < space; i++)
			std::cout << " ";
		std::cout << root->_item << " (" << (root->_color ? "К" : "Ч") << ") " << "\n";
		postorder(root->_left, space);
	}


public:
	void visual() {
		if (!Root) {
			std::cout << "Пустое\n";
			return;
		}
		postorder(Root, 0);
	}

	void printInorder(NType* n) {
		if (!n) {
			return;
		}
		printInorder(n->_left);
		std::cout << n->_item << " ";
		printInorder(n->_right);
	}


};



template <class T> void printNodeInfo(Node<T>* n) {
	if (n) {
		std::cout<< "Ключ в дерево: " << n->_item <<'\n';
		
		std::cout << "Родитель: ";
		if (n->_parent)
			std::cout << n->_parent->_item;
		else
			std::cout << "Нуль";
		std::cout << '\n';
		std::cout << "Цвет узлы: ";
		if (n->_color)
			std::cout << "красный";
		else
			std::cout << "черный ";
		std::cout << '\n';
		std::cout << "Левый ребенок: ";
		if (n->_left)
			std::cout << n->_left->_item;
		else
			std::cout << "Нуль ";
		std::cout << '\n';
		std::cout << "Правый ребенок: ";
		if (n->_right)
			std::cout << n->_right->_item;
		else
			std::cout << "Нуль ";
		std::cout << '\n';
	}
	else
		printf("Нуль\n");
}