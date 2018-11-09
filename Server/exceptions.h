#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <string>
#include <exception>

/**
 * A collection of strucs for error handling in the Stackd game API's server.
 */
struct socketexception;
struct packetexception;
struct authenticationexception;
struct timeoutexception;

struct timeoutexception : public std::exception {
   const char * what() const throw(){
      return "timeout reached";
   }
};

struct authenticationexception : public std::exception {
    const char * what() const throw(){
       return "authentication error";
    }
};

struct socketexception : public std::exception {
   std::string h;
   unsigned int p;
   socketexception(std::string host, unsigned int port ){
      h=host;
      p=port;
   }
   const char * what () const throw () {
      const char * what = ("Exception on connection: Host{"+h+"} Port{"+(""+p)+"}").c_str();
      return what;
   }
};

struct packetexception : public std::exception {
   const char * what () const throw () {
      const char * what = (std::string("packet was not formed correctly")).c_str();
      return what;
   }
};

#endif // EXCEPTIONS_H
