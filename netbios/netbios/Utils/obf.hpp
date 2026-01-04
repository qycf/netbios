#pragma once

#include <array>
#include <string>
#include <atomic>
#include <mutex>
#include <type_traits>

namespace obf {

    // 移除const和引用修饰
    template<class Ty>
    using clean_type = std::remove_const_t<std::remove_reference_t<Ty>>;

    // 生成随机密钥 (编译期常量)
    constexpr uint8_t generate_key(size_t idx) noexcept {
        return static_cast<uint8_t>(0x9A + (idx * 0x37) % 0xFF);
    }

    // 主模板
    template<typename CharT, size_t S, uint8_t Key1, uint8_t Key2>
    class XorString {
        std::array<CharT, S> storage;
        mutable std::atomic<CharT*> cached{ nullptr };

    public:
        constexpr XorString(const CharT(&str)[S]) noexcept {
            for (size_t i = 0; i < S; ++i) {
                storage[i] = str[i] ^ (Key1 + (i % (1 + Key2)));
            }
        }

        const CharT* get() const noexcept {
            if (auto ptr = cached.load(std::memory_order_acquire); ptr) {
                return ptr;
            }

            static std::mutex mtx;
            std::lock_guard lock(mtx);
            if (auto ptr = cached.load(std::memory_order_relaxed); !ptr) {
                CharT* buf = new CharT[S];
                for (size_t i = 0; i < S; ++i) {
                    buf[i] = storage[i] ^ (Key1 + (i % (1 + Key2)));
                }
                buf[S - 1] = '\0';
                cached.store(buf, std::memory_order_release);
            }

            return cached.load(std::memory_order_relaxed);
        }

        ~XorString() {
            if (auto ptr = cached.load(std::memory_order_relaxed)) {
                delete[] ptr;
            }
        }

        XorString(const XorString&) = delete;
        XorString& operator=(const XorString&) = delete;
    };

    // 自动推导辅助
    template<typename CharT, size_t N>
    constexpr auto MakeXorString(const CharT(&str)[N]) noexcept {
        return XorString<CharT, N, generate_key(0), generate_key(1)>(str);
    }
}

// 使用宏定义
#define obf(str) obf::MakeXorString(str).get()