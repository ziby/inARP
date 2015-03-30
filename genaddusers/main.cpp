#include <User.h>
#include <Logger.h>
#include <stdlib.h>

using namespace std;

void usage()
{
    cout << "addusers - утилита для пакетного добавления пользователей" << std::endl
    << "adduser [file], где file - путь до файла, в котором хранится информация о пользователе в виде:" << std::endl
    << "login1; name1;" << std::endl << "login2; name2" << std::endl << "..." << std::endl;
}

int main(int argc, char *argv[])
{

    if (geteuid ())
    {
		fprintf (stderr, "Команда запущена не от имени суперпользователя\n" );
		return 0;
        // проверка наличия необходимых прав
    }

    if (argc != 2)
    {
        cout << "Неверное количество аргументов" << endl;
        usage();
        return 0;
    }
    if (string (argv[1]) == "-h") {
        usage();
        return 0;
    }

    Logger *logger = new Logger();
    vector<User> users;
    try
    {
        users = User::ReadFromFile(argv[1], logger);
    }
    catch (exception e)
    {
        logger->PrintError();
        return 1;
    }

    try
    {
        User::AddFewUsers (users, logger);
    }
    catch(exception e)
    {
        logger->PrintError();
        return 1;
    }

    logger->PrintError();
    logger->PrintSuccess();

    delete logger;

    return 0;
}
