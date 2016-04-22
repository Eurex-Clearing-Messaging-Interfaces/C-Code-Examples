#ifndef __BROADCASTER_H__
#define __BROADCASTER_H__

#include <string>

#include "ServerOptions.h"

class Broadcaster
{

    private:

        ServerOptions _options;
        const bool _useAmqp10;
        const unsigned int _capacity;
        unsigned int _count;
        std::string _exchange;
        std::string _routingKey;

    public:

        // Constructor
        explicit Broadcaster(const ServerOptions &options,
                             bool useAmqp10,
                             const std::string &exchange,
                             const std::string &routingKey,
                             unsigned int count = 1);

        // Run method
        void run();

};

#endif // __BROADCASTER_H__
