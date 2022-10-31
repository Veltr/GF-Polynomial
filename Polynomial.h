#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <list>
#include "GF.h"

// x^2 + 2x + 3 ==> {1, 2, 3}

class PolynomGF{
public:
    PolynomGF(std::list<GF::Element> vals) : _vals(vals) {}
    PolynomGF(std::initializer_list<GF::Element> vals) : _vals(vals) {}
    PolynomGF(const PolynomGF& p) : _vals(p._vals) {}


    PolynomGF add(const PolynomGF&);
    PolynomGF sub(const PolynomGF&);
    PolynomGF mul(const PolynomGF&);
    PolynomGF div(const PolynomGF&);

    PolynomGF operator+(const PolynomGF&);
    PolynomGF operator+=(const PolynomGF&);

    PolynomGF operator-(const PolynomGF&);
    PolynomGF operator-=(const PolynomGF&);

    PolynomGF operator*(const PolynomGF&);
    PolynomGF operator*=(const PolynomGF&);

    PolynomGF operator/(const PolynomGF&);
    PolynomGF operator/=(const PolynomGF&);

private:
    std::list<GF::Element> _vals;
};

PolynomGF PolynomGF::add(const PolynomGF& p){
    std::list<GF::Element> out;

    auto i1 = --_vals.end();
    auto i2 = --p._vals.end();

    while(1){
        if(i1 == --_vals.begin()){
            while(i2 != --p._vals.begin()) out.push_front(*(i2--));
            break;
        }
        if(i2 == --p._vals.begin()){
            while(i1 != --_vals.begin()) out.push_front(*(i1--));
            break;
        }
        //std::cout << *i1 << " ++ " << *i2 << '\n';

        out.push_front(*(i1--) + *(i2--));
    }

    return PolynomGF(out);
}

PolynomGF PolynomGF::sub(const PolynomGF& p){
    std::list<GF::Element> out;

    auto i1 = --_vals.end();
    auto i2 = --p._vals.end();

    while(1){
        if(i1 == --_vals.begin()){
            while(i2 != --p._vals.begin()) out.push_front( -((GF::Element)(*(i2--))) );
            break;
        }
        if(i2 == --p._vals.begin()){
            while(i1 != --_vals.begin()) out.push_front(*(i1--));
            break;
        }
        //std::cout << *i1 << " -- " << *i2 << '\n';

        out.push_front(*(i1--) - *(i2--));
    }

    return PolynomGF(out);
}

PolynomGF PolynomGF::mul(const PolynomGF& p){
    // coming soon...
}

PolynomGF PolynomGF::div(const PolynomGF& p){
    // coming soon...
}

PolynomGF PolynomGF::operator+(const PolynomGF& p){
    return add(p);
}

PolynomGF PolynomGF::operator+=(const PolynomGF& p){
    *this = add(p);
    return *this;
}

PolynomGF PolynomGF::operator-(const PolynomGF& p){
    return sub(p);
}

PolynomGF PolynomGF::operator-=(const PolynomGF& p){
    *this = sub(p);
    return *this;
}

PolynomGF PolynomGF::operator*(const PolynomGF& p){
    return mul(p);
}

PolynomGF PolynomGF::operator*=(const PolynomGF& p){
    *this = mul(p);
    return *this;
}

PolynomGF PolynomGF::operator/(const PolynomGF& p){
    return div(p);
}

PolynomGF PolynomGF::operator/=(const PolynomGF& p){
    *this = div(p);
    return *this;
}

#endif // POLYNOMIAL_H
