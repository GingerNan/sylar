#include <cstdint>
#include <iostream>

#include "sylar/ranklist.hpp"

struct UidCompare {
    int operator()(uint64_t lhs_uid, int64_t lhs_score,
                   uint64_t rhs_uid, int64_t rhs_score) const {
        (void)lhs_score;
        (void)rhs_score;
        if (lhs_uid < rhs_uid) {
            return -1;
        }
        if (lhs_uid > rhs_uid) {
            return 1;
        }
        return 0;
    }
};

int main()
{
    UidCompare cmp;
    RankList<uint64_t, int64_t, UidCompare> ranklist(cmp);

    ranklist.Insert(1001, 100);
    ranklist.Insert(1002, 200);
    ranklist.Insert(1003, 300);

    std::cout << "size: " << ranklist.GetSize() << std::endl;
    std::cout << "rank of 1001: " << ranklist.RankOf(1001) << std::endl;
    std::cout << "rank of 1003: " << ranklist.RankOf(1003) << std::endl;
    std::cout << "uid at rank 1: " << ranklist.GetByRank(1) << std::endl;
    std::cout << "uid at rank 2: " << ranklist.GetByRank(2) << std::endl;

    ranklist.Update(1001, 400);
    std::cout << "rank of 1001 after update: " << ranklist.RankOf(1001) << std::endl;

    std::cout << "score of 1002: " << *ranklist.GetScore(1002) << std::endl;

    return 0;
}
