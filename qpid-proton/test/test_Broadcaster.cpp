#define BOOST_TEST_MODULE BroadcasterTest

#include <boost/test/unit_test.hpp>

#include <string>

#include "BroadcastReceiver.h"

#include "Broadcaster.h"
#include "ClientOptions.h"

const static ServerOptions serverOptions("admin","admin","ecag-fixml-dev1",35672);
const static ClientOptions clientOptions("ABCFR_ABCFRALMMACC1","ecag-fixml-dev1",35671,"ecag-fixml-dev1.crt","ABCFR_ABCFRALMMACC1.pem","123456");
const static std::string broadcastExchange("broadcast");
const static std::string broadcastRoutingKey("broadcast.ABCFR.TradeConfirmation");

static void replaceOptions(ClientOptions &options)
{
    for (int i=0;i<boost::unit_test::framework::master_test_suite().argc-1;i++)
    {
        std::string arg = boost::unit_test::framework::master_test_suite().argv[i];
        if (arg == "--cert-dir")
        {
            std::string certDir(boost::unit_test::framework::master_test_suite().argv[i+1]);
            options.setHostCert(certDir + "/" + options.getHostCert());
            options.setPrivateKey(certDir + "/" + options.getPrivateKey());
        }
    }
}

BOOST_AUTO_TEST_CASE(test_BroadcastReceiver)
{
    BOOST_TEST_MESSAGE("test_BroadcastReceiver");

    ClientOptions options(clientOptions);
    replaceOptions(options);

    Broadcaster(serverOptions,broadcastExchange,broadcastRoutingKey,1).run();
    BroadcastReceiver br(options);

    br.run();

    BOOST_CHECK_EQUAL(1, br.getNReceived());
}

