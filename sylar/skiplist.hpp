#ifndef __SYLAR_SKIPLIST_H__
#define __SYLAR_SKIPLIST_H__

#include <cstdlib>
#include <new>
#include <utility>

#include "macro.h"

/**
 * Data 数据
 * Comparator 比较器
 */
template <typename Data, class Comparator>
class SkipList
{
public:
    struct Node;

    // 迭代器
    class Iterator {
        friend class SkipList;
    public:
        explicit Iterator(const SkipList* list);

        const Data& GetData() const;
        const Node* GetNode() const;

        bool Valid() const;

        void Next();
        void Prev();

        void Seek(const Data& target);
        void SeekToFirst();
        void SeekToLast();
        void SeekByRank(int rank);

        Iterator& operator++();
        Iterator& operator++(int);

        // 重载解引用运算符
        const Data& operator*();

        bool operator==(const Iterator& rhs) const;
        bool operator!=(const Iterator& rhs) const;
    
    protected:
        const SkipList* list_;
        Node* node_;
    };

    explicit SkipList(const Comparator& cmp);
    virtual ~SkipList();

    SkipList(const SkipList&) = delete;
    SkipList& operator=(const SkipList&) = delete;

    inline Iterator begin() {
        Iterator itr(this);
        itr.SeekToFirst();
        return itr;
    }

    inline Iterator begin() const {
        Iterator itr(this);
        itr.SeekToFirst();
        return itr;
    }

    inline Iterator end() {
        Iterator itr(this);
        itr.node_ = nullptr;
        return itr;
    }

    inline int GetSize() { return size_; }

    inline const Node* GetHead() const { return head_; }

    template <typename ...Args>
    Node* CtorNode(Args&& ...args)
    {
        int height = RandomHeight();
        char* node_memory = (char*)malloc(sizeof(Node) + sizeof(typename Node::LevelContext) * (height - 1));
        return new (node_memory) Node(height, std::forward<Args>(args)...);
    }

    void DtorNode(Node* node) {
		node->~Node();
		free(node);
	}

    inline Node* Insert(const Data& data) {
        Node* node = NewNode(RandomHeight(), data);
        Insert(node);
        return node;
    }

    template <typename ...Args>
    inline Node* Emplace(Args&& ...args)
    {
        Node* node = CtorNode(std::forward<Args>(args)...);
        Insert(node);
        return node;
    }

    void Insert(Node* node);

    int Delete(const Data& data, bool dtor = true);
    int Detele(const Node* node);

    bool Contains(const Data& data) const;

    // 根据排名查找数据
    Node* FindByRank(int rank) const;

protected:
    static constexpr int kMaxHeight = 32;       // 跳表最大层数

    inline int GetMaxHeight() const { return max_height_; }

    inline bool Equal(const Data& a, const Data& b) const { return compare_(a, b) == 0; }

    // 创建一个新的 Node 节点
    Node* NewNode(int height, const Data& data) {
        char* node_memory = (char*)malloc(sizeof(Node) + sizeof(typename SkipList<Data, Comparator>::Node::LevelContext) * (height - 1));
        return new (node_memory) Node(height, data);
    }

    int RandomHeight();

    bool KeyIsAfterNode(const Data& data, Node* n) const;

    Node* FindGreaterOrEqual(const Data& data, Node** prev, int* rank = nullptr) const;
    Node* FindLessThan(const Data& data) const;
    Node* FindLast() const;

protected:
    const Comparator compare_;      // 比较器

    Node* head_;                    // 头节点
    Node* tail_;

    int max_height_;
    int size_;                      // Node节点的数量，skiplist的总数
};

/**
 * Node 节点的实现
 */
template <typename Data, class Comparator>
struct SkipList<Data, Comparator>::Node
{
    struct LevelContext {
        Node* next_;        // 下一个node节点
        int span_;          // 跨度

        LevelContext() { span_ = 0; }
    };

    explicit Node(int h, const Data& d) : data(d), height_(h), prev_(nullptr) {}
    
    template <typename ...Args>
    explicit Node(int h, Args&& ...args) : data(std::forward<Args>(args)...), height_(h), prev_(nullptr) {}

    Data data;

    int Span(int n) const {
        return level_[n].span_;
    }

    void SetSpan(int n, int span) {
        level_[n].span_ = span;
    }

    Node* Next(int n) const {
        SYLAR_ASSERT(n >= 0);
        return level_[n].next_;
    }

    void SetNext(int n, Node* x) {
        SYLAR_ASSERT(n >= 0);
        level_[n].next_ = x;
    }

    int height_;
    Node* prev_;
    LevelContext level_[1];     // 这里 level_ 只是用来声明这个是个数组，真正的数组大小是根据 height_ 来动态分配的
};

// 迭代器 Iterator 的构造函数
template <typename Data, class Comparator>
inline SkipList<Data, Comparator>::Iterator::Iterator(const SkipList* list)
: list_(list)
, node_(nullptr)
{
}

// 获取 Node 的数据
template <typename Data, class Comparator>
inline const Data& SkipList<Data, Comparator>::Iterator::GetData() const {
	SYLAR_ASSERT(Valid());
	return node_->data;
}

// 获取 Node 节点
template <typename Data, class Comparator>
inline const typename SkipList<Data, Comparator>::Node* SkipList<Data, Comparator>::Iterator::GetNode() const {
	return node_;
}

// 判断迭代器是否有效
template <typename Data, class Comparator>
inline bool SkipList<Data, Comparator>::Iterator::Valid() const {
	return node_ != nullptr;
}

template <typename Data, class Comparator>
inline void SkipList<Data, Comparator>::Iterator::Next()
{
    SYLAR_ASSERT(Valid());
    node_ = node_->Next(0);
}

template <typename Data, class Comparator>
inline void SkipList<Data, Comparator>::Iterator::Prev()
{
    SYLAR_ASSERT(Valid());
    node_ = list_->FindLessThan(node_->data);
    if (node_ == list_->head_) {
        node_ = nullptr;
    }
}

template <typename Data, class Comparator>
inline void SkipList<Data, Comparator>::Iterator::Seek(const Data& target)
{
    node_ = list_->FindGreaterOrEqual(target, nullptr);
    if (node_ != nullptr && !list_->Equal(target, node_->data))
    {
        node_ = nullptr;
    }
}
        
template <typename Data, class Comparator>
inline void SkipList<Data, Comparator>::Iterator::SeekToFirst()
{
    node_ = list_->head_->Next(0);
}

template <typename Data, class Comparator>
inline void SkipList<Data, Comparator>::Iterator::SeekToLast()
{
    node_ = list_->FindLast();
    if (node_ == list_->head_) {
        node_ = nullptr;
    }
}

template <typename Data, class Comparator>
inline void SkipList<Data, Comparator>::Iterator::SeekByRank(int rank)
{
    node_ = list_->FindByRank(rank);
}

template <typename Data, class Comparator>
inline typename SkipList<Data, Comparator>::Iterator& SkipList<Data, Comparator>::Iterator::operator++()
{
    Next();
    return *this;
}
       
template <typename Data, class Comparator>
inline typename SkipList<Data, Comparator>::Iterator& SkipList<Data, Comparator>::Iterator::operator++(int)
{
    Next();
    return *this;
}

template <typename Data, class Comparator>
inline const Data& SkipList<Data, Comparator>::Iterator::operator*()
{
    return node_->data;
}

template <typename Data, class Comparator>
inline bool SkipList<Data, Comparator>::Iterator::operator==(const Iterator& rhs) const
{
    return node_ == rhs.node_;
}

template <typename Data, class Comparator>
inline bool SkipList<Data, Comparator>::Iterator::operator!=(const Iterator& rhs) const
{
    return node_ != rhs.node_;
}

template <typename Data, class Comparator>
int SkipList<Data, Comparator>::RandomHeight()
{
	int level = 1;
	while ((rand() & 0xffff) < (0.25 * 0xffff))
		level += 1;
	return (level < kMaxHeight) ? level : kMaxHeight;
}

template <typename Data, class Comparator>
bool SkipList<Data, Comparator>::KeyIsAfterNode(const Data& data, Node* n) const
{
    return (n != nullptr) && (compare_(n->data, data) < 0);
}

/**
 * rank 这里rank是数组
 */
template <typename Data, class Comparator>
typename SkipList<Data, Comparator>::Node* SkipList<Data, Comparator>::FindGreaterOrEqual(const Data& data, Node** prev, int* rank) const
{
    Node* cur = head_;
    for (int level = GetMaxHeight() - 1; level >= 0; level--)
    {
        if (rank) {
            rank[level] = level == (GetMaxHeight() - 1) ? 0 : rank[level + 1];
        }

        while (KeyIsAfterNode(data, cur->Next(level)))
        {
            if (rank) {
                rank[level] += cur->level_[level].span_;
            }
            cur = cur->Next(level);
        }

        if (prev) {
            prev[level] = cur;
        }
    }
    return cur->Next(0);
}
    
template <typename Data, class Comparator>
typename SkipList<Data, Comparator>::Node* SkipList<Data, Comparator>::FindLessThan(const Data& data) const
{
    Node* cur = head_;
    int level = GetMaxHeight() - 1;
    while (true)
    {
        SYLAR_ASSERT(cur == head_ || compare_(cur->data, data) < 0);
        Node* next = cur->Next(level);

        if (next == nullptr || compare_(next->data, data) >= 0)
        {
            if (level == 0)
            {
                return cur;
            }
            else
            {
                level--;
            }
        } 
        else
        {
            cur = next;
        }
    }
}
    
template <typename Data, class Comparator>
typename SkipList<Data, Comparator>::Node* SkipList<Data, Comparator>::FindLast() const
{
    Node* x = head_;
    int level = GetMaxHeight() - 1;
    while (true)
    {
        Node* next = x->Next(level);
        if (next == nullptr)
        {
            if (level == 0)
            {
                return x;
            }
            else
            {
                level--;
            }
        }
        else
        {
            x = next;
        }
    }
    
}

template <typename Data, class Comparator>
SkipList<Data, Comparator>::SkipList(const Comparator& cmp)
: compare_(std::move(const_cast<Comparator&>(cmp)))
, head_(NewNode(kMaxHeight, Data()))
, tail_(nullptr)
, max_height_(1)
, size_(0)
{
    for (int i = 0; i < kMaxHeight; i++) {
        head_->SetNext(i, nullptr);
        head_->SetSpan(i, 0);
    }
}

template <typename Data, class Comparator>
SkipList<Data, Comparator>::~SkipList()
{
    Node* next = nullptr;
    for (Node* node = head_; node != nullptr; node = next)
    {
        next = node->level_[0].next_;
        node->~Node();
        free(node);
    }
}

template <typename Data, class Comparator>
void SkipList<Data, Comparator>::Insert(SkipList<Data, Comparator>::Node* node)
{
    Node* prev[kMaxHeight];
    int rank[kMaxHeight];
    Node* x = FindGreaterOrEqual(node->data, prev, rank);

    SYLAR_ASSERT(x == nullptr || x == head_ || !Equal(node->data, x->data));

    int height = node->height_;
    if (height > max_height_)
    {
        for (int i = max_height_; i < height; i++)
        {
            rank[i] = 0;
            prev[i] = head_;
            prev[i]->level_[i].span_ = size_;
        }
        max_height_ = height;
    }

    for (int i = 0; i < height; i++)
    {
        node->SetNext(i, prev[i]->Next(i));
        prev[i]->SetNext(i, node);
        node->level_[i].span_ = prev[i]->level_[i].span_ - (rank[0] - rank[i]);
        prev[i]->level_[i].span_ = rank[0] - rank[i] + 1;
    }

    for (int i = height; i < max_height_; i++)
    {
        prev[i]->level_[i].span_++;
    }

    node->prev_ = (prev[0] == head_) ? nullptr : prev[0];
    if (node->level_[0].next_)
    {
        node->level_[0].next_->prev_ = node;
    }
    else
    {
        tail_ = node;
    }
    size_++;
}

template <typename Data, class Comparator>
int SkipList<Data, Comparator>::Delete(const Data& data, bool dtor)
{
    Node* prev[kMaxHeight];
    Node* node = FindGreaterOrEqual(data, prev);
    if (node && Equal(data, node->data))
    {
        for (int i = 0; i < max_height_; ++i)
        {
            if (prev[i]->level_[i].next_ == node)
            {
                prev[i]->level_[i].span_ += node->level_[i].span_ - 1;
                prev[i]->level_[i].next_ = node->level_[i].next_;
            }
            else
            {
                prev[i]->level_[i].span_ -= 1;
            }
        }

        if (node->level_[0].next_ != nullptr)
        {
            node->level_[0].next_->prev_ = node->prev_;
        }
        else
        {
            tail_ = node->prev_;
        }

        while (max_height_ > 1 && head_->level_[max_height_ - 1].next_ == nullptr)
        {
            max_height_--;
        }

        size_--;

        if (dtor) {
            DtorNode(node);
        }

        return 0;
    }
    return -1;
}
    
template <typename Data, class Comparator>
int SkipList<Data, Comparator>::Detele(const SkipList<Data, Comparator>::Node* node)
{
    return Delete(node->data, false);
}

template <typename Data, class Comparator>
bool SkipList<Data, Comparator>::Contains(const Data& data) const
{
    Node* node = FindGreaterOrEqual(data, nullptr);
    if (node != nullptr && Equal(data, node->data))
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <typename Data, class Comparator>
typename SkipList<Data, Comparator>::Node* SkipList<Data, Comparator>::FindByRank(int rank) const
{
    int traversed = 0;  // 穿过
    Node* p = head_;
    for (int level = GetMaxHeight() - 1; level >= 0; level--)
    {
        while (p->Next(level) && p->Span(level) + traversed <= rank)
        {
            traversed += p->Span(level);
            p = p->Next(level);
        }

        if (traversed == rank)
        {
            return p;
        }
    }
    return nullptr;
}

#endif