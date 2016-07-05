#include <iostream>
#include "secp256k1.hpp"
#include "nistp224.hpp"

typedef struct {

	std::string name;
	cpp_int secret;
	Point p;
	Point sharedSecret;
} Person;

void printPerson(Person p) {

	std::cout << std::hex;
	std::cout << "Name         : " << p.name << std::endl;
	std::cout << "Secret       : " << p.secret << std::endl;
	std::cout << "Point        : " << p.p << std::endl;
	std::cout << "SharedSecret : " << p.sharedSecret << std::endl;
	std::cout << std::endl;
}

int main(int argc, char** argv) {

	EllipticCurve* curve;

	int option;
	do {
		std::cout << "Choose a curve: " << std::endl;
		std::cout << "1) nistp224 " << std::endl;
		std::cout << "2) secp256k1 " << std::endl;
		std::cout << "Option: ";
		std::cin >> option;
		std::cin.clear();
		std::cin.ignore(100, '\n');
	} while(option != 1 && option != 2);

	if( option == 1 )
		curve = new NISTP224();
	else 
		curve = new Secp256k1();

	Person Alice, Bob;
	Alice.name = "Alice";
	Bob.name = "Bob";

	Alice.secret = curve->getSecretKey();
	Bob.secret   = curve->getSecretKey();

	Alice.p = curve->getPublicValue(Alice.secret);
	Bob.p   = curve->getPublicValue(Bob.secret);
	
	Alice.sharedSecret = curve->computeSharedSecret(Alice.secret, Bob.p);
	Bob.sharedSecret   = curve->computeSharedSecret(Bob.secret, Alice.p);

	printPerson(Alice);
	printPerson(Bob);

	if( Alice.sharedSecret == Bob.sharedSecret ){
		std::cout << "The secrets match: ";
		std::cout << "Diffie-Hellman was performed successfully!" << std::endl;
	}
	else {
		std::cout << "The secrets don't match..." << std::endl;
		std::cout << "Diffie-Hellman failed miserably!" << std::endl;
	}

	delete curve;
	return 0;
}