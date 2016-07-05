#include <iostream>
#include "secp256k1.hpp"

typedef struct {

    bigint secret;
    Point public_key;
} Person;

/* This module examplifies my implementation of ECDSA.
   The module uses a secp256k1 curve. */
/* A file is signed and the signature is checked. */

int main(int argc, char** argv) {

    if( argc != 2 ){
        std::cerr << "Run the program with -s <file_to_sign.txt>" << std::endl;
        exit(0);
    }

    EllipticCurve curve = Secp256k1();

    std::string file_to_sign(argv[1]);
    Person Bob; 
    Bob.secret = curve.getSecretKey();
    Bob.public_key = curve.getPublicValue(Bob.secret);
    
    std::cout << std::hex;
    std::cout << "Signing with public key: " << Bob.public_key << std::endl;
    std::cout << "Signing with secret key: " << Bob.secret << std::endl;
    std::cout << "Signature file         : " << file_to_sign << ".sig" << std::endl;

    /* This is where the actual signing occurs */
    curve.Sign(file_to_sign.c_str(), Bob.secret, Bob.public_key, true);

    /* This validation would be performed independently
    by anyone who had the signature, the file, and Bob's public key. */
    if( curve.Validate(file_to_sign, file_to_sign + ".sig", Bob.public_key) ) {
        std::cout << "===> Signature is valid!" << std::endl;
    }
    else {
        std::cout << "===> WARNING: Signature is invalid!" << std::endl;
    }

    return 0;
}