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
        root_tmp_ = (Node*)arena_.Translate(root_);
        ifs.close();
    }
}

BTree::~BTree() {
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

// father->child[pos] is lchild
void BTree::SplitNode(Node *father, int pos, Node *lchild) {
    // split lchild to lchild and rchild with mid key in lchild, and insert the mid key to their father 
    int64_t rchild_offset;
    Node *rchild = NewNode(rchild_offset);

    rchild->is_leaf = lchild->is_leaf;
    rchild->keynum = min_degree - 1;

    for (int i = 0; i < rchild->keynum; i++) {
        rchild->key[i] = lchild->key[i + min_degree];
        rchild->key_len[i] = lchild->key_len[i + min_degree];
    }

    if (!rchild->is_leaf) {
        for (int i = 0; i < rchild->keynum + 1; i++) {
            rchild->child[i] = lchild->child[i + min_degree];
        }
    }

    lchild->keynum = min_degree - 1;

    for (int i = father->keynum; i > pos; i--) {
        father->child[i + 1] = father->child[i];
    }
    father->child[pos + 1] = rchild_offset;

    for (int i = father->keynum - 1; i >= pos; i--) {
        father->key[i + 1] = father->key[i];
    }
    father->key[pos] = lchild->key[min_degree - 1];

    father->keynum++;
}

int BTree::Compare(int64_t k1, size_t kl1, int64_t k2, size_t kl2) {
    std::string str1((char*)arena_.Translate(k1), kl1);
    std::string str2((char*)arena_.Translate(k2), kl2);
    if(str1 == str2) {
        return 0;
    } else if (str1 < str2) {
        return -1;
    } else {
        return 1;
    }
}

// equal -> update in place!
void BTree::InsertNode(Node* n, int64_t k, size_t kl, int64_t v, size_t vl) {
    if (n->is_leaf) {
        // node is a leaf, insert directly
        int pos = n->keynum - 1;
        while (pos >= 0 && (Compare(k, kl, n->key[pos], n->key_len[pos]) == -1)) {
            pos--;
        }

        if (pos >= 0 && Compare(k, kl, n->key[pos], n->key_len[pos]) == 0) {
            // update
            n->value[pos] = v;
            n->value_len[pos] = vl;
        } else {
            // insert
            int tmp = n->keynum;
            while (tmp > pos + 1) {
                n->key[tmp] = n->key[tmp - 1];
                n->key_len[tmp] = n->key[tmp - 1];
                n->value[tmp] = n->value[tmp - 1];
                n->value_len[tmp] = n->value_len[tmp - 1];
                tmp--;
            }
            n->key[pos + 1] = k;
            n->key_len[pos + 1] = kl;
            n->value[pos + 1] = v;
            n->value_len[pos + 1] = vl;
        }
    } else {
        int pos = n->keynum - 1;
        while (pos >= 0 && (Compare(k, kl, n->key[pos], n->key_len[pos]) == -1)) {
            pos--;
        }

        if (pos >= 0 && (Compare(k, kl, n->key[pos], n->key_len[pos]) == 0)) {
            // update
            n->value[pos] = v;
            n->value_len[pos] = vl;
        } else {
            // insert
            Node *c = (Node*)arena_.Translate(n->child[pos + 1]);
            if (c->keynum == 2 * min_degree - 1) {
                SplitNode(n, pos + 1, c);
                if (Compare(k, kl, n->key[pos + 1], n->key_len[pos + 1]) == 1) {
                    pos++;
                }
            }

            InsertNode(c, k, kl, v, vl);
        }
    }
}

bool BTree::Write(const std::string key, const std::string value) {
    // store key and value with pmdk pool
    int64_t k, v;
    size_t kl = key.length();
    size_t vl = value.length();

    char *pmdk_key = (char *)arena_.Allocate(kl, k);
    memcpy(pmdk_key, key.data(), kl);
    
    char *pmdk_value = (char *)arena_.Allocate(vl, v);
    memcpy(pmdk_value, value.data(), vl);

    if (root_tmp_->keynum == 2 * min_degree - 1) {
        // root is full
        int64_t n_offset;
        Node *n = NewNode(n_offset);
        n->is_leaf = false;
        n->child[0] = root_;
        SplitNode(n, 0, root_tmp_);
        InsertNode(n, k, kl, v, vl);
        root_ = n_offset;
        root_tmp_ = n;

    } else {
        InsertNode(root_tmp_, k, kl, v, vl);
    }
}

int BTree::Compare(int64_t k1, size_t kl1, std::string str2) {
    std::string str1((char*)arena_.Translate(k1), kl1);
    if(str1 == str2) {
        return 0;
    } else if (str1 < str2) {
        return -1;
    } else {
        return 1;
    }
}

bool BTree::Read(const std::string key, std::string *value) {
    assert(root_tmp_ != NULL);
    Node *n = root_tmp_;
    while (true) {
        int i;
        for (i = 0; i < n->keynum; i++) {
            int result = Compare(n->key[i], n->key_len[i], key);
            if (result == 0) {
                *value = std::string((char*)arena_.Translate(n->key[i]), n->key_len[i]);
                return true;
            } else if (result == 1) {
                break;
            }
        }
        if (n->is_leaf) {
            return false;
        } else {
            n = (Node*)arena_.Translate(n->child[i]);
        }
    }
    return false;
}

bool BTree::Delete(const std::string key) {
    return false;
}

/*Iterator* BTree::NewIterator() {
    return NULL;
}*/
} //pmbtree