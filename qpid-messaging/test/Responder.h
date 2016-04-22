#ifndef __RESPONDER_H__
#define __RESPONDER_H__

#include <string>
#include <mutex>

#include "ServerOptions.h"

class Responder
{

    private:

        ServerOptions _options;
        const bool _useAmqp10;
        const unsigned int _capacity;
        const std::string _requestQueue;
        std::mutex _mutex;

    public:

        // Constructor
        explicit Responder(const ServerOptions &options,
                           bool useAmqp10,
                           const std::string &requestQueue);

        // Run method
        void run();

        // Wait till responder is ready and listening.
        void waitTillReady();

};

#endif // __RESPONDER_H__
