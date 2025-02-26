/**
 * @file bigUnsigned.cpp
 * @brief Implementación de la clase BigUnsigned.
*/

#include "bigUnsigned.h"

/**
 * @brief Constructor que inicializa un BigUnsigned a partir de un número entero sin signo.
 * @param n Número entero sin signo.
 */
BigUnsigned::BigUnsigned(unsigned n ) {
  if ( n == 0) digits_.push_back(n + '0');
  while (n > 0) {
    char digit = n % 10;
    digits_.push_back(digit + '0');
    n /= 10;
  }
  if (digits_.empty()) {
    digits_.push_back(0);
  }
}

/**
 * @brief Constructor que inicializa un BigUnsigned a partir de una cadena de caracteres.
 * @param digits Puntero a una cadena de caracteres terminada en '\0'.
 */
BigUnsigned::BigUnsigned(const unsigned char* digits) {
  unsigned char digit ;
  while ((digit = *digits++) != '\0') {
    this->digits_.push_back(digit);
  }
  std::reverse(this->digits_.begin(), this->digits_.end());
}

/**
 * @brief Constructor de copia.
 * @param other Otro objeto BigUnsigned a copiar.
 */
BigUnsigned::BigUnsigned(const BigUnsigned& other) {
  this->digits_ = other.digits_;
}

/**
 * @brief Operador de asignación.
 * @param other Otro objeto BigUnsigned.
 * @return Referencia a este objeto BigUnsigned modificado.
 */
BigUnsigned& BigUnsigned::operator=(const BigUnsigned& other) {
  if (this != &other) {
    digits_ = other.digits_;
  }
  return *this;
}

/**
 * @brief Operador de igualdad.
 * @param other Otro objeto BigUnsigned.
 * @return true si ambos objetos son iguales, false en caso contrario.
 */
bool BigUnsigned::operator==(const BigUnsigned& other) const {
  return digits_ == other.digits_;
}

/**
 * @brief Operador de comparación menor que.
 * @param other Otro objeto BigUnsigned.
 * @return true si este objeto es menor que el otro, false en caso contrario.
 */
bool BigUnsigned::operator<(const BigUnsigned& other) const {
  if (this->digits_.size() < other.digits_.size()) {
    return true;
  } else if (this->digits_.size() > other.digits_.size()) {
    return false;
  } else {
    for (int i = this->digits_.size() - 1; i >= 0; --i) {
      if (this->digits_[i] < other.digits_[i]) {
        return true;
      } else if (this->digits_[i] > other.digits_[i]) {
        return false;
      }
    }
  }
  return false;
}

/**
 * @brief Operador de suma.
 * @param a Primer operando.
 * @param b Segundo operando.
 * @return Suma de los operandos.
 */
BigUnsigned operator+(const BigUnsigned& a, const BigUnsigned& b) {
  bool carry = false;
  BigUnsigned result;
  result.digits_.clear();
  int i = 0;
  while (i < a.digits_.size() || i < b.digits_.size() || carry) {
    int digitA = (i < a.digits_.size()) ? (a.digits_[i] - '0') : 0;
    int digitB = (i < b.digits_.size()) ? (b.digits_[i] - '0') : 0;
    int sum = digitA + digitB + (carry ? 1 : 0);
    carry = sum >= 10;
    result.digits_.push_back((sum % 10)+ '0');
    i++;
  }
  return result;
}

/**
 * @brief Operador de resta.
 * @param other Otro objeto BigUnsigned.
 * @return Resultado de la resta.
 * @throw std::invalid_argument si el resultado sería negativo.
 */
BigUnsigned BigUnsigned::operator-(const BigUnsigned& other) const {
  if ((*this < other)) {
    throw std::invalid_argument("La operación da un número negativo (con signo).");
  }
  int carry = 0;
  BigUnsigned result;
  result.digits_.clear();
  for (size_t i = 0; i < this->digits_.size(); i++) {
    int digitA = (i < this->digits_.size()) ? (this->digits_[i] - '0') : 0;
    int digitB = (i < other.digits_.size()) ? (other.digits_[i] - '0') : 0;
    int substract = digitA - digitB - carry;
    if (substract < 0) {
      substract += 10;
      carry = 1;
    } else {
      carry = 0;
    }
    result.digits_.push_back(substract + '0');
  }
  while (!result.digits_.empty() && result.digits_.back() == '0') {
    result.digits_.pop_back();
  }
  if (result.digits_.empty()) {
    result.digits_.push_back('0');
  }
  return result;
}

/**
 * @brief Multiplica dos números grandes representados como BigUnsigned.
 * 
 * Implementa la multiplicación por el método tradicional de suma de productos parciales.
 * 
 * @param other El número a multiplicar con el objeto actual.
 * @return El resultado de la multiplicación como un nuevo objeto BigUnsigned.
 */
BigUnsigned BigUnsigned::operator*(const BigUnsigned& other) const {
  BigUnsigned result;
  result.digits_.clear();
  for (int i = 0; i < other.digits_.size(); i++) {
    int carry = 0;
    int digitB = other.digits_[i] - '0';
    BigUnsigned temp; 
    temp.digits_.clear();
    for (int z = 0; z < i; z++) { 
      temp.digits_.push_back('0');
    }
    for (int j = 0; j < this->digits_.size(); j++) {
      int digitA = this->digits_[j] - '0';
      int product = (digitA * digitB) + carry;
      temp.digits_.push_back((product % 10) + '0');
      carry = product / 10;
    }
    if (carry > 0) {
      temp.digits_.push_back(carry + '0'); 
    }
    result = result + temp;
  }
  while (result.digits_.size() > 1 && result.digits_.back() == '0') {
    result.digits_.pop_back();
  }
  return result;
}

/**
 * @brief Realiza la división entre dos números grandes representados como BigUnsigned.
 * 
 * Implementa la división larga para obtener el cociente de la operación.
 * 
 * @param a El dividendo.
 * @param b El divisor.
 * @return El cociente de la división.
 * @throws std::invalid_argument Si se intenta dividir por cero.
 */
BigUnsigned operator/(const BigUnsigned& a, const BigUnsigned& b) {
  BigUnsigned zero;
  if (b == zero) {
    throw std::invalid_argument("La operación da error (dividir por cero).");
  }
  if (a < b) {
    return zero;
  }
  if (a == b) {
    return BigUnsigned(1);
  }
  BigUnsigned cociente;
  BigUnsigned residuo;
  for (int i = a.digits_.size() - 1; i >= 0; --i) {
    residuo.digits_.insert(residuo.digits_.begin(), a.digits_[i]);
    while (residuo.digits_.size() > 1 && residuo.digits_.back() == '0') {
      residuo.digits_.pop_back();
    }
    int q = 0;
    BigUnsigned tempB = b;
    while (!(residuo < tempB)) {
      residuo = residuo - tempB;
      q++;
    }
    cociente.digits_.insert(cociente.digits_.begin(), q + '0');
  }
  while (cociente.digits_.size() > 1 && cociente.digits_.back() == '0') {
    cociente.digits_.pop_back();
  }
  return cociente;
}


/**
 * @brief Calcula el módulo entre dos números grandes representados como BigUnsigned.
 * 
 * @param other El divisor.
 * @return El residuo de la división.
 * @throws std::invalid_argument Si se intenta calcular módulo por cero.
 */
BigUnsigned BigUnsigned::operator%(const BigUnsigned& other) const {
  BigUnsigned zero;
  if (other == zero) {
    throw std::invalid_argument("La operación da error (módulo por cero).");
  }
  return *this - ((*this / other) * other);
}

/**
 * @brief Operador de post-incremento.
 * 
 * @return El valor original antes del incremento.
 */
BigUnsigned BigUnsigned::operator++(int) {
  BigUnsigned uno(1);
  BigUnsigned temp = *this;
  *this = *this + uno;
  return temp;
}

/**
 * @brief Operador de pre-decremento.
 * 
 * @return Referencia al objeto decrementado.
 */
BigUnsigned& BigUnsigned::operator--() {
  BigUnsigned uno(1);
  *this = *this - uno;
  return *this;
}

/**
 * @brief Operador de post-decremento.
 * 
 * @return El valor original antes del decremento.
 */
BigUnsigned BigUnsigned::operator--(int) {
  BigUnsigned uno(1);
  BigUnsigned temp = *this;
  *this = *this - uno;
  return temp;
}

/**
 * @brief Operador de salida.
 * @param os Flujo de salida.
 * @param num Objeto BigUnsigned a imprimir.
 * @return Flujo de salida modificado.
 */
std::ostream& operator<<(std::ostream& os, const BigUnsigned& num) {
  for (int i = num.digits_.size() - 1; i >= 0; --i) {
    os << num.digits_[i];
  }
  return os;
}

/**
 * @brief Operador de entrada.
 * @param is Flujo de entrada.
 * @param num Objeto BigUnsigned donde almacenar el valor leído.
 * @return Flujo de entrada modificado.
 */
std::istream& operator>>(std::istream& is, BigUnsigned& num) {
  unsigned number;
  is >> number;
  BigUnsigned result(number);
  num = result;
  return is;
}
