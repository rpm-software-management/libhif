// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef DNFDAEMON_SERVER_THREADS_MANAGER_HPP
#define DNFDAEMON_SERVER_THREADS_MANAGER_HPP

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>


class ThreadsManager {
public:
    ThreadsManager();
    virtual ~ThreadsManager();
    void register_thread(std::thread && thread);
    void mark_thread_finished(std::thread::id thread_id);
    void current_thread_finished() { mark_thread_finished(std::this_thread::get_id()); };
    void join_threads(const bool only_finished);
    void finish();

private:
    std::mutex running_threads_mutex;
    // flag whether to break the finished threads collector infinite loop
    std::atomic<bool> finish_collector{false};
    // thread that joins finished worker threads
    std::thread running_threads_collector;
    // vector of started worker threads
    std::vector<std::thread> running_threads{};
    // vector of finished threads id
    std::vector<std::thread::id> finished_threads{};
};

#endif
