#include "manager.h"

constexpr char escChar = '\n';

Manager::Manager()
{}

Manager::~Manager()
{
}

Manager* Manager::m_instance = nullptr;

Manager *Manager::getInstance()
{
    if(m_instance == nullptr)
        m_instance = new Manager();
    return m_instance;
}

handle_t Manager::start(size_t N)
{
    {
       std::lock_guard<std::mutex> guard{m_mutex};
       Context data(N);
       m_contexts.push_back(std::move(data));
       m_contexts.back().m_registrator = std::make_shared<Registrator>();
       m_contexts.back().m_reader.addObserver(m_contexts.back().m_registrator);
    }

   if(!m_freeContextIDs.empty()) {
       size_t temp = m_freeContextIDs.front();
       m_freeContextIDs.pop();
       return temp;
   }

   return m_contexts.size() - 1;
}

void Manager::work(handle_t handle, const char *data, std::size_t size)
{
    assert(handle <= m_contexts.size());
    std::unique_lock<std::mutex> guard{m_mutex};
    std::string commands = std::string(data, size);
    m_contexts.at(handle).m_data += commands;

    guard.unlock();

    if(commands[commands.size() - 1] == escChar)
    {
        m_contexts.at(handle).m_isValid = true;
        auto preparedData = parseData(m_contexts.at(handle).m_data);
        for(auto& command: preparedData)
            m_contexts.at(handle).m_reader.readCommands(command);
        guard.lock();
        m_contexts.at(handle).m_data.clear();
    }
}

void Manager::stop(handle_t handle)
{
    std::lock_guard<std::mutex> guard{m_mutex};
    std::string stopStr("");
    m_contexts.at(handle).m_reader.readCommands(stopStr);
    m_contexts.at(handle) = Context();
    m_freeContextIDs.push(handle);
}

std::vector<std::string> Manager::parseData(std::string data)
{
    size_t pos = 0;
    std::string token;
    std::vector<std::string> result;
    while((pos = data.find(escChar)) != std::string::npos) {
        token = data.substr(0, pos);
        result.push_back(token);
        data.erase(0, pos + 1);
    }
    return result;
}
