#include <functional>

namespace util {

template <typename T>
struct HashCompute {
    size_t operator()(const T& key) const {
        return std::hash<T>()(key);
    }
};

template <typename K, typename V>
struct Bucket {
    K key;
    V value;
    Bucket* next;
    Bucket(const K& k, const V& v) : key(k), value(v), next{nullptr} {}
};

template <typename K, typename V, typename F = HashCompute<K>>
class HashMap {
public:
    HashMap(size_t initialCapacity = DEFAULT_CAPACITY, double loadFactor = DEFAULT_LOAD_FACTOR)
        : capacity(initialCapacity), size(0), loadFactor(loadFactor), HashCompute(F()) {
        buckets = new Bucket<K, V>*[capacity]();
    }

    ~HashMap() {
        for (size_t i = 0; i < capacity; ++i) {
            Bucket<K, V>* curr = buckets[i];
            while (curr) {
                Bucket<K, V>* next = curr->next;
                delete curr;
                curr = next;
            }
        }
        delete[] buckets;
    }

    void put(const K& key, const V& value) {
        size_t index = HashCompute(key) % capacity;
        Bucket<K, V>* newBucket = new Bucket<K, V>(key, value);
        
        if (!buckets[index]) {
            buckets[index] = newBucket;
            size++;
        } else {
            Bucket<K, V>* curr = buckets[index];
            while (curr) {
                if (curr->key == key) {
                    curr->value = value; // Update existing key
                    delete newBucket;
                    return;
                }
                if (!curr->next) {
                    curr->next = newBucket; // Insert at the end
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
        Bucket<K, V>** newBuckets = new Bucket<K, V>*[newCapacity]();
        
        for (size_t i = 0; i < capacity; ++i) {
            Bucket<K, V>* curr = buckets[i];
            while (curr) {
                size_t newIndex = HashCompute(curr->key) % newCapacity;
                Bucket<K, V>* next = curr->next;
                
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

    V get(const K& key) const {
        size_t index = HashCompute(key) % capacity;
        Bucket<K, V>* curr = buckets[index];
        
        while (curr) {
            if (curr->key == key) {
                return curr->value; // Key found
            }
            curr = curr->next;
        }
        
        return V(); // Key not found, return default value
    }

    bool containsKey(const K& key) const {
        size_t index = HashCompute(key) % capacity;
        Bucket<K, V>* curr = buckets[index];
        
        while (curr) {
            if (curr->key == key) {
                return true; // Key found
            }
            curr = curr->next;
        }
        
        return false; // Key not found
    }
    

    void remove(const K& key) {
        size_t index = HashCompute(key) % capacity;
        Bucket<K, V>* curr = buckets[index];
        Bucket<K, V>* prev = nullptr;

        while (curr) {
            if (curr->key == key) {
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
        using BucketType = Bucket<K, V>;

        iterator(BucketType** buckets, size_t capacity, size_t index, BucketType* node)
            : buckets(buckets), capacity(capacity), index(index), node(node) {
            advanceToNextValid();
        }

        std::pair<const K&, V&> operator*() const {
            return { node->key, node->value };
        }

        iterator& operator++() {
            if (node) node = node->next;
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
        BucketType** buckets;
        size_t capacity;
        size_t index;
        BucketType* node;

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
    
    Bucket<K, V> **buckets;
    F HashCompute;
    size_t capacity;
    size_t size;
    double loadFactor;
};

} // namespace util end