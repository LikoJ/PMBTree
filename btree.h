#ifndef PMBTREE_BTREE_H
#define PMBTREE_BTREE_H

#include "arena.h"
#include <string>
#include <stdint.h>

namespace pmbtree {
static const int t = 20;    // node except root, t - 1 <= keynum <= 2t - 1, t <= child <= 2t
struct Node {
    int64_t key[2t];
    size_t key_len[2t];
    int64_t value[2t];
    size_t value_len[2t];
    Node *child[2t + 1];
};

class Iterator;

class BTree {
    friend class Iterator;
public:
    explicit BTtree();
    ~BTree();

    bool Write(const std::string key, const std::string value);
    bool Read(const std::string key, std::string* value);
    bool Delete(const std::string key);

    Iterator* NewIterator();
private:
    Node* FindGreaterOrEqual(const std::string key, Node* father);

    Arena arena_;
    Node *root_;
};

class Iterator {
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
};

} // bmptree
#endif // PMBTREE_BTREE_H