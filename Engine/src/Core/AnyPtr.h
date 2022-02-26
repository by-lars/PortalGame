#pragma once

#include "Core/Base.h"

namespace Engine {
        struct AnyPtr {
            AnyPtr() {
                ENGINE_DEBUG("DEFAULT YEYE");
                Pointer = nullptr;
                Destroy = nullptr;
                TypeName = nullptr;
            }

            ~AnyPtr() {
                if (Destroy != nullptr) {
                    Destroy(Pointer);
                }
            }

            AnyPtr(const void* _pointer, void(*_destroy)(const void*), const char* _typename) {
                Pointer = _pointer;
                Destroy = _destroy;
                TypeName = _typename;
            }

            AnyPtr(AnyPtr&& other) noexcept {
                Pointer = other.Pointer;
                Destroy = other.Destroy;
                TypeName = other.TypeName;

                other.Pointer = nullptr;
                other.Destroy = nullptr;
                other.TypeName = nullptr;
            }

            AnyPtr& operator=(AnyPtr&& other) noexcept {
                if (this != &other) {
                    Pointer = other.Pointer;
                    Destroy = other.Destroy;
                    TypeName = other.TypeName;

                    other.Pointer = nullptr;
                    other.Destroy = nullptr;
                    other.TypeName = nullptr;
                }

                return *this;
            }

            const char* TypeName;
            const void* Pointer;
            void(*Destroy)(const void*);
        };


        template<typename T>
        AnyPtr MakeAnyPtr() {
            AnyPtr ptr{
                (const void*) new T(),
                [](const void* x) { static_cast<const T*>(x)->~T(); },
                typeid(T).name()
            };

            return ptr;
        }

        template<typename T> 
        AnyPtr ToAnyPtr(T* pointer) {
            AnyPtr ptr{
                (const void*)pointer,
                [](const void* x) { static_cast<const T*>(x)->~T(); },
                 typeid(T).name()
            };

            return ptr;
        }

        template<typename T>
        T* AnyCast(AnyPtr& ptr) {
            ENGINE_ASSERT(strcmp(ptr.TypeName, typeid(T).name()) == 0, "Bad Cast Any Cast - " << ptr.TypeName << " != " << typeid(T).name());
            return (T*)ptr.Pointer;
        }
}