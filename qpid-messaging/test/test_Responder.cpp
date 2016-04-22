#define BOOST_TEST_MODULE ResponderTest

#include <boost/test/unit_test.hpp>

#include <string>
#include <thread>

#include "RequestResponse.h"

#include "Responder.h"

const static ServerOptions serverOptions("admin","admin","ecag-fixml-dev1",35672,10);
const static Options clientOptions("ABCFR_ABCFRALMMACC1","ecag-fixml-dev1",35671,10);
const static std::string broadcastExchange("broadcast");
const static std::string broadcastRoutingKey("broadcast.ABCFR.TradeConfirmation");
const static std::string requestQueue("request_be.ABCFR_ABCFRALMMACC1");

static void responderThread(Responder *pResponder)
{
    pResponder->run();
}

BOOST_AUTO_TEST_CASE(test_RequestResponse)
{
    BOOST_TEST_MESSAGE("test_RequestResponse");

    Responder r(serverOptions,false,requestQueue);
    RequestResponse rr(clientOptions,false);

    std::thread responder(responderThread,&r);

    r.waitTillReady();

    rr.run();

    responder.join();

    BOOST_CHECK_EQUAL(1, rr.getNReceived());
}

BOOST_AUTO_TEST_CASE(test_RequestResponse_AMQP_10)
{
    BOOST_TEST_MESSAGE("test_RequestResponse_AMQP_10");

    Responder r(serverOptions,true,requestQueue);
    RequestResponse rr(clientOptions,true);

    std::thread responder(responderThread,&r);

    r.waitTillReady();

    rr.run();

    responder.join();

    BOOST_CHECK_EQUAL(1, rr.getNReceived());
}

