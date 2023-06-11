#include<vector>
#include<string>
#include<iostream>
struct HashItem {
    std::string _key;
    HashItem* _next = nullptr;
    ~HashItem() {
        if (_next) delete _next;
    }

};

struct HashTable
{
    unsigned int _capacity;
    HashItem** _arr = new HashItem*[_capacity] { nullptr};

    ~HashTable() {
        if (_arr) {
            for (int i = 0; i < _capacity; i++) {
                if (_arr[i]) delete _arr[i];
            }
            delete _arr;
        }
    };
        

private:
    //hash the key
    unsigned int hashFunction(std::string key)
    {
        return  (unsigned int)(key[0] - 'а') % _capacity;
    }


public:
    HashItem* Insert(std::string key)
    {
        //while (getLoadFactor() > 0.9) // when load factor > 0.5
        //{
        //    rehashing(); //request more memory for new key
        //}
 
        unsigned int bucketIndex = hashFunction(key);
        if (!_arr[bucketIndex]) // when there is no linked list at bucket
        {
            HashItem* newNode = new HashItem{key};
            _arr[bucketIndex] = newNode;
            return newNode;
        }
        else // adding at the head of current linked list
        {
            // check if key already exist:
            HashItem* tmpNode = _arr[bucketIndex];
            while (tmpNode) {
                if (tmpNode->_key == key) {
                    //!!!
                    return nullptr;
                }
                tmpNode = tmpNode->_next;
            }
            HashItem* newNode = new HashItem{key};
            newNode->_next = _arr[bucketIndex];
            _arr[bucketIndex] = newNode;
            return newNode;
        }
    }

    HashItem* Search(std::string key)
    {
        int bucketIndex = hashFunction(key); // getting bucket index
        HashItem* bucketHead = _arr[bucketIndex];
       
        while (bucketHead) // searching in the linked list which is present at bucket for given key
        {
            if (bucketHead->_key == key)
            {
                return bucketHead;
            }
            bucketHead = bucketHead->_next; // moving to next node in linked list
        }
        return nullptr;
    }

    int Delete(std::string key) {
        int bucketIndex = hashFunction(key); // getting bucket index
        HashItem* bucketHead = _arr[bucketIndex];
        HashItem* bucketPrev = nullptr;
        while (bucketHead) // searching in the linked list which is present at bucket for given key
        {
            if (bucketHead->_key == key) //found
            {
                if (bucketPrev) {
                    bucketPrev->_next = bucketHead->_next;
                }
                else
                    _arr[bucketIndex] = bucketHead->_next;

                bucketHead->_next = nullptr;
                delete bucketHead;
                return 0;
            }
            bucketPrev = bucketHead;
            bucketHead = bucketHead->_next; // moving to next node in linked list
        }
        return 1;
    }

    void Clear() {
        for (int i = 0; i < _capacity; i++) {
            if (_arr[i]) delete _arr[i];
        }
        delete[] _arr;
        _arr = new HashItem*[_capacity] { nullptr};
    }

    void visual() {
        for (int j = 0; j < _capacity; j++)
            if (_arr[j]) {
                for (int i = 0; i < _capacity; i++) {
                    std::cout << i << " ";
                    if (_arr[i]) {
                        HashItem* bucketHead = _arr[i];
                        while (bucketHead && bucketHead->_next) {
                            std::cout << bucketHead->_key << " <- ";
                            bucketHead = bucketHead->_next;
                        }
                        std::cout << bucketHead->_key << '\n';
                    }
                    else {
                        std::cout << "___\n";
                    }
                }
                return;
            }
        std::cout << "ѕустое!!\n";
    }

    void printRow(HashItem* bucketHead) {
        while (bucketHead->_next) {
            std::cout << bucketHead->_key << " <- ";
            bucketHead = bucketHead->_next;
        }
        std::cout << bucketHead->_key << '\n';
    }
};
