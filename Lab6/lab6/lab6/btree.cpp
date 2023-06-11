#include"config.h"
template <class T> struct BNode {
	std::vector<T> _item; //key is _item[0]
	BNode<T>* _parent = nullptr;
	std::vector<BNode*> _children = {}; //size _children = size _item+1 if internal, or else = 0
	BNode<T>* _prev = nullptr;
	BNode<T>* _next = nullptr;
	bool _isLeaf = true;
	int find_index(T key) {
		for (int i = 0; i < _item.size(); i++) {
			if (_item[i] == key) {
				return i;
			}
		}
		return -1;
	}
	int find_index_child(T key) {
		for (int i = 0; i < _item.size(); i++) {
			if (key < _item[i]) {
				return i;
			}
		}
		return _item.size();
	}
};


//find position of the first item in arr which is larger or equal to key
template<class T> int _find_pos_insert(std::vector<T> arr, T key) {
	int pos = 0;
	while (pos < arr.size()) {
		if (arr[pos] >= key)
			return pos;
		pos++;
	}
	return pos;
}

//insert key to the position pos of arr (the new item will at pos after insertion)
template <class T> void _insert_to_pos(std::vector<T>& arr, int pos, T key) {
	if (pos >= arr.size()) {
		arr.push_back(key);
	}
	else {
		//insert
		arr.push_back(arr.back());
		for (int j = arr.size() - 2; j > pos; j--) {
			arr[j] = arr[j - 1];
		}
		arr[pos] = key;
	}
}

//insert to an ascending array
template <class T> void _insert_to_sorted_array(std::vector<T>& arr, T key) {
	_insert_to_pos(arr, _find_pos_insert(arr, key), key);
}


template <class T> void printNodeInfo(BNode<T>* node) {
	if (node == nullptr) {
		std::cout << "Нуль\n";
	}
	else {
		if (node->_children.size()) {
			std::cout << "Внутренний узел\n";
		}
		else
			std::cout << "Листовый узел\n";
		std::cout << "Значении: ";
		for (T x : node->_item)
			std::cout << x << " ";
		std::cout << '\n';

		std::cout << "Родитель: ";
		if (node->_parent) {
			for (T x : node->_parent->_item)
				std::cout << x << " ";
		}
		else {
			std::cout << "Нуль";
		}
		std::cout << '\n';
		std::cout << "Предыдущий: ";
		if (node->_prev) {
			for (T x : node->_prev->_item)
				std::cout << x << " ";
		}
		else {
			std::cout << "Нуль";
		}
		std::cout << '\n';
		std::cout << "Следующий: ";
		if (node->_next) {
			for (T x : node->_next->_item)
				std::cout << x << " ";
		}
		else {
			std::cout << "Нуль";
		}
		std::cout << '\n';
	}
}

template <class T> void printTree(BNode<T>* node, int space) {
	if (!node) {
		return;
	}
	else {
		space += 10;
		int j = node->_children.size() - 1;
		while (j >= ceil(float(node->_children.size()) / 2) && j > 0) {
			printTree(node->_children[j], space);
			std::cout << '\n';
			j--;
		}

		int i = node->_item.size();
		while (i--) {
			for (int t = 0; t < space; t++) {
				std::cout << " ";
			}
			std::cout << node->_item[i]<<'\n';
		}

		std::cout << '\n';
		while (j >= 0) {
			printTree(node->_children[j], space);
			std::cout << '\n';
			j--;
		}
	}
}

template <class T> struct BTree {
	using NType = BNode<T>;
	NType* Root = nullptr;
	unsigned int _ary = 4;
	unsigned int _min_capac = (_ary % 2) ? (_ary / 2) : ((_ary + 1) / 2);
	unsigned int height = -1;
private:
	void _split_leaf(NType* leaf) {
		// find pos
		int x = leaf->_item.size();
		int pos = (x % 2) ? x / 2 : (x - 1) / 2;

		//split the leaf
		std::vector<T> new_item(leaf->_item.begin() + pos, leaf->_item.end());
		int s = leaf->_item.size();
		while (leaf->_item.size() > pos) {
			leaf->_item.pop_back();
		}

		//now the initial leaf is splitted to 2 nodes;
		//assign to parent;
		NType* par = leaf->_parent;
		if (par == nullptr) { //root
			par = new NType;
			Root = par;
			par->_isLeaf = false;
			par->_children.push_back(leaf);
			leaf->_parent = par;
			height++;
		}

		NType* new_child = new NType{ new_item, par, {}, leaf , leaf->_next };
		leaf->_next = new_child;
		_insert_to_sorted_array(par->_item, new_item[0]);

		//find pos of child
		int f = 0;
		for (f = 0; f < par->_children.size(); f++) {
			if (par->_children[f] == leaf) {
				break;
			}
		}
		_insert_to_pos(par->_children, f + 1, new_child);

		if (par->_item.size() >= _ary) {
			_split_internal(par);
		}

	}

	void _split_internal(NType* internal) {

		// find pos
		int x = internal->_item.size();
		int pos = (x % 2) ? x / 2 : (x - 1) / 2;

		//split the internal.Note that the internal has children size more than 1 to the items size.
		std::vector<T> new_item(internal->_item.begin() + pos + 1, internal->_item.end());
		std::vector<NType*> new_children(internal->_children.begin() + pos + 1, internal->_children.end());


		//now the initial internal is splitted to 2 nodes;
		//assign to parent;
		NType* par = internal->_parent;
		if (par == nullptr) {//internal = root
			par = new NType;
			Root = par;
			par->_isLeaf = false;
			par->_children.push_back(internal);
			internal->_parent = par;
			height++;
		}

		_insert_to_sorted_array(par->_item, internal->_item[pos]);

		NType* new_child = new NType{ new_item, par, new_children, nullptr, nullptr, false };

		while (internal->_children.size() > pos + 1) {
			internal->_children.pop_back();
		}
		while (internal->_item.size() > pos) {
			internal->_item.pop_back();
		}

		//set parent!! this is INTERNAL

		for (NType* child : new_children) {
			child->_parent = new_child;
		}

		//insert pointer to new right child to parent of internal
		//find pos of child
		int f = 0;
		for (f = 0; f < par->_children.size(); f++) {
			if (par->_children[f] == internal) {
				break;
			}
		}
		_insert_to_pos(par->_children, f + 1, new_child);
		if (par->_item.size() >= _ary) {
			_split_internal(par);
		}
	}


	void _insert(T key) {
		if (!Root) {
			Root = new NType{ {key} };
			height++;
		}
		else {
			NType* query = Root;
			int pos_child = 0;// index of the current node in its parent's children list
			while (!query->_isLeaf) {
				int pos_tmp = 0;
				//first item which > key
				while (pos_tmp < query->_item.size()) {
					if (query->_item[pos_tmp] > key)
						break;
					pos_tmp++;
				}
#ifdef DEBUG
				std::cout << "Pos: " << pos_tmp << '\n';
#endif // DEBUG				
				// found the right branch, trace down
				query = query->_children[pos_tmp];
				pos_child = pos_tmp;
			}

			//query now is a leaf. Start inserting
			_insert_to_sorted_array(query->_item, key);
			//if inserted to head of node, then the key is changed. Need to update.
			if (query->_item[0] == key && query->_parent) {
				NType* par = query->_parent;

				;				par->_item[pos_child] = key;
			}
			//if overflow then resolve
			if (query->_item.size() == _ary) {
				_split_leaf(query);
			}
		}
	}
	NType* _search(T key) {
		NType* query = Root;
		if (!query)
			return nullptr;
		while (!query->_isLeaf) {
			int pos_tmp = 0;
			while (pos_tmp < query->_item.size()) {
				if (query->_item[pos_tmp] > key)
					break;
				pos_tmp++;
			}
			// found the right branch, trace down
			query = query->_children[pos_tmp];
		}

		for (T cand : query->_item) {
			if (cand == key)
				return query;
		}
		return nullptr;
	};


	//delete function for leaf node
	void _delete_leaf(T key, NType* node) {
		//find the index of key
		int index = node->find_index(key);
		//delete the key
		node->_item.erase(node->_item.begin() + index);
		if (node->_parent) {
			int index_in_parent = node->_parent->find_index_child(key);
			if (index_in_parent)
				node->_parent->_item[index_in_parent - 1] = node->_item.front();
		}
#ifdef DEBUG
		printTree(Root, 1);
#endif // DEBUG
	}

	//delete function for internal node
	void _delete_internal(T key, NType* node) {
		int index = node->find_index(key);
		if (index != -1) {
			NType* leftmost_leaf = node->_children[index + 1];
			//find leftmost leaf
			while (!leftmost_leaf->_isLeaf)
				leftmost_leaf = leftmost_leaf->_children.front();
			node->_item[index] = leftmost_leaf->_item.front();
#ifdef DEBUG
			printTree(Root, 1);
#endif // DEBUG
		}
	}

	//find and move new key to deleted item's leaf, from the right side
	void _borrow_key_from_right_leaf(NType* node, NType* next) {

		//the key is its successive leaf
		node->_item.push_back(next->_item.front());
		next->_item.erase(next->_item.begin());

		for (int i = 0; i < node->_parent->_children.size(); i++) {
			if (node->_parent->_children[i] == next) {
				node->_parent->_item[i - 1] = next->_item.front();
				break;
			}
		}
	}

	//find and move new key to deleted item's leaf, from the left side
	void _borrow_key_from_left_leaf(NType* node, NType* prev) {

		//the key is its predecessive leaf
		node->_item.insert(node->_item.begin(), prev->_item.back());
		prev->_item.erase(prev->_item.end() - 1);

		for (int i = 0; i < node->_parent->_children.size(); i++) {
			if (node->_parent->_children[i] == node) {
				node->_parent->_item[i - 1] = node->_item.front();
				break;
			}
		}
	}

	//merge leaf with its right sibling
	void _merge_with_right_leaf(NType* node, NType* next) {

		node->_item.insert(node->_item.end(), next->_item.begin(), next->_item.end());
		node->_next = next->_next;
		if (node->_next) node->_next->_prev = node;

		for (int i = 0; i < next->_parent->_children.size(); i++) {
			if (node->_parent->_children[i] == next) {
				node->_parent->_item.erase(node->_parent->_item.begin() + i - 1);
				node->_parent->_children.erase(node->_parent->_children.begin() + i);

				break;
			}
		}
	}

	//merge leaf with its left sibling
	void _merge_with_left_leaf(NType* node, NType* prev) {

		prev->_item.insert(prev->_item.end(), node->_item.begin(), node->_item.end());
		prev->_next = node->_next;
		if (prev->_next)
			prev->_next->_prev = prev;

		for (int i = 0; i < node->_parent->_children.size(); i++) {
			if (node->_parent->_children[i] == node) {
				node->_parent->_item.erase(node->_parent->_item.begin() + i - 1);
				node->_parent->_children.erase(node->_parent->_children.begin() + i);

				break;
			}
		}
	}


	//find and move new key to deleted item's internal, from the left side
	void _borrow_key_from_right_internal(int posInPar, NType* node, NType* next) {

		node->_item.insert(node->_item.end(), node->_parent->_item[posInPar]);
		node->_parent->_item[posInPar] = next->_item.front();
		next->_item.erase(next->_item.begin());

		node->_children.insert(node->_children.end(), next->_children.front());
		next->_children.erase(next->_children.begin());
		node->_children.back()->_parent = node;
	}

	//find and move new key to deleted item's internal, from the left side
	void _borrow_key_from_left_internal(int posInPar, NType* node, NType* prev) {

		node->_item.insert(node->_item.begin(), node->_parent->_item[posInPar - 1]);
		node->_parent->_item[posInPar - 1] = prev->_item.back();
		prev->_item.erase(prev->_item.end() - 1);

		node->_children.insert(node->_children.begin(), prev->_children.back());
		prev->_children.erase(prev->_children.end() - 1);
		node->_children.front()->_parent = node;
	}

	//merge internal with its right sibling
	void _merge_with_right_internal(int posInPar, NType* node, NType* next) {

		node->_item.insert(node->_item.end(), node->_parent->_item[posInPar]);
		node->_parent->_item.erase(node->_parent->_item.begin() + posInPar);
		node->_parent->_children.erase(node->_parent->_children.begin() + posInPar + 1);

		//modify the new created node
		node->_item.insert(node->_item.end(), next->_item.begin(), next->_item.end());
		node->_children.insert(node->_children.end(), next->_children.begin(), next->_children.end());
		for (NType* child : node->_children) {
			child->_parent = node;
		}
	}

	//merge internal with its left sibling
	void _merge_with_left_internal(int posInPar, NType* node, NType* prev) {

		prev->_item.insert(prev->_item.end(), node->_parent->_item[posInPar - 1]);
		node->_parent->_item.erase(node->_parent->_item.begin() + posInPar - 1);
		node->_parent->_children.erase(node->_parent->_children.begin() + posInPar);

		//modify the new created node
		prev->_item.insert(prev->_item.end(), node->_item.begin(), node->_item.end());
		prev->_children.insert(prev->_children.end(), node->_children.begin(), node->_children.end());
		for (NType* child : prev->_children) {
			child->_parent = prev;
		}
	}

	void _delete(T key, NType*& node) {
		if (node->_isLeaf) {
			_delete_leaf(key, node);
		}
		else {
			_delete_internal(key, node);
		}

		if (node->_item.size() < _min_capac) {
			if (node == Root) {
				if (Root->_item.empty() && !Root->_children.empty()) {
					Root = Root->_children[0];
					Root->_parent = nullptr;
					height--;
				}
				return;
			}

			else if (node->_isLeaf) {
				NType* next = node->_next;
				NType* prev = node->_prev;

				if (next && next->_parent == node->_parent && next->_item.size() > _min_capac) {
					_borrow_key_from_right_leaf(node, next);
				}
				else if (prev && prev->_parent == node->_parent && prev->_item.size() > _min_capac) {
					_borrow_key_from_left_leaf(node, prev);
				}

				//min capac = ceil(ary/2)-1 -> min capac + min capac - 1 <= ary - 1-> valid
				else if (next && next->_parent == node->_parent && next->_item.size() <= _min_capac) {
					_merge_with_right_leaf(node, next);
				}
				else if (prev && prev->_parent == node->_parent && prev->_item.size() <= _min_capac) {
					_merge_with_left_leaf(node, prev);
				}
			}
			else {
				int posInPar = -1;

				//find index in parent
				for (int i = 0; i < node->_parent->_children.size(); i++) {
					if (node->_parent->_children[i] == node) {
						posInPar = i;
						break;
					}
				}

				NType* next;
				NType* prev;

				if (node->_parent->_children.size() > posInPar + 1) {
					next = node->_parent->_children[posInPar + 1];
				}
				else {
					next = nullptr;
				}

				if (posInPar) {
					prev = node->_parent->_children[posInPar - 1];
				}
				else {
					prev = nullptr;
				}

				if (next && next->_parent == node->_parent && next->_item.size() > _min_capac) {
					_borrow_key_from_right_internal(posInPar, node, next);
				}

				else if (prev && prev->_parent == node->_parent && prev->_item.size() > _min_capac) {
					_borrow_key_from_left_internal(posInPar, node, prev);
				}

				//min capac = ceil(ary/2)-1 -> min capac + min capac - 1 <= ary - 1-> valid
				else if (next && next->_parent == node->_parent && next->_item.size() <= _min_capac) {
					_merge_with_right_internal(posInPar, node, next);
				}

				else if (prev && prev->_parent == node->_parent && prev->_item.size() <= _min_capac) {
					_merge_with_left_internal(posInPar, node, prev);
				}
			}
		}
#ifdef DEBUG
		printTree(Root, 1);
#endif // DEBUG
		if (node->_parent) {
			_delete(key, node->_parent);
		}
	}

public:
	int remove(T key) {
		NType* node_contain = _search(key);
		if (node_contain) {
			_delete(key, node_contain);
			return 0;
		}
		else
			return -1;
	}

	NType* search(T key) {
		return _search(key);
	}

	void insert(T key) {
		_insert(key);
	}

	void _clear_node(NType* n) {
		if (n)
			for (NType* x : n->_children)
				if (x)
					_clear_node(x);
	}
	void clear() {
		if (Root) _clear_node(Root);
		Root = nullptr;
		height = -1;
	}
	void visual() {
		if (!Root) {
			printf("Пустое!\n");
		}
		else {
			printTree(Root, 2);
		}
	}
	void printInOrder() {
		NType* tmp = Root;
		while (!tmp->_isLeaf) {
			tmp = tmp->_children[0];
		}
		while (tmp) {
			for (auto item : tmp->_item) {
				std::cout << item <<" ";
			}
			if (tmp->_next)
				std::cout << "->";
			tmp = tmp->_next;
		}
		std::cout << "\n\n";
	}
};

