

#include"config.h"
//TODO: Delete function
template <class T> struct Node
{
	//T class is an ordered set (derived <) 
	T* item;
	bool color;
	Node<T>* left;
	Node<T>* right;
	Node<T>* parent;

	~Node() {
		try {
			printf("delete %s: \n", item->key.c_str());
		}
		catch (...) {};
		if (item) {
			delete item;
			item = nullptr;
		}
		if (left) {
			delete left;
			left = nullptr;
		}
		if (right) {
			delete right;
			right = nullptr;
		}
	}
	// 1 Red, 0 Black
};

template <class T>  void swapColors(Node<T>* x1, Node<T>* x2) {
	bool temp;
	temp = x1->color;
	x1->color = x2->color;
	x2->color = temp;
}

template <class T>  void swapValues(Node<T>* u, Node<T>* v) {
	T* temp;
	temp = u->item;
	u->item = v->item;
	v->item = temp;
}

template <class T>  bool hasRedChild(Node<T>* x) {
	if (x->right)
		if (x->right->color == 1)
			return true;
	if (x->left)
		if (x->left->color == 1)
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
	Node<T>* Root = nullptr;
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

// rotate when tree is unbalanced
// rotate is not really rotation. It's like chop the tree head then graft it to a branch
// return the new node which is replaced into the place (x node)
// ROOT IS NOT nullptr

//Start right rotate
//destroy the only edge connecting root with and set x as new root
//the old root become the right leaf of x
// the old right leaf of x becomes the left leaf of root
//    root                          x
//	/     \              =>       /   \ 
// x     root's right           ...   root
//   \                               /    \ 
//     x's right                x's right  root's right

	Node<T>* rightRotate(Node<T>* root) {
		Node<T>* x = root->left;
		root->left = x->right;

		// for the red black tree, as parent is included, this attrib
		// must be considered at every modification.
		// x->right IS NOT X!!
		if (x->right)
			(x->right)->parent = root;

		// root to x.right
		x->parent = root->parent;
		if (!(root->parent)) // ROOT
			Root = x; // new root update
		//update children of parent node
		else if (root->parent->left == root)
			root->parent->left = x;
		else
			root->parent->right = x;

		x->right = root;
		root->parent = x;

		return x;
	}


	// ROOT IS NOT nullptr
	//         root                           x
	//	    /       \              =>       /   \ 
	// root's left   x           ...   root     x's right
	//              /  \                /   \    
	//        x's left  x's right      root's left  x's left

	Node<T>* leftRotate(Node<T>* root) {

		Node<T>* x = root->right;
		root->right = x->left;

		if (x->left)
			x->left->parent = root;

		x->parent = root->parent;

		if (!(root->parent)) // ROOT
			Root = x; // new root update
		//update children of parent node
		else if (root->parent->left == root)
			root->parent->left = x;
		else
			root->parent->right = x;

		x->left = root;
		root->parent = x;

		return x;
	}



	// the insertHelp function doesn't consider to color of the parent node
	// it may lead to the RED CONFLICT. Need to consider later

	/*insert to tree with root with item item
	input: root, item
	output: the root node (also the tree) which was modified for the item*/
	Node<T>* insertHelp(Node<T>* root, T itemInsert) {
		bool red_conflict = false;

		if (!root)
			// always add red node
			return new Node<T>{ new T(itemInsert) };

		//if the root has higher order than item then bring the item to the left child tree
		else if (itemInsert < *(root->item)) {
			//Node X: root->_ left 
			root->left = insertHelp(root->left, itemInsert);
			root->left->parent = root;

			if (Root != root) {
				if (root->color == root->left->color == 1) {
					// same color, raise error
					red_conflict = true;
				}
			}
		}
		//or else bring it to the right one
		else if (itemInsert > *(root->item)){
			// Node X: root ->_ right
			root->right = insertHelp(root->right, itemInsert);
			root->right->parent = root;

			if (Root != root) {
				if (root->color == root->right->color == 1) {
					// same color, raise error
					red_conflict = true;
				}
			}
		}
		else {
#ifdef APPEND
			*(root->item) += itemInsert;
#endif // APPEND

		}

		//handle the red conflict
		// this routine must be placed here, so it only starts from 2nd iteration,
		// when the grandpa was defined
		if (ll) {
			//left left
			//     Black GP                    Black P
			//    /        \                   /     \
			//   Red P    Black U    =>     Red X    Red GP
			//  /                                      \
			// Red X                                  Black U
			// Rotate right GrandPa
			root = rightRotate(root);
			// P <- Black
			root->color = 0;
			// GP <- Red 
			root->right->color = 1;

			ll = false;
		}
		else if (lr) {
			//left right
			//     Black GP                    Black GP
			//    /        \                   /     \
			//   Red P    Black U    =>     Red X    Black U  => LL case
			//       \                      /     
			//       Red X                Red P        
			// Rotate left P
			root->left = leftRotate(root->left);
			root->left->parent = root;
			//ll case
			// Rotate right GrandPa
			root = rightRotate(root);
			// P <- Black
			root->color = 0;
			// GP <- Red 
			root->right->color = 1;

			lr = false;
		}
		else if (rr) {
			//left right
			//     Black GP                    Black P
			//    /        \                   /     \
			// Black U    Red P    =>		Red GP    Red X
			//                \             /     
			//               Red X       Black U 

			//rotate left GP
			root = leftRotate(root);
			// P is black
			root->color = 0;
			// GP is red
			root->left->color = 1;

			rr = false;
		}
		else if (rl) {
			//left right
			//     Black GP                    Black GP
			//    /        \                   /     \
			// Black U    Red P    =>		Black U  Red X        => RR
			//            /                             \
			//          Red X                           Red P

			// rotate right P
			root->right = rightRotate(root->right);
			//rr case
			//rotate left GP
			root = leftRotate(root);
			// P is black
			root->color = 0;
			// GP is red
			root->left->color = 1;

			rl = false;
		}

		// check whether is ll, lr, rr or rl
		if (red_conflict) {
			// check if the root is right or left of its parent
			if (root->parent->right == root) {
				if (!(root->parent->left && root->parent->left->color)) {
					//no uncle or uncle = black
					if (root->left && root->left->color == 1)
						rl = true;
					if (root->right && root->right->color == 1)
						rr = true;
				}
				else {
					// red p red u
					// black u
					root->color = root->parent->left->color = 0;
					// red gp
					if (root != Root)
						root->parent->color = 1;
				}
			}
			else {
				if (root->parent->right == nullptr || root->parent->right->color == 0) {
					//no uncle or uncle = black
					if (root->left && root->left->color == 1)
						rl = true;
					if (root->right && root->right->color == 1)
						rr = true;
				}
				else {
					// red p red u
					// black u
					root->color = root->parent->right->color = 0;
					// red gp
					if (root != Root)
						root->parent->color = 1;
				}
			}
			red_conflict = false;
		}

		return root;
	}

	Node<T>* maxValueNode(Node<T>* root) {
		Node<T>* current = root;
		while (current) {
			current = current->right;
		}
		return current;
	}

	Node<T>* sibling(Node<T>* root) {
		if (!(root->parent))
			return nullptr;

		if (root->parent->left == root)
			return root->parent->right;
		else 
			return root->parent->left;
	}

	void fixDoubleBlack(Node<T>* root) {
		if (root == Root)
			return;
		Node<T>* sib = sibling(root);
		Node<T>* par = root->parent;

		//no sibling
		if (!sib)
			//
			fixDoubleBlack(par);
		else {
			//
			if (sib->color == 1) {
				par->color = 1;
				sib->color = 0;

				if (sib->parent->left == sib)
					par = rightRotate(par);
				else
					par = leftRotate(par);

				fixDoubleBlack(root);
			}
			else {
				if (hasRedChild(sib)) {
					// 
					if (sib->parent->left == sib) {
						if (sib->left && sib->left->color) {
							sib->left->color = sib->color;
							sib->color = par->color;
							par->color = 0;

							par = rightRotate(par);
						}
						else {
							sib->right->color = par->color;
							par->color = 0;
							sib = leftRotate(sib);
							par = rightRotate(par);
						}
					}
					else {
						if (sib->right && sib->right->color) {
							sib->right->color = sib->color;
							sib->color = par->color;
							par->color = 0;

							par = leftRotate(par);
						}
						else {
							sib->left->color = par->color;
							par->color = 0;
							sib = rightRotate(sib);
							par = leftRotate(par);
						}
					}
				}
				else {
					sib->color = 1;
					if (par->color == 0)
						fixDoubleBlack(par);
					else
						par->color = 0;
				}
			}
		}
	}

	void deleteNode(Node<T>* vDelete) {
		Node<T> *uReplace;

		if (vDelete->left && vDelete->right)
			uReplace = maxValueNode(vDelete->left);
		else if (vDelete->left)
			uReplace = vDelete->left;
		else if (vDelete->right)
			uReplace = vDelete->right;
		else
			uReplace = nullptr;

		bool uvBlack = (!(uReplace && uReplace->color) && !(vDelete->color));
		Node<T> *par = vDelete->parent;
		Node<T> *sib = sibling(vDelete);
		if (uReplace == nullptr) {
			if (vDelete == Root) {
				Root = nullptr;
			}
			else {
				if (uvBlack)
					fixDoubleBlack(vDelete);
				else if (sib)
					sib->color = 1;

				if (vDelete->parent->left == vDelete)
					par->left = nullptr;
				else
					par->right = nullptr;
			}
			delete vDelete;
			return;
		}

		if (!(vDelete->left && vDelete->right)) {
			if (vDelete == Root) {
				vDelete->item = uReplace->item;
				vDelete->left = vDelete->right = nullptr;
				delete uReplace;
			}
			else {
				if (vDelete->parent->left == vDelete)
					par->left = uReplace;
				else
					par->right = uReplace;
				delete vDelete;

				uReplace->parent = par;
				if (uvBlack)
					fixDoubleBlack(uReplace);
				else
					uReplace->color = 0;
			}
			return;
		}
		swapValues(uReplace, vDelete);
		deleteNode(uReplace);
	}

public:
	//insert new items
	void Insert(T item) {
		if (!Root)
			//Root always be black (r1)
			Root = new Node<T>{ new T(item), 0 };
		else {
			Root = insertHelp(Root, item);
			//change the color (r1)
			if (Root->color)
				Root->color = 0;
		}
	}

	//search for item
	Node<T>* Search(T item) {
		Node<T> *temp = Root;
		while (temp) {
			if (item < *(temp->item)) {
				if (! temp->left)
					return nullptr;
				else
					temp = temp->left;
			}
			else if (item == *(temp->item)) {
				break;
			}
			else {
				if (! temp->right)
					return nullptr;
				else
					temp = temp->right;
			}
		}
		return temp;
	}

	//delete item
	int Delete(T item) {
		Node<T>* vDelete = Search(item);
		if (!vDelete) {
			return -1;
		}
		else {
			deleteNode(vDelete);
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
};




