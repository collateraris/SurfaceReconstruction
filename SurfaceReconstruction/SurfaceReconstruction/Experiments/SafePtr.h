#pragma once

#include <memory>
#include <mutex>

namespace Experiments::Threads 
{
    template<typename T, typename mutex_type = std::recursive_mutex, 
        typename x_lock_t = std::unique_lock<mutex_type>, 
        typename const_lock_t = std::unique_lock<mutex_type>>
    class SafePtr
    {
        std::shared_ptr<mutex_type> mMutex;
        std::shared_ptr<T> mContainer;

        public:
            template<typename req_lock>
            class autoLock
            {
                req_lock lock;
                T * const mP;
                public:
                autoLock(T* const _p, mutex_type& mtx) : mP(_p), lock(mtx){};

                T* operator->() { return mP; };
                T* const operator->() const { return mP; };
            };

            template<typename req_lock>
            class autoObjLock
            {
                req_lock lock;
                T * const mP;
                public:
                autoObjLock(T* const _p, mutex_type& mtx) : mP(_p), lock(mtx){};

                template<typename arg_t>
                auto operator[](arg_t _arg) -> decltype((*mP)[_arg]) { return (*mP)[_arg]; };
            }; 

            template<typename ...Args>
            SafePtr(Args ... args) : mMutex(std::make_shared<mutex_type>()) , mContainer(std::make_shared<T>(args...)) {};

            autoLock<x_lock_t> operator->() {return autoLock<x_lock_t>(mContainer.get(), *mMutex);};
            autoObjLock<x_lock_t> operator*() {return autoObjLock<x_lock_t>(mContainer.get(), *mMutex);};

            const autoLock<const_lock_t> operator->() const { return autoLock<const_lock_t>(mContainer.get(), *mMutex); };
            const autoObjLock<const_lock_t> operator*() const { return autoObjLock<const_lock_t>(mContainer.get(), *mMutex); };

            template<typename mutex_type> friend class  std::lock_guard;

    };
}