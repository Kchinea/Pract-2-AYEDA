#include <iostream>
#include <fstream>
#include "bigInteger.h"

BigUnsigned promedio(std::vector<BigUnsigned> others) {
  BigUnsigned total;
  for (int i = 0; i < others.size() ; i++) {
    total = total + others[i]; 
  }
  int tamaño =others.size();
  return total/tamaño;
}

BigUnsigned Euclides(const BigUnsigned& a, const BigUnsigned& b) {
  BigUnsigned zero;
  if (b == zero) {
    throw std::invalid_argument("El divisor no puede ser cero.");
  }
  BigUnsigned x = a;
  BigUnsigned y = b;
  while (!(y == zero)) {
    BigUnsigned temp = y;
    y = x % y;
    x = temp;
  }
  return x;  
}

void readBigUnsignedFromFile(const std::string& filename, BigUnsigned& number) {
  std::ifstream file(filename);
  if (!file) {
    throw std::runtime_error("Error al abrir el archivo: " + filename);
  }
  std::string numStr;
  file >> numStr;
  number = BigUnsigned(reinterpret_cast<const unsigned char*>(numStr.c_str()));
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Uso: " << argv[0] << " <archivo1> <archivo2>" << std::endl;
    return 1;
  }
  try {
    BigUnsigned num1, num2;
    readBigUnsignedFromFile(argv[1], num1);
    readBigUnsignedFromFile(argv[2], num2);
    std::vector<BigUnsigned> modifi{num1,num2};
    std::cout << "Número 1: " << num1 << std::endl;
    std::cout << "Número 2: " << num2 << std::endl;
    std::cout << "div: " <<  num1 / num2 << std::endl;
    std::cout << "Modi: " <<  promedio(modifi) << std::endl;
    std::cout << "mcd: " <<  Euclides(num1,num2) << std::endl;
  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
