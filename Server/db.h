#ifndef DB_H
#define DB_H
#include <string>
#include <mysql.h>

/**
 * @brief The DB class interfaces the server and a MySQL database for the Stackd game API.
 */
class DB
{
    std::string regUser(MYSQL* connection, std::string payload);
    std::string getStudents(MYSQL* connection, std::string payload);
    std::string authenticate(MYSQL* connection, std::string payload);
    std::string teachAuthenticate(MYSQL* connection, std::string payload);
    std::string puzzleSolved(MYSQL* connection, std::string payload);
    std::string studentlist(MYSQL* connection, std::string payload);
    std::string solvedPuzzles(MYSQL* connection, std::string payload);
    std::string deleteStudent(MYSQL* connection, std::string payload);
public:
    DB();
    std::string executeCommand(std::string command, std::string payload);
};

#endif // DB_H
