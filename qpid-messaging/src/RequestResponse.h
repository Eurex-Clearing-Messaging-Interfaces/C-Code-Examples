#ifndef __REQUEST_RESPONSE_H__
#define __REQUEST_RESPONSE_H__

#include <string>

#include "Options.h"

class RequestResponse
{

    private:

        Options _options;
        const bool _useAmqp10;
        const unsigned int _capacity;
        unsigned int _messageCounter;
        std::string _replyAddress;
        std::string _requestAddress;
        std::string _responseAddress;

    public:

        // Constructor
        explicit RequestResponse(const Options &options, bool useAmqp10);

        // Run method
        void run();

        // Return number of recieved messages
        unsigned int getNReceived() const { return _messageCounter; }

};

#endif // __REQUEST_RESPONSE_H__
