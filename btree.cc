#include "btree.h"

namespace pmbtree {

BTree::BTree(Options& opt) : arena_(opt.pm_path + opt.index_name + ".pool", opt.pmem_len),
                             manifest_(opt.pm_path + opt.index_name + ".fest") {
    std::ifstream ifs(manifest_);
    if (!ifs.is_open()) {
        // Create new pmskiplist
        if ((root_tmp_ = (Node*)arena_.Allocate(sizeof(Node), &root_)) == NULL) {
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

Node* BTree::FindGreaterOrEqual(const std::string key, Node* father) {

}

bool BTree::Write(const std::string key, const std::string value) {
    
}

bool BTree::Read(const std::string key, std::string *value) {

}

bool BTree::Delete(const std::string key) {

}

Iterator* BTree::NewIterator() {

}
} //pmbtree