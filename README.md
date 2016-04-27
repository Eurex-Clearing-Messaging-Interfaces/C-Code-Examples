[![Build Status](https://travis-ci.org/Eurex-Clearing-Messaging-Interfaces/C-Code-Examples.svg?branch=master)](https://travis-ci.org/Eurex-Clearing-Messaging-Interfaces/C-Code-Examples)

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

## BroadcastReceiver.cpp

This example connects to the AMQP broker using AMQP version 0-10 or 1.0, opens a consumer to the broadcast queue and starts consuming the broadcasts.

## RequestResponse.cpp

This example connects to the AMQP broker using AMQP version 0-10 or 1.0, sends a request message and wait for a response, which should be sent by the Eurex system.

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

# Documentation

More details about C++ APIs and code examples can be found in the Volume B of Eurex Clearing Messaging Interfaces documentation on [Eurex Clearing website](http://www.eurexclearing.com/clearing-en/technology/eurex-release14/system-documentation/system-documentation/861464?frag=861450).
