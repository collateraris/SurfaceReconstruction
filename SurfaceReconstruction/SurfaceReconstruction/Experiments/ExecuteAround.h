#pragma once

#include <iostream>
#include <memory>
#include <mutex>

namespace Experiments::Threads
{
    template<typename T, typename mutex_type = std::recursive_mutex>
    class ExecureAround
    {
        std::shared_ptr<mutex_type> mMutex;
        std::shared_ptr<T> mP;
    public:
        class Proxy
        {
            T * const mP;
            std::unique_lock<mutex_type> mLock;
        public:
            Proxy(T* const p, mutex_type& mtx) : mP(p), mLock(mtx)
            { std::cout << "locked \n"; };

            ~Proxy() { std::cout << "unlocked \n"; };

            T* operator -> () { return mP; }

        };

        Proxy operator -> () { return Proxy(mP.get(), *mMutex); };

        template<typename ...Args>
        ExecureAround(Args ... args) : mMutex{std::make_shared<mutex_type>()}, mP{std::make_shared<T>(args...)}{};

        template<class Args> friend class std::lock_guard;
    };
}