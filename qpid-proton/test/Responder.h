#ifndef __RESPONDER_H__
#define __RESPONDER_H__

#include <string>

#include <proton/handler.hpp>
#include <proton/sender.hpp>
#include <proton/receiver.hpp>
#include <proton/event.hpp>

#include "ServerOptions.h"

class Responder : public proton::handler
{

    private:

        ServerOptions _options;
        const std::string _requestQueue;
        proton::sender _sender;
        proton::receiver _receiver;

    public:

        // Constructor
        explicit Responder(const ServerOptions &options,
                           const std::string &requestQueue);

        // On start callback
        void on_start(proton::event &e);

        // On message callback
        void on_message(proton::event &e);

        // On delivery accept
        void on_delivery_accept(proton::event &e);

        // Run method
        void run();

};

#endif // __RESPONDER_H__
