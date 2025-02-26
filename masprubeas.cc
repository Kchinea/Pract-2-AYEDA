
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>


template <int Base>
class BigUnsignedBase {
 public:
    BigUnsignedBase(unsigned n = 0);
    BigUnsignedBase(const unsigned char*);
    BigUnsignedBase(const BigUnsignedBase&);
    BigUnsignedBase& operator=(const BigUnsignedBase&);
    // entrada salida
    template <int B>
    friend std::ostream& operator<<(std::ostream&, const BigUnsignedBase<B>&);
    template <int B>
    friend std::istream& operator>>(std::istream&, BigUnsignedBase<B>&);
    // comparacion
    bool operator==(const BigUnsignedBase&) const;
    bool operator<(const BigUnsignedBase&) const;
    // aumento y decremento
    BigUnsignedBase& operator++(); // Pre-incremento  ++i
    BigUnsignedBase operator++(int); // Post-incremento i++
    BigUnsignedBase& operator--(); // Pre-decremento
    BigUnsignedBase operator--(int); // Post-decremento i*i
    // operaciones
    template <int B>
    friend BigUnsignedBase<B> operator+(const BigUnsignedBase<B>&, const BigUnsignedBase<B>&);
    BigUnsignedBase operator-(const BigUnsignedBase&) const;
    BigUnsignedBase operator*(const BigUnsignedBase&) const;
    template <int B>
    friend BigUnsignedBase<B> operator/(const BigUnsignedBase<B>&, const BigUnsignedBase<B>&);
    BigUnsignedBase operator%(const BigUnsignedBase&) const;
 private:
    std::vector<unsigned char> digits_;
};

template <int Base>
BigUnsignedBase<Base>::BigUnsignedBase(unsigned n) {
    if (n == 0) digits_.push_back('0');
    while (n > 0) {
        int digit = n % 10;
        digits_.push_back( digit + '0');
        n /= 10;
    }
    if (digits_.empty()) {
        digits_.push_back('0');
    }
}

template <int Base>
BigUnsignedBase<Base>::BigUnsignedBase(const unsigned char* digits) {
    unsigned char digit;
    while ((digit = *digits++) != '\0') {
        this->digits_.push_back(digit);
    }
    std::reverse(this->digits_.begin(), this->digits_.end());
}

template <int Base>
BigUnsignedBase<Base>::BigUnsignedBase(const BigUnsignedBase& other) {
    this->digits_ = other.digits_;
}

template <int Base>
BigUnsignedBase<Base>& BigUnsignedBase<Base>::operator=(const BigUnsignedBase& other) {
    if (this != &other) {
        digits_ = other.digits_;
    }
    return *this;
}

template <int Base>
bool BigUnsignedBase<Base>::operator==(const BigUnsignedBase& other) const {
    return digits_ == other.digits_;
}

template <int Base>
bool BigUnsignedBase<Base>::operator<(const BigUnsignedBase& other) const {
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

template <int Base>
BigUnsignedBase<Base> operator+(const BigUnsignedBase<Base>& a, const BigUnsignedBase<Base>& b) {
    bool carry = false;
    BigUnsignedBase<Base> result;
    result.digits_.clear();
    int i = 0;
    while (i < a.digits_.size() || i < b.digits_.size() || carry) {
        int digitA = (i < a.digits_.size()) ? (a.digits_[i] - '0') : 0;
        int digitB = (i < b.digits_.size()) ? (b.digits_[i] - '0') : 0;
        int sum = digitA + digitB + (carry ? 1 : 0);
        carry = sum >= Base;
        result.digits_.push_back((sum % Base) + '0');
        i++;
    }
    return result;
}

template <int Base>
BigUnsignedBase<Base> BigUnsignedBase<Base>::operator-(const BigUnsignedBase& other) const {
    if ((*this < other)) {
        throw std::invalid_argument("La operación da un número negativo (con signo).");
    }
    int carry = 0;
    BigUnsignedBase<Base> result;
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

template <int Base>
BigUnsignedBase<Base> BigUnsignedBase<Base>::operator*(const BigUnsignedBase& other) const {
    BigUnsignedBase<Base> result;
    result.digits_.clear();
    for (int i = 0; i < other.digits_.size(); i++) {
        int carry = 0;
        int digitB = other.digits_[i] - '0';
        BigUnsignedBase<Base> temp;
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

template <int Base>
BigUnsignedBase<Base> operator/(const BigUnsignedBase<Base>& a, const BigUnsignedBase<Base>& b) {
    BigUnsignedBase<Base> zero;
    if (b == zero) {
        throw std::invalid_argument("La operación da error (dividir por cero).");
    }
    if (a < b) {
        return zero;
    }
    if (a == b) {
        return BigUnsignedBase<Base>(1);
    }
    BigUnsignedBase<Base> cociente;
    BigUnsignedBase<Base> residuo;
    for (int i = a.digits_.size() - 1; i >= 0; --i) {
        residuo.digits_.insert(residuo.digits_.begin(), a.digits_[i]);
        while (residuo.digits_.size() > 1 && residuo.digits_.back() == '0') {
            residuo.digits_.pop_back();
        }
        int q = 0;
        BigUnsignedBase<Base> tempB = b;
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

template <int Base>
BigUnsignedBase<Base> BigUnsignedBase<Base>::operator%(const BigUnsignedBase& other) const {
    BigUnsignedBase<Base> zero;
    if (other == zero) {
        throw std::invalid_argument("La operación da error (módulo por cero).");
    }
    return *this - ((*this / other) * other);
}

template <int Base>
std::ostream& operator<<(std::ostream& os, const BigUnsignedBase<Base>& num) {
    for (int i = num.digits_.size() - 1; i >= 0; --i) {
        os << num.digits_[i];
    }
    return os;
}

template <int Base>
std::istream& operator>>(std::istream& is, BigUnsignedBase<Base>& num) {
    unsigned number;
    is >> number;
    BigUnsignedBase<Base> result(number);
    num = result;
    return is;
}

template <int Base>
class BigInteger {
 public:
    BigInteger(int n = 0);
    BigInteger(const unsigned char*);
    BigInteger(const BigInteger&);
    BigInteger(const BigUnsignedBase<Base>&);
    BigInteger(const BigUnsignedBase<Base>& magnitude, bool sign);
    BigInteger& operator=(const BigInteger&);
    // entrada salida
    template <int B>
    friend std::ostream& operator<<(std::ostream&, const BigInteger<B>&);
    template <int B>
    friend std::istream& operator>>(std::istream&, BigInteger<B>&);
    // comparacion
    bool operator==(const BigInteger&) const;
    bool operator<(const BigInteger&) const;
    // operaciones
    BigInteger operator+(const BigInteger&) const;
    BigInteger operator-(const BigInteger&) const;
    BigInteger operator*(const BigInteger&) const;
    BigInteger operator/(const BigInteger&) const;
    BigInteger operator%(const BigInteger&) const;
 private:
    BigUnsignedBase<Base> magnitude_;
    bool sign_; 
};

template <int Base>
BigInteger<Base>::BigInteger(int n) : magnitude_(std::abs(n)), sign_(n < 0) {}

template <int Base>
BigInteger<Base>::BigInteger(const unsigned char* digits) : magnitude_(digits), sign_(false) {}

template <int Base>
BigInteger<Base>::BigInteger(const BigInteger& other) : magnitude_(other.magnitude_), sign_(other.sign_) {}

template<int Base>
BigInteger<Base>::BigInteger(const BigUnsignedBase<Base>& magnitude, bool sign)
    : magnitude_(magnitude), sign_(sign) {}

template<int Base>
BigInteger<Base>::BigInteger(const BigUnsignedBase<Base>& magnitude)
    : magnitude_(magnitude), sign_(false) {}

template <int Base>
BigInteger<Base>& BigInteger<Base>::operator=(const BigInteger& other) {
    if (this != &other) {
        magnitude_ = other.magnitude_;
        sign_ = other.sign_;
    }
    return *this;
}

template <int Base>
bool BigInteger<Base>::operator==(const BigInteger& other) const {
    return sign_ == other.sign_ && magnitude_ == other.magnitude_;
}

template <int Base>
bool BigInteger<Base>::operator<(const BigInteger& other) const {
    if (sign_ != other.sign_) {
        return sign_;
    }
    if (sign_) {
        return other.magnitude_ < magnitude_;
    }
    return magnitude_ < other.magnitude_;
}

template <int Base>
BigInteger<Base> BigInteger<Base>::operator+(const BigInteger& other) const {
    if (sign_ == other.sign_) {
        BigUnsignedBase<Base> result = magnitude_ + other.magnitude_;
        return BigInteger<Base>(result, sign_);
    }
    if (magnitude_ < other.magnitude_) {
        return BigInteger<Base>(other.magnitude_ - magnitude_, other.sign_);
    }
    return BigInteger<Base>(magnitude_ - other.magnitude_, sign_);
}

template <int Base>
BigInteger<Base> BigInteger<Base>::operator-(const BigInteger& other) const {
    if (sign_ != other.sign_) {
        return BigInteger<Base>(magnitude_ + other.magnitude_, sign_);
    }
    if (magnitude_ < other.magnitude_) {
        return BigInteger<Base>(other.magnitude_ - magnitude_, !sign_);
    }
    return BigInteger<Base>(magnitude_ - other.magnitude_, sign_);
}

template <int Base>
BigInteger<Base> BigInteger<Base>::operator*(const BigInteger& other) const {
    return BigInteger<Base>(magnitude_ * other.magnitude_, sign_ != other.sign_);
}

template <int Base>
BigInteger<Base> BigInteger<Base>::operator/(const BigInteger& other) const {
    return BigInteger<Base>(magnitude_ / other.magnitude_, sign_ != other.sign_);
}

template <int Base>
BigInteger<Base> BigInteger<Base>::operator%(const BigInteger& other) const {
    return BigInteger<Base>(magnitude_ % other.magnitude_, sign_);
}

template <int Base>
std::ostream& operator<<(std::ostream& os, const BigInteger<Base>& num) {
    if (num.sign_) {
        os << '-';
    }
    os << num.magnitude_;
    return os;
}

template <int Base>
std::istream& operator>>(std::istream& is, BigInteger<Base>& num) {
    std::string input;
    is >> input;
    if (input[0] == '-') {
        num.sign_ = true;
        input = input.substr(1);
    } else {
        num.sign_ = false;
    }
    num.magnitude_ = BigUnsignedBase<Base>(reinterpret_cast<const unsigned char*>(input.c_str()));
    return is;
}



template <>
class BigInteger<2> {
public:
    BigInteger(int n = 0);
    BigInteger(const unsigned char*);
    BigInteger(const BigInteger&);
    BigInteger(const BigUnsignedBase<2>&);
    BigInteger(const BigUnsignedBase<2>& magnitude, bool sign);
    BigInteger& operator=(const BigInteger&);

    // Operaciones
    BigInteger operator+(const BigInteger&) const;
    BigInteger operator-(const BigInteger&) const;
    BigInteger operator*(const BigInteger&) const;
    BigInteger operator/(const BigInteger&) const;
    BigInteger operator%(const BigInteger&) const;

private:
    BigUnsignedBase<2> toTwosComplement() const;
    BigUnsignedBase<2> magnitude_;
    bool sign_;
};

// Constructor desde entero
BigInteger<2>::BigInteger(int n) {
    sign_ = (n < 0);
    magnitude_ = BigUnsignedBase<2>(sign_ ? -n : n);
}

// Constructor desde cadena de caracteres
BigInteger<2>::BigInteger(const unsigned char* digits) {
    // Convertir `digits` a `magnitude_`
}

// Constructor de copia
BigInteger<2>::BigInteger(const BigInteger& other)
    : magnitude_(other.magnitude_), sign_(other.sign_) {}

// Constructor desde BigUnsignedBase<2>
BigInteger<2>::BigInteger(const BigUnsignedBase<2>& magnitude, bool sign)
    : magnitude_(magnitude), sign_(sign) {}

BigInteger<2>::BigInteger(const BigUnsignedBase<2>& magnitude)
    : magnitude_(magnitude), sign_(false) {}

// Operador de asignación
BigInteger<2>& BigInteger<2>::operator=(const BigInteger& other) {
    if (this != &other) {
        magnitude_ = other.magnitude_;
        sign_ = other.sign_;
    }
    return *this;
}

// Conversión a complemento a dos
BigUnsignedBase<2> BigInteger<2>::toTwosComplement() const {
    if (!sign_) {
        return magnitude_;
    }
    std::string inverted_str;
    // for (size_t i = 0; i < magnitude_.digits_.size(); ++i) {
    //     unsigned char inverted_digit = (magnitude_.digits_[i] == '0') ? '1' : '0';
    //     inverted_str.push_back(inverted_digit);
    // }

    BigUnsignedBase<2> one(1);
    BigUnsignedBase<2> inverted(reinterpret_cast<const unsigned char*>(inverted_str.c_str()));
    return one;
 
}

// Operadores aritméticos
BigInteger<2> BigInteger<2>::operator+(const BigInteger& other) const {
    bool resultSign = sign_ && other.sign_;
    BigUnsignedBase<2> result = magnitude_ + other.magnitude_;
    return BigInteger<2>(result, resultSign);
}

BigInteger<2> BigInteger<2>::operator-(const BigInteger& other) const {
    bool resultSign = sign_ && other.sign_;
    BigUnsignedBase<2> result = magnitude_ - other.magnitude_;
    return BigInteger<2>(result, resultSign);
}

BigInteger<2> BigInteger<2>::operator*(const BigInteger& other) const {
    bool resultSign = sign_ && other.sign_;
    BigUnsignedBase<2> result = magnitude_ * other.magnitude_;
    return BigInteger<2>(result, resultSign);
}

BigInteger<2> BigInteger<2>::operator/(const BigInteger& other) const {
    bool resultSign = sign_ && other.sign_;
    BigUnsignedBase<2> result = magnitude_ / other.magnitude_;
    return BigInteger<2>(result, resultSign);
}

BigInteger<2> BigInteger<2>::operator%(const BigInteger& other) const {
    BigUnsignedBase<2> result = magnitude_ % other.magnitude_;
    return BigInteger<2>(result, sign_);
}


template <int Base>
class Rational {
public:
    Rational(int numerator = 0, int denominator = 1);
    Rational(const BigInteger<Base>& numerator, const BigUnsignedBase<Base>& denominator);
    Rational(const Rational& other);
    Rational& operator=(const Rational& other);

    // Operaciones aritméticas
    Rational operator+(const Rational& other) const;
    Rational operator-(const Rational& other) const;
    Rational operator*(const Rational& other) const;
    Rational operator/(const Rational& other) const;

    // Entrada y salida
    template <int B>
    friend std::ostream& operator<<(std::ostream& os, const Rational<B>& rational);
    template <int B>
    friend std::istream& operator>>(std::istream& is, Rational<B>& rational);

private:
    BigInteger<Base> numerator_;
    BigUnsignedBase<Base> denominator_;
    void reduce();
};

template <int Base>
Rational<Base>::Rational(int numerator, int denominator)
    : numerator_(numerator), denominator_(denominator) {
    reduce();
}

template <int Base>
Rational<Base>::Rational(const BigInteger<Base>& numerator, const BigUnsignedBase<Base>& denominator)
    : numerator_(numerator), denominator_(denominator) {
    reduce();
}

template <int Base>
Rational<Base>::Rational(const Rational& other)
    : numerator_(other.numerator_), denominator_(other.denominator_) {}

template <int Base>
Rational<Base>& Rational<Base>::operator=(const Rational& other) {
    if (this != &other) {
        numerator_ = other.numerator_;
        denominator_ = other.denominator_;
    }
    return *this;
}

template <int Base>
Rational<Base> Rational<Base>::operator+(const Rational& other) const {
    BigInteger<Base> newNumerator = numerator_ * BigInteger<Base>(other.denominator_) + other.numerator_ * BigInteger<Base>(denominator_);
    BigUnsignedBase<Base> newDenominator = denominator_ * other.denominator_;
    return Rational(newNumerator, newDenominator);
}

template <int Base>
Rational<Base> Rational<Base>::operator-(const Rational& other) const {
    BigInteger<Base> newNumerator = numerator_ * BigInteger<Base>(other.denominator_) - other.numerator_ * BigInteger<Base>(denominator_);
    BigUnsignedBase<Base> newDenominator = denominator_ * other.denominator_;
    return Rational(newNumerator, newDenominator);
}

template <int Base>
Rational<Base> Rational<Base>::operator*(const Rational& other) const {
    BigInteger<Base> newNumerator = numerator_ * other.numerator_;
    BigUnsignedBase<Base> newDenominator = denominator_ * other.denominator_;
    return Rational(newNumerator, newDenominator);
}

template <int Base>
Rational<Base> Rational<Base>::operator/(const Rational& other) const {
    BigInteger<Base> newNumerator = numerator_ * BigInteger<Base>(other.denominator_);
    BigUnsignedBase<Base> newDenominator = denominator_ * BigUnsignedBase<Base>(other.numerator_);
    return Rational(newNumerator, newDenominator);
}

template <int Base>
void Rational<Base>::reduce() {
    // Implementar la reducción de la fracción
}

template <int Base>
std::ostream& operator<<(std::ostream& os, const Rational<Base>& rational) {
    os << rational.numerator_ << "/" << rational.denominator_;
    return os;
}

template <int Base>
std::istream& operator>>(std::istream& is, Rational<Base>& rational) {
    std::string input;
    is >> input;
    size_t pos = input.find('/');
    if (pos != std::string::npos) {
        std::string numStr = input.substr(0, pos);
        std::string denomStr = input.substr(pos + 1);
        rational.numerator_ = BigInteger<Base>(reinterpret_cast<const unsigned char*>(numStr.c_str()));
        rational.denominator_ = BigUnsignedBase<Base>(reinterpret_cast<const unsigned char*>(denomStr.c_str()));
    }
    return is;
}

/**
 * This program demonstrates the usage of the BigUnsignedBase, BigInteger, and Rational classes
 * with various base representations (binary, octal, decimal, hexadecimal).
 * 
 * Expected output:
 * 
 * num1: 12345
 * num2: 6789
 * num1 + num2: 19134
 * num1 - num2: 4556
 * num1 * num2: 83810205
 * num1 / num2: 1
 * num1 % num2: 4556
 * 
 * bin1: 1010
 * bin2: 1100
 * bin1 + bin2: 10110
 * 
 * oct1: 123
 * oct2: 456
 * oct1 + oct2: 601
 * 
 * hex1: 1A
 * hex2: 2B
 * hex1 + hex2: 45
 * 
 * bint1: 12345
 * bint2: -6789
 * bint1 + bint2: 4556
 * bint1 - bint2: 19134
 * bint1 * bint2: -83810205
 * bint1 / bint2: -1
 * bint1 % bint2: 4556
 * 
 * rat1: 12345/6789
 * rat2: 6789/12345
 * rat1 + rat2: 152399076/83810205
 * rat1 - rat2: 83810205/152399076
 * rat1 * rat2: 83810205/83810205
 * rat1 / rat2: 152399076/83810205
 */
int main() {
    BigUnsignedBase<10> num1(12345);
    BigUnsignedBase<10> num2(6789);

    std::cout << "num1: " << num1 << std::endl;
    std::cout << "num2: " << num2 << std::endl;

    BigUnsignedBase<10> sum = num1 + num2;
    std::cout << "num1 + num2: " << sum << std::endl;

    BigUnsignedBase<10> diff = num1 - num2;
    std::cout << "num1 - num2: " << diff << std::endl;

    BigUnsignedBase<10> prod = num1 * num2;
    std::cout << "num1 * num2: " << prod << std::endl;

    BigUnsignedBase<10> quot = num1 / num2;
    std::cout << "num1 / num2: " << quot << std::endl;

    BigUnsignedBase<10> mod = num1 % num2;
    std::cout << "num1 % num2: " << mod << std::endl;

    BigUnsignedBase<2> bin1(1010);
    BigUnsignedBase<2> bin2(1100);
    std::cout << "bin1: " << bin1 << std::endl;
    std::cout << "bin2: " << bin2 << std::endl;
    std::cout << "bin1 + bin2: " << (bin1 + bin2) << std::endl;

    BigUnsignedBase<8> oct1(123);
    BigUnsignedBase<8> oct2(456);
    std::cout << "oct1: " << oct1 << std::endl;
    std::cout << "oct2: " << oct2 << std::endl;
    std::cout << "oct1 + oct2: " << (oct1 + oct2) << std::endl;

    unsigned char sex[] = {'1', 'A'}; 
    unsigned char sex2[] = {'2', 'B'};

    BigUnsignedBase<16> hex1(sex);
    BigUnsignedBase<16> hex2(sex2);
    std::cout << "hex1: " << hex1 << std::endl;
    std::cout << "hex2: " << hex2 << std::endl;
    std::cout << "hex1 + hex2: " << (hex1 + hex2) << std::endl;

    BigInteger<10> bint1(12345);
    BigInteger<10> bint2(-6789);

    std::cout << "bint1: " << bint1 << std::endl;
    std::cout << "bint2: " << bint2 << std::endl;

    BigInteger<10> bint_sum = bint1 + bint2;
    std::cout << "bint1 + bint2: " << bint_sum << std::endl;

    BigInteger<10> bint_diff = bint1 - bint2;
    std::cout << "bint1 - bint2: " << bint_diff << std::endl;

    BigInteger<10> bint_prod = bint1 * bint2;
    std::cout << "bint1 * bint2: " << bint_prod << std::endl;

    BigInteger<10> bint_quot = bint1 / bint2;
    std::cout << "bint1 / bint2: " << bint_quot << std::endl;

    BigInteger<10> bint_mod = bint1 % bint2;
    std::cout << "bint1 % bint2: " << bint_mod << std::endl;
    Rational<10> rat1(12345, 6789);
    Rational<10> rat2(6789, 12345);

    std::cout << "rat1: " << rat1 << std::endl;
    std::cout << "rat2: " << rat2 << std::endl;

    Rational<10> rat_sum = rat1 + rat2;
    std::cout << "rat1 + rat2: " << rat_sum << std::endl;

    Rational<10> rat_diff = rat1 - rat2;
    std::cout << "rat1 - rat2: " << rat_diff << std::endl;

    Rational<10> rat_prod = rat1 * rat2;
    std::cout << "rat1 * rat2: " << rat_prod << std::endl;

    // Rational<10> rat_quot = rat1 / rat2;
    // std::cout << "rat1 / rat2: " << rat_quot << std::endl;

    return 0;
}
 
    // Expected output:
    // bint1: 12345
    // bint2: -6789
    // bint1 + bint2: 4556
    // bint1 - bint2: 19134
    // bint1 * bint2: -83810205
    // bint1 / bint2: -1
    // bint1 % bint2: 4556
