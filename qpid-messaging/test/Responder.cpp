#include <iostream>

#include <qpid/messaging/Address.h>
#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>
#include <qpid/messaging/exceptions.h>
#include <qpid/messaging/Logger.h>

#include "Responder.h"

using namespace qpid::messaging;

Responder::Responder(const ServerOptions &options,
                     bool useAmqp10,
                     const std::string &requestQueue)
    : _options(options)
    , _useAmqp10(useAmqp10)
    , _capacity(1000)
    , _requestQueue(requestQueue)
{
}

void Responder::run()
{
    const char *options[] = {"", "--log-enable", "trace+", "--log-to-stdout", "on", "--log-time", "on", "--log-level", "on"};
    Logger::configure(9, options);

    std::string url = "amqp:" + _options.getHost() + ":" + std::to_string(_options.getPort());

    Connection connection(url, _useAmqp10 ? "{ protocol: amqp1.0 }" : "");
    connection.setOption("username", _options.getAccount());
    connection.setOption("password", _options.getPassword());
    if (_useAmqp10)
        connection.setOption("heartbeat", "30");

    Address request(_requestQueue + "; { node: { type: queue }, assert: never, mode: consume }");
    Duration timeout = Duration::SECOND * _options.getTimeout();

    try
    {
        connection.open();
        Session session = connection.createSession();

        std::cout << "-I- Responder: Connection opened, session created" << std::endl;

        Receiver receiver = session.createReceiver(request);
        receiver.setCapacity(_capacity);

        std::cout << "-I- Responder: Receiver created " << receiver.getName() << std::endl;

        try
        {
            Message requestMsg = receiver.fetch(timeout);
            std::cout << "-I- Responder: Received message with content: " << requestMsg.getContent() << std::endl;

            Sender sender = session.createSender(requestMsg.getReplyTo());
            sender.setCapacity(_capacity);

            std::cout << "-I- Responder: Sender created " << sender.getName() << std::endl;

            Message responseMsg("Responding to " + requestMsg.getContent());
            responseMsg.setDurable(false);
            sender.send(responseMsg, true);

            std::cout << "-I- Responder: Response message sent" << std::endl;

            session.acknowledge(true);
            session.sync(true);

            sender.close();
        }
        catch (NoMessageAvailable noMessage)
        {
            std::cout << "-I- Responder: No message received for " << timeout.getMilliseconds()/1000 << " seconds" << std::endl;
        }

        receiver.close();
        session.close();
        connection.close();
    }
    catch (const std::exception &error)
    {
        connection.close();
        std::cerr << "-E- Responder: Caught exception: " << error.what() << std::endl;
        throw error;
    }
}

