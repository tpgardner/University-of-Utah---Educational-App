#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <string>
#include <exception>

struct socketexception;
struct packetexception;
struct authenticationexception;
struct timeoutexception;

/**
 * Exception structs for the Stackd game API.
 */
struct timeoutexception : public std::exception {
   const char * what() const throw(){
      return "timeout reached";
   }
};

struct filenotfoundexception : public std::exception {
    std::string msg;
    filenotfoundexception(std::string filename){
        msg=filename;
    }

    const char * what() const throw(){
        return ("file : "+ msg+"not found").c_str();
    }
};


struct authenticationexception : public std::exception {
    const char * what() const throw(){
       return "authentication error";
    }
};
struct reguserexception : public std::exception {
    const char * what() const throw(){
       return "user invalid error";
    }
};
struct regclassexception : public std::exception {
    const char * what() const throw(){
       return "class invalid error";
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
