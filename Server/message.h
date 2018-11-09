#ifndef MESSAGE_H
#define MESSAGE_H
#include <SFML/Network/Packet.hpp>
#include <SFML/System/String.hpp>
#include <string>

/**
 * @brief The Message class is a message for the Stackd game API server to communicate with the
 * client.
 */
class Message{

public:
    sf::String command;
    sf::String payload;
    sf::Uint16 numerical;
    sf::String sessionid;
    Message(std::string cmd, std::string pl, unsigned short);
    Message();
    ~Message();
    std::string tostring();
    void addSessionId(sf::String);
    friend sf::Packet& operator <<(sf::Packet& packet, Message& msg);
    friend sf::Packet& operator >>(sf::Packet& packet, Message& msg);
};

#endif
