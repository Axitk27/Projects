# What are these files used for

|File|Function|
|----|--------|
|mt-lab-t4-ca.crt|Certification authority cert|
|mt-lab-t4-ca.key|Key for the certification authoritiy|
|mt-lab-t4-server.crt|Signed certificate for MQTT server|
|mt-lab-t4-server.csr|Certificate request for server|
|mt-lab-t4-server.key|Private key used on server|

## How were those certificates generated? 

Generation of CA's private key:
```bash
openssl genrsa -des3 -out mt-lab-t4-ca.key 2048
```

Generation of CA's certificate:
```bash
openssl req -x509 -new -nodes -key mt-lab-t4-ca.key -sha256 -days 3650 -out mt-lab-t4-ca.crt
```

Generate server key, certificate request and signed server certificate:
```bash
openssl genrsa -out mt-lab-t4-server.key 2048
openssl req -new -key mt-lab-t4-server.key -out mt-lab-t4-server.csr
openssl x509 -req -in mt-lab-t4-server.csr -CA mt-lab-t4-ca.crt -CAkey mt-lab-t4-ca.key -CAcreateserial -out mt-lab-t4-server.crt -days 1460
```
