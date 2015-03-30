#include "userlib.h"

char *md5crypt(char *password, char *salt)
{
    return NULL;
}

int useradd (string login, string password, string name)
{
    passwd add_passwd; // cтруктура для хранения строки passwd
    add_passwd.pw_name = login.c_str();

    char salt[total_salt_len]; // в стеке выделяется память под соль
    // формирование соли с префиксом, определяющим алгоритм шифрования - md5
    // используя функцию crypt()
    strncpy(salt, md5_prefix);
    strncpy((char *)(salt + md5_prefix_len), salt_default);
    strncpy((char *)(salt + md5_prefix_len + salt_len), salt_end);
    add_passwd.pw_passwd = crypt(password.c_str(), salt);

    add_passwd.gid = pw_gid_default;
    add_passwd.pw_gecos = name.c_str();

    char home_dir[100];
    strncpy(home_dir, home_dir_default);
    add_passed.pw_dir = home_dir;

    char shell[100];
    strncpy(shell, shell_default);
    add_passed.pw_shell = shell;

    add_passwd.pw_uid = start_uid;
    while (getpwuid(add_passwd.pw_uid) != NULL)
    {
        add_passwd.pw_uid++;
    }
    return 1;
}

int userdel (string login)
{
    return 1;
}

int userdel (uid_t uid)
{
    return 1;
}
