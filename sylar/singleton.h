#ifndef __SYLAR_SINGLETON_H__
#define __SYLAR_SINGLETON_H__

#include <memory>

namespace sylar
{

template<typename T, typename X = void, int N = 0>
class Singleton
{
public:
    static T* GetInstance()
    {
        static T instance;
        return &instance;
    }
};

template<typename T, typename X = void , int N = 0>
class SingletonPtr
{
public:
    static std::shared_ptr<T> GetInstance()
    {
        static std::shared_ptr<T> instance(new T);
        return instance;
    }
};

} // namespace sylar


#endif