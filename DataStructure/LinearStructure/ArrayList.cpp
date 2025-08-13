#include <iostream>
#include <string>

#include "ArrayList.h"


int main(int argc, char **argv){
    ArrayList<std::string> arr;

    arr.push_back("apple");
    arr.push_back("banana");
    arr.push_back("cherry");

    std::cout << "ArrayList contents:\n";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << i << ": " << arr[i] << "\n";
    }

    std::cout << "\nInsert 'date' at position 1:\n";
    arr.insert(arr.begin() + 1, "date");

    for (auto it = arr.begin(); it != arr.end(); ++it) {
        std::cout << *it << "\n";
    }

    std::cout << "\nErase element at position 2:\n";
    arr.erase(arr.begin() + 2);

    for (const auto& s : arr) {
        std::cout << s << "\n";
    }

    return 0;

}