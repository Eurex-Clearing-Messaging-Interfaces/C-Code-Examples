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

#include "Responder.h"

Responder::Responder(const ServerOptions &options,
                     const std::string &requestQueue)
    : _options(options)
    , _requestQueue(requestQueue)
{
}

void Responder::on_start(proton::event &e)
{
    proton::connection_options connectionOptions;
    connectionOptions.allow_insecure_mechs(true);
    connectionOptions.allowed_mechs("PLAIN");
    e.container().client_connection_options(connectionOptions);

    std::string requestUrl = "amqp://" + _options.getAccount() + ":" + _options.getPassword() + "@" + _options.getHost() + ":" + std::to_string(_options.getPort()) + "/" + _requestQueue;
    std::string responseUrl = "amqp://" + _options.getAccount() + ":" + _options.getPassword() + "@" + _options.getHost() + ":" + std::to_string(_options.getPort()) + "/response";

    _receiver = e.container().open_receiver(requestUrl);
    std::cout << "-I- Responder: Listening on " << requestUrl << std::endl;

    _sender = _receiver.connection().open_sender("response", proton::link_options().dynamic_address(false));
    std::cout << "-I- Responder: Responder created " << std::endl;
}

void Responder::on_message(proton::event &e)
{
    proton::message& msg = e.message();

    std::cout << "-I- Responder: message = " << msg.body() << std::endl;

    if (e.link() == _receiver)
    {
        proton::message res;
        res.body("Hello - response");
        std::string subject = msg.reply_to();
        // Cut off leading exchange
        std::size_t sPos = subject.find('/');
        if (sPos != std::string::npos)
        {
            subject.erase(0,sPos+1);
        }
        res.subject(subject);
        _sender.send(res);
        std::cout << "-I- Responder: Response message sent" << std::endl;
    }
}

void Responder::on_delivery_accept(proton::event &e)
{
    std::cout << "-I- Responder: Accepted" << std::endl;

    e.connection().close();
    _sender.close();
    _receiver.close();
}

void Responder::run()
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

