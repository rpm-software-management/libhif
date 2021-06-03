// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "threads_manager.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>

ThreadsManager::ThreadsManager() {
    // collecting finished worker threads
    running_threads_collector = std::thread([this]() {
        while (!finish_collector) {
            join_threads(true);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

ThreadsManager::~ThreadsManager() {}

void ThreadsManager::register_thread(std::thread && thread) {
    std::lock_guard<std::mutex> lock(running_threads_mutex);
    running_threads.emplace_back(std::move(thread));
}

void ThreadsManager::mark_thread_finished(std::thread::id thread_id) {
    std::lock_guard<std::mutex> lock(running_threads_mutex);
    finished_threads.emplace_back(std::move(thread_id));
}

void ThreadsManager::join_threads(const bool only_finished) {
    std::vector<std::thread> to_be_joined{};

    {
        std::lock_guard<std::mutex> lock(running_threads_mutex);
        for (auto thread = running_threads.begin(); thread < running_threads.end();) {
            auto in_finished = std::find(finished_threads.begin(), finished_threads.end(), thread->get_id());
            if (thread->joinable() && (!only_finished || (in_finished != finished_threads.end()))) {
                to_be_joined.push_back(std::move(*thread));
                running_threads.erase(thread);
                if (in_finished != finished_threads.end()) {
                    finished_threads.erase(in_finished);
                }
            } else {
                ++thread;
            }
        }
    }

    for (auto thread = to_be_joined.begin(); thread < to_be_joined.end(); ++thread) {
        // join the thread and remove it from registry
        thread->join();
    }
}

void ThreadsManager::finish() {
    if (!running_threads_collector.joinable()) {
        return;
    }
    // join all threads
    finish_collector = true;
    join_threads(false);
    running_threads_collector.join();
}
