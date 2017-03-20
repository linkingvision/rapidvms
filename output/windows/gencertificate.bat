mkdir certificate
del certificate\client.*
del certificate\server.*
REM using "pass" for every password

openssl genrsa -des3 -out certificate\client.key 1024
openssl req -new -key certificate\client.key -out certificate\client.csr

copy certificate\client.key certificate\client.key.orig

openssl rsa -in certificate\client.key.orig -out certificate\client.key

openssl x509 -req -days 3650 -in certificate\client.csr -signkey certificate\client.key -out certificate\client.crt

copy certificate\client.crt certificate\client.pem
type certificate\client.key >> certificate\client.pem

openssl pkcs12 -export -inkey certificate\client.key -in certificate\client.pem -name certificate\clientName -out certificate\client.pfx



openssl genrsa -des3 -out certificate\server.key 1024
openssl req -new -key certificate\server.key -out certificate\server.csr

copy certificate\server.key certificate\server.key.orig

openssl rsa -in certificate\server.key.orig -out certificate\server.key

openssl x509 -req -days 3650 -in certificate\server.csr -signkey certificate\server.key -out certificate\server.crt

copy certificate\server.crt certificate\server.pem
type certificate\server.key >> certificate\server.pem

openssl pkcs12 -export -inkey certificate\server.key -in certificate\server.pem -name certificate\serverName -out certificate\server.pfx