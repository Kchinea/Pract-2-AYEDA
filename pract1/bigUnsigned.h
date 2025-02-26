#ifndef BUGUNSIGNED_H 
#define BUGUNSIGNED_H 

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

class BigUnsigned{
 public:
  BigUnsigned(unsigned n = 0);
  BigUnsigned(const unsigned char* );
  BigUnsigned(const BigUnsigned&); 
  BigUnsigned& operator=(const BigUnsigned&);
  //entrada salida
  friend std::ostream& operator<<(std::ostream&, const BigUnsigned&);
  friend std::istream& operator>>(std::istream&, BigUnsigned&);
  //comparacion
  bool operator==(const BigUnsigned&) const;
  bool operator<(const BigUnsigned& ) const;
  //aumento y decremento
  BigUnsigned& operator++(); // Pre-incremento  ++i
  BigUnsigned operator++(int); // Post-incremento i++
  BigUnsigned& operator--(); // Pre-decremento
  BigUnsigned operator--(int); // Post-decremento i*i
  //operaciones
  friend BigUnsigned operator+(const BigUnsigned&, const BigUnsigned&);
  BigUnsigned operator-(const BigUnsigned&) const;
  BigUnsigned operator*(const BigUnsigned&) const;
  friend BigUnsigned operator/(const BigUnsigned&, const BigUnsigned&);
  BigUnsigned operator%(const BigUnsigned&) const;
 private:
 std::vector<unsigned char> digits_;
};

#endif