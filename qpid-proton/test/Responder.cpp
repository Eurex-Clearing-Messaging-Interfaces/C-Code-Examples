#include <iostream>

#include <proton/default_container.hpp>
#include <proton/delivery.hpp>
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

void Responder::on_container_start(proton::container &c)
{
    proton::connection_options connectionOptions;
    connectionOptions.sasl_allow_insecure_mechs(true);
    connectionOptions.sasl_allowed_mechs("PLAIN");
    c.client_connection_options(connectionOptions);

    std::string requestUrl = "amqp://" + _options.getAccount() + ":" + _options.getPassword() + "@" + _options.getHost() + ":" + std::to_string(_options.getPort()) + "/" + _requestQueue;
    std::string responseUrl = "amqp://" + _options.getAccount() + ":" + _options.getPassword() + "@" + _options.getHost() + ":" + std::to_string(_options.getPort()) + "/response";

    _receiver = c.open_receiver(requestUrl);
    std::cout << "-I- Responder: Listening on " << requestUrl << std::endl;

    //_sender = _receiver.connection().open_sender("response", proton::link_options().dynamic_address(false));
    _sender = _receiver.connection().open_sender("response");
    std::cout << "-I- Responder: Responder created " << std::endl;
}

void Responder::on_message(proton::delivery &d, proton::message &m)
{
    std::cout << "-I- Responder: message = " << m.body() << std::endl;

    proton::message res;
    res.body("Hello - response");
    std::string subject = m.reply_to();
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

void Responder::on_tracker_accept(proton::tracker &t)
{
    std::cout << "-I- Responder: Accepted" << std::endl;

    t.connection().close();
    _sender.close();
    _receiver.close();
}

void Responder::run()
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

