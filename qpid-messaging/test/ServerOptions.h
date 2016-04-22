#ifndef __SERVER_OPTIONS_H__
#define __SERVER_OPTIONS_H__

#include <string>

#include "Options.h"

class ServerOptions : public Options
{

    private:

        std::string _password;

    public:

        // Constructor
        explicit ServerOptions(std::string account,
                               std::string password,
                               std::string host,
                               unsigned int port,
                               unsigned int timeout)
            : Options(account,host,port,timeout)
            , _password(password)
        {

        }

        const std::string &getPassword() const
        {
            return _password;
        }

};

#endif // __SERVER_OPTIONS_H__
