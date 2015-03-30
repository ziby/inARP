#include "Logger.h"
using namespace std;

const unsigned int Logger::ERROR = 1;
const unsigned int Logger::SUCCESS = 0;

Logger::Logger()
{
}

void Logger::AddLog(unsigned int status, string message)
{
    m_logs.push_back(message);
    m_status.push_back(status);
}

void Logger::PrintError()
{
    for (int i = 0; i < m_logs.size(); i++)
        if (m_status[i] == ERROR) cerr << m_logs[i] << endl;
}

void Logger::PrintSuccess()
{
    for (int i = 0; i < m_logs.size(); i++)
        if (m_status[i] == SUCCESS) cout << m_logs[i] << endl;
}
