#include <unistd.h>
#include <iostream>
#include <string>

#include "BroadcastReceiver.h"
#include "RequestResponse.h"

#define DEFAULT_ACCOUNT "ABCFR_ABCFRALMMACC1"
#define DEFAULT_HOST    "ecag-fixml-simu1.deutsche-boerse.com"
#define DEFAULT_PORT    10170
#define DEFAULT_TIMEOUT 10

#define BROADCAST_RECEIVER "broadcast_receiver"
#define REQUEST_RESPONSE   "request_response"

int main(int argc, char *argv[])
{
    std::string account(DEFAULT_ACCOUNT);
    std::string host(DEFAULT_HOST);
    unsigned int port(DEFAULT_PORT);
    unsigned int timeout(DEFAULT_TIMEOUT);
    bool amqp10 = false;

    try
    {
        char c;
        extern char *optarg;
        extern int optind, optopt;
        while ((c = getopt (argc, argv, ":a:h:p:t:1")) != -1)
        {
            switch (c)
            {
                case 'a':
                    account = optarg;
                    break;
                case 'h':
                    host = optarg;
                    break;
                case 'p':
                    port = (unsigned int)atoi(optarg);
                    break;
                case 't':
                    timeout = (unsigned int)atoi(optarg);
                    break;
                case '1':
                    amqp10 = true;
                    break;
                case ':':
                    throw std::runtime_error("-E- Option -" + std::string(1,char(optopt)) + " requires an argument.");
                case '?':
                    throw std::runtime_error("-E- Unknown option -" + std::string(1,char(optopt)) + ".");
                default:
                    throw std::runtime_error("-E- Unknown error in getopt().");
            }
        }

        if (optind >= argc)
        {
            throw std::runtime_error("-E- Missing mode parameter.");
        }
        else
        {
            std::string mode(argv[optind]);

            if (mode == BROADCAST_RECEIVER)
            {
                BroadcastReceiver(Options(account,host,port,timeout),amqp10).run();
            }
            else if (mode == REQUEST_RESPONSE)
            {
                RequestResponse(Options(account,host,port,timeout),amqp10).run();
            }
            else
            {
                throw std::runtime_error("-E- Unknown mode \'" + mode + "\'.");
            }
        }
    }
    catch (const std::exception &error)
    {
        std::cerr << error.what();
        std::cerr << std::endl;
        std::cerr << "Usage:" << std::endl;
        std::cerr << "  " << argv[0] << " [options] <mode>" << std::endl;
        std::cerr << "  Options:" << std::endl;
        std::cerr << "    -a <account>   Account (default=" << DEFAULT_ACCOUNT << ")" << std::endl;
        std::cerr << "    -h <hostname>  Hostname (default=" << DEFAULT_HOST << ")" << std::endl;
        std::cerr << "    -p <port>      Port (default=" << DEFAULT_PORT << ")" << std::endl;
        std::cerr << "    -t <timeout>   Timeout (default=" << DEFAULT_TIMEOUT << ")" << std::endl;
        std::cerr << "    -1             Enable AMQP 1.0 protocol" << std::endl;
        std::cerr << "  Modes:" << std::endl;
        std::cerr << "    " << BROADCAST_RECEIVER << " - Receive broadcasts." << std::endl;
        std::cerr << "    " << REQUEST_RESPONSE << "   - Send request and receive response." << std::endl;

        return 1;
    }

    return 0;
}

