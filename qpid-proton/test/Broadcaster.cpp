#include <iostream>

#include <proton/acceptor.hpp>
#include <proton/connection.hpp>
#include <proton/container.hpp>
#include <proton/event.hpp>
#include <proton/handler.hpp>
#include <proton/value.hpp>

#include "Broadcaster.h"

Broadcaster::Broadcaster(const ServerOptions &options,
                         const std::string &exchange,
                         const std::string &routingKey,
                         unsigned int count)
    : _options(options)
    , _count(count)
    , _sent(0)
    , _confirmed(0)
    , _exchange(exchange)
    , _routingKey(routingKey)
{
}

void Broadcaster::on_start(proton::event &e)
{
    proton::connection_options connectionOptions;
    connectionOptions.allow_insecure_mechs(true);
    connectionOptions.allowed_mechs("PLAIN");
    e.container().client_connection_options(connectionOptions);

    std::string url = "amqp://" + _options.getAccount() + ":" + _options.getPassword() + "@" + _options.getHost() + ":" + std::to_string(_options.getPort()) + "/" + _exchange;

    _sender = e.container().open_sender(url);
}

void Broadcaster::on_sendable(proton::event &e)
{
    proton::sender sender = e.sender();
    while (sender.credit() && _sent < _count)
    {
        proton::message msg;
        std::map<std::string, int> m;
        m["sequence"] = _sent + 1;
        msg.id(_sent + 1);
        msg.subject(_routingKey);
        msg.body(m);
        sender.send(msg);
        _sent++;
    }
}

void Broadcaster::on_delivery_accept(proton::event &e)
{
    _confirmed++;
    if (_confirmed == _count)
    {
        std::cout << "-I- All messages (" << _confirmed << ") confirmed" << std::endl;
        e.connection().close();
    }
}

void Broadcaster::on_transport_close(proton::event &e)
{
    _sent = _confirmed;
}

void Broadcaster::run()
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

