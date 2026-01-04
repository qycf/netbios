#pragma once

// 辅助宏用于正确拼接标识符
#define CONCAT(a, b) a##b
#define CONCAT3(a, b, c) a##b##c
// 支持命名空间和函数名的宏定义
#define DECLARE_NAMESPACE_DETOUR(namespace_name, func_name) \
    extern "C" void CONCAT3(namespace_name, _Detour, func_name)(); \
    extern "C" void*&CONCAT3(namespace_name, _fn, func_name) = namespace_name::CONCAT(fn, func_name); \
    extern "C" void (&CONCAT3(namespace_name, _Indirect, func_name))(DWORD_PTR) = namespace_name::CONCAT(Indirect, func_name); \
    void (&namespace_name::CONCAT(Detour, func_name))() = ::CONCAT3(namespace_name, _Detour, func_name); 

#define DECLARE_NAMESPACE_BOOL_DETOUR(namespace_name, func_name) \
    extern "C" void CONCAT3(namespace_name, _Detour, func_name)(); \
    extern "C" void*&CONCAT3(namespace_name, _fn, func_name) = namespace_name::CONCAT(fn, func_name); \
    extern "C" bool (&CONCAT3(namespace_name, _Indirect, func_name))(DWORD_PTR) = namespace_name::CONCAT(Indirect, func_name); \
    void (&namespace_name::CONCAT(Detour, func_name))() = ::CONCAT3(namespace_name, _Detour, func_name); 