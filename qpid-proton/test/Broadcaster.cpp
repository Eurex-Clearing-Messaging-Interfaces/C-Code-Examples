#include <iostream>

#include <proton/default_container.hpp>
#include <proton/acceptor.hpp>
#include <proton/connection.hpp>
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

void Broadcaster::on_container_start(proton::container &c)
{
    proton::connection_options connectionOptions;
    connectionOptions.sasl_allow_insecure_mechs(true);
    connectionOptions.sasl_allowed_mechs("PLAIN");
    c.client_connection_options(connectionOptions);

    std::string url = "amqp://" + _options.getAccount() + ":" + _options.getPassword() + "@" + _options.getHost() + ":" + std::to_string(_options.getPort()) + "/" + _exchange;

    _sender = c.open_sender(url);
}

void Broadcaster::on_sendable(proton::sender &s)
{
    while (s.credit() && _sent < _count)
    {
        proton::message msg;
        std::map<std::string, int> m;
        m["sequence"] = _sent + 1;
        msg.id(_sent + 1);
        msg.subject(_routingKey);
        msg.body(m);
        s.send(msg);
        _sent++;
    }
}

void Broadcaster::on_tracker_accept(proton::tracker &t)
{
    _confirmed++;
    if (_confirmed == _count)
    {
        std::cout << "-I- All messages (" << _confirmed << ") confirmed" << std::endl;
        t.connection().close();
    }
}

void Broadcaster::on_transport_close(proton::transport &t)
{
    _sent = _confirmed;
}

void Broadcaster::run()
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

