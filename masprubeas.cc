
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
    return 0;
}