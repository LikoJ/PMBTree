#include <iostream>
#include <string>
#include "btree.h"
#define KEY_NUM 200
int main() {
    pmbtree::Options opt;
    pmbtree::BTree *bt = new pmbtree::BTree(opt);
    std::string key, value;

    std::cout << "--------Insert--------" << std::endl;
    for (int i = 0; i < KEY_NUM; i++, i++) {
        key = "k";
        key += std::to_string(i);
        value = "v";
        value += std::to_string(i);
        bt->Write(key, value);
        std::cout << key << ": " << value << std::endl;
    }

    std::cout << "---------Read---------" << std::endl;
    for (int i = 0; i < KEY_NUM; i++, i++) {
        key = "k";
        key += std::to_string(i);
        if (bt->Read(key, &value)) {
            std::cout << key << ": " << value << std::endl;
        } else {
            std::cout << key << ": not found!" << std::endl;
        }
    }

    std::cout << "--------Insert--------" << std::endl;
    for (int i = 1; i < KEY_NUM; i++, i++) {
        key = "k";
        key += std::to_string(i);
        value = "v";
        value += std::to_string(i);
        bt->Write(key, value);
        std::cout << key << ": " << value << std::endl;
    }

    std::cout << "---------Read---------" << std::endl;
    for (int i = 0; i < KEY_NUM; i++) {
        key = "k";
        key += std::to_string(i);
        if (bt->Read(key, &value)) {
            std::cout << key << ": " << value << std::endl;
        } else {
            std::cout << key << ": not found!" << std::endl;
        }
    }

    // not found
    key = "k";
    if (bt->Read(key, &value)) {
        std::cout << key << ": " << value << std::endl;
    } else {
        std::cout << key << ": not found!" << std::endl;
    }

    std::cout << "--------Update--------" << std::endl;
    for (int i = 0; i < KEY_NUM; i += 3) {
        key = "k";
        key += std::to_string(i);
        value = "u";
        value += std::to_string(i);
        bt->Write(key, value);
        std::cout << key << ": " << value << std::endl;
    }
    
    std::cout << "---------Read---------" << std::endl;
    for (int i = 0; i < KEY_NUM; i++) {
        key = "k";
        key += std::to_string(i);
        if (bt->Read(key, &value)) {
            std::cout << key << ": " << value << std::endl;
        } else {
            std::cout << key << ": not found!" << std::endl;
        }
    }

    
    std::cout << "-Scan-from-k20-to-k50-" << std::endl;
    pmbtree::Iterator *it = bt->NewIterator();
    key = "k";
    key += std::to_string(20);
    for (it->Seek(key); it->Valid(); it->Next()) {
        key = it->Key();
        value = it->Value();
        if (key > "k50") {
            break;
        }
        std::cout << key << ": " << value << std::endl;
    }

    /*
    std::cout << "-Delete-from-k0-to-k5-" << std::endl;
    for (int i = 0; i < 6; i++) {
        key = "k";
        key += std::to_string(i);
        l->Delete(key);
        std::cout << key << ": " << "delete" << std::endl;
    }

    std::cout << "---------Scan---------" << std::endl;
    it->SeekToFirst();
    for (; it->Valid(); it->Next()) {
        key = it->Key();
        value = it->Value();
        std::cout << key << ": " << value << std::endl;
    }
    delete it;
    delete l;

    std::cout << "--------Reopen--------" << std::endl;
    l = new pmskiplist::Skiplist("test");
    std::cout << "---------Read---------" << std::endl;
    for (int i = 0; i < 10; i++) {
        key = "k";
        key += std::to_string(i);
        if (l->Read(key, &value)) {
            std::cout << key << ": " << value << std::endl;
        } else {
            std::cout << key << ": not found!" << std::endl;
        }
    }*/
    delete bt;
    return 0;
}