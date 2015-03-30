#include "User.h"
#include <stdlib.h>

using namespace std;

const string User::md5_prefix = "$1$";
const string User::end_salt = "$";
const string User::pathpassgen = "./passgen.sh";

const int User::basicGid = 65555;
const int User::basicUid = 65555;
const int User::lstchg = 17000;
const int User::passMin = 0;
const int User::passMax = 100;
const int User::passWarn = 7;

const unsigned int User::defaultLength = 8;

User::User(string login, string name)
{
    m_login = login;
    m_name = name;
    PassGen(defaultLength);
    m_home_dir = "/home/" + login;
}

string User::ToString()
{
    return m_login + "; " + m_name + "; " + m_noncrypt_password + " ; " + m_home_dir;
}

void User::PassGen(unsigned int length)
{
    stringstream cmd;
    cmd << pathpassgen << " -c 2 -l " << length;
    FILE *pfd = popen (cmd.str().c_str(), "r");
    char *ppass = (char *)calloc(length, sizeof(char));
    fscanf (pfd, "%s", ppass);
    m_noncrypt_password = string(ppass);
    char *psalt = (char *)calloc(length, sizeof(char));
    fscanf (pfd, "%s", psalt);
    m_password = crypt(ppass, (md5_prefix + string(psalt) + end_salt).c_str());
}

vector<User> User::ReadFromFile(char *path, Logger* plogger)
{
    ifstream fin(path);
    if (fin.fail())
    {
        if (plogger != NULL) plogger->AddLog (Logger::ERROR, string ("Ошибка открытия файла ") + path);
        throw std::exception();
    }
    char buffer = 0;

    vector<User> users;
    while (!fin.eof()) {

        string login ="";
        string name = "";

        while (!fin.eof() && buffer != ';')
        {
            if (buffer != 0) login += buffer;
            fin >> buffer;
            if (buffer == ';') {
                fin >> buffer;
                break;
            }
        }

        while (!fin.eof() && buffer != ';')
        {
            name += buffer;
            fin >> buffer;
            if (buffer == ';') {
                fin >> buffer;
                break;
            }
        }
        if (login == "") {
            if (plogger != NULL)
            {
                stringstream ss;
                ss << (users.size() + 1);
                plogger->AddLog(Logger::ERROR, "Для пользователя №" + ss.str() + "не задан логин");
            }
            continue;
        }
        users.push_back(User(login, name));
    }
    return users;
}


void User::AddFewUsers(vector<User> users, Logger *plogger)
{
    vector<passwd> usersData;
    vector<spwd> usersPass;
    vector<group> usersGroup;

    for (unsigned int i = 0 ; i < users.size(); i++)
    {
        //заполнение структуры для passwd
        passwd userData;
        userData.pw_name = (char *) users[i].GetLogin().c_str();
        userData.pw_passwd = (char *) users[i].GetPassword().c_str();
        userData.pw_uid = basicUid + i;
        userData.pw_gid = basicGid + i;
        userData.pw_gecos = (char *) users[i].GetName().c_str();
        userData.pw_dir = (char *) users[i].GetHomeDir().c_str();
        userData.pw_shell = "/bin/bash";
        usersData.push_back(userData);

        //заполнение структуры для shadow
        spwd userPass;
        userPass.sp_namp = (char *) users[i].GetLogin().c_str();
        userPass.sp_pwdp = (char *) users[i].GetPassword().c_str();
        userPass.sp_lstchg = lstchg;
        userPass.sp_min = passMin;
        userPass.sp_max = passMax;
        userPass.sp_warn = passWarn;
        usersPass.push_back(userPass);

        // заполнение структуры для group
        group userGroup;
        userGroup.gr_gid = basicGid + i;
        userGroup.gr_name = (char *) users[i].GetName().c_str();
        userGroup.gr_passwd = (char *) users[i].GetPassword().c_str();
        userGroup.gr_mem = 0;
        usersGroup.push_back(userGroup);
    }

        FILE* ppassw;
        if ((ppassw = fopen("/etc/passwd","a")) == NULL)
        {
            plogger->AddLog(Logger::ERROR, "/etc/passwd не был открыт.");
            throw exception();
        }

        FILE* pshadw;
        if ((pshadw = fopen("/etc/shadow","a")) == NULL)
        {
            if (plogger != NULL) plogger->AddLog(Logger::ERROR, "/etc/shadow не был открыт");
            fclose(ppassw);
            throw exception();
        }

        FILE* pgroup;
        if ((pgroup = fopen("/etc/group","a")) == NULL )
        {
            if (plogger != NULL) plogger->AddLog(Logger::ERROR, "/etc/group не был открыт");
            fclose(ppassw);
            fclose(pshadw);
            throw exception();
        }

        for (unsigned int i = 0 ; i < users.size(); i ++)
        {
            putpwent(&usersData[i], ppassw);
            putspent(&usersPass[i], pshadw);
            putgrent(&usersGroup[i], pgroup);
            if (plogger != NULL) plogger->AddLog(Logger::SUCCESS, users[i].ToString());
        }
        fclose(ppassw);
        fclose(pshadw);
        fclose(pgroup);
}
