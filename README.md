# Installing dependencies

```bash
$ chmod a+x dependencies.sh 
$ ./dependencies.sh
```
The script dependencies.sh is designed to work with Fedora.
If any problems arise, contact me at miguelrodriguesdossantos@gmail.com.

# Tests

To test the correctness of the implementation, I wrote some test cases.

* signature.cpp

...Signs a document and verifies the validity of the signature.
* verify.cpp

...Verifies the validity of a given signature.
* diffie-hellman.cpp

...Uses ECC to exchange a symmetric key between two individuals (ECDH).
* performance-benchmark.cpp

...Measure how many key pairs per second a given machine is capable of generating using this implementation.

## Compiling the tests

$ make all

This creates all the executables: "signature", "verify", "diffie-hellman", "performance-benchmark", .

## Running the tests

To run "signature", pass a file as an argument.
This file will be signed and the signature will be verified.
Example:

```bash
$ ./signature README.txt 
Signing with public key: D3DF610A580209070840D4BA9078D9DCCD0F6EE526BFCF858D38FB4A93D9E9F8 3AE61BB01033A451FCA47758A87A572CE658A958DA8BB6DB421AD11EAF23B797
Signing with secret key: B1ED407D2799D701D2C0708FF158A0D34EBC4501C42B60FD6BC209E3D2EFDC00
Signature file         : README.txt.sig
File hash: 78e2a90ba3ede7cadd20df69044871b0b4c180b8096fd7de1b1b4748b28e33cb
===> Signature is valid!
```
You can then verify the signature using
```bash
$ ./verify-sig README.txt README.txt.sig D3DF610A580209070840D4BA9078D9DCCD0F6EE526BFCF858D38FB4A93D9E9F8 3AE61BB01033A451FCA47758A87A572CE658A958DA8BB6DB421AD11EAF23B797
===> Signature is valid!
```
To run diffie-hellman:

```bash
$ ./diffie-hellman 
Choose a curve: 
1) nistp224 
2) secp256k1 
Option: 1
Name         : Alice
Secret       : B1905C680555D1E6B20D90C204466A115152B48C80C699F96BD645D9
Point        : B01AB449B8C90DCEF018ED787FBD136A0E98A2BB93F01B93833E9788 3D0401D6A9C27AECF13C08F7BC284171D3BBE469588FEEE9A0687301
SharedSecret : F150CC687DAA804271C37B2E322FFD6688E0B8C00934BA3CE3F32489 E029A730490F781BC02C91639BBF592189F6BAFD157805CA2B9B30FB

Name         : Bob
Secret       : DBD5112A09BA2D636014DE980FC6A1D2CA66615FAE4379A92F7D2FC2
Point        : B16352798D723DE9492AF01D74637ABF8606FDEDF3EFC844A9808FD6 8B59ED91C00DB588058E4B939A6D55DD8BAF3474429D49A5BE2B5E20
SharedSecret : F150CC687DAA804271C37B2E322FFD6688E0B8C00934BA3CE3F32489 E029A730490F781BC02C91639BBF592189F6BAFD157805CA2B9B30FB

The secrets match: Diffie-Hellman was performed successfully!
```

To run the performance tests:

```bash
$ ./performance-benchmark 
Choose a curve: 
1) nistp224     
2) secp256k1    
Option: 1
Benchmark: 21.2766 key pairs per second
```
