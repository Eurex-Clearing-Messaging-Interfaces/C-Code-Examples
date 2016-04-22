#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <string>

class Options
{

    private:

        std::string _account;
        std::string _host;
        unsigned int _port;
        unsigned int _timeout;

    public:

        // Constructor
        explicit Options(std::string account,
                         std::string host,
                         unsigned int port,
                         unsigned int timeout)
            : _account(account)
            , _host(host)
            , _port(port)
            , _timeout(timeout)
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

        unsigned int getTimeout() const
        {
            return _timeout;
        }

};

#endif // __OPTIONS_H__
