#ifndef __CLIENT_OPTIONS_H__
#define __CLIENT_OPTIONS_H__

#include <string>

#include "Options.h"

class ClientOptions : public Options
{

    public:

        // Constructor
        explicit ClientOptions(std::string account,
                               std::string host,
                               unsigned int port,
                               std::string hostCert,
                               std::string privateKey,
                               std::string privateKeyPassword)
            : Options(account,host,port,hostCert,privateKey,privateKeyPassword)
        {

        }

        void setHostCert(const std::string &val)
        {
            _hostCert = val;
        }

        void setPrivateKey(const std::string &val)
        {
            _privateKey = val;
        }

};

#endif // __CLIENT_OPTIONS_H__
