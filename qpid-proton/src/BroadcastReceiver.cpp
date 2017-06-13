#include <iostream>

#include <proton/connection_options.hpp>
#include <proton/container.hpp>
#include <proton/default_container.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/ssl.hpp>
#include <proton/thread_safe.hpp>

#include "BroadcastReceiver.h"

BroadcastReceiver::BroadcastReceiver(const Options &options)
    : _options(options)
    , _expected(1)
    , _received(0)
{
    _address = "broadcast." + _options.getAccount() + ".TradeConfirmation";
}

void BroadcastReceiver::on_container_start(proton::container &c)
{
    proton::ssl_certificate certificate(_options.getPrivateKey(),_options.getPrivateKey(),_options.getPrivateKeyPassword());
    proton::ssl_client_options sslClientOptions(certificate,_options.getHostCert(),proton::ssl::VERIFY_PEER_NAME);
    proton::connection_options connectionOptions;
    connectionOptions.ssl_client_options(sslClientOptions).sasl_allowed_mechs("EXTERNAL");
    // Validate the server certificate against this name:
    connectionOptions.virtual_host(_options.getHost());
    c.client_connection_options(connectionOptions);

    std::string url = "amqps://" + _options.getHost() + ":" + std::to_string(_options.getPort()) + "/" + _address;

    _receiver = c.open_receiver(url);
    std::cout << "-I- Listening on " << url << std::endl;
}

void BroadcastReceiver::on_message(proton::delivery &d, proton::message &m)
{
    if (_expected == 0 || _received < _expected)
    {
        std::cout << m.body() << std::endl;
        _received++;
    }

    if (_received == _expected)
    {
        d.receiver().close();
        d.connection().close();

        if (!!_receiver) _receiver.close();
    }
}


void BroadcastReceiver::run()
{
    try
    {
        proton::default_container(*this).run();
    }
    catch (const std::exception &error)
    {
        std::cerr << "-E- Caught exception: " << error.what() << std::endl;
        throw error;
    }
}

