#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iostream>

namespace kyli {
  int pow(int base, int exponente) {
    if (exponente < 0) {
        std::cerr << "Error: exponente negativo no soportado para enteros." << std::endl;
        return -1; // Manejo de error (podrías lanzar una excepción en lugar de esto).
    }

    int resultado = 1;
    while (exponente > 0) {
        if (exponente % 2 == 1) {  // Si el exponente es impar, multiplicamos por la base
            resultado *= base;
        }
        base *= base;  // Elevamos la base al cuadrado
        exponente /= 2; // Reducimos el exponente a la mitad
    }
    return resultado;
}
}
template <int Base>
class BigUnsigned {
public:
    // Constructores
    BigUnsigned();
    BigUnsigned(unsigned n);
    BigUnsigned(const char* digits);
    BigUnsigned(const BigUnsigned& other);

    // Operadores
    BigUnsigned& operator=(const BigUnsigned& other);
    bool operator==(const BigUnsigned& other) const;
    bool operator<(const BigUnsigned& other) const;
    bool operator>(const BigUnsigned& other) const;
    BigUnsigned operator+(const BigUnsigned& other) const;
    BigUnsigned operator-(const BigUnsigned& other) const;
    BigUnsigned operator*(const BigUnsigned& other) const;
    BigUnsigned operator/(const BigUnsigned& other) const;
    BigUnsigned operator%(const BigUnsigned& other) const;
    BigUnsigned operator++(int);
    BigUnsigned& operator--();
    BigUnsigned operator--(int);

    // Métodos de conversión
    BigUnsigned<10> toDecimal() const;
    template <int NewBase>
    BigUnsigned<NewBase> toBase() const;

    // Operadores de flujo
    template <int B>
    friend std::ostream& operator<<(std::ostream& os, const BigUnsigned<B>& num);
    template <int B>
    friend std::istream& operator>>(std::istream& is, BigUnsigned<B>& num);

private:
    std::vector<char> digits_;
};

// Constructor por defecto
template <int Base>
BigUnsigned<Base>::BigUnsigned() {}

// Constructor que inicializa un BigUnsigned<Base> a partir de un número entero sin signo
template <int Base>
BigUnsigned<Base>::BigUnsigned(unsigned n) {
    if (n == 0) digits_.push_back(n + '0');
    while (n > 0) {
        char digit = n % Base;
        digits_.push_back(digit + '0');
        n /= Base;
    }
    if (digits_.empty()) {
        digits_.push_back(0);
    }
}

/**
 * @brief Constructor que inicializa un BigUnsigned<Base> a partir de una cadena de caracteres.
 * @param digits Puntero a una cadena de caracteres terminada en '\0'.
 */
template <int Base>
BigUnsigned<Base>::BigUnsigned(const char* digits) {
  if (digits == nullptr) {
    throw std::invalid_argument("Null pointer passed to constructor");
  }
  while (*digits != '\0') {
    if (*digits < '0' || *digits >= '0' + Base) {
      throw std::invalid_argument("Invalid digit in input string");
    }
    this->digits_.push_back(*digits - '0');
    digits++;
  }
  std::reverse(this->digits_.begin(), this->digits_.end());
}

/**
 * @brief Constructor de copia.
 * @param other Otro objeto BigUnsigned<Base> a copiar.
 */
template <int Base>
BigUnsigned<Base>::BigUnsigned(const BigUnsigned<Base>& other) {
  this->digits_ = other.digits_;
}

template <int Base>
BigUnsigned<10> BigUnsigned<Base>::toDecimal() const {
  std::string vectorResult;
  for(int i = 0; i < this->digits_.size(); i++) {
    int digit =(this->digits_[i] - '0') * kyli::pow(Base,i);
    vectorResult.push_back(static_cast<char>(digit));
  }
  BigUnsigned<10> result(vectorResult.c_str());
  return result;
}


// template <int Base>
// BigUnsigned<10> BigUnsigned<Base>::toDecimal() const {
//     BigUnsigned<10> result;  // Inicialmente en 0
//     BigUnsigned<10> basePower = 1;  // Base^0 = 1

//     for (int i = this->digits_.size() - 1; i >= 0; --i) {
//         int digit;

//         // Convertimos el carácter a número
//         if (this->digits_[i] >= '0' && this->digits_[i] <= '9') {
//             digit = this->digits_[i] - '0';
//         } else if (Base > 10 && this->digits_[i] >= 'A' && this->digits_[i] <= 'F') {
//             digit = this->digits_[i] - 'A' + 10;
//         } else {
//             throw std::invalid_argument("Dígito inválido en la base dada");
//         }

//         // Multiplicación en lugar de suma
//         result = (result * Base) + digit;

//         // `basePower` ya no es necesario porque multiplicamos `result` directamente
//     }

//     return result;
// }


/**
 * @brief Operador de asignación.
 * @param other Otro objeto BigUnsigned<Base>.
 * @return Referencia a este objeto BigUnsigned<Base> modificado.
 */
template <int Base>
BigUnsigned<Base>& BigUnsigned<Base>::operator=(const BigUnsigned<Base>& other) {
  if (this != &other) {
    digits_ = other.digits_;
  }
  return *this;
}

/**
 * @brief Operador de igualdad.
 * @param other Otro objeto BigUnsigned<Base>.
 * @return true si ambos objetos son iguales, false en caso contrario.
 */
template <int Base>
bool BigUnsigned<Base>::operator==(const BigUnsigned<Base>& other) const {
  return digits_ == other.digits_;
}

/**
 * @brief Operador de comparación menor que.
 * @param other Otro objeto BigUnsigned<Base>.
 * @return true si este objeto es menor que el otro, false en caso contrario.
 */
template <int Base>
bool BigUnsigned<Base>::operator<(const BigUnsigned<Base>& other) const {
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
template <int Base>
BigUnsigned<Base> operator+(const BigUnsigned<Base>& a, const BigUnsigned<Base>& b) {
  bool carry = false;
  BigUnsigned<Base> result;
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
 * @param other Otro objeto BigUnsigned<Base>.
 * @return Resultado de la resta.
 * @throw std::invalid_argument si el resultado sería negativo.
 */
template <int Base>
BigUnsigned<Base> BigUnsigned<Base>::operator-(const BigUnsigned<Base>& other) const {
  if ((*this < other)) {
    throw std::invalid_argument("La operación da un número negativo (con signo).");
  }
  int carry = 0;
  BigUnsigned<Base> result;
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
 * @brief Multiplica dos números grandes representados como BigUnsigned<Base>.
 * 
 * Implementa la multiplicación por el método tradicional de suma de productos parciales.
 * 
 * @param other El número a multiplicar con el objeto actual.
 * @return El resultado de la multiplicación como un nuevo objeto BigUnsigned<Base>.
 */
template <int Base>
BigUnsigned<Base> BigUnsigned<Base>::operator*(const BigUnsigned<Base>& other) const {
  BigUnsigned<Base> result;
  result.digits_.clear();
  for (int i = 0; i < other.digits_.size(); i++) {
    int carry = 0;
    int digitB = other.digits_[i] - '0';
    BigUnsigned<Base> temp; 
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
 * @brief Realiza la división entre dos números grandes representados como BigUnsigned<Base>.
 * 
 * Implementa la división larga para obtener el cociente de la operación.
 * 
 * @param a El dividendo.
 * @param b El divisor.
 * @return El cociente de la división.
 * @throws std::invalid_argument Si se intenta dividir por cero.
 */
template <int Base>
BigUnsigned<Base> operator/(const BigUnsigned<Base>& a, const BigUnsigned<Base>& b) {
  BigUnsigned<Base> zero;
  if (b == zero) {
    throw std::invalid_argument("La operación da error (dividir por cero).");
  }
  if (a < b) {
    return zero;
  }
  if (a == b) {
    return BigUnsigned<Base>(1);
  }
  BigUnsigned<Base> cociente;
  BigUnsigned<Base> residuo;
  for (int i = a.digits_.size() - 1; i >= 0; --i) {
    residuo.digits_.insert(residuo.digits_.begin(), a.digits_[i]);
    while (residuo.digits_.size() > 1 && residuo.digits_.back() == '0') {
      residuo.digits_.pop_back();
    }
    int q = 0;
    BigUnsigned<Base> tempB = b;
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
 * @brief Calcula el módulo entre dos números grandes representados como BigUnsigned<Base>.
 * 
 * @param other El divisor.
 * @return El residuo de la división.
 * @throws std::invalid_argument Si se intenta calcular módulo por cero.
 */
template <int Base>
BigUnsigned<Base> BigUnsigned<Base>::operator%(const BigUnsigned<Base>& other) const {
  BigUnsigned<Base> zero;
  if (other == zero) {
    throw std::invalid_argument("La operación da error (módulo por cero).");
  }
  return *this - ((*this / other) * other);
}

/**
 * @brief Operador de salida.
 * @param os Flujo de salida.
 * @param num Objeto BigUnsigned<Base> a imprimir.
 * @return Flujo de salida modificado.
 */
template <int Base>
std::ostream& operator<<(std::ostream& os, const BigUnsigned<Base>& num) {
  for (int i = num.digits_.size() - 1; i >= 0; --i) {
    os << num.digits_[i];
  }
  return os;
}

/**
 * @brief Operador de entrada.
 * @param is Flujo de entrada.
 * @param num Objeto BigUnsigned<Base> donde almacenar el valor leído.
 * @return Flujo de entrada modificado.
 */
template <int Base>
std::istream& operator>>(std::istream& is, BigUnsigned<Base>& num) {
  unsigned number;
  is >> number;
  BigUnsigned<Base> result(number);
  num = result;
  return is;
}


int main() {
    // Crear números en diferentes bases
    BigUnsigned<8> num1(12);
    BigUnsigned<10> num2(6789);
    BigUnsigned<10> num3 = num1.toDecimal();
    std::cout << num3 << std::endl;

    // // Suma en base 10
    // BigUnsigned<10> sum = num1 + num2;
    // std::cout << "Suma en base 10: " << sum << std::endl;

    // // Resta en base 10
    // BigUnsigned<10> diff = num1 - num2;
    // std::cout << "Resta en base 10: " << diff << std::endl;

    // // Multiplicación en base 10
    // BigUnsigned<10> prod = num1 * num2;
    // std::cout << "Multiplicación en base 10: " << prod << std::endl;

    // // División en base 10
    // BigUnsigned<10> quot = num1 / num2;
    // std::cout << "División en base 10: " << quot << std::endl;

    // // Módulo en base 10
    // BigUnsigned<10> mod = num1 % num2;
    // std::cout << "Módulo en base 10: " << mod << std::endl;

    // // Crear números en base 16
    // BigUnsigned<16> num3("ABCD");
    // BigUnsigned<16> num4(1234);

    // // Suma en base 16
    // BigUnsigned<16> sum16 = num3 + num4;
    // std::cout << "Suma en base 16: " << sum16 << std::endl;

    // // Resta en base 16
    // BigUnsigned<16> diff16 = num3 - num4;
    // std::cout << "Resta en base 16: " << diff16 << std::endl;

    // // Multiplicación en base 16
    // BigUnsigned<16> prod16 = num3 * num4;
    // std::cout << "Multiplicación en base 16: " << prod16 << std::endl;

    // // División en base 16
    // BigUnsigned<16> quot16 = num3 / num4;
    // std::cout << "División en base 16: " << quot16 << std::endl;

    // // Módulo en base 16
    // BigUnsigned<16> mod16 = num3 % num4;
    // std::cout << "Módulo en base 16: " << mod16 << std::endl;

    // // Crear números en base 8
    // BigUnsigned<8> num5("12345");
    // BigUnsigned<8> num6("567");

    // // Suma en base 8
    // BigUnsigned<8> sum8 = num5 + num6;
    // std::cout << "Suma en base 8: " << sum8 << std::endl;

    // // Resta en base 8
    // BigUnsigned<8> diff8 = num5 - num6;
    // std::cout << "Resta en base 8: " << diff8 << std::endl;

    // // Multiplicación en base 8
    // BigUnsigned<8> prod8 = num5 * num6;
    // std::cout << "Multiplicación en base 8: " << prod8 << std::endl;

    // // División en base 8
    // BigUnsigned<8> quot8 = num5 / num6;
    // std::cout << "División en base 8: " << quot8 << std::endl;

    // // Módulo en base 8
    // BigUnsigned<8> mod8 = num5 % num6;
    // std::cout << "Módulo en base 8: " << mod8 << std::endl;

    return 0;
}