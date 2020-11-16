#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <exception>
#include <stdexcept>
using namespace std;

class Rational {
public:
    Rational() : num(0), den(1) { }

    Rational(int numerator, int denominator)
    {
        if (denominator == 0)
        {
            throw std::invalid_argument("Invalid argument");
        }
        int gcd = std::gcd(numerator, denominator);
        num = numerator / gcd;
        den = denominator / gcd;
        if (num < 0 && den < 0)
        {
            num = abs(num);
            den = abs(den);
        }
        else if (den < 0)
        {
            num = -num;
            den = -den;
        }
    }

    int Numerator() const
    {
        return num;
    }

    int Denominator() const
    {
        return den;
    }

    bool operator== (const Rational& rhs) const
    {
        return num * rhs.Denominator() == rhs.Numerator() * den;
    }

    bool operator< (const Rational& rhs) const
    {
        return num * rhs.Denominator() < rhs.Numerator() * den;
    }

    bool operator> (const Rational& rhs) const
    {
        return num * rhs.Denominator() > rhs.Numerator() * den;
    }

    Rational operator+ (const Rational& rhs) const
    {
        return Rational(num * rhs.Denominator() + rhs.Numerator() * den, den * rhs.Denominator());
    }

    Rational operator- (const Rational& rhs) const
    {
        return Rational(num * rhs.Denominator() - rhs.Numerator() * den, den * rhs.Denominator());
    }

    Rational operator* (const Rational& rhs) const
    {
        return Rational(num * rhs.Numerator(), den * rhs.Denominator());
    }

    Rational operator/ (const Rational& rhs) const
    {
        if (rhs.Numerator() == 0)
        {
            throw std::domain_error("Division by zero");
        }
        return Rational(num * rhs.Denominator(), den * rhs.Numerator());
    }
private:
    int num;
    int den;
};

istream& operator>> (istream& stream, Rational& r)
{
    int p;
    int q;
    char op;
    stream >> p >> op >> q;
    if (stream && op == '/')
        r = Rational(p, q);
    return stream;
}

ostream& operator<< (ostream& stream, const Rational& r)
{
    stream << r.Numerator() << '/' << r.Denominator();
    return stream;
}



int main() {
    {
        const Rational r(3, 10);
        if (r.Numerator() != 3 || r.Denominator() != 10) {
            cout << "Rational(3, 10) != 3/10" << endl;
            return 1;
        }
    }

    {
        const Rational r(8, 12);
        if (r.Numerator() != 2 || r.Denominator() != 3) {
            cout << "Rational(8, 12) != 2/3" << endl;
            return 2;
        }
    }

    {
        const Rational r(-4, 6);
        if (r.Numerator() != -2 || r.Denominator() != 3) {
            cout << "Rational(-4, 6) != -2/3" << endl;
            return 3;
        }
    }

    {
        const Rational r(4, -6);
        if (r.Numerator() != -2 || r.Denominator() != 3) {
            cout << "Rational(4, -6) != -2/3" << endl;
            return 3;
        }
    }

    {
        const Rational r(0, 15);
        if (r.Numerator() != 0 || r.Denominator() != 1) {
            cout << "Rational(0, 15) != 0/1" << endl;
            return 4;
        }
    }

    {
        const Rational defaultConstructed;
        if (defaultConstructed.Numerator() != 0 || defaultConstructed.Denominator() != 1) {
            cout << "Rational() != 0/1" << endl;
            return 5;
        }
    }

    {
        Rational r1(4, 6);
        Rational r2(2, 3);
        bool equal = r1 == r2;
        if (!equal) {
            cout << "4/6 != 2/3" << endl;
            return 1;
        }
    }

    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a + b;
        bool equal = c == Rational(2, 1);
        if (!equal) {
            cout << "2/3 + 4/3 != 2" << endl;
            return 2;
        }
    }

    {
        Rational a(5, 7);
        Rational b(2, 9);
        Rational c = a - b;
        bool equal = c == Rational(31, 63);
        if (!equal) {
            cout << "5/7 - 2/9 != 31/63" << endl;
            return 3;
        }
    }

    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a * b;
        bool equal = c == Rational(8, 9);
        if (!equal) {
            cout << "2/3 * 4/3 != 8/9" << endl;
            return 1;
        }
    }

    {
        Rational a(5, 4);
        Rational b(15, 8);
        Rational c = a / b;
        bool equal = c == Rational(2, 3);
        if (!equal) {
            cout << "5/4 / 15/8 != 2/3" << endl;
            return 2;
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    {
        ostringstream output;
        output << Rational(-6, 8);
        if (output.str() != "-3/4") {
            cout << "Rational(-6, 8) should be written as \"-3/4\"" << endl;
            return 1;
        }
    }

    {
        istringstream input("5/7");
        Rational r;
        input >> r;
        bool equal = r == Rational(5, 7);
        if (!equal) {
            cout << "5/7 is incorrectly read as " << r << endl;
            return 2;
        }
    }

    {
        istringstream input("");
        Rational r;
        bool correct = !(input >> r);
        if (!correct) {
            cout << "Read from empty stream works incorrectly" << endl;
            return 3;
        }
    }

    {
        istringstream input("5/7 10/8");
        Rational r1, r2;
        input >> r1 >> r2;
        bool correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Multiple values are read incorrectly: " << r1 << " " << r2 << endl;
            return 4;
        }

        input >> r1;
        input >> r2;
        correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Read from empty stream shouldn't change arguments: " << r1 << " " << r2 << endl;
            return 5;
        }
    }

    {
        istringstream input1("1*2"), input2("1/"), input3("/4");
        Rational r1, r2, r3;
        input1 >> r1;
        input2 >> r2;
        input3 >> r3;
        bool correct = r1 == Rational() && r2 == Rational() && r3 == Rational();
        if (!correct) {
            cout << "Reading of incorrectly formatted rationals shouldn't change arguments: "
                << r1 << " " << r2 << " " << r3 << endl;

            return 6;
        }
    }
    {
        const set<Rational> rs = { {1, 2}, {1, 25}, {3, 4}, {3, 4}, {1, 2} };
        if (rs.size() != 3) {
            cout << "Wrong amount of items in the set" << endl;
            return 1;
        }

        vector<Rational> v;
        for (auto x : rs) {
            v.push_back(x);
        }
        if (v != vector<Rational>{ {1, 25}, { 1, 2 }, { 3, 4 }}) {
            cout << "Rationals comparison works incorrectly" << endl;
            return 2;
        }
    }

    {
        map<Rational, int> count;
        ++count[{1, 2}];
        ++count[{1, 2}];

        ++count[{2, 3}];

        if (count.size() != 2) {
            cout << "Wrong amount of items in the map" << endl;
            return 3;
        }
    }

    try {
        Rational r(1, 0);
        cout << "Doesn't throw in case of zero denominator" << endl;
        return 1;
    }
    catch (std::invalid_argument&) {
    }

    try {
        auto x = Rational(1, 2) / Rational(0, 1);
        cout << "Doesn't throw in case of division by zero" << endl;
        return 2;
    }
    catch (std::domain_error&) {
    }

    cout << "OK" << endl;
    return 0;
}
