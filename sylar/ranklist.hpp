#ifndef __SYLAR_RANKLIST_H__
#define __SYLAR_RANKLIST_H__

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <utility>

#include "log.h"
#include "skiplist.hpp"

template <typename K, typename V>
struct RankContext
{
    K uid_;
    V score_;

    RankContext() : uid_(), score_() {}
    RankContext(const K& uid, const V& score) : uid_(uid), score_(score) {}
};

template <typename K, typename V, typename Callable>
struct RankCompare
{
    Callable cb_;   // 回调函数
    bool order_;    // true 表示升序，false 表示降序

    // order 默认为升序排列，
    RankCompare(const Callable& cb, bool order = true) : cb_(std::move(const_cast<Callable&>(cb))), order_(order) {}

    int operator()(const RankContext<K, V>& lhs, const RankContext<K, V>& rhs) const {
		ptrdiff_t diff = (const char*)&lhs - (const char*)&rhs;
		if (diff == 0) {
			return 0;
		}
		if (lhs.score_ != rhs.score_) {
			if (order_) {
				return lhs.score_ > rhs.score_ ? 1 : -1;
			} else {
				return lhs.score_ > rhs.score_ ? -1 : 1;
			}
		}
		return cb_(lhs.uid_, lhs.score_, rhs.uid_, rhs.score_);
    }
};

template <typename K, typename V, typename Comparator>
class RankList : public SkipList<RankContext<K, V>, RankCompare<K, V, Comparator>>
{
public:
    using Super = SkipList<RankContext<K, V>, RankCompare<K, V, Comparator>>;
    using Node = typename Super::Node;

    class Iterator : public Super::Iterator
    {
    public:
        explicit Iterator(const RankList<K, V, Comparator>* list) : Super::Iterator(list) {}

        void SeekGreaterOrEqual(const V& score) {
            this->node_ = ((RankList*)this->list_)->FindGreaterOrEqual(score);
            if (this->node_ == this->list_->GetHead())
            {
                this->node_ = nullptr;
            }
        }

        void SeekLessOrEqual(const V& score) {
            this->node_ = ((RankList*)this->list_)->FindLessOrEqual(score);
            if (this->node_ == this->list_->GetHead())
            {
                this->node_ = nullptr;
            }
        }
    };

public:
    // order 默认是升序排列，第一名在默认，要改成降序order传false
    explicit RankList(const Comparator& cmp, bool order = true);
    ~RankList();

    int Insert(const K& uid, const V& score);

    int Update(const K& uid, const V& score);

    int Delete(const K& uid);

    // 判断uid是否存在在排行榜中
    bool Contain(const K& uid);

    // 根据排名获取对应的uid
    uint64_t GetByRank(int rank);

    uint64_t DelByRank(int rank);

    // *获取当前用户uid的排名
    uint64_t RankOf(const K& uid);

    int DeleteByRankRange(int min, int max);

    int GetRank(const RankContext<K, V>& data);

    int GetRank(Node* node);

    int GetRank(const V& score);

    const V* GetScore(const K& uid);

    int RankOfScore(const V& score);

    int GetSize() { return Super::GetSize(); }

    void Dump();

protected:
    Node* FindGreaterOrEqual(const V& score) const;

    Node* FindLessOrEqual(const V& score) const;
private:
    std::unordered_map<K, Node*> node_list_;
};

template <typename K, typename V, typename Comparator>
RankList<K, V, Comparator>::RankList(const Comparator& cmp, bool order)
: Super(RankCompare<K, V, Comparator>(cmp, order))
{
}

template <typename K, typename V, typename Comparator>
RankList<K, V, Comparator>::~RankList()
{
}

template <typename K, typename V, typename Comparator>
int RankList<K, V, Comparator>::Insert(const K& uid, const V& score)
{
    auto it = node_list_.find(uid);
    if (it != node_list_.end()) {       // 当 it != node_list_.end() 时，表示该 uid 已经在该排行榜中
        return -1;
    }

    auto node = Super::Emplace(uid, score);
    node_list_.insert(std::make_pair(uid, node));
    return 0;
}

template <typename K, typename V, typename Comparator>
int RankList<K, V, Comparator>::Update(const K& uid, const V& score)
{
    auto it = node_list_.find(uid);
    if (it == node_list_.end())
    {
        return Insert(uid, score);
    }

    auto node = it->second;
    Super::Detele(node);

    node->data.score_ = score;
    Super::Insert(node);

    return 0;
}

template <typename K, typename V, typename Comparator>
int RankList<K, V, Comparator>::Delete(const K& uid)
{
    auto it = node_list_.find(uid);
    if (it == node_list_.end()) {
        return -1;
    }

    auto node = it->second;
    node_list_.erase(it);

    Super::Detele(node);
    delete node;
    return 0;
}

template <typename K, typename V, typename Comparator>
bool RankList<K, V, Comparator>::Contain(const K& uid)
{
    auto it = node_list_.find(uid);
    return it != node_list_.end();
}

template <typename K, typename V, typename Comparator>
uint64_t RankList<K, V, Comparator>::GetByRank(int rank)
{
    if (rank <= 0 || rank > GetSize())
    {
        return 0;
    }

    Iterator it(this);
    if (rank == 1) {
        it.SeekToFirst();
    }
    else if (rank == GetSize())
    {
        it.SeekToLast();
    }
    else
    {
        it.SeekByRank(rank);
    }

    if (!it.Valid()) {
        return 0;
    }

    return it.GetData().uid_;
}

template <typename K, typename V, typename Comparator>
uint64_t RankList<K, V, Comparator>::DelByRank(int rank)
{
    if (rank <= 0 || rank > GetSize())
    {
        return 0;
    }

    Iterator it(this);
    if (rank == 1) {
        it.SeekToFirst();
    }
    else if (rank == GetSize())
    {
        it.SeekToLast();
    }
    else
    {
        it.SeekByRank(rank);
    }

    if (!it.Valid()) {
        return 0;
    }

    Node* node = const_cast<Node*>(it.GetNode());
    if (Super::Detele(node) < 0)
    {
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "compare function implementation maybe error";
        return -1;
    }

    return it.GetData().uid_;
}

template <typename K, typename V, typename Comparator>
uint64_t RankList<K, V, Comparator>::RankOf(const K& uid)
{
    auto it = node_list_.find(uid);
    if (it == node_list_.end()) {
        return 0;
    }

    return GetRank(it->second);
}

template <typename K, typename V, typename Comparator>
int RankList<K, V, Comparator>::DeleteByRankRange(int min, int max)
{
    assert(1 <= min && min <= max && min <= this->size_);
	assert(1 <= max && min <= max && max <= this->size_);

	int traversed = 0;
	Node* p = this->head_;
	for (int level = this->GetMaxHeight() - 1; level >= 0; level--) {
		while (p->level_[level].next_ && (traversed + p->level_[level].span_) < min) {
			traversed += p->level_[level].span_;
			p = p->Next(level);
		}
	}

	int removed = 0;
	traversed++;
	p = p->level_[0].next_;
	while (p && traversed <= max) {
		Node* next = p->level_[0].next_;
		node_list_.erase(p->data.uid_);
		Super::Detele(p);
		delete p;
		traversed++;
		removed++;
		p = next;
	}
	return removed;
}

template <typename K, typename V, typename Comparator>
int RankList<K, V, Comparator>::GetRank(const RankContext<K, V>& data)
{
    int traversed = 0;
	Node* p = this->head_;
	for (int level = this->GetMaxHeight() - 1; level >= 0; level--) {
		while (p->level_[level].next_ && this->compare_(p->level_[level].next_->data, data) <= 0) {
			traversed += p->level_[level].span_;
			p = p->Next(level);
		}
		if (p != this->head_ && this->Equal(data, p->data)) {
			return traversed;
		}
	}
	return 0;
}

template <typename K, typename V, typename Comparator>
int RankList<K, V, Comparator>::GetRank(Node* node)
{
    if (!node) {
        return 0;
    }

    return GetRank(node->data);
}

template <typename K, typename V, typename Comparator>
int RankList<K, V, Comparator>::GetRank(const V& score)
{
    int traversed = 0;
	Node* p = this->head_;
	for (int level = this->GetMaxHeight() - 1; level >= 0; level--)
    {
        while (p->level_[level].next_ && p->level_[level].next_->data.score_ <= score)
        {
			traversed += p->level_[level].span_;
			p = p->Next(level);
		}
    }
    return traversed;
}

template <typename K, typename V, typename Comparator>
const V* RankList<K, V, Comparator>::GetScore(const K& uid)
{
    auto it = node_list_.find(uid);
    if (it == node_list_.end())
    {
        return nullptr;
    }

    return &it->second->data.score_;
}

template <typename K, typename V, typename Comparator>
int RankList<K, V, Comparator>::RankOfScore(const V& score)
{
    return GetRank(score);
}

template <typename K, typename V, typename Comparator>
void RankList<K, V, Comparator>::Dump()
{
    SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "RankList::Dump()";
}

template <typename K, typename V, typename Comparator>
typename RankList<K, V, Comparator>::Node* RankList<K, V, Comparator>::FindGreaterOrEqual(const V& score) const
{
    Node* p = this->head_;
    for (int level = this->GetMaxHeight() - 1; level >= 0; level--)
    {
        while (p->level_[level].next_ && p->level_[level].next_->data.score_ < score)
        {
            p = p->Next(level);
        }
    }
    return p->level_[0].next_;
}

template <typename K, typename V, typename Comparator>
typename RankList<K, V, Comparator>::Node* RankList<K, V, Comparator>::FindLessOrEqual(const V& score) const
{
    Node* p = this->head_;
    for (int level = this->GetMaxHeight() - 1; level >= 0; level--)
    {
        while (p->level_[level].next_ && p->level_[level].next_->data.score_ <= score)
        {
			p = p->Next(level);
		}
    }
    return p;
}

#endif