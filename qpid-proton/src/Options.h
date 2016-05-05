#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <string>

class Options
{

    protected:

        std::string _account;
        std::string _host;
        unsigned int _port;
        std::string _hostCert;
        std::string _privateKey;
        std::string _privateKeyPassword;

    public:

        // Constructor
        explicit Options(std::string account,
                         std::string host,
                         unsigned int port,
                         std::string hostCert,
                         std::string privateKey,
                         std::string privateKeyPassword)
            : _account(account)
            , _host(host)
            , _port(port)
            , _hostCert(hostCert)
            , _privateKey(privateKey)
            , _privateKeyPassword(privateKeyPassword)
        {

        }

        const std::string &getAccount() const
        {
            return _account;
        }

        const std::string &getHost() const
        {
            return _host;
        }

        unsigned int getPort() const
        {
            return _port;
        }

        const std::string &getHostCert() const
        {
            return _hostCert;
        }

        const std::string &getPrivateKey() const
        {
            return _privateKey;
        }

        const std::string &getPrivateKeyPassword() const
        {
            return _privateKeyPassword;
        }

};

#endif // __OPTIONS_H__
