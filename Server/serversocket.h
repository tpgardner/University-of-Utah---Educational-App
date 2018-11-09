#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H
#include <QPair>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network.hpp>
#include <message.h>
#include <exception>
#include <exceptions.h>
#include <iostream>

/**
 * @brief The ServerSocket class is a socket for the Stackd game API server to communicate
 * with a client.
 */
class ServerSocket
{
public:

    ServerSocket(sf::IpAddress, unsigned int);
    ServerSocket();
    ~ServerSocket();
    QPair<Message,sf::IpAddress> waitForResponse();
    sf::String getHostname();
    unsigned int getPortnumber();
    bool sendPayload(sf::String);
    void bind(unsigned int);
private:
    unsigned int portnumber = 0;
    sf::IpAddress host;
    sf::TcpListener listener;
};

#endif // SERVERSOCKET_H

