#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>

template<unsigned char Base>
class BigUnsigned {
private:
    std::vector<unsigned char> digits_; // Para almacenar los dígitos en base

public:
    // Constructor que acepta un número entero
    BigUnsigned(unsigned int n) {
        // Lógica para convertir el número entero a dígitos en la base correspondiente
        while (n > 0) {
            digits_.push_back(n % Base);
            n /= Base;
        }
        std::reverse(digits_.begin(), digits_.end());
    }

    // Constructor que acepta const char*
    BigUnsigned(const char* digits) {
        while (*digits) {
            if (*digits < '0' || *digits >= '0' + Base) {
                throw std::invalid_argument("Invalid digit for the base.");
            }
            digits_.push_back(*digits - '0'); // Convertir char a unsigned char
            ++digits; // Avanzar al siguiente carácter
        }
    }

    // Método para mostrar el número (solo para pruebas)
    void print() const {
        for (auto digit : digits_) {
            std::cout << (int)digit; // Imprimir cada dígito como entero
        }
        std::cout << std::endl;
    }
};
