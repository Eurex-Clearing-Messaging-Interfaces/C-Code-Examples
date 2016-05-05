#include <iostream>

#include <proton/container.hpp>
#include <proton/receiver.hpp>
#include <proton/event.hpp>
#include <proton/handler.hpp>
#include <proton/link.hpp>
#include <proton/url.hpp>
#include <proton/ssl.hpp>
#include <proton/sasl.hpp>
#include <proton/transport.hpp>

#include "BroadcastReceiver.h"

BroadcastReceiver::BroadcastReceiver(const Options &options)
    : _options(options)
    , _expected(1)
    , _received(0)
{
    _address = "broadcast." + _options.getAccount() + ".TradeConfirmation";
}

void BroadcastReceiver::on_start(proton::event &e)
{
    proton::ssl_certificate certificate(_options.getPrivateKey(),_options.getPrivateKey(),_options.getPrivateKeyPassword());
    proton::ssl_client_options sslClientOptions(certificate,_options.getHostCert(),proton::ssl::VERIFY_PEER_NAME);
    proton::connection_options connectionOptions;
    connectionOptions.ssl_client_options(sslClientOptions).allowed_mechs("EXTERNAL");
    // Validate the server certificate against this name:
    connectionOptions.peer_hostname(_options.getHost());
    e.container().client_connection_options(connectionOptions);

    std::string url = "amqps://" + _options.getHost() + ":" + std::to_string(_options.getPort()) + "/" + _address;

    _receiver = e.container().open_receiver(url);
    std::cout << "-I- Listening on " << url << std::endl;
}

void BroadcastReceiver::on_message(proton::event &e)
{
    proton::message& msg = e.message();

    if (_expected == 0 || _received < _expected)
    {
        std::cout << msg.body() << std::endl;
        _received++;
    }

    if (_received == _expected)
    {
        e.receiver().close();
        e.connection().close();

        if (!!_receiver) _receiver.close();
    }
}


void BroadcastReceiver::run()
{
    try
    {
        proton::container(*this).run();
    }
    catch (const std::exception &error)
    {
        std::cerr << "-E- Caught exception: " << error.what() << std::endl;
        throw error;
    }
}

