#include <iostream>

#include <qpid/messaging/Address.h>
#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>
#include <qpid/messaging/exceptions.h>
#include <qpid/messaging/Logger.h>

#include "Broadcaster.h"

using namespace qpid::messaging;

Broadcaster::Broadcaster(const ServerOptions &options,
                         bool useAmqp10,
                         const std::string &exchange,
                         const std::string &routingKey,
                         unsigned int count)
    : _options(options)
    , _useAmqp10(useAmqp10)
    , _capacity(1000)
    , _count(count)
    , _exchange(exchange)
    , _routingKey(routingKey)
{
}

void Broadcaster::run()
{
    const char *options[] = {"", "--log-enable", "trace+", "--log-to-stdout", "on", "--log-time", "on", "--log-level", "on"};
    Logger::configure(9, options);

    std::string url = "amqp:" + _options.getHost() + ":" + std::to_string(_options.getPort());

    Connection connection(url, _useAmqp10 ? "{ protocol: amqp1.0 }" : "");
    connection.setOption("username", _options.getAccount());
    connection.setOption("password", _options.getPassword());

    std::string address(_exchange + "/" + _routingKey);

    if (_useAmqp10)
        connection.setOption("heartbeat", "30");
    else
        address += "; { node: { type: topic }, assert: never }";

    Address broadcast(address);
    Duration timeout = Duration::SECOND * _options.getTimeout();

    try
    {
        connection.open();
        Session session = connection.createSession();

        std::cout << "-I- Broadcaster: Connection opened, session created" << std::endl;

        Sender sender = session.createSender(broadcast);
        sender.setCapacity(_capacity);

        std::cout << "-I- Broadcaster: Sender created " << sender.getName() << std::endl;

        for (unsigned int i=0;i<_count;i++)
        {
            Message msg("<FIXML>" + std::to_string(i+1) + "</FIXML>");;
            msg.setDurable(false);
            sender.send(msg, true);
        }

        std::cout << "-I- Broadcaster: Send " << _count << " messages" << std::endl;

        sender.close();
        session.close();
        connection.close();
    }
    catch (const std::exception &error)
    {
        connection.close();
        std::cerr << "-E- Broadcaster: Caught exception: " << error.what() << std::endl;
        throw error;
    }
}

