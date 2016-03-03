//============================================================================
// Name        : broadcast_receiver.cpp
// Version     : $Id: broadcast_receiver.cpp 3292 2015-03-12 20:21:28Z schojak $
// Description : FIXML Clearing API C++ broadcast receiver example
//============================================================================

#include <qpid/messaging/Address.h>
#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>
#include <qpid/messaging/exceptions.h>
#include <qpid/messaging/Logger.h>

#include <iostream>

using namespace qpid::messaging;

using std::stringstream;
using std::string;

int main(int argc, char** argv) 
{
    const char *options[] = {"", "--log-enable", "trace+", "--log-to-stdout", "on", "--log-time", "on", "--log-level", "on"};
    qpid::messaging::Logger::configure(9, options);

    const char* url = "amqp:ssl:ecag-fixml-simu1.deutsche-boerse.com:10170";
    std::string connectionOptions = "{ protocol: amqp1.0}";

    Connection connection(url, connectionOptions);
    connection.setOption("sasl_mechanisms", "EXTERNAL");

    Address reply("response/response.ABCFR_ABCFRALMMACC1; { node: { type: topic }, assert: never, create: never }");
    Address request("request.ABCFR_ABCFRALMMACC1; { node: { type: topic }, assert: never, create: never }");
    Address response("response.ABCFR_ABCFRALMMACC1; {create: never, assert: never, node: { type: queue } }");

    Message requestMsg, responseMsg;
    qpid::types::Variant::Map msgProperties;

    Duration timeout = Duration::SECOND * 100;

    try 
    {
        Connection connection(url, connectionOptions);

        try 
        {
            connection.open();
            Session session = connection.createSession();

            std::cout << "-I- Connection opened, session created" << std::endl;

            Sender sender = session.createSender(request);
            std::cout << "-I- Sender created " << sender.getName() << std::endl;
            sender.setCapacity(1000);

            Receiver receiver = session.createReceiver(response);
            std::cout << "-I- Receiver created " << receiver.getName() << std::endl;
            receiver.setCapacity(1000);

            requestMsg = Message("<FIXML>...</FIXML>");
            requestMsg.setDurable(false);
            requestMsg.setReplyTo(reply);
            sender.send(requestMsg, true);

            std::cout << "-I- Request message sent" << std::endl;

            while (true)
            {
                try 
                {
                    responseMsg = receiver.fetch(timeout);
                    msgProperties = responseMsg.getProperties();

                    std::cout << "-I- Response message received with content " << responseMsg.getContent() << std::endl;
                    session.acknowledge(true);
                } catch (NoMessageAvailable noMessage) 
                {
                    std::cout << "-I- No response message received for " << timeout.getMilliseconds() / 1000 << " seconds" << std::endl;
                    break;
                }
            }

            session.sync(true);
            sender.close();
            receiver.close();
            session.close();
            connection.close();
            return 0;
        } catch (const std::exception& error) 
        {
            std::cout << "Exception: " << error.what() << std::endl;
            connection.close();
        }
    } catch (const std::exception& error) 
    {
        std::cout << "Exception: " << error.what() << std::endl;
    }

    return 1;
}
