#include "polynomgf_old.h"

PolynomGF_old::PolynomGF_old(GF* gf) : _gf(gf) {}
PolynomGF_old::PolynomGF_old(GF* gf, std::list<GF::Element> vals) : PolynomGF_old(gf) {
	for(auto i : vals){
		gf->check_and_set_element(i);
		_vals.push_back(i);
	}
}
PolynomGF_old::PolynomGF_old(const PolynomGF_old& p) : _gf(p._gf), _vals(p._vals) {}


PolynomGF_old PolynomGF_old::add(const PolynomGF_old& p){
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

	return PolynomGF_old(_gf, out);
}

PolynomGF_old PolynomGF_old::sub(const PolynomGF_old& p){
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

	auto z = _gf->get_element_by_power(-1);
	for(auto i = out.begin(); *i == z && i != --out.end();){
		i++;
		out.pop_front();
	}

	return PolynomGF_old(_gf, out);
}

PolynomGF_old PolynomGF_old::mul(const PolynomGF_old& p){
	PolynomGF_old out(_gf);
	std::list<GF::Element> t;
	for(auto i = _vals.begin(); i != _vals.end(); i++) t.push_back(*i);

	for(auto i = --p._vals.end(); i != --p._vals.begin(); i--){
		PolynomGF_old tt(_gf);
		for(auto ii = t.begin(); ii != t.end(); ii++) tt._vals.push_back(*ii * *i);
		out += tt;
		t.push_back(_gf->get_element_by_power(-1));
	}
	return out;
}

PolynomGF_old PolynomGF_old::div(const PolynomGF_old& p){
	std::vector<GF::Element> k(_vals.size() - p._vals.size() + 1, _gf->get_element_by_power(-1));
//    std::list<GF::Element> r;
//    for(auto i = _vals.begin(); i != _vals.end(); i++) r.push_back(*i);
	PolynomGF_old r(*this);

	while(r._vals.size() >= p._vals.size()){
		GF::Element e1 = r._vals.front() / p._vals.front();
		k[k.size() - (r._vals.size() - p._vals.size()) - 1] = e1;

		PolynomGF_old p1(p); for(auto i = p1._vals.begin(); i != p1._vals.end(); i++) *i *= e1;
		r -= p1;
		std::cout << r << '\n';
	}
	std::cout << 'a';
}

PolynomGF_old PolynomGF_old::operator+(const PolynomGF_old& p){
	return add(p);
}

PolynomGF_old PolynomGF_old::operator+=(const PolynomGF_old& p){
	*this = add(p);
	return *this;
}

PolynomGF_old PolynomGF_old::operator-(const PolynomGF_old& p){
	return sub(p);
}

PolynomGF_old PolynomGF_old::operator-=(const PolynomGF_old& p){
	*this = sub(p);
	return *this;
}

PolynomGF_old PolynomGF_old::operator*(const PolynomGF_old& p){
	return mul(p);
}

PolynomGF_old PolynomGF_old::operator*=(const PolynomGF_old& p){
	*this = mul(p);
	return *this;
}

PolynomGF_old PolynomGF_old::operator/(const PolynomGF_old& p){
	return div(p);
}

PolynomGF_old PolynomGF_old::operator/=(const PolynomGF_old& p){
	*this = div(p);
	return *this;
}

#define _x(n) << (n >= 1 ? "x" : "") << (n >= 2 ? "^" + std::to_string(n) : "")

std::ostream& operator<<(std::ostream& out, const PolynomGF_old& p){
	auto zero = p._gf->get_element_by_power(-1);
	int ip = p._vals.size() - 1;
	out << '(' << *p._vals.begin() <<')' _x(ip); ip--;
	for(auto i = ++p._vals.begin(); i != p._vals.end(); i++, ip--) if(zero != *i) out << " + (" << *i << ')' _x(ip);
	return out;
}
