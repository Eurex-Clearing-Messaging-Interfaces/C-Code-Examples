#ifndef __BROADCASTER_H__
#define __BROADCASTER_H__

#include <string>

#include <proton/messaging_handler.hpp>

#include "ServerOptions.h"

class Broadcaster : public proton::messaging_handler
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

        void on_container_start(proton::container &c);

        void on_sendable(proton::sender &s);

        void on_tracker_accept(proton::tracker &t);

        void on_transport_close(proton::transport &t);

        // Run method
        void run();

};

#endif // __BROADCASTER_H__
