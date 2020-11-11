#include "btree.h"

namespace pmbtree {

BTree::BTree(Options& opt) : arena_(opt.pm_path + opt.index_name + ".pool", opt.pmem_len),
                             manifest_(opt.pm_path + opt.index_name + ".fest") {
    std::ifstream ifs(manifest_);
    if (!ifs.is_open()) {
        // Create new pmskiplist
        if ((root_tmp_ = NewNode(root_)) == NULL) {
            std::cout << "Create BTree failed!" << std::endl;
        }
    } else {
        // Recover
        arena_.Recover(ifs);
        ifs >> root_;
        root_tmp_ = (Node*)Arena.Translate(root_);
        ifs.close();
    }
}

BTree::~Btree() {
    std::ofstream ofs(manifest_);
    arena_.Save(ofs);
    ofs << root_ << std::endl;
}

Node* BTree::NewNode(int64_t &offset) {
    Node *x = (Node*)arena_.Allocate(sizeof(Node), offset);
    x->keynum = 0;
    x->is_leaf = true;
    for (int i = 0; i < 2 * min_degree; i++) {
        x->key[i] = -1;
        x->key_len[i] = 0;
        x->value[i] = -1;
        x->value_len[i] = 0;
        x->child[i] = -1;
    }
    x->child[2 * min_degree] = -1;
    return x;
}

bool BTree::Write(const std::string key, const std::string value) {
    return false;
}

bool BTree::Read(const std::string key, std::string *value) {
    return false;
}

bool BTree::Delete(const std::string key) {
    return false;
}

/*Iterator* BTree::NewIterator() {
    return NULL;
}*/
} //pmbtree