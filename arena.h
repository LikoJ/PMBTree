#ifndef PMBTREE_ARENA_H
#define PMBTREE_ARENA_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <libpmem.h>
#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>

namespace pmbtree {

class Arena {
public:
    Arena();
    ~Arena();
    void Sync(void *start, size_t len);
    void* Allocate(size_t bytes, int64_t &offset);
    inline void* Translate(int64_t offset) {
        return (pmemaddr + offset);
    };
    void Recover(std::ifstream &ifs);
    void Save(std::ofstream &ofs);

private:
    void *pmemaddr;
    size_t mapped_len;
    int is_pmem;
    size_t used;
    size_t free;
};

}   // pmbtree
#endif // PMBTREE_ARENA_H