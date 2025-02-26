#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iostream>

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

// Constructor que inicializa un BigUnsigned a partir de un número entero sin signo
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

// Constructor que inicializa un BigUnsigned a partir de una cadena de caracteres
template <int Base>
BigUnsigned<Base>::BigUnsigned(const char* digits) {
    unsigned char digit;
    while ((digit = *digits++) != '\0') {
        this->digits_.push_back(digit);
    }
    std::reverse(this->digits_.begin(), this->digits_.end());
}

// Constructor de copia
template <int Base>
BigUnsigned<Base>::BigUnsigned(const BigUnsigned& other) {
    this->digits_ = other.digits_;
}

// Operador de asignación
template <int Base>
BigUnsigned<Base>& BigUnsigned<Base>::operator=(const BigUnsigned& other) {
    if (this != &other) {
        digits_ = other.digits_;
    }
    return *this;
}

// Operador de igualdad
template <int Base>
bool BigUnsigned<Base>::operator==(const BigUnsigned& other) const {
    return digits_ == other.digits_;
}

// Operador de comparación menor que
template <int Base>
bool BigUnsigned<Base>::operator<(const BigUnsigned& other) const {
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

// Operador de suma
template <int Base>
BigUnsigned<Base> BigUnsigned<Base>::operator+(const BigUnsigned& other) const {
    bool carry = false;
    BigUnsigned<Base> result;
    result.digits_.clear();
    int i = 0;
    while (i < this->digits_.size() || i < other.digits_.size() || carry) {
        int digitA = (i < this->digits_.size()) ? (this->digits_[i] - (this->digits_[i] >= 'A' ? 'A' - 10 : '0')) : 0;
        int digitB = (i < other.digits_.size()) ? (other.digits_[i] - (other.digits_[i] >= 'A' ? 'A' - 10 : '0')) : 0;
        int sum = digitA + digitB + (carry ? 1 : 0);
        carry = sum >= Base;
        int resultDigit = sum % Base;
        result.digits_.push_back(resultDigit < 10 ? resultDigit + '0' : resultDigit - 10 + 'A');
        i++;
    }
    return result;
}


// Operador de resta
template <int Base>
BigUnsigned<Base> BigUnsigned<Base>::operator-(const BigUnsigned& other) const {
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
      substract += Base;
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

// Operador de multiplicación
template <int Base>
BigUnsigned<Base> BigUnsigned<Base>::operator*(const BigUnsigned& other) const {
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
      temp.digits_.push_back((product % Base) + '0');
      carry = product / Base;
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

// Operador de división
template <int Base>
BigUnsigned<Base> BigUnsigned<Base>::operator/(const BigUnsigned& other) const {
  BigUnsigned<Base> zero;
  if (other == zero) {
    throw std::invalid_argument("La operación da error (dividir por cero).");
  }
  if (*this < other) {
    return zero;
  }
  if (*this == other) {
    return BigUnsigned<Base>(1);
  }
  BigUnsigned<Base> cociente;
  BigUnsigned<Base> residuo;
  for (int i = this->digits_.size() - 1; i >= 0; --i) {
    residuo.digits_.insert(residuo.digits_.begin(), this->digits_[i]);
    while (residuo.digits_.size() > 1 && residuo.digits_.back() == '0') {
      residuo.digits_.pop_back();
    }
    int q = 0;
    BigUnsigned<Base> tempB = other;
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

// Operador de módulo
template <int Base>
BigUnsigned<Base> BigUnsigned<Base>::operator%(const BigUnsigned& other) const {
  BigUnsigned<Base> zero;
  if (other == zero) {
    throw std::invalid_argument("La operación da error (módulo por cero).");
  }
  return *this - ((*this / other) * other);
}

// Método para convertir a decimal
template <int Base>
BigUnsigned<10> BigUnsigned<Base>::toDecimal() const {
    BigUnsigned<10> decimalValue;
    BigUnsigned<10> basePower = 1; // Para la potencia de la base

    // Leer los dígitos en orden normal
    for (size_t i = digits_.size(); i > 0; --i) {
        BigUnsigned<10> digitValue;
        // Obtener el valor del dígito según su representación
        if (digits_[i - 1] >= '0' && digits_[i - 1] <= '9') {
            digitValue.digits_.push_back(digits_[i - 1] - '0'); // Convertir carácter a entero
        } else {
            digitValue.digits_.push_back(digits_[i - 1] - 'A' + 10); // Convertir carácter a entero (para A-F)
        }
        // Sumar el valor del dígito multiplicado por su potencia de base
        decimalValue = decimalValue + digitValue * basePower;
        basePower = basePower * Base;
    }
    return decimalValue;
}

// Método para convertir a una base específica
// Método para convertir a una base específica
template <int Base>
template <int NewBase>
BigUnsigned<NewBase> BigUnsigned<Base>::toBase() const {
    std::string baseValueStr;
    BigUnsigned<10> tempValue = this->toDecimal();
    BigUnsigned<10> base = NewBase;
    BigUnsigned<10> zero;

    while (zero < tempValue) {
        BigUnsigned<10> remainder = tempValue % base;
        tempValue = tempValue / base;
        
        // Convertir el dígito a carácter y agregarlo a la representación de la nueva base
        if (remainder.digits_[0] < 10) {
            baseValueStr.insert(baseValueStr.begin(), '0' + remainder.digits_[0]);
        } else {
            baseValueStr.insert(baseValueStr.begin(), 'A' + (remainder.digits_[0] - 10));
        }
    }

    return BigUnsigned<NewBase>(reinterpret_cast<const char*>(baseValueStr.c_str()));
}

// Operador de salida
template <int Base>
std::ostream& operator<<(std::ostream& os, const BigUnsigned<Base>& num) {
    for (int i = num.digits_.size() - 1; i >= 0; --i) {
        os << num.digits_[i];
    }
    return os;
}

// Operador de entrada
template <int Base>
std::istream& operator>>(std::istream& is, BigUnsigned<Base>& num) {
    unsigned number;
    is >> number;
    BigUnsigned<Base> result(number);
    num = result;
    return is;
}

int main() {
    BigUnsigned<10> num1(12345);
    BigUnsigned<10> num2(6789);

    // Suma en base 10
    BigUnsigned<10> sum = num1 + num2;
    std::cout << "Suma en base 10: " << sum << std::endl;

    // Resta en base 10
    BigUnsigned<10> diff = num1 - num2;
    std::cout << "Resta en base 10: " << diff << std::endl;

    // Multiplicación en base 10
    BigUnsigned<10> prod = num1 * num2;
    std::cout << "Multiplicación en base 10: " << prod << std::endl;

    // División en base 10
    BigUnsigned<10> quot = num1 / num2;
    std::cout << "División en base 10: " << quot << std::endl;

    // Módulo en base 10
    BigUnsigned<10> mod = num1 % num2;
    std::cout << "Módulo en base 10: " << mod << std::endl;

    // Crear números en base 16
    BigUnsigned<16> num3("ABCD");
    BigUnsigned<16> num4(1234);

    // Suma en base 16
    BigUnsigned<16> sum16 = num3 + num4;
    std::cout << "Suma en base 16: " << sum16 << std::endl;

    // Resta en base 16
    BigUnsigned<16> diff16 = num3 - num4;
    std::cout << "Resta en base 16: " << diff16 << std::endl;

    // Multiplicación en base 16
    BigUnsigned<16> prod16 = num3 * num4;
    std::cout << "Multiplicación en base 16: " << prod16 << std::endl;

    // División en base 16
    BigUnsigned<16> quot16 = num3 / num4;
    std::cout << "División en base 16: " << quot16 << std::endl;

    // Módulo en base 16
    BigUnsigned<16> mod16 = num3 % num4;
    std::cout << "Módulo en base 16: " << mod16 << std::endl;

    // Crear números en base 8
    BigUnsigned<8> num5("12345");
    BigUnsigned<8> num6("567");

    // Suma en base 8
    BigUnsigned<8> sum8 = num5 + num6;
    std::cout << "Suma en base 8: " << sum8 << std::endl;

    // Resta en base 8
    BigUnsigned<8> diff8 = num5 - num6;
    std::cout << "Resta en base 8: " << diff8 << std::endl;

    // Multiplicación en base 8
    BigUnsigned<8> prod8 = num5 * num6;
    std::cout << "Multiplicación en base 8: " << prod8 << std::endl;

    // División en base 8
    BigUnsigned<8> quot8 = num5 / num6;
    std::cout << "División en base 8: " << quot8 << std::endl;

    // Módulo en base 8
    BigUnsigned<8> mod8 = num5 % num6;
    std::cout << "Módulo en base 8: " << mod8 << std::endl;

    return 0;
}