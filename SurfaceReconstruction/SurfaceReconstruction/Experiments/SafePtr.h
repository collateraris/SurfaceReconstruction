#pragma once

#include <memory>
#include <mutex>

namespace Experiments::Threads 
{

    struct link_safe_ptrs {
        template<typename T1, typename... Args>
        link_safe_ptrs(T1 &first_ptr, Args&... args) {
            std::lock_guard<T1> lock(first_ptr);
            typedef typename T1::mtx_t mutex_t;
            std::shared_ptr<mutex_t> old_mtxs[] = { args.mtx_ptr ... }; // to unlock before mutexes destroyed
            std::shared_ptr<std::lock_guard<mutex_t>> locks[] = { std::make_shared<std::lock_guard<mutex_t>>(*args.mtx_ptr) ... };
            std::shared_ptr<mutex_t> mtxs[] = { args.mtx_ptr = first_ptr.mtx_ptr ... };
        }
    };

    template<typename T, typename mutex_type = std::recursive_mutex, 
        typename x_lock_t = std::unique_lock<mutex_type>, 
        typename const_lock_t = std::unique_lock<mutex_type>>
    class SafePtr
    {
        std::shared_ptr<mutex_type> mMutex;
        std::shared_ptr<T> mContainer;

        void lock() { mMutex->lock(); };
        void unlock() { mMutex->unlock(); };

        friend struct link_safe_ptrs;

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
            

            /* thread unsafe
            T* operator -> () { return mContainer.get(); }
            T& operator * () { return *mContainer.get(); }
            const T* operator -> () const { return mContainer.get(); }
            const T& operator * () const { return *mContainer.get(); }
            */
            template<typename mutex_type> friend class  std::lock_guard;

    };
}