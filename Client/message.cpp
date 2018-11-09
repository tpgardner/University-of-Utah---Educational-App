#include <message.h>

Message::Message(std::string cmd, std::string pl, unsigned short num) {
    command = cmd;
    payload = pl;
    port = num;
    sessionid = "";
}

Message::Message() {
    command = "";
    payload = "";
    sessionid = "";
    port = 0;
}

Message::~Message() {
}

void Message::addSessionId(sf::String sess) {
    sessionid = sess;
}

std::string Message::tostring() {
    return command + payload + ("" + port) + sessionid;
}

sf::Packet& operator <<(sf::Packet& packet, Message& msg) {
    return packet << msg.command << msg.payload << msg.port << msg.sessionid;
}

sf::Packet& operator >>(sf::Packet& packet, Message& msg) {
    return packet >> msg.command >> msg.payload >> msg.port >> msg.sessionid;
}
