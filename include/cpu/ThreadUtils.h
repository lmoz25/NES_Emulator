#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>

static std::condition_variable thread_waker;
static std::mutex mtx;