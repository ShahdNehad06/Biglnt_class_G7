#include <cstdint>
#include <iostream>
#include <string>
#include <cstdint>
#include <algorithm>
using namespace std;

class BigInt {
    string number;    // Stores the number as a string
    bool isNegative;  // True if number is negative

    // Remove unnecessary leading zeros from the number string
    void removeLeadingZeros() {
    size_t pos = number.find_first_not_of('0');
    if (pos == string::npos) {
        number = "0";
        isNegative = false;
    } else {
        number = number.substr(pos);
    }
    }

    // Compare absolute values of two BigInts (ignore signs)
    // Returns: 1 if |this| > |other|, 0 if equal, -1 if |this| < |other|
    int compareMagnitude(const BigInt& other) const {
    if (number.size() > other.number.size()) return 1;
    if (number.size() < other.number.size()) return -1;
    if (number == other.number) return 0;
    return (number > other.number) ? 1 : -1;

    }

public:
    // Default constructor - initialize to zero
    BigInt() {
    number = "0";
    isNegative = false;
}

    // Constructor from 64-bit integer
    BigInt(int64_t value) {
    if (value < 0) {
        isNegative = true;
        value = -value;
    } else {
        isNegative = false;
    }
    number = to_string(value);
}

    // Constructor from string representation.
BigInt(const string& str) {
    if (str.empty()) {
        number = "0";
        isNegative = false;
        return;
    }
    if (str[0] == '-') {
        isNegative = true;
        number = str.substr(1);
    } else {
        isNegative = false;
        number = str;
    }
    removeLeadingZeros();
}
    // Copy constructor
  BigInt(const BigInt& other) {
    number = other.number;
    isNegative = other.isNegative;
}

    // Destructor
   ~BigInt() {
    // Nothing to clean up
}

    // Assignment operator
    BigInt& operator=(const BigInt& other) {
        if(this !=  &other){
            number = other.number;
        }
        return *this;
    }

    // Unary negation operator (-x)
    BigInt operator-() const {
        BigInt result = *this;
        if(result.number != "0")
            result.isNegative = !isNegative;
        
        return result;
    }

    // Unary plus operator (+x)
    BigInt operator+() const {
        return *this;
    }

    string addStrings(string a, string b) {
        string result = "";
        int carry = 0;
        
        reverse(a.begin(), a.end());
        reverse(b.begin(), b.end());
        
        while (a.size() < b.size()) a.push_back('0');
        while (b.size() < a.size()) b.push_back('0');
        
        for (int i = 0; i < a.size(); i++) {
            int sum = (a[i]-'0') + (b[i]-'0') + carry;
            result.push_back((sum % 10) + '0');
            carry = sum / 10;
        }
        
        if (carry) result.push_back(carry + '0');
        
        reverse(result.begin(), result.end());
        return result;
    }
    
    string subtractStrings(string a, string b) {
        string result = "";
        reverse(a.begin(), a.end());
        reverse(b.begin(), b.end());
        
        int borrow = 0;
        
        while (b.size() < a.size()) b.push_back('0');
        
        for (int i = 0; i < a.size(); i++) {
            int diff = (a[i]-'0') - (b[i]-'0') - borrow;
            
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            
            result.push_back(diff + '0');
        }
        
        while (result.size() > 1 && result.back() == '0')
        result.pop_back();
        
        reverse(result.begin(), result.end());
        return result;
    }
    
    // Addition assignment operator (x += y)
    BigInt& operator+=(const BigInt& other) {
        if (isNegative == other.isNegative) {
            number = addStrings(number, other.number);
        }
        else {
            if (number == other.number) {
                number = "0";
                isNegative = false;
            }
            else if (number.size() > other.number.size() ||
                    (number.size() == other.number.size() && number > other.number)) {
                number = subtractStrings(number, other.number);
            }
            else {
                number = subtractStrings(other.number, number);
                isNegative = other.isNegative;
            }
        }
    
        return *this;
    }
    // Substraction assignment operator(x -= y)
    BigInt& operator-=(const BigInt& other) {
        *this += (-other);
        return *this;
    }
    // Multiplcation assignment operator(x *= y)
    BigInt& operator*=(const BigInt& other) {
        string result(number.size() + other.number.size(), '0');
    
        for (int i = number.size()-1; i >= 0; i--) {
            for (int j = other.number.size()-1; j >= 0; j--) {
                int mul = (number[i]-'0') * (other.number[j]-'0');
                int sum = (result[i+j+1]-'0') + mul;
    
                result[i+j+1] = (sum % 10) + '0';
                result[i+j] += sum / 10;
            }
        }
    
        while (result.size() > 1 && result[0] == '0')
            result.erase(0,1);
    
        number = result;
        isNegative = (isNegative != other.isNegative);
    
        if (number == "0")
            isNegative = false;
    
        return *this;
    }
    // Division assignment operator (x /= y)
    BigInt& operator/=(const BigInt& other) {
        if(other.number == "0")
            throw runtime_error("Division By Zero");
            
            BigInt dividend = *this;
            BigInt divisor = other;
        
            dividend.isNegative = false;
            divisor.isNegative = false;
        
            // If dividend < divisor → result = 0
            if (dividend < divisor) {
                number = "0";
                isNegative = false;
                return *this;
            }
        
            BigInt quotient("0");
            BigInt one("1");
        
            while (dividend >= divisor) {
                dividend -= divisor;
                quotient += one;
            }
        
            quotient.isNegative = (isNegative != other.isNegative);
        
            if (quotient.number == "0")
                quotient.isNegative = false;
        
            *this = quotient;
            return *this;
            return *this;
        }
        
    // Modulus assignment operator (x %= y)
    BigInt& operator%=(const BigInt& other) {
        if(other.number == "0")
            throw runtime_error("Division By Zero");
        
    BigInt original = *this;
    *this = *this - ((*this / other) * other);
    isNegative = original.isNegative;

    if (number == "0")
        isNegative = false;

        return *this;
    }


    // Pre-increment operator (++x)
    BigInt& operator++() {
        // TODO: Implement this operator
       *this+=BigInt(1);//we do new object of class bigint using constructor and store it this
        return *this;
    }

    // Post-increment operator (x++)
    BigInt operator++(int) {
        BigInt temp = *this;//let temp = the current value and it will print the current value
        ++*this;//this line store in the value of this the incremented value
        return temp;
    }

    // Pre-decrement operator (--x)
    BigInt& operator--() {
        // TODO: Implement this operator
       *this-=BigInt(1);
        return *this;
    }

    // Post-decrement operator (x--)
    BigInt operator--(int) {
        BigInt temp =*this;
        --*this;
        return temp;
    }

    // Convert BigInt to string representation
    string toString() const {
        if (isNegative) {
            return "-" + number;
        }
       else {
           return "" + number;
       }
    }

    // Output stream operator (for printing)
    friend ostream& operator<<(ostream& os, const BigInt& num) {
        // TODO: Implement this operator
       if (num.isNegative && num.number!="0") {
           os<<"- ";
       }
       os << num.number;
        return os;
    }

    // Input stream operator (for reading from input)
    friend istream& operator>>(istream& is, BigInt& num) {
        // TODO: Implement this operator
        string x;
        is >> x;
        num=BigInt(x);
        return is;
    }

    // Friend declarations for comparison operators
    friend bool operator==(const BigInt& lhs, const BigInt& rhs);
    friend bool operator<(const BigInt& lhs, const BigInt& rhs);
    friend bool operator>=(const BigInt& lhs,const BigInt& rhs);
    friend BigInt operator-(const BigInt lhs,const BigInt& rhs);
    friend BigInt operator*(const BigInt lhs,const BigInt& rhs);
    friend BigInt operator/(const BigInt lhs,const BigInt& rhs);
};

// Binary addition operator (x + y)
BigInt operator+(BigInt lhs, const BigInt& rhs) {
    BigInt result=lhs;
    result+=rhs;
    return result;
}

// Binary subtraction operator (x - y)
BigInt operator-(BigInt lhs, const BigInt& rhs) {
    BigInt result=lhs;
    result-=rhs;
    return result;
}

// Binary multiplication operator (x * y)
BigInt operator*(BigInt lhs, const BigInt& rhs) {
    BigInt result=lhs;
    result*=rhs;
    return result;
}

// Binary division operator (x / y)
BigInt operator/(BigInt lhs, const BigInt& rhs) {
    BigInt result=lhs;
    result/=rhs;
    return result;
}

// Binary modulus operator (x % y)
BigInt operator%(BigInt lhs, const BigInt& rhs) {
    BigInt result=lhs;
    result%=rhs;
    return result;
}

// Equality comparison operator (x == y)
bool operator==(const BigInt& lhs, const BigInt& rhs) {
    return (lhs.isNegative == rhs.isNegative) && (lhs.number == rhs.number);
}

// Inequality comparison operator (x != y)
bool operator!=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs == rhs);
}

// Less-than comparison operator (x < y)
bool operator<(const BigInt& lhs, const BigInt& rhs) {
    if (lhs.isNegative != rhs.isNegative)
        return lhs.isNegative;

    int cmp = lhs.compareMagnitude(rhs);

    if (lhs.isNegative)
        return cmp > 0;  // reverse for negatives
    else
        return cmp < 0;
}

// Less-than-or-equal comparison operator (x <= y)
bool operator<=(const BigInt& lhs, const BigInt& rhs) {
    return (lhs < rhs) || (lhs == rhs);
}


// Greater-than comparison operator (x > y)
bool operator>(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs <= rhs);
}

// Greater-than-or-equal comparison operator (x >= y)
bool operator>=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs < rhs);
}

int main() {
    cout << "=== BigInt Class Test Program ===" << endl << endl;
    cout << "NOTE: All functions are currently empty." << endl;
    cout << "Your task is to implement ALL the functions above." << endl;
    cout << "The tests below will work once you implement them correctly." << endl << endl;

    /*
    // Test 1: Constructors and basic output
    cout << "1. Constructors and output:" << endl;
    BigInt a(12345);              // Should create BigInt from integer
    BigInt b("-67890");           // Should create BigInt from string
    BigInt c("0");                // Should handle zero correctly
    BigInt d = a;                 // Should use copy constructor
    cout << "a (from int): " << a << endl;        // Should print "12345"
    cout << "b (from string): " << b << endl;     // Should print "-67890"
    cout << "c (zero): " << c << endl;            // Should print "0"
    cout << "d (copy of a): " << d << endl << endl; // Should print "12345"

    // Test 2: Arithmetic operations
    cout << "2. Arithmetic operations:" << endl;
    cout << "a + b = " << a + b << endl;          // Should calculate 12345 + (-67890)
    cout << "a - b = " << a - b << endl;          // Should calculate 12345 - (-67890)
    cout << "a * b = " << a * b << endl;          // Should calculate 12345 * (-67890)
    cout << "b / a = " << b / a << endl;          // Should calculate (-67890) / 12345
    cout << "a % 100 = " << a % BigInt(100) << endl << endl; // Should calculate 12345 % 100

    // Test 3: Relational operators
    cout << "3. Relational operators:" << endl;
    cout << "a == d: " << (a == d) << endl;       // Should be true (12345 == 12345)
    cout << "a != b: " << (a != b) << endl;       // Should be true (12345 != -67890)
    cout << "a < b: " << (a < b) << endl;         // Should be false (12345 < -67890)
    cout << "a > b: " << (a > b) << endl;         // Should be true (12345 > -67890)
    cout << "c == 0: " << (c == BigInt(0)) << endl << endl; // Should be true (0 == 0)

    // Test 4: Unary operators and increments
    cout << "4. Unary operators and increments:" << endl;
    cout << "-a: " << -a << endl;                 // Should print "-12345"
    cout << "++a: " << ++a << endl;               // Should increment and print "12346"
    cout << "a--: " << a-- << endl;               // Should print "12346" then decrement
    cout << "a after decrement: " << a << endl << endl; // Should print "12345"

    // Test 5: Large number operations
    cout << "5. Large number operations:" << endl;
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    cout << "Very large addition: " << num1 + num2 << endl;
    cout << "Very large multiplication: " << num1 * num2 << endl << endl;

    // Test 6: Edge cases and error handling
    cout << "6. Edge cases:" << endl;
    BigInt zero(0);
    BigInt one(1);
    try {
        BigInt result = one / zero;               // Should throw division by zero error
        cout << "Division by zero succeeded (unexpected)" << endl;
    } catch (const runtime_error& e) {
        cout << "Division by zero correctly threw error: " << e.what() << endl;
    }
    cout << "Multiplication by zero: " << one * zero << endl;        // Should be "0"
    cout << "Negative multiplication: " << BigInt(-5) * BigInt(3) << endl;  // Should be "-15"
    cout << "Negative division: " << BigInt(-10) / BigInt(3) << endl;       // Should be "-3"
    cout << "Negative modulus: " << BigInt(-10) % BigInt(3) << endl;        // Should be "-1"
    */

    return 0;
}