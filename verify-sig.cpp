#include <iostream>
#include <string>
#include "secp256k1.hpp"

int main(int argc, char** argv){

	if( argc != 5 ){
        std::cerr << "Run the program with <file_to_verify> <signature_file> <pub_key_x> <pub_key_y>" << std::endl;
        exit(0);
    }

	EllipticCurve curve = Secp256k1();
	std::string file_to_sign = argv[1];
	std::string sig_file = argv[2];
	std::string hex_str = "0x";
	std::string pub_x = argv[3];
	std::string pub_y = argv[4];

	Point pub_key = {
		bigint(hex_str + pub_x),
		bigint(hex_str + pub_y)
	};

 	if( curve.Validate(file_to_sign, sig_file, pub_key) ) {
        std::cout << "===> Signature is valid!" << std::endl;
    }
    else {
        std::cout << "===> WARNING: Signature is invalid!" << std::endl;
    }

	return 0;
}