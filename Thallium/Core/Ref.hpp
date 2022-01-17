#pragma once

#include "Thallium/Core/Base.hpp"

namespace Thallium {

    class IRef {
        template<typename T>
        friend class Ref;
    private:
        void IncRefCount() const {
            RefCount++;
        }

        void DecRefCount() const {
            RefCount--;
        }
    public:
        int64_t GetRefCount() const {
            return RefCount;
        }
    private:
        mutable int64_t RefCount = 0;
    };

    template<typename T>
    class Ref {
        template<typename U>
        friend class Ref;

        static_assert(!IsComplete<T>::value || std::is_base_of<IRef, T>::value, "Object must inherit from IRef");
    public:
        Ref() : Instance(nullptr) {}
        Ref(std::nullptr_t) : Instance(nullptr) {}
        Ref(T* instance) : Instance(instance) {
            IncRefCount();
        }

        Ref(const Ref<T>& other) : Instance(other.Instance) {
            IncRefCount();
        }

        template<typename U>
        Ref(const Ref<U>& other) : Instance(other.Instance) {
            static_assert(!IsComplete<T>::value || std::is_base_of<T, U>::value, "Invalid conversion");
            IncRefCount();
        }

        Ref(Ref<T>&& other) : Instance(other.Instance) {
            other.Instance = nullptr;
        }

        template<typename U>
        Ref(Ref<U>&& other) : Instance(other.Instance) {
            static_assert(!IsComplete<T>::value || std::is_base_of<T, U>::value, "Invalid conversion");
            other.Instance = nullptr;
        }

        ~Ref() {
            DecRefCount();
        }

        Ref& operator=(std::nullptr_t) {
            DecRefCount();
            Instance = nullptr;
            return *this;
        }

        template<typename U>
        Ref& operator=(const Ref<U>& other) {
            static_assert(!IsComplete<T>::value || std::is_base_of<T, U>::value, "Invalid conversion");
            DecRefCount();
            Instance = other.Instance;
            IncRefCount();
            return *this;
        }

        template<typename U>
        Ref& operator=(Ref<U>&& other) {
            static_assert(!IsComplete<T>::value || std::is_base_of<T, U>::value, "Invalid conversion");
            DecRefCount();
            Instance       = other.Instance;
            other.Instance = nullptr;
            return *this;
        }

        explicit operator bool() const {
            return Instance != nullptr;
        }

        template<typename U>
        bool operator==(const Ref<U>& other) const {
            return other.Instance == Instance;
        }

        T* operator->() {
            return Instance;
        }

        const T* operator->() const {
            return Instance;
        }

        T& operator*() {
            return *Instance;
        }

        const T& operator*() const {
            return *Instance;
        }

        Ref<T>& operator=(const Ref<T>& other) {
            DecRefCount();
            Instance = other.Instance;
            IncRefCount();
            return *this;
        }

        Ref<T>& operator=(Ref<T>&& other) {
            Instance       = other.Instance;
            other.Instance = nullptr;
            return *this;
        }

        T* Raw() {
            return Instance;
        }

        const T* Raw() const {
            return Instance;
        }

        template<typename... Args>
        static Ref<T> Create(Args&&... args) {
            return Ref<T>(new T(std::forward<Args>(args)...));
        }

        template<typename U>
        Ref<U> As() const {
            static_assert(!IsComplete<T>::value || std::is_base_of<U, T>::value || std::is_base_of<T, U>::value,
                          "Invalid conversion");
            return Ref<U>((U*)Instance);
        }
    public:
        void IncRefCount() const {
            if (Instance)
                Instance->IncRefCount();
        }

        void DecRefCount() {
            if (Instance) {
                Instance->DecRefCount();
                if (Instance->GetRefCount() <= 0) {
                    delete Instance;
                    Instance = nullptr;
                }
            }
        }
    private:
        T* Instance = nullptr;
    };

}
