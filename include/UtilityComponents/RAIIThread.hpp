#pragma once

#include <thread>

/// A wrapper for std::thread which can't be detached, and which will always
/// join in its destructor (if the thread is joinable).
class RAIIThread final {
public:
    RAIIThread() noexcept = default;

    template <class Fn, class... Args>
    explicit RAIIThread(Fn&& fn, Args&&... args)
            : thread(std::forward<Fn>(fn), std::forward<Args>(args)...) {
    }

    RAIIThread(const RAIIThread&) = delete;
    RAIIThread(RAIIThread&&) noexcept = default;

    ~RAIIThread() noexcept;

    RAIIThread& operator=(const RAIIThread&) = delete;
    RAIIThread& operator=(RAIIThread&&) noexcept = default;

    std::thread::id get_id() const noexcept;

    bool joinable() const noexcept;
    void join();

    void swap(RAIIThread& x) noexcept;

private:
    std::thread thread;
};

void swap(RAIIThread& x, RAIIThread& y) noexcept;