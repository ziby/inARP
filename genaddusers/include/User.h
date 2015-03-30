#ifndef USER_H
#define USER_H
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <crypt.h>
#include <string.h>

#include <pwd.h>
#include <shadow.h>
#include <grp.h>

#include <unistd.h>
#include <sys/types.h>

#include "Logger.h"

class User
{
    public:

        User(std::string login, std::string name);
        std::string GetLogin() { return m_login; }
        std::string GetName() { return m_name; }
        std::string GetPassword() { return m_password; }
        std::string GetHomeDir() { return m_home_dir; }
        static std::vector<User> ReadFromFile(char *path, Logger *logger);
        static void AddFewUsers(std::vector<User>, Logger *logger);
        std::string ToString();
        void PassGen(unsigned int length);

    protected:
    private:

        static const std::string md5_prefix;
        static const std::string end_salt;
        static const std::string pathpassgen;

        static const int lstchg;
        static const int passMin;
        static const int passMax;
        static const int passWarn;

        static const int basicUid;
        static const int basicGid;

        static const unsigned int defaultLength;

        std::string m_login;
        std::string m_name;
        std::string m_noncrypt_password;
        std::string m_password;
        std::string m_home_dir;

};

#endif // USER_H
