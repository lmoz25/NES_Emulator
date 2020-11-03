#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>

std::condition_variable thread_waker;
std::mutex mtx;