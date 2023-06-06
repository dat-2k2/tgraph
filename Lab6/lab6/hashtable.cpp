#include<vector>
#include<string>
// Linked List
//template <typename T> struct node
//{
//    T* _data = nullptr;
//    node* _next = nullptr;
//    node(T data) {
//        _data = new T(data);
//    }
//    ~node(){
//        if (_next) delete _next;
//    }
//};

template <class T> struct HashItem {
    std::string _key;
	T* _value = nullptr;
    HashItem* _next = nullptr;
    ~HashItem() {
        if (_value) delete _value;
        if (_next) delete _next;
    }
};

template <class T> struct HashTable
{
    //typedef HashItem<T> HashItem<T>;
    int _size = 0;
    int _capacity = 1;
    HashItem<T>** _arr = new HashItem<T>*[_capacity] { nullptr};
    HashTable(){
        _arr[0] = nullptr;
    };
    ~HashTable() {
        if (_arr) {
            for (int i = 0; i < _size; i++) {
                delete _arr[i];
            }
            delete _arr;
        }
    };
        

private:
    //hash the key
    int hashFunction(std::string key)
    {
        int bucketIndex;
        long long sum = 0, factor = 31;
        for (int i = 0; i < key.size(); i++)
        {
            sum = ((sum % _capacity) + ((int(key[i])) * factor) % _capacity) % _capacity;
            factor = ((factor % INT16_MAX) * (31 % INT16_MAX)) % INT16_MAX;
        }

        bucketIndex = sum;
        return bucketIndex;
    }

    float getLoadFactor()
    {
        // number of elements in hash table / total numbers of buckets
        return (float)(_size) / (float)(_capacity);
    }

    void rehashing()
    {
        int oldCapacity = _capacity;
        HashItem<T>** temp = _arr; 

        //request more memory
        _capacity = oldCapacity * 2; 
        _arr = new HashItem<T> *[_capacity];

        for (int i = 0; i < _capacity; i++){
            _arr[i] = NULL;
        }
        //copy data to the new list
        for (int i = 0; i < oldCapacity; i++) 
        {
            HashItem<T>* currBucketHead = temp[i];
            while (currBucketHead)
            {
                //insert, which now doesn't call to rehash
                Insert(currBucketHead->_key, *(currBucketHead->_value));
                currBucketHead = currBucketHead->_next;
            }
        }
        //delete old array
        delete[] temp;
        return;
    }
public:
    int Insert(std::string key, T value)
    {
        while (getLoadFactor() > 0.5f) // when load factor > 0.5
        {
            rehashing(); //request more memory for new key
        }

        int bucketIndex = hashFunction(key);
        if (!_arr[bucketIndex]) // when there is no linked list at bucket
        {
            HashItem<T>* newNode = new HashItem<T>{key, new T(value)};
            _arr[bucketIndex] = newNode;
        }
        else // adding at the head of current linked list
        {
            // check if key already exist:
            HashItem<T>* tmpNode = _arr[bucketIndex];
            while (tmpNode) {
                if (tmpNode->_key == key) {
                    return 1;
                }
                tmpNode = tmpNode->_next;
            }
            HashItem<T>* newNode = new HashItem<T>{key, new T(value)};
            newNode->_next = _arr[bucketIndex];
            _arr[bucketIndex] = newNode;
        }
        return 0;
    }

    T* Search(std::string key)
    {
        int bucketIndex = hashFunction(key); // getting bucket index
        HashItem<T>* bucketHead = _arr[bucketIndex];
        while (bucketHead) // searching in the linked list which is present at bucket for given key
        {
            if (bucketHead->_key == key)
            {
                return new T(*(bucketHead->_value));
            }
            bucketHead = bucketHead->_next; // moving to next node in linked list
        }
        return nullptr;
    }

    int Delete(std::string key) {
        int bucketIndex = hashFunction(key); // getting bucket index
        HashItem<T>* bucketHead = _arr[bucketIndex];
        HashItem<T>* bucketPrev = nullptr;
        while (bucketHead) // searching in the linked list which is present at bucket for given key
        {
            if (bucketHead->_key == key) //found
            {
                if (bucketPrev) {
                    bucketPrev->_next = bucketHead->_next;
                }
                else
                    _arr[bucketIndex] = nullptr;

                bucketHead->_next = nullptr;
                delete bucketHead;
                return 0;
            }
            bucketHead = bucketHead->_next; // moving to next node in linked list
            bucketPrev = bucketHead;
        }
        return 1;
    }

    void Clear() {
        for (int i = 0; i < _capacity; i++) {
            if (_arr[i]) delete _arr[i];
        }
        delete[] _arr;
        int _size = 0;
        int _capacity = 1;
        _arr = new HashItem<T>*[_capacity] { nullptr};
        _arr[0] = nullptr;
    }
};


//template <class T> struct doubleHashTable {
//    HashTable<HashTable<T>> table;
//    int Insert
//};