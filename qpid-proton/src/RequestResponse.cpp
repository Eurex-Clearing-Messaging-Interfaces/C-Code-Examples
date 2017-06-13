#include <iostream>

#include <proton/connection_options.hpp>
#include <proton/container.hpp>
#include <proton/default_container.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/ssl.hpp>
#include <proton/thread_safe.hpp>
#include <proton/tracker.hpp>

#include "RequestResponse.h"

RequestResponse::RequestResponse(const Options &options)
    : _options(options)
    , _received(0)
{
}

void RequestResponse::on_container_start(proton::container &c)
{
    proton::ssl_certificate certificate(_options.getPrivateKey(),_options.getPrivateKey(),_options.getPrivateKeyPassword());
    proton::ssl_client_options sslClientOptions(certificate,_options.getHostCert(),proton::ssl::VERIFY_PEER_NAME);
    proton::connection_options connectionOptions;
    connectionOptions.ssl_client_options(sslClientOptions).sasl_allowed_mechs("EXTERNAL");
    // Validate the server certificate against this name:
    connectionOptions.virtual_host(_options.getHost());
    c.client_connection_options(connectionOptions);

    std::string senderUrl = "amqps://" + _options.getHost() + ":" + std::to_string(_options.getPort()) + "/request." + _options.getAccount();
    std::string responseUrl = "amqps://" + _options.getHost() + ":" + std::to_string(_options.getPort()) + "/response." + _options.getAccount();

    _sender = c.open_sender(senderUrl);
    std::cout << "-I- Sender created " << senderUrl << std::endl;

    //_receiver = _sender.connection().open_receiver("response." + _options.getAccount() , proton::link_options().dynamic_address(false));
    _receiver = _sender.connection().open_receiver("response." + _options.getAccount());
    std::cout << "-I- Listening on " << responseUrl << std::endl;
}

void RequestResponse::on_sender_open(proton::sender &s)
{
    proton::message req;
    req.body("Hello - request");
    req.reply_to("response/response." + _options.getAccount());
    _sender.send(req);
    std::cout << "-I- Request message sent" << std::endl;
}

void RequestResponse::on_message(proton::delivery &d, proton::message &m)
{
    std::cout << m.body() << std::endl;
    _received++;

    d.receiver().close();
    d.connection().close();

    if (!!_receiver) _receiver.close();
    if (!!_sender) _sender.close();
}


void RequestResponse::run()
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

