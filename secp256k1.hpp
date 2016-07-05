#include "EllipticCurve.hpp"

class Secp256k1: public EllipticCurve {

public:

	Secp256k1() :
	EllipticCurve(
		bigint("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F"),
		0,7,
		{
			bigint("0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"),
			bigint("0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8")
		},
		bigint("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"),1) 
	{}
	~Secp256k1(){};
};