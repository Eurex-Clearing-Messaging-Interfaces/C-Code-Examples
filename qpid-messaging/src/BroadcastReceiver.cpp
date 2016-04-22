#include <iostream>

#include <qpid/messaging/Address.h>
#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>
#include <qpid/messaging/exceptions.h>
#include <qpid/messaging/Logger.h>

#include "BroadcastReceiver.h"

using namespace qpid::messaging;

BroadcastReceiver::BroadcastReceiver(const Options &options, bool useAmqp10)
    : _options(options)
    , _useAmqp10(useAmqp10)
    , _blockSize(10)
    , _capacity(1000)
    , _messageCounter(0)
{
    _address = "broadcast." + _options.getAccount() + ".TradeConfirmation; { node: { type: queue } , create: never , mode: consume , assert: never }";
}


void BroadcastReceiver::run()
{
    const char *options[] = {"", "--log-enable", "trace+", "--log-to-stdout", "on", "--log-time", "on", "--log-level", "on"};
    Logger::configure(9, options);

    std::string url = "amqp:ssl:" + _options.getHost() + ":" + std::to_string(_options.getPort());

    Connection connection(url, _useAmqp10 ? "{ protocol: amqp1.0 }" : "");
    connection.setOption("sasl_mechanisms", "EXTERNAL");
    if (_useAmqp10)
        connection.setOption("heartbeat", "30");

    Address broadcast(_address);
    Duration timeout = Duration::SECOND * _options.getTimeout();

    try
    {
        connection.open();
        Session session = connection.createSession();

        std::cout << "-I- Connection opened, session created" << std::endl;

        Receiver receiver = session.createReceiver(broadcast);
        receiver.setCapacity(_capacity);

        std::cout << "-I- Receiver created " << receiver.getName() << std::endl;

        while (true)
        {
            try
            {
                Message msg = receiver.fetch(timeout);

                std::cout << "-I- Received message with content: " << msg.getContent() << std::endl;
                _messageCounter++;

                if (_messageCounter % _blockSize == 0)
                {
                    session.acknowledge(false);
                    session.sync(true);
                }
            }
            catch (NoMessageAvailable noMessage)
            {
                std::cout << "-I- No message received for " << timeout.getMilliseconds()/1000 << " seconds" << std::endl;
                break;
            }
        }

        session.acknowledge(false);
        session.sync(true);

        receiver.close();
        session.close();
        connection.close();
    }
    catch (const std::exception &error)
    {
        connection.close();
        std::cerr << "-E- Caught exception: " << error.what() << std::endl;
        throw error;
    }
}

