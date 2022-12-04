#ifndef POLYNOMGF_H
#define POLYNOMGF_H

#include "GF_polynom_global.h"

#include <list>
#include "gf.h"

// x^2 + 2x + 3 ==> {1, 2, 3}

class GF_POLYNOM_EXPORT PolynomGF_old{
public:
	PolynomGF_old(GF* gf);
	PolynomGF_old(GF* gf, std::list<GF::Element> vals);
	PolynomGF_old(const PolynomGF_old& p);


	PolynomGF_old add(const PolynomGF_old&);
	PolynomGF_old sub(const PolynomGF_old&);
	PolynomGF_old mul(const PolynomGF_old&);
	PolynomGF_old div(const PolynomGF_old&);

	PolynomGF_old operator+(const PolynomGF_old&);
	PolynomGF_old operator+=(const PolynomGF_old&);

	PolynomGF_old operator-(const PolynomGF_old&);
	PolynomGF_old operator-=(const PolynomGF_old&);

	PolynomGF_old operator*(const PolynomGF_old&);
	PolynomGF_old operator*=(const PolynomGF_old&);

	PolynomGF_old operator/(const PolynomGF_old&);
	PolynomGF_old operator/=(const PolynomGF_old&);

	friend GF_POLYNOM_EXPORT std::ostream& operator<<(std::ostream&, const PolynomGF_old&);

private:
	GF* _gf;
	std::list<GF::Element> _vals;
};

#endif // POLYNOMGF_H
