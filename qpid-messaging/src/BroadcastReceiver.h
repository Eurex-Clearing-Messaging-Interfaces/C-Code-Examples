#ifndef __BROADCAST_RECIVER_H__
#define __BROADCAST_RECIVER_H__

#include <string>

#include "Options.h"

class BroadcastReceiver
{

    private:

        Options _options;
        const bool _useAmqp10;
        const unsigned int _blockSize;
        const unsigned int _capacity;
        unsigned int _messageCounter;
        std::string _address;

    public:

        // Constructor
        explicit BroadcastReceiver(const Options &options, bool useAmqp10);

        // Run method
        void run();

        // Return number of recieved messages
        unsigned int getNReceived() const { return _messageCounter; }

};

#endif // __BROADCAST_RECIVER_H__
