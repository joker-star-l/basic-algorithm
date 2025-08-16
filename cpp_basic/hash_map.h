#include <functional>

namespace util {

template <typename T>
struct HashCompute {
    size_t operator()(const T& key) const {
        return std::hash<T>()(key);
    }
};

template <typename K, typename V>
struct HashNode {
    std::pair<K, V> kv;
    HashNode* next;
    HashNode(const K& k, const V& v) : kv(k, v), next{nullptr} {}
};

template <typename K, typename V, typename F = HashCompute<K>>
class HashMap {
public:
    HashMap(size_t initialCapacity = DEFAULT_CAPACITY, double loadFactor = DEFAULT_LOAD_FACTOR)
        : capacity(initialCapacity), size(0), loadFactor(loadFactor), HashCompute(F()) {
        buckets = new HashNode<K, V>*[capacity]();
    }

    ~HashMap() {
        for (size_t i = 0; i < capacity; ++i) {
            HashNode<K, V>* curr = buckets[i];
            while (curr) {
                HashNode<K, V>* next = curr->next;
                delete curr;
                curr = next;
            }
        }
        delete[] buckets;
    }

    void put(const K& key, const V& value) {
        size_t index = HashCompute(key) % capacity;
        
        if (!buckets[index]) {
            buckets[index] = new HashNode<K, V>(key, value);
            size++;
        } else {
            HashNode<K, V>* curr = buckets[index];
            while (curr) {
                if (curr->kv.first == key) {
                    curr->kv.second = value; // Update existing key
                    return;
                }
                if (!curr->next) {
                    curr->next = new HashNode<K, V>(key, value); // Insert at the end
                    size++;
                    break;
                }
                curr = curr->next;
            }
        }

        if (static_cast<double>(size) / capacity > loadFactor) {
            rehash();
        }
    }

    void rehash() {
        size_t newCapacity = capacity * 2;
        HashNode<K, V>** newBuckets = new HashNode<K, V>*[newCapacity]();
        
        for (size_t i = 0; i < capacity; ++i) {
            HashNode<K, V>* curr = buckets[i];
            while (curr) {
                size_t newIndex = HashCompute(curr->kv.first) % newCapacity;
                HashNode<K, V>* next = curr->next;
                
                // 头插法
                curr->next = newBuckets[newIndex];
                newBuckets[newIndex] = curr;
                
                curr = next;
            }
        }

        delete[] buckets;
        buckets = newBuckets;
        capacity = newCapacity;
    }

    V& get(const K& key) {
        size_t index = HashCompute(key) % capacity;
        HashNode<K, V>* curr = buckets[index];

        while (curr) {
            if (curr->kv.first == key) {
                return curr->kv.second; // Key found
            }
            curr = curr->next;
        }

        curr = new HashNode<K, V>(key, V()); // If key not found, insert with default value
        curr->next = buckets[index]; // Insert at the head
        buckets[index] = curr;
        size++;
        if (static_cast<double>(size) / capacity > loadFactor) {
            rehash();
        }
        return curr->kv.second; // Return the default value
    }

    bool containsKey(const K& key) const {
        size_t index = HashCompute(key) % capacity;
        HashNode<K, V>* curr = buckets[index];
        
        while (curr) {
            if (curr->kv.first == key) {
                return true; // Key found
            }
            curr = curr->next;
        }
        
        return false; // Key not found
    }
    
    void remove(const K& key) {
        size_t index = HashCompute(key) % capacity;
        HashNode<K, V>* curr = buckets[index];
        HashNode<K, V>* prev = nullptr;

        while (curr) {
            if (curr->kv.first == key) {
                if (prev) {
                    prev->next = curr->next; // Remove from middle or end
                } else {
                    buckets[index] = curr->next; // Remove from head
                }
                delete curr;
                size--;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    size_t getSize() const {
        return size;
    }

    class iterator {
    public:
        using NodeType = HashNode<K, V>;

        iterator(NodeType** buckets, size_t capacity, size_t index, NodeType* node)
            : buckets(buckets), capacity(capacity), index(index), node(node) {
            advanceToNextValid();
        }

        std::pair<K, V>& operator*() const {
            return node->kv;
        }

        std::pair<K, V>* operator->() const {
            return &(node->kv);
        }

        iterator& operator++() {
            if (node) {
                node = node->next;
            }
            if (!node) {
                ++index;
                advanceToNextValid();
            }
            return *this;
        }

        bool operator!=(const iterator& other) const {
            return index != other.index || node != other.node;
        }

    private:
        NodeType** buckets;
        size_t capacity;
        size_t index;
        NodeType* node;

        void advanceToNextValid() {
            while (index < capacity && (node = buckets[index]) == nullptr) {
                ++index;
            }
        }
    };

    iterator begin() {
        return iterator(buckets, capacity, 0, buckets[0]);
    }

    iterator end() {
        return iterator(buckets, capacity, capacity, nullptr);
    }


private:
    static constexpr size_t DEFAULT_CAPACITY = 16;
    static constexpr double DEFAULT_LOAD_FACTOR = 0.75;
    
    HashNode<K, V> **buckets;
    F HashCompute;
    size_t capacity;
    size_t size;
    double loadFactor;
};

} // namespace util end