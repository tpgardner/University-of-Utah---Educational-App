#include "usersocket.h"

UserSocket::UserSocket(std::string hostname, unsigned int portnumber) {
    this->host = sf::IpAddress(hostname);
    this->portnumber = portnumber;
}

UserSocket::UserSocket(sf::IpAddress hostname, unsigned int portnumber) {
    this->host = hostname;
    this->portnumber = portnumber;
}

//you can use this to reuse a session id
UserSocket::UserSocket(sf::IpAddress hostname, unsigned int portnumber, sf::String sessionId) {
    this->sessionId = sessionId;
    this->host = hostname;
    this->portnumber = portnumber;
    this->authenticated = true;
}

UserSocket::~UserSocket() {
}

sf::String UserSocket::sid() {
    return sessionId;
}

//sends an authentication packet to the sever.
//if no exceptions are thrown, we are authenticated
void UserSocket::authenticate(std::string username, std::string passwd) {
    if (!this->authenticated) {
        sf::TcpSocket socket;
        sf::Socket::Status status = socket.connect(host, portnumber);

        if (status == sf::Socket::Status::Error) {
            socketexception ex(host.toString(), portnumber);
            throw ex;
        } else {
            sf::TcpListener li;
            li.listen(sf::TcpListener::AnyPort);
            sf::Packet pack;
            Message msg("authenticate", username + "," + passwd, li.getLocalPort());
            pack << msg;
            socket.send(pack);
            Message results = waitForResponse(li);

            if (results.payload != "AUTHFAILURE") {
                sessionId = results.payload;
                std::cout << sessionId.toAnsiString() << std::endl;
                this->authenticated = true;
            } else {
                authenticationexception ex;
                throw ex;
            }
        }
    }
}

//payloads to the server will require a session id, I will integrate that when I get a chance.
//the packets from the server will follow this format: command; payload
Message UserSocket::sendPayload(std::string command, std::string payload) {
    if ((this->authenticated || command == "register")) {
        sf::TcpSocket socket;
        sf::Socket::Status status = socket.connect(host, portnumber);

        if (status == sf::Socket::Status::Error) {
            socketexception ex(host.toString(), portnumber);
            throw ex;
            // some error handling here
        } else {
            sf::TcpListener li;
            li.listen(sf::TcpListener::AnyPort);
            Message msg(command, payload, li.getLocalPort());
            msg.addSessionId(this->sessionId);
            sf::Packet pack;
            pack << msg;
            socket.send(pack);
            Message results = waitForResponse(li);

            if (results.payload == "AUTHFAILURE") {
                authenticationexception ex;
                throw ex;
            } else if (results.payload == "USERERROR") {
                reguserexception ex;
                throw ex;
            } else if (results.payload == "CLASSERROR") {
                regclassexception ex;
                throw ex;
            }

            return results;
        }
    } else {
        // if we are not authenticated anymore then we will need to reauth.
        authenticationexception ex;
        throw ex;
    }
}

//if no exceptions occur then the deauth worked
void UserSocket::deauthenticate() {
    if (authenticated) {
        sf::TcpListener li;
        sf::TcpSocket sock;
        sf::Packet pack;
        sf::Socket::Status status = sock.connect(host, portnumber);

        if (status == sf::Socket::Status::Error) {
            socketexception ex(host.toString(), portnumber);
            throw ex;
            // some error handling here
        } else {
            li.listen(sf::TcpListener::AnyPort);
            Message msg("deauthenticate", "null", li.getLocalPort());
            msg.addSessionId(sessionId);

            pack << msg;
            sock.send(pack);

            Message m = waitForResponse(li);

            if (m.payload == "SUCCESS") {
                authenticated = false;
                sessionId = sf::IpAddress::LocalHost.toString();
            } else if (m.payload == "AUTHFAILURE") {
                authenticationexception ex;
                throw ex;
            }
        }
    }
}

Message UserSocket::waitForResponse(sf::TcpListener &listener) {
    sf::Packet pack;
    sf::TcpSocket sock;
    sf::SocketSelector select;
    select.add(listener);

    if (select.wait(sf::seconds(10))) {
        listener.accept(sock);
        sock.receive(pack);
        Message msg;

        if (pack >> msg) {
            return msg;
        } else {
            packetexception ex;
            throw ex;
        }
    } else {
        timeoutexception ex;
        throw ex;
    }
}
