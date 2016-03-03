# qpid-messaging

Examples in qpid.messaging folder are using the Qpid Messaging C++ API. It supports both AMQP 1.0 and 0-10. To run the examples:
- Install Qpid Proton C library (only needed for AMQP 1.0 support)
- Install Qpid Messaging C++ API
- Change the hostname / IP address, port number, paths to the certificates and queue names
- Run the examples

## broadcast_receiver.cpp

This example connects to the AMQP broker using AMQP 0-10, opens a consumer to the broadcast queue and starts consuming the broadcasts.

## request_response.cpp

This example connects to the AMQP broker using AMQP 0-10, sends a request message and wait for a response, which should be sent by the Eurex system.

## 10_broadcast_receiver.cpp

This example connects to the AMQP broker using AMQP 1.0, opens a consumer to the broadcast queue and starts consuming the broadcasts.

## 10_request_response.cpp

This example connects to the AMQP broker using AMQP 1.0, sends a request message and wait for a response, which should be sent by the Eurex system.

# Documentation

More details about Python APIs and code examples can be found in the Volume B of Eurex Clearing Messaging Interfaces documentation on http://www.eurexclearing.com/clearing-en/technology/eurex-release14/system-documentation/system-documentation/861464?frag=861450
