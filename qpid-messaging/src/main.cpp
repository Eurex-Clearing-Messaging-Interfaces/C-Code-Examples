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

    char c;
    extern char *optarg;
    extern int optind, optopt;
    bool errFlag = false;
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
                std::cerr << "-E- Option -" << char(optopt) << " requires an argument." << std::endl;
                errFlag = true;
                break;
            case '?':
                std::cerr << "-E- Unknown option -" << char(optopt) << "." << std::endl;
                errFlag = true;
                break;
            default:
                return 1;
        }
    }

    if (optind >= argc)
    {
        std::cerr << "-E- Missing mode parameter." << std::endl;
        errFlag = true;
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
            std::cerr << "-E- Unknown mode \'" + mode + "\'." << std::endl;
            errFlag = true;
        }
    }

    if (errFlag)
    {
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

