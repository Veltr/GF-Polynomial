#ifndef POLYNOMGF_H
#define POLYNOMGF_H

#include "GF_polynom_global.h"

#include "gf.h"
#include <map>

// x^3 + 2x + 3 ==> map{{0, 3}, {1, 2}, {3, 1}}

class GF_POLYNOM_EXPORT PolynomGF{
public:
	PolynomGF(GF* gf);
	PolynomGF(GF* gf, std::map<int, GF::Element> vals);
	PolynomGF(const PolynomGF& p);

	void append(const GF::Element&, int p);

	GF::Element calc_value(const GF::Element&);

	PolynomGF add(const PolynomGF&);
	PolynomGF sub(const PolynomGF&);
	PolynomGF mul(const PolynomGF&);
	PolynomGF div(const PolynomGF&);

	std::tuple<PolynomGF, PolynomGF> div_with_rem(const PolynomGF&);

	PolynomGF operator+(const PolynomGF&);
	PolynomGF operator+=(const PolynomGF&);

	PolynomGF operator-(const PolynomGF&);
	PolynomGF operator-=(const PolynomGF&);

	PolynomGF operator*(const GF::Element&);
	PolynomGF operator*=(const GF::Element&);

	PolynomGF operator*(const PolynomGF&);
	PolynomGF operator*=(const PolynomGF&);

	PolynomGF operator/(const PolynomGF&);
	PolynomGF operator/=(const PolynomGF&);

	GF::Element get_by_power(int);
	GF::Element& operator[](int);

	friend GF_POLYNOM_EXPORT std::ostream& operator<<(std::ostream&, const PolynomGF&);

private:
	GF* _gf;
	std::map<int, GF::Element> _vals;
};

#endif // POLYNOMGF_H
