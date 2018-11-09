#include <message.h>
//small data class to clean up code.

Message::Message(std::string cmd, std::string pl, unsigned short num) {
    command = cmd;
    numerical = num;
    payload = pl;
    sessionid = "";
}

Message::Message() {
    command = "";
    numerical = 0;
    payload = "";
    sessionid = "";
}

Message::~Message() {
}

std::string Message::tostring() {
    return command + payload + ("" + numerical);
}

void Message::addSessionId(sf::String sess) {
    sessionid = sess;
}

sf::Packet& operator <<(sf::Packet& packet, Message& msg) {
    return packet << msg.command << msg.payload << msg.numerical << msg.sessionid;
}

sf::Packet& operator >>(sf::Packet& packet, Message& msg) {
    return packet >> msg.command >> msg.payload >> msg.numerical >> msg.sessionid;
}
