#include <iostream>

#include <proton/container.hpp>
#include <proton/receiver.hpp>
#include <proton/sender.hpp>
#include <proton/event.hpp>
#include <proton/handler.hpp>
#include <proton/link.hpp>
#include <proton/url.hpp>
#include <proton/ssl.hpp>
#include <proton/sasl.hpp>
#include <proton/transport.hpp>

#include "RequestResponse.h"

RequestResponse::RequestResponse(const Options &options)
    : _options(options)
    , _received(0)
{
}

void RequestResponse::on_start(proton::event &e)
{
    proton::ssl_certificate certificate(_options.getPrivateKey(),_options.getPrivateKey(),_options.getPrivateKeyPassword());
    proton::ssl_client_options sslClientOptions(certificate,_options.getHostCert(),proton::ssl::VERIFY_PEER_NAME);
    proton::connection_options connectionOptions;
    connectionOptions.ssl_client_options(sslClientOptions).allowed_mechs("EXTERNAL");
    // Validate the server certificate against this name:
    connectionOptions.peer_hostname(_options.getHost());
    e.container().client_connection_options(connectionOptions);

    std::string senderUrl = "amqps://" + _options.getHost() + ":" + std::to_string(_options.getPort()) + "/request." + _options.getAccount();
    std::string responseUrl = "amqps://" + _options.getHost() + ":" + std::to_string(_options.getPort()) + "/response." + _options.getAccount();

    _sender = e.container().open_sender(senderUrl);
    std::cout << "-I- Sender created " << senderUrl << std::endl;

    _receiver = _sender.connection().open_receiver("response." + _options.getAccount() , proton::link_options().dynamic_address(false));
    std::cout << "-I- Listening on " << responseUrl << std::endl;
}

void RequestResponse::on_link_open(proton::event &e)
{
    if (e.link() == _sender)
    {
        proton::message req;
        req.body("Hello - request");
        req.reply_to("response/response." + _options.getAccount());
        _sender.send(req);
        std::cout << "-I- Request message sent" << std::endl;
    }
}

void RequestResponse::on_message(proton::event &e)
{
    proton::message& msg = e.message();

    std::cout << msg.body() << std::endl;
    _received++;

    e.receiver().close();
    e.connection().close();

    if (!!_receiver) _receiver.close();
    if (!!_sender) _sender.close();
}


void RequestResponse::run()
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

