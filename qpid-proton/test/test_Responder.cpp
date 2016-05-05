#define BOOST_TEST_MODULE ResponderTest

#include <boost/test/unit_test.hpp>

#include <string>
#include <thread>

#include "RequestResponse.h"

#include "Responder.h"
#include "ClientOptions.h"

const static ServerOptions serverOptions("admin","admin","ecag-fixml-dev1",35672);
const static ClientOptions clientOptions("ABCFR_ABCFRALMMACC1","ecag-fixml-dev1",35671,"ecag-fixml-dev1.crt","ABCFR_ABCFRALMMACC1.pem","123456");
const static std::string requestQueue("request_be.ABCFR_ABCFRALMMACC1");

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

static void responderThread(Responder *pResponder)
{
    try
    {
        pResponder->run();
    }
    catch (const std::exception &error)
    {
        BOOST_TEST_MESSAGE(std::string("Responder has failed with message: ") + error.what());
    }
}

BOOST_AUTO_TEST_CASE(test_RequestResponse)
{
    BOOST_TEST_MESSAGE("test_RequestResponse");

    ClientOptions options(clientOptions);
    replaceOptions(options);

    Responder r(serverOptions,requestQueue);
    RequestResponse rr(options);

    std::thread responder(responderThread,&r);

    rr.run();

    responder.join();

    BOOST_CHECK_EQUAL(1, rr.getNReceived());
}

