#ifndef PMBTREE_OPTIONS_H
#define PMBTREE_OPTIONS_H
#include <string>

namespace pmbtree {

class Options {
public:
    Options();
    ~Options();

    std::string pm_path = "/mnt/persist-memory/pmem_fs_lhd/";

    std::string index_name = "test";

    size_t pmem_len = 80L * 1024 * 1024 * 1024; // pmdk pool size
};

} // pmbtree
#endif // PMBTREE_OPTIONS_H