#pragma once

#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include <map>
#include <queue>
#include <cassert>

#include "observer.h"
#include "subject.h"
#include "utility.h"

using handle_t = size_t;

struct Context {
    Context() = default;
    Context(size_t N):
        m_reader(N)
    {}

    std::string                     m_data;
    bool                            m_isValid = false;
    std::shared_ptr<Registrator>    m_registrator = nullptr;
    Reader                          m_reader;
};

// 2. Manager class
class Manager {
public:
        Manager();
        ~Manager();

        static Manager* getInstance();

        handle_t start(size_t N);
        void work(handle_t handle, const char *data, std::size_t size);
        void stop(handle_t handle);

#ifdef TEST_MODE
        std::vector<std::string> m_testDataFileList;
#endif

private:
        std::vector<std::string> parseData(std::string);

private:

        std::mutex              m_mutex;
        static Manager*         m_instance;

        std::vector<Context>    m_contexts;
        std::queue<size_t>      m_freeContextIDs;
};
