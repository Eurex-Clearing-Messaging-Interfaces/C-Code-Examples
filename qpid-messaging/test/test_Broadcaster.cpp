#define BOOST_TEST_MODULE BroadcasterTest

#include <boost/test/unit_test.hpp>

#include <string>

#include "BroadcastReceiver.h"

#include "Broadcaster.h"

const static ServerOptions serverOptions("admin","admin","ecag-fixml-dev1",35672,10);
const static Options clientOptions("ABCFR_ABCFRALMMACC1","ecag-fixml-dev1",35671,10);
const static std::string broadcastExchange("broadcast");
const static std::string broadcastRoutingKey("broadcast.ABCFR.TradeConfirmation");


BOOST_AUTO_TEST_CASE(test_BroadcastReceiver)
{
    BOOST_TEST_MESSAGE("test_BroadcastReceiver");

    Broadcaster(serverOptions,false,broadcastExchange,broadcastRoutingKey,1).run();
    BroadcastReceiver br(clientOptions,false);

    br.run();

    BOOST_CHECK_EQUAL(1, br.getNReceived());
}

BOOST_AUTO_TEST_CASE(test_BroadcastReceiver_AMQP_10)
{
    BOOST_TEST_MESSAGE("test_BroadcastReceiver_AMQP_10");

    Broadcaster(serverOptions,true,broadcastExchange,broadcastRoutingKey,1).run();
    BroadcastReceiver br(clientOptions,true);

    br.run();

    BOOST_CHECK_EQUAL(1, br.getNReceived());
}

