#include "hash_map.h"
#include <iostream>
#include <string>

int main() {
    util::HashMap<std::string, int> map;
    map.put("apple", 1);
    map.put("banana", 2);
    map.put("orange", 3);
    map.put("apple", 4); // Update existing key
    map.put("grape", 5);
    map.remove("banana"); // Remove a key
    map.remove("kiwi"); // Attempt to remove a non-existing key
    std::cout << "Size of map: " << map.getSize() << std::endl;
    for (auto it = map.begin(); it != map.end(); ++it) {
        std::cout << (*it).first << ": " << (*it).second << std::endl;
    }
    return 0;
}
