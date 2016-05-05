#ifndef __BROADCAST_RECIVER_H__
#define __BROADCAST_RECIVER_H__

#include <string>

#include <proton/handler.hpp>
#include <proton/receiver.hpp>
#include <proton/event.hpp>

#include "Options.h"

class BroadcastReceiver : public proton::handler
{

    private:

        Options _options;
        const uint64_t _expected;
        uint64_t _received;
        std::string _address;
        proton::receiver _receiver;

    public:

        // Constructor
        explicit BroadcastReceiver(const Options &options);

        // On start callback
        void on_start(proton::event &e);

        // On message callback
        void on_message(proton::event &e);

        // Run method
        void run();

        // Return number of recieved messages
        unsigned int getNReceived() const { return _received; }

};

#endif // __BROADCAST_RECIVER_H__
