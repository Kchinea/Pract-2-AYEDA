/**
 * @file bigInteger.h
 * @brief Implementación de la clase BigInteger.
*/

#include "bigInteger.h"

/**
 * @brief Constructor que inicializa un BigInteger a partir de un entero.
 * @param n Número entero a almacenar.
 */
BigInteger::BigInteger(int n) {
  if (n < 0) {
    BigUnsigned result(n * (-1));
    number_ = result;
    sing_ = false;
  } else {
    BigUnsigned result(n);
    number_ = result;
    sing_ = true;
  }
}

/**
 * @brief Constructor que inicializa un BigInteger a partir de un BigUnsigned.
 * @param other Objeto de tipo BigUnsigned.
 */
BigInteger::BigInteger(const BigUnsigned& other) {
  this->sing_ = true;
  this->number_ = other;
}

/**
 * @brief Operador de asignación.
 * @param other Objeto BigInteger a copiar.
 * @return Referencia al objeto actual.
 */
BigInteger& BigInteger::operator=(const BigInteger& other) {
  this->number_ = other.number_;
  this->sing_ = other.sing_;
  return *this;
}

/**
 * @brief Operador de comparación de igualdad.
 * @param other Objeto BigInteger a comparar.
 * @return true si son iguales, false en caso contrario.
 */
bool BigInteger::operator==(const BigInteger& other) const {
  return (this->number_ == other.number_ && this->sing_ == other.sing_);
}

/**
 * @brief Operador de comparación menor que.
 * @param other Objeto BigInteger a comparar.
 * @return true si el objeto actual es menor que other, false en caso contrario.
 */
bool BigInteger::operator<(const BigInteger& other) const {
  bool result;
  result = ((!this->sing_ && other.sing_) ? true :
            (this->sing_ && !other.sing_) ? false :
            (this->sing_ && other.sing_) ? this->number_ < other.number_ :
            other.number_ < this->number_);
  return result;
}

/**
 * @brief Operador de suma.
 * @param a Primer operando.
 * @param b Segundo operando.
 * @return Resultado de la suma de los dos BigInteger.
 */
BigInteger operator+(const BigInteger& a, const BigInteger& b) {
  BigUnsigned Zero;
  BigInteger result;
  if (a.sing_ == b.sing_) {
    result.sing_ = a.sing_;
    result.number_ = (a.number_ + b.number_);
  } else {
    result.sing_ = (a.number_ < b.number_) ? b.sing_ :
                   (b.number_ < a.number_) ? a.sing_ : true;
    result.number_ = (a.number_ < b.number_) ? b.number_ - a.number_ :
                   (b.number_ < a.number_) ? a.number_ - b.number_ : Zero;
  }
  return result;
}

/**
 * @brief Operador de resta.
 * @param other Objeto BigInteger a restar.
 * @return Resultado de la resta.
 */
BigInteger BigInteger::operator-(const BigInteger& other) const {
  BigInteger result;
  BigUnsigned zero;
  if (this->sing_ == other.sing_) {
    if (this->number_ == other.number_) {
      result.number_ = zero;
      result.sing_ = true;
    } else if (this->number_ < other.number_) {
      result.sing_ = !this->sing_;
      result.number_ = other.number_ - this->number_;
    } else {
      result.sing_ = this->sing_;
      result.number_ = this->number_ - other.number_;
    }
  } else {
    result.sing_ = this->sing_;
    result.number_ = this->number_ + other.number_;
  }
  return result;
}

/**
 * @brief Operador de multiplicación.
 * @param other Objeto BigInteger a multiplicar.
 * @return Resultado de la multiplicación.
 */
BigInteger BigInteger::operator*(const BigInteger& other) const {
  BigInteger result;
  result.sing_ = (this->sing_ == other.sing_);
  result.number_ = this->number_ * other.number_;
  return result;
}

/**
 * @brief Operador de división.
 * @param a Dividendo.
 * @param b Divisor.
 * @return Resultado de la división.
 */
BigInteger operator/(const BigInteger& a, const BigInteger& b) {
  BigInteger result;
  result.sing_ = (a.sing_ == b.sing_);
  result.number_ = a.number_ / b.number_;
  return result;
}

/**
 * @brief Operador de módulo.
 * @param other Objeto BigInteger para calcular el módulo.
 * @return Resultado del módulo.
 */
BigInteger BigInteger::operator%(const BigInteger& other) const {
  BigInteger result;
  result.sing_ = (this->sing_ == other.sing_);
  result.number_ = this->number_ % other.number_;
  return result;
}

/**
 * @brief Operador de incremento prefijo.
 * @return Referencia al objeto actual tras incrementar.
 */
BigInteger& BigInteger::operator++() {
  BigInteger uno(1);
  *this = *this + uno;
  return *this;
}

/**
 * @brief Operador de incremento postfijo.
 * @return Copia del objeto antes del incremento.
 */
BigInteger BigInteger::operator++(int) {
  BigInteger uno(1);
  BigInteger temp = *this;
  *this = *this + uno;
  return temp;
}

/**
 * @brief Operador de decremento prefijo.
 * @return Referencia al objeto actual tras decrementar.
 */
BigInteger& BigInteger::operator--() {
  BigInteger uno(1);
  *this = *this - uno;
  return *this;
}

/**
 * @brief Operador de decremento postfijo.
 * @return Copia del objeto antes del decremento.
 */
BigInteger BigInteger::operator--(int) {
  BigInteger uno(1);
  BigInteger temp = *this;
  *this = *this - uno;
  return temp;
}

/**
 * @brief Operador de salida.
 * @param os Flujo de salida.
 * @param num Objeto BigInteger a imprimir.
 * @return Flujo de salida modificado.
 */
std::ostream& operator<<(std::ostream& os, const BigInteger& num) {
  os << ((num.sing_) ? "" : "-");
  return os << num.number_;
}

/**
 * @brief Operador de entrada.
 * @param is Flujo de entrada.
 * @param num Objeto BigInteger a leer.
 * @return Flujo de entrada modificado.
 */
std::istream& operator>>(std::istream& is, BigInteger& num) {
  int entry;
  is >> entry;
  num = BigInteger(entry);
  return is;
}
