#include "UtilityComponents/RAIIThread.hpp"

RAIIThread::~RAIIThread() noexcept {
    if (thread.joinable()) {
        thread.join();
    }
}

std::thread::id RAIIThread::get_id() const noexcept {
    return thread.get_id();
}

bool RAIIThread::joinable() const noexcept {
    return thread.joinable();
}

void RAIIThread::join() {
    thread.join();
}

void RAIIThread::swap(RAIIThread& x) noexcept {
    thread.swap(x.thread);
}

void swap(RAIIThread& x, RAIIThread& y) noexcept {
    x.swap(y);
}