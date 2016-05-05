#ifndef __REQUEST_RESPONSE_H__
#define __REQUEST_RESPONSE_H__

#include <string>

#include <proton/handler.hpp>
#include <proton/sender.hpp>
#include <proton/receiver.hpp>
#include <proton/event.hpp>

#include "Options.h"

class RequestResponse : public proton::handler
{

    private:

        Options _options;
        uint64_t _received;
        proton::sender _sender;
        proton::receiver _receiver;

    public:

        // Constructor
        explicit RequestResponse(const Options &options);

        // On start callback
        void on_start(proton::event &e);

        // On link open callback
        void on_link_open(proton::event &e);

        // On message callback
        void on_message(proton::event &e);

        // Run method
        void run();

        // Return number of recieved messages
        unsigned int getNReceived() const { return _received; }

};

#endif // __REQUEST_RESPONSE_H__
