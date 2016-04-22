#!/bin/sh

myPath="$(dirname  $(readlink -f ${BASH_SOURCE[0]}))"
dbPath=${myPath}
pwdFile="${myPath}/amq_cert_db.pwd"
hostName="eclbgc01"
hostName="ecag-fixml-dev1"
account="CBKFR_TESTCALMMACC1"
account="ABCFR_ABCFRALMMACC1"
nodeCert="${myPath}/${hostName}.crt"
accountPem="${myPath}/${account}.pem"
accountP12="${myPath}/${account}.p12"
nssPassword="12345678"
laccount=$(echo $account | tr '[:upper:]' '[:lower:]')

rm -rf ${dbPath}/cert9.db ${dbPath}/key4.db ${dbPath}/pkcs11.txt
echo $nssPassword > $pwdFile

certutil -N -d sql:$dbPath -f $pwdFile
certutil -A -n $hostName -i $nodeCert -d sql:$dbPath -t P,, -f $pwdFile
openssl pkcs12 -export -in $accountPem -out $accountP12 -name $laccount -password pass:'123456'
pk12util -i $accountP12 -d sql:$dbPath -k $pwdFile -W 123456 > /dev/null
export QPID_SSL_CERT_DB=sql:$dbPath
export QPID_SSL_CERT_PASSWORD_FILE=$pwdFile
export QPID_SSL_CERT_NAME=$laccount
