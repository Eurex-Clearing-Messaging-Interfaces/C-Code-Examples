[![Build Status](https://travis-ci.org/Eurex-Clearing-Messaging-Interfaces/C-Code-Examples.svg?branch=master)](https://travis-ci.org/Eurex-Clearing-Messaging-Interfaces/C-Code-Examples)
[![CircleCI](https://circleci.com/gh/Eurex-Clearing-Messaging-Interfaces/C-Code-Examples.svg?style=svg)](https://circleci.com/gh/Eurex-Clearing-Messaging-Interfaces/C-Code-Examples)

# building

In top directory execute following commands
```
mkdir build
cd build
cmake ..
make
ctest
```

# qpid-messaging

Examples in qpid-messaging folder are using the Qpid Messaging C++ API. It supports both AMQP 1.0 and 0-10. To run the examples:
- Install Qpid Proton C library (only needed for AMQP 1.0 support)
- Install Qpid Messaging C++ API
- Change the hostname / IP address, port number, paths to the certificates and queue names
- Run the examples

## SSL (Linux)

The C++ libraries are using the NSS tools to handle SSL. The NSS database has to be prepared before running the client and configured using environment variables.

### Preparing the NSS database

1. Create the database: `certutil -N -d sql:<path>`
2. Load the broker public key as a trusted peer `certutil -A -d sql:<path> -t "P,," -n broker -i certutil -A -d sql:./ -t "P,," -n ecag-fixml-simu1 -i ecag-fixml-simu1_deutsche-boerse_com.crt`
3. Load the member public key `certutil -A -d sql:<path> -t ",," -n ABCFR_ABCFRALMMACC1 -i ABCFR_ABCFRALMMACC1.crt`
4. Load the member private key `pk12util -i ABCFR_ABCFRALMMACC1.p12 -d sql:<path>`
5. Create the password file `echo "<password>" > <passwordFile>`

### Configuring the Qpid libraries

The Qpid Messaging C++ libraries are using environment varible to configure the SSL certificates. There are three relevant variables:
- `QPID_SSL_CERT_DB` contains the path to the NSS database
- `QPID_SSL_CERT_PASSWORD_FILE` contains the path to the password file for the database
- `QPID_SSL_CERT_NAME` contains the nick name of the certificate which should be used for the connection

for example:
```
export QPID_SSL_CERT_DB=sql:./tests/resources/
export QPID_SSL_CERT_PASSWORD_FILE=tests/resources/pwdfile
export QPID_SSL_CERT_NAME=ABCFR_ABCFRALMMACC1
```

## SSL (Windows)

The Qpid client application under the Windows can either use certificates for authentication against the broker from the system’s certificate store or the certificates may be provided to the application from files. To use certificates from the system’s store, one has to first properly import them. 

### Broker public key

The public key for verifying the identity of Eurex’s AMQP broker has to be always stored directly in Windows certificate store. To import it, follow these steps:

1. Start the certmgr.msc for managing windows certificates
2. Expand the Trusted Root Certification Authorities store
3. Right-click on the Certificates folder and select "Import"
4. Import the broker public key

### Private key in Windows certificate store

To store the private key in the Windows certificate store and use it in your application, follow these steps:

1. Prepare the private key in PKCS12 file
2. Double click the PKCS12 file
3. A dialog window pops up which guides you through the import
4. Use environment variables to point your application to the private key
```
set QPID_SSL_CERT_STORE=<CertificateStore>
set QPID_SSL_CERT_NAME=<friendlyName>
```
For example:
```
set QPID_SSL_CERT_STORE=Personal
set QPID_SSL_CERT_NAME=CN=ABCFR_ABCFRALMMACC1
```

### Using private key directly from PKCS12 file

The private key can be also used directly from the PKCS12 file.

1. Prepare the private key in PKCS12 file
2. Create the password file (a text file containing the password to the PKCS12 file)
3. Use environment variables to point your application to the private key
```
set QPID_SSL_CERT_FILENAME=<certificateFile>
set QPID_SSL_CERT_PASSWORD_FILE=<passwordFile>
set QPID_SSL_CERT_NAME=<friendlyName>
```

For example:
```
set QPID_SSL_CERT_FILENAME=ABCFR_ABCFRALMMACC1.p12
set QPID_SSL_CERT_PASSWORD_FILE=ABCFR_ABCFRALMMACC1.pwd
set QPID_SSL_CERT_NAME=abcfr_abcfralmmacc1
```

## Running the examples

Example executable `demo_qpid-messaging` is created in `<build directory>/qpid-messaging/src`. This executable can be run in two modes - as broadcast receiver or as request sender / response receiver. The `demo_qpid-messaging` program accepts following options:
```
./demo_qpid-messaging [options] <mode>
  Options:" << std::endl;
    -a <account>   Account (default=ABCFR_ABCFRALMMACC1)
    -h <hostname>  Hostname (default=ecaf-fixml-simu1.deutsche-boerse.com)
    -p <port>      Port (default=10170)
    -t <timeout>   Timeout (default=10)
    -1             Enable AMQP 1.0 protocol
  Modes:
    broadcast_receiver - Receive broadcasts.
    request_response   - Send request and receive response.
```

To run the executable:

1. Configure SSL
2. Run `./demo_qpid-messaging -a ABCFR_ABCFRALMMACC1 -h ecag-fixml-dev1 -p 5671 request_response` to send request / receive response
3. Run `./demo_qpid-messaging -a ABCFR_ABCFRALMMACC1 -h ecag-fixml-dev1 -p 5671 broadcast_receiver` to receive broadcasts
4. Use the option -1 to use AMQP 1.0

## Source code

### Broadcast Receiver

The code responsible for receiving broadcasts is in the files `BroadcastReceiver.h` and `BroadcastReceiver.cpp`. This code handles both AMQP 0-10 and 1.0.

### Request / Response

The code responsible for sending requests / receiving responses is in the files `RequestResponse.h` and `RequestResponse.cpp`. This code handles both AMQP 0-10 and 1.0.

# qpid-proton

Examples in qpid-proton folder are using the (C++ binding) of Qpid Proton API. It supports both AMQP 1.0. To run the examples:

- Install Qpid Proton C library along with C++ binding
- Change the hostname / IP address, port number, paths to the certificates and queue names
- Run examples

## SSL

To use SSL private key in PEM format and public key in crt format are needed.

## Running the examples

Example executable `demo_qpid-proton` is created in `<build directory>/qpid-proton/src`. This executable can be run in two modes - as broadcast receiver or as request sender / response receiver. The `demo_qpid-proton` program accepts following options:
```
./demo_qpid-proton [options] <mode>
  Options:
    -a <account>   Account (default=ABCFR_ABCFRALMMACC1)
    -h <hostname>  Hostname (default=ecag-fixml-simu1.deutsche-boerse.com)
    -p <port>      Port (default=10170)
    -c <path>      Path to host certificate (default=ecag-fixml-simu1.crt)
    -k <path>      Path to account private key (default=ABCFR_ABCFRALMMACC1.pem)
    -w <password>  Account private key password (default=123456)
  Modes:
    broadcast_receiver - Receive broadcasts.
    request_response   - Send request and receive response.
```

To run the executable:

1. Prepare private key and public certificate
2. Run `./demo_qpid-proton -a ABCFR_ABCFRALMMACC1 -h ecag-fixml-dev1 -p 5671 -c ecag-fixml-simu1.crt -k ABCFR_ABCFRALMMACC1.pem request_response` to send request / receive response
3. Run `./demo_qpid-proton -a ABCFR_ABCFRALMMACC1 -h ecag-fixml-dev1 -p 5671 -c ecag-fixml-simu1.crt -k ABCFR_ABCFRALMMACC1.pem broadcast_receiver` to receive broadcasts

## Source code

### Broadcast Receiver

The code responsible for receiving broadcasts is in the files `BroadcastReceiver.h` and `BroadcastReceiver.cpp`. This code handles only AMQP 1.0.

### Request / Response

The code responsible for sending requests / receiving responses is in the files `RequestResponse.h` and `RequestResponse.cpp`. This code handles only AMQP 1.0.

# Integration tests

The project is using Travis-CI and Circle CI to run its own integration tests. The tests are executed against Docker images which contain the AMQP broker with configuration corresponding to Eurex Clearing FIXML Interface. The details of the integration tests can be found in the .travis.yml and circle.yml files.

# Documentation

More details about C++ APIs and code examples can be found in the Volume B of Eurex Clearing Messaging Interfaces documentation on [Eurex Clearing website](http://www.eurexclearing.com/clearing-en/technology/eurex-release14/system-documentation/system-documentation/861464?frag=861450).
