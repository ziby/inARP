#ifndef USERLIB_H_INCLUDED
#define USERLIB_H_INCLUDED

#define _XOPEN_SOURCE
#include <unistd.h>

#include <string>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>

const int pw_gid_default = 100;
const char home_dir_default[] = "/home";
const char shell_default[] = "/bin/bash";
//const int pass_len = 8;

const char md5_prefix[] = "$1$";
const int md5_prefix_len = 3;

const char salt_default[] = "12345678";
const int salt_len = 8;

const char salt_end[] = "$";
const int salt_end_len = 1;

const int start_uid = 1010;

const int total_salt_len = md5_prefix + salt_len + salt_end_len;

int useradd (string login, string password, string name);

int userdel (string login);

int userdel (uid_t uid);

#endif // USERLIB_H_INCLUDED
