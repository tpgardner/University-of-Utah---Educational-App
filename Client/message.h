#ifndef MESSAGE_H
#define MESSAGE_H
#include <SFML/Network/Packet.hpp>
#include <SFML/System/String.hpp>
#include <string>

/**
 * @brief The Message class is used by the Stackd game API client to communicate with the server.
 */
class Message{

public:
    sf::String command;
    sf::String payload;
    sf::Uint16 port;
    sf::String sessionid;
    Message(std::string cmd, std::string pl, unsigned short);
    Message();
    ~Message();
    std::string tostring();
    void addSessionId(sf::String);
    //small utility class to clean up code.
    friend sf::Packet& operator <<(sf::Packet& packet, Message& msg);
    friend sf::Packet& operator >>(sf::Packet& packet, Message& msg);
};


#endif
