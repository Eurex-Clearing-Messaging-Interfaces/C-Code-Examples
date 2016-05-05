#ifndef __BROADCASTER_H__
#define __BROADCASTER_H__

#include <string>

#include <proton/handler.hpp>
#include <proton/sender.hpp>
#include <proton/event.hpp>

#include "ServerOptions.h"

class Broadcaster : public proton::handler
{

    private:

        ServerOptions _options;
        unsigned int _count;
        unsigned int _sent;
        unsigned int _confirmed;
        std::string _exchange;
        std::string _routingKey;
        proton::sender _sender;

    public:

        // Constructor
        explicit Broadcaster(const ServerOptions &options,
                             const std::string &exchange,
                             const std::string &routingKey,
                             unsigned int count = 1);

        void on_start(proton::event &e);

        void on_sendable(proton::event &e);

        void on_delivery_accept(proton::event &e);

        void on_transport_close(proton::event &e);

        // Run method
        void run();

};

#endif // __BROADCASTER_H__
