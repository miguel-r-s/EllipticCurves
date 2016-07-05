#include <iostream>
#include <vector>
#include <chrono>
#include "nistp224.hpp"
#include "secp256k1.hpp"

using ns = std::chrono::nanoseconds;
using get_time = std::chrono::system_clock;

int main(void) {

	EllipticCurve* curve;
	std::vector<Point> points_vector;
	int sampleSize = 1E2;

	int option;
	do {
		std::cout << "Choose a curve: " << std::endl;
		std::cout << "1) nistp224     " << std::endl;
		std::cout << "2) secp256k1    " << std::endl;
		std::cout << "Option: ";
		std::cin >> option;
		std::cin.clear();
		std::cin.ignore(100, '\n');
	} while(option != 1 && option != 2);

	if( option == 1 )
		curve = new NISTP224();
	else 
		curve = new Secp256k1();


	/* Key generation benchmarks */
	auto start = get_time::now();
	for( int i = 0; i < sampleSize; i++){
		cpp_int secretKey = curve->getSecretKey();
		Point publicValue = curve->getPublicValue(secretKey);
		points_vector.push_back(publicValue);
	}
	
	std::chrono::duration<double> diff = get_time::now() - start;
	int seconds = diff.count();
	std::cout << "Benchmark for key generation: " << static_cast<double>(sampleSize)/(seconds) << " key pairs per second" << std::endl;

	int i;
	std::cin >> i;
	/* Have to print this so the compiler knows it can't 
	  optimize this away and skip the computations*/
	for( int i = 0; i < sampleSize; i++){
		std::cout << points_vector.at(i) << std::endl;
	}
	
	delete curve;
	return 0;
}