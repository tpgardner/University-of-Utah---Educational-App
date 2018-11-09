#ifndef SERVER_H
#define SERVER_H
#include <QtSql/QSqlDatabase>
#include <QtConcurrent/QtConcurrent>
#include <QPair>
#include <QVector>
#include <QFuture>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/String.hpp>
#include <sstream>
#include <iostream>
#include <serversocket.h>
#include <message.h>
#include <mutex>
#include <db.h>

/**
 * @brief The Server class is a hybrid TCP/UDP server for for Stackd game API clients.
 * It is the top level class of the Stackd API's server structure.
 */
class Server
{
    ServerSocket listener;
    DB database;
    unsigned int rport;
    std::vector<sf::String> sessionids;
    void decode(Message, sf::IpAddress);
    bool verifysid(sf::String);
    void tryToSend(unsigned int, ServerSocket&, sf::String);
    void deleteSessionId(sf::String);
    std::string RandomString(unsigned int);
    std::mutex mute;

public:
    Server(int portnumber);
    ~Server();
    void listen();
};

#endif // SERVER_H
