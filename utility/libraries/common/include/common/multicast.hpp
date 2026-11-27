/**
 * @brief
 * Multi-listener event wrapper parameterized by a full `std::function<void(Args...)>` type.
 * Subscribe with `<<` (or `subscribe`), unsubscribe with `>>` (or `unsubscribe`).
**/

#pragma once

#include "headers.hpp"
#include <concepts>
#include <type_traits>

namespace Common {
    // Primary template left undefined: only `std::function<void(Args...)>` types are accepted
    template <typename StdFunctionType>
    class Multicast;

    template <typename... Args>
    class Multicast<std::function<void(Args...)>> {
        static_assert(
            (!std::is_rvalue_reference_v<Args> && ...),
            "Multicast cannot safely broadcast rvalue-reference (T&&) parameters to multiple listeners."
        );

        public:
            using Handler = std::function<void(Args...)>;
            using ListenerId = uint32_t;

        private:
            struct Entry {
                ListenerId id = 0;
                Handler fn = nullptr;
            };

            std::vector<Entry> listeners;
            ListenerId nextId = 1;

        public:
            Multicast() = default;
            Multicast(std::nullptr_t) noexcept {} // Allows `= nullptr` default member initializers

            // Preserve struct copy/move (e.g. moving `DriverConfigs` into `JugDriver`),
            // while disallowing `event = handler` or `event = nullptr` at compile time
            Multicast(const Multicast&) = default;
            Multicast(Multicast&&) noexcept = default;
            Multicast& operator=(const Multicast&) = default;
            Multicast& operator=(Multicast&&) noexcept = default;

            ListenerId subscribe(Handler fn) {
                if (fn == nullptr) {
                    return 0;
                }
                const ListenerId id = this->nextId++;
                this->listeners.push_back(Entry{.id = id, .fn = std::move(fn)});
                return id;
            }

            bool unsubscribe(ListenerId id) noexcept {
                if (id == 0) {
                    return false;
                }
                return std::erase_if(this->listeners, [id](const Entry &e) {
                    return e.id == id;
                }) > 0;
            }

            void clear() noexcept {
                this->listeners.clear();
            }

            void operator()(Args... args) const {
                for (const Entry &e : this->listeners) {
                    e.fn(args...);
                }
            }

            [[nodiscard]] bool empty() const noexcept {
                return this->listeners.empty();
            }
            [[nodiscard]] explicit operator bool() const noexcept {
                return !this->listeners.empty();
            }

            // Subscribe via `<<`: returns ListenerId (safe to ignore, or capture with `auto id = event << fn;`)
            template <typename F>
                requires (!std::same_as<std::remove_cvref_t<F>, Multicast>) &&
                         std::constructible_from<Handler, F>
            ListenerId operator<<(F &&fn) {
                return this->subscribe(Handler(std::forward<F>(fn)));
            }

            // Unsubscribe via `>>`: supports `event >> id;` and chaining `event >> id1 >> id2;`
            Multicast& operator>>(ListenerId id) noexcept {
                this->unsubscribe(id);
                return *this;
            }

            // `event = nullptr;` clears (empties) all registered listeners
            Multicast& operator=(std::nullptr_t) noexcept {
                this->empty();
                return *this;
            }

            friend bool operator==(const Multicast &ev, std::nullptr_t) noexcept {
                return ev.empty();
            }
            friend bool operator!=(const Multicast &ev, std::nullptr_t) noexcept {
                return !ev.empty();
            }
    };
}
