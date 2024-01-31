/* Copyright (C) 2023 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope_threading
 */

#pragma once

#include <atomic>

namespace hope::log {

    template<typename TItem>
    class logger_mpsc_queue final {
    public:
        explicit logger_mpsc_queue() {
            m_head = m_tail = m_buffer_head = new node();
        }

        ~logger_mpsc_queue() {
            while (m_tail != nullptr) {
                auto* cur_node = m_tail;
                m_tail = m_tail->next;

                delete cur_node;
            }
        }

        template<typename T>
        void enqueue(T&& in_value) {
            node* new_node = new node(std::forward<T>(in_value));;
            auto* old_head = m_head.exchange(new_node);
            old_head->next = new_node;
        }

        bool try_dequeue(TItem& v) {
            auto* popped = m_tail->next;

            if (popped) {
                v = std::move(popped->value);
                auto* old_tail = m_tail;
                m_tail = popped;
                m_tail->value = { };
                delete old_tail;
                return true;
            }

            return false;
        }

    private:
        // internal node structure 
        struct node final {
            template<typename T = TItem>
            explicit node(T&& in_value = T{})
                : value(std::forward<T>(in_value)) { }

            node* volatile next = nullptr;
            TItem value;
        };

        // consumer part 
        // accessed mainly by consumer, infrequently be producer 
        node* m_tail = nullptr; // tail of the queue 

        // cache line size on modern x86 processors (in bytes) 
        constexpr static std::size_t CacheLineSize = 64;

        // delimiter between consumer part and producer part
        // is needed to put this parts to the different cache lines
        const uint8_t m_cache_line_pad_[CacheLineSize]{};

        // producer part 
        // accessed only by producer 
        std::atomic<node*> m_buffer_head = nullptr;
        std::atomic<node*> m_head = nullptr; // head of the queue 
    };
}