#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

template<typename Event, std::size_t Capacity = 16>
class MulticastDelegate {
public:
    using Handle = std::uint64_t;

    static constexpr Handle InvalidHandle = 0;

    MulticastDelegate() = default;

    MulticastDelegate(const MulticastDelegate&) = delete;
    MulticastDelegate& operator=(const MulticastDelegate&) = delete;

    template<typename T, void (T::*Method)(const Event&)>
    Handle Subscribe(T& object) {
        for (Slot& slot : slots_) {
            if (slot.handle != InvalidHandle) {
                continue;
            }

            if (nextHandle_ == InvalidHandle) {
                throw std::overflow_error("Delegate handle overflow.");
            }

            const Handle handle = nextHandle_++;

            slot = {
                handle,
                &object,
                [](void* receiver, const Event& event) {
                    (static_cast<T*>(receiver)->*Method)(event);
                }
            };

            return handle;
        }

        throw std::logic_error("Delegate subscription capacity exceeded.");
    }

    bool Unsubscribe(Handle handle) {
        if (handle == InvalidHandle) {
            return false;
        }

        for (Slot& slot : slots_) {
            if (slot.handle == handle) {
                slot = {};
                return true;
            }
        }

        return false;
    }

    void Broadcast(const Event& event) {
        std::array<Handle, Capacity> handles{};

        for (std::size_t i = 0; i < Capacity; ++i) {
            handles[i] = slots_[i].handle;
        }

        for (std::size_t i = 0; i < Capacity; ++i) {
            const Handle handle = handles[i];

            if (handle == InvalidHandle) {
                continue;
            }

            if (slots_[i].handle != handle) {
                continue;
            }

            const Slot slot = slots_[i];
            slot.callback(slot.receiver, event);
        }
    }

private:
    struct Slot {
        Handle handle = InvalidHandle;
        void* receiver = nullptr;
        void (*callback)(void*, const Event&) = nullptr;
    };

    std::array<Slot, Capacity> slots_{};
    Handle nextHandle_ = 1;
};