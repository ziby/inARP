#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <vector>
#include <string>

class Logger
{
    public:
        static const unsigned int ERROR;
        static const unsigned int SUCCESS;

        Logger();
        void AddLog(unsigned int status, std::string message);
        void PrintError();
        void PrintSuccess();
    protected:
    private:
        std::vector<std::string> m_logs;
        std::vector<unsigned int> m_status;
};

#endif // LOGGER_H
