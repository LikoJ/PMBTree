#ifndef PMBTREE_OPTIONS_H
#define PMBTREE_OPTIONS_H

namespace pmbtree {

class Options {
public:
    Options();
    ~Options();

    int pm_path = "/mnt/persist-memory/pmem_fs_lhd/";

    int index_name = "test";

    size_t pmem_len = 80L * 1024 * 1024 * 1024; // pmdk pool size
};

} // pmbtree
#endif // PMBTREE_OPTIONS_H