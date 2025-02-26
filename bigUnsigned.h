template<unsigned char Base>
class BigUnsigned {
public:
    // ... otros miembros y métodos ...
 
    // Constructor que acepta const char*
    BigUnsigned(const char* digits) {
        // Convierte const char* a unsigned char*
        size_t len = digits.length();
        for (size_t i = 0; i < len; ++i) {
            if (digits[i] < '0' || digits[i] >= '0' + Base) {
                throw std::invalid_argument("Invalid digit for the base.");
            }
            digits_.push_back(digits[i] - '0'); // Guardamos como números
        }
    }
};
