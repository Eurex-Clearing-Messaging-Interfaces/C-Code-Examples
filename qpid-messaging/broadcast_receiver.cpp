//============================================================================
// Name        : broadcast_receiver.cpp
// Version     : $Id: broadcast_receiver.cpp 3306 2015-03-17 12:45:24Z schojak $
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
    std::string connectionOptions = "{ }";

    Connection connection(url, connectionOptions);
    connection.setOption("sasl_mechanisms", "EXTERNAL");
    connection.setOption("heartbeat", "30");

    Address broadcast("broadcast.ABCFR_ABCFRALMMACC1.TradeConfirmation; { node: { type: queue }, assert: never, create: never, mode: consume }");

    Duration timeout = Duration::SECOND * 100;

    try 
    {
        connection.open();
        Session session = connection.createSession();

        std::cout << "-I- Connection opened, session created" << std::endl;

        Receiver receiver = session.createReceiver(broadcast);
        std::cout << "-I- Receiver created " << receiver.getName() << std::endl;
        receiver.setCapacity(100);

        while (true)
        {
            try
            {
                Message msg = receiver.fetch(timeout);

                std::cout << "-I- Received message with content: " << msg.getContent() << std::endl;
                
                session.acknowledge(false);
                session.sync(true);
                continue;
            }
            catch (NoMessageAvailable noMessage) {
                std::cout << "-I- No message received for " << timeout.getMilliseconds()/1000 << " seconds" << std::endl;
                break;
            }
        }

        receiver.close();
        session.close();
        connection.close();
        return 0;
    } catch(const std::exception& error) 
    {
        std::cout << "-E- " << error.what() << std::endl;
        connection.close();
    }

    return 1;
}
