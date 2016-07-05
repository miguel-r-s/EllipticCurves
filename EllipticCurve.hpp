#include <cmath>
#include <ctime>
#include <cmath>
#include <bitset>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <utility>
#include <iostream>

#include <openssl/sha.h>

#pragma once

/* Arbitrary precision integers */
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/multiprecision/random.hpp>
#include <boost/random/random_device.hpp>

using namespace boost::multiprecision;
using namespace boost::random;

typedef boost::multiprecision::cpp_int bigint;
typedef independent_bits_engine<mt19937, 256, bigint> generator_type;

random_device dev;
generator_type gen(dev);

// http://stackoverflow.com/a/10632725
std::string sha256(const std::string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string sha256_file(const std::string file_name) {
	std::ifstream ifs(file_name);
	std::string content( (std::istreambuf_iterator<char>(ifs) ),
						(std::istreambuf_iterator<char>()    ) );

	return sha256(content);
}

inline cpp_int modulo(cpp_int a, cpp_int b) {
	cpp_int m = a%b;
	return m>=0 ? m:m+b;
}

/* This is ugly, bad practice, yet extremely convenient.
	Boost is not allowing me to make an array
	cpp_int x[3]. So I just had to emulate an array
	by using the macro "correct_val" to select the wanted variable.
	To my knowledge, there is no better alternative.
*/
#define correct_val(x0,x1,x2,i) (((i)%3)?(((i)&1)?x1:x2):x0)

cpp_int modInverse(cpp_int a, cpp_int b) {

	cpp_int x0, x1, x2;
	cpp_int y0, y1, y2;
	cpp_int quotient  = abs(a / b);
	cpp_int remainder = modulo(a, b);
	
	x0 = 0;
	y0 = 1;
	x1 = 1;
	y1 = -quotient;

	cpp_int i = 2;
	for (; (b % (a%b)) != 0; i++) {
		a = b;
		b = remainder;
		quotient = abs(a / b);
		remainder = modulo(a, b);
		correct_val(x0,x1,x2,i) = (-quotient * correct_val(x0,x1,x2,i-1)) + correct_val(x0,x1,x2,i-2);
		correct_val(y0,y1,y2,i) = (-quotient * correct_val(y0,y1,y2,i-1)) + correct_val(y0,y1,y2,i-2);
	}
	
	return correct_val(x0,x1,x2,i-1);
}

typedef struct point {
	cpp_int x,y;

	inline bool operator==(const point& rhs){
		return (x == rhs.x && y == rhs.y);
	}

	inline bool operator!=(const point& rhs){
		return !(*this == rhs);
	}

} Point;


std::ostream& operator<<(std::ostream& out, const point& obj) {
   	return out << std::hex 
   				<< obj.x << " " << obj.y;
}

std::istream& operator>>(std::istream& in, point& obj){
	std::string x, y;
	in >> x >> y;
	obj.x = bigint("0x" + x);
	obj.y = bigint("0x" + y);
	return in;
}

std::string string_to_binary(const std::string& input)
{
    std::ostringstream oss;
    for(auto c : input) {
        oss << std::bitset<8>(c);
    }
    return oss.str();
}

class EllipticCurve {

/* To do: change to private */
private:
/* p, a, b parameters of the curve y^2 = x^3 + ax + b */
	const cpp_int p; /* Modulus of the group */
	const cpp_int a;
	const cpp_int b;

	/* Generator */
	const Point G;

	/* Order of G */
	const cpp_int n;

	/* Cofactor */
	const cpp_int h;

	Point PointAdd(Point p1, Point p2){

		if( p1 == p2 ){
			return PointDouble(p1);
		}

		cpp_int xp = p1.x, yp = p1.y;
		cpp_int xq = p2.x, yq = p2.y;
		
		if( xp == xq ){
			return {0,0};
		}

		if( p1 == Point{0,0} ){
			return p2;
		}

		if( p2 == Point{0,0} ){
			return p1;
		}

		cpp_int inv = modInverse(xq-xp, p);
		cpp_int lambda = (yq-yp)*inv;
		cpp_int xr = modulo((pow(lambda, 2) - xp - xq), p);
		cpp_int yr = modulo((lambda * (xp - xr) - yp), p);
		
		if(p1.y == p2.y)
			std::cout << p1 << " + " << p2 << " = " << Point{xr,yr} << std::endl;
		return {xr,yr};
	}

	Point PointDouble(Point p1){

		cpp_int xp = p1.x, yp = p1.y;

		cpp_int inv = modInverse(2*yp, p);
		cpp_int lambda = (3*static_cast<cpp_int>(pow(xp,2))+a)*inv;
		cpp_int xr = modulo((static_cast<cpp_int>(pow(lambda, 2)) - 2*xp),p);
		cpp_int yr = modulo((lambda * (xp - xr) - yp),p);
		return {xr,yr};
	}

	Point PointMultiplication(cpp_int k, Point P) {

		if( k == 0 ) {
			return {0,0};
		}
		if( k == 1 ){ /* Base case for the recursion */
			return P;
		}
		if( !(k&1) ) { /* k is even */
			return PointDouble(PointMultiplication(k/2, P));
		}
		else { /* k is odd */
			return PointAdd(P, PointDouble(PointMultiplication((k-1)/2, P)));
		}
	}

	// wikipedia.org/wiki/Elliptic_Curve_Digital_Signature_Algorithm
	Point Signature(std::string msg_hash, cpp_int secret_key, Point public_point) {
		
		cpp_int r = 0;
		cpp_int s = 0;
		cpp_int z = bigint(string_to_binary(msg_hash));

		do {
			cpp_int k = getSecretKey(); /* This is just a random value, less than the order of the curve. */
			Point p1 = PointMultiplication(k, G);
			r = modulo(p1.x, n);
			s = modulo(modInverse(k,n) * (z + r * secret_key), n) ;
		} while(r == 0 || s == 0);

		return {r,s};
	}

	bool VerifySignature(std::string msg_hash, Point signature, Point public_key) {
		/* Checking parameters */
		if( public_key == Point{0,0}){
			return false;
		}
		if( !pointIsInEllipticCurve(public_key)){
			return false;
		}
		if( !(PointMultiplication(n, public_key) == Point{0,0})) {
			return false;
		}

		cpp_int r = signature.x;
		cpp_int s = signature.y;
		cpp_int z = bigint(string_to_binary(msg_hash));

		if( !(r > 0 && r < n && s > 0 && s < n) ) {
			return false;
		}

		cpp_int w  = modInverse(s, n);
		cpp_int u1 = modulo(z * w, n);
		cpp_int u2 = modulo(r * w, n);

		Point curve_point = PointAdd(PointMultiplication(u1, G),
									 PointMultiplication(u2, public_key));
		
		if( ! pointIsInEllipticCurve(curve_point)){
			return false;
		}

		return (r == curve_point.x);
	}

public:

	EllipticCurve(cpp_int p, cpp_int a, cpp_int b, Point generator, cpp_int n, cpp_int h):
	p(p), a(a), b(b), G(generator), n(n), h(h)
	{
		
	}

	cpp_int getSecretKey(){
		return gen()%(n-1) + 1;
	}

	Point getPublicValue(cpp_int secretKey) {
		return PointMultiplication(secretKey, G);
	}

	Point computeSharedSecret(cpp_int secretKey, Point publicPoint) {
		return PointMultiplication(secretKey, publicPoint);
	}

	bool pointIsInEllipticCurve(Point pt) {
		return modulo(pow(pt.y, 2), p) == modulo(pow(pt.x,3) + a*pt.x + b, p);
	}

	void Sign(const std::string fileName, cpp_int secretKey, Point publicPoint, bool printFileHash = false) {

		std::ofstream ofs(fileName + ".sig");
		std::string fileHash = sha256_file(fileName);
		if( printFileHash ){
			std::cout << "File hash: " << fileHash << std::endl;
		}

		Point signature = Signature(fileHash, secretKey, publicPoint);
		ofs << signature;
	}

	bool Validate(const std::string fileName, const std::string sigFileName, Point publicKey) {
		
		Point sig;
		std::string fileHash = sha256_file(fileName);
		std::ifstream ifs(sigFileName);

		ifs >> sig;
		return VerifySignature(fileHash, sig, publicKey);
	}
};


