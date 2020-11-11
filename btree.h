#ifndef PMBTREE_BTREE_H
#define PMBTREE_BTREE_H

#include "arena.h"
#include "options.h"
#include "global.h"
#include <string>
#include <stdint.h>
#include <iostream>
#include <fstream>

namespace pmbtree {

//static const int min_degree = 20;    // min_degree of a node except root, min_degree - 1 <= keynum <= 2 * min_degree - 1, min_degree <= childnum <= 2 * min_degree

struct Node {
    bool is_leaf;
    int keynum;
    int64_t key[2 * min_degree];
    size_t key_len[2 * min_degree];
    int64_t value[2 * min_degree];
    size_t value_len[2 * min_degree];
    int64_t child[2 * min_degree + 1];
};

class Iterator;

class BTree {
    friend class Iterator;
public:
    explicit BTtree(std::string name);
    ~BTree();

    bool Write(const std::string key, const std::string value);
    bool Read(const std::string key, std::string* value);
    bool Delete(const std::string key);

    //Iterator* NewIterator();
private:
    
    Node* NewNode(int64_t &offset);
    Arena arena_;
    int64_t root_;
    Node* root_tmp_;
    std::string manifest_;
};

/*class Iterator {
public:
    explicit Iterator(Skiplist* list);
    ~Iterator();
    bool Valid();
    void Next();
    std::string Key();
    std::string Value();
    void Seek(const std::string key);
    void SeekToFirst();
private:
    BTree* tree;
    Node* node_;
};*/

} // pmbtree
#endif // PMBTREE_BTREE_H