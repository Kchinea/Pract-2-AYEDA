#ifndef BIGINTEGER_H 
#define BIGINTEGER_H 

#include "bigUnsigned.h"

class BigInteger{
 public:
  BigInteger(int n = 0);
  BigInteger(const BigUnsigned&); 
  BigInteger& operator=(const BigInteger&);
  //entrada salida
  friend std::ostream& operator<<(std::ostream&, const BigInteger&);
  friend std::istream& operator>>(std::istream&, BigInteger&);
  //comparacion
  bool operator==(const BigInteger&) const;
  bool operator<(const BigInteger& ) const;
  //aumento y decremento
  BigInteger& operator++(); // Pre-incremento  ++i
  BigInteger operator++(int); // Post-incremento i++
  BigInteger& operator--(); // Pre-decremento
  BigInteger operator--(int); // Post-decremento i*i
  //operaciones
  friend BigInteger operator+(const BigInteger&, const BigInteger&);
  BigInteger operator-(const BigInteger&) const;
  BigInteger operator*(const BigInteger&) const;
  friend BigInteger operator/(const BigInteger&, const BigInteger&);
  BigInteger operator%(const BigInteger&) const;
 private:
 BigUnsigned number_;
 bool sing_;
};

#endif