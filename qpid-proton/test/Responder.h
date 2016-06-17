#ifndef __RESPONDER_H__
#define __RESPONDER_H__

#include <string>

#include <proton/messaging_handler.hpp>
#include <proton/sender.hpp>
#include <proton/receiver.hpp>

#include "ServerOptions.h"

class Responder : public proton::messaging_handler
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
        void on_container_start(proton::container &c);

        // On message callback
        void on_message(proton::delivery &d, proton::message &m);

        // On tracker accept
        void on_tracker_accept(proton::tracker &t);

        // Run method
        void run();

};

#endif // __RESPONDER_H__
