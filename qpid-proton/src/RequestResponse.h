#ifndef __REQUEST_RESPONSE_H__
#define __REQUEST_RESPONSE_H__

#include <string>

#include <proton/messaging_handler.hpp>
#include <proton/sender.hpp>
#include <proton/receiver.hpp>

#include "Options.h"

class RequestResponse : public proton::messaging_handler
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
        void on_container_start(proton::container &c);

        // On sender open callback
        void on_sender_open(proton::sender &s);

        // On message callback
        void on_message(proton::delivery &d, proton::message &m);

        // Run method
        void run();

        // Return number of recieved messages
        unsigned int getNReceived() const { return _received; }

};

#endif // __REQUEST_RESPONSE_H__
