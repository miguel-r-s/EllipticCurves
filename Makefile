CC=clang++
FLAGS=-O3 -std=c++11 -Wall -pedantic -lgmp -lboost_system -lcrypto -lboost_random

SIGNATURE=signature
VERIFYSIG=verify-sig
DIFFIEHELLMAN=diffie-hellman
PERFBENCHMARK=performance-benchmark

all: signature verify-sig diffie-hellman performance-benchmark

signature: signature.cpp EllipticCurve.hpp secp256k1.hpp
	@echo "== Compiling signature example =="
	$(CC) $(FLAGS) signature.cpp -o $(SIGNATURE)

verify-sig: verify-sig.cpp EllipticCurve.hpp secp256k1.hpp
	@echo "== Compiling signature example =="
	$(CC) $(FLAGS) verify-sig.cpp -o $(VERIFYSIG)

diffie-hellman: diffie-hellman.cpp EllipticCurve.hpp secp256k1.hpp nistp224.hpp
	@echo "== Compiling Diffie-Hellman example =="
	$(CC) $(FLAGS) diffie-hellman.cpp -o $(DIFFIEHELLMAN)

performance-benchmark: performance-benchmark.cpp EllipticCurve.hpp secp256k1.hpp nistp224.hpp
	@echo "== Compiling performance-benchmark example =="
	$(CC) $(FLAGS) performance-benchmark.cpp -o $(PERFBENCHMARK)

c: clean
clean:
	rm -rf $(SIGNATURE) $(DIFFIEHELLMAN) $(PERFBENCHMARK) $(VERIFYSIG) *.sig