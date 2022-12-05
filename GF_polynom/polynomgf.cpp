#include "polynomgf.h"

PolynomGF::PolynomGF(GF* gf) : _gf(gf) {}
PolynomGF::PolynomGF(GF* gf, std::map<int, GF::Element> vals) : PolynomGF(gf) {
	for(auto i = vals.begin(); i != vals.end(); i++){
		if(gf->check_and_set_element(i->second)) _vals[i->first] = i->second;
	}
}
PolynomGF::PolynomGF(const PolynomGF& p) : _gf(p._gf), _vals(p._vals) {}

void PolynomGF::append(const GF::Element& e, int p){
	GF::Element in = e;
	if(_gf->check_and_set_element(in)) _vals[p] = in;
}

GF::Element PolynomGF::calc_value(const GF::Element& e){
	GF::Element out = _gf->get_element_by_power(-1);

	for(auto i = _vals.begin(); i != _vals.end(); i++){
		GF::Element t = _gf->get_element_by_power(0);
		for(int ii = 0; ii < i->first; ii++) t *= e;
		out += t * i->second;
	}

	return out;
}

PolynomGF PolynomGF::add(const PolynomGF& p){
	PolynomGF out(*this);

	auto zero = _gf->get_element_by_power(-1);
	for(auto i = p._vals.begin(); i != p._vals.end(); i++){
		if(auto ii = out._vals.find(i->first); ii != out._vals.end()) ii->second += i->second;
		else out._vals[i->first] = i->second;

		if(out._vals[i->first] == zero) out._vals.erase(i->first);
	}
	if(out._vals.empty()) out._vals[0] = zero;

	return out;
}

PolynomGF PolynomGF::sub(const PolynomGF& p){
	PolynomGF out(*this);

	auto zero = _gf->get_element_by_power(-1);
	for(auto i = p._vals.begin(); i != p._vals.end(); i++){
		if(auto ii = out._vals.find(i->first); ii != out._vals.end()) ii->second -= i->second;
		//else { auto t = i->second; out._vals[i->first] = -t; }
		else out._vals[i->first] = -((GF::Element)i->second);

		if(out._vals[i->first] == zero) out._vals.erase(i->first);
	}
	if(out._vals.empty()) out._vals[0] = zero;

	return out;
}

PolynomGF PolynomGF::mul(const PolynomGF& p){
	PolynomGF out(_gf);

	auto zero = _gf->get_element_by_power(-1);
	for(auto i = p._vals.begin(); i != p._vals.end(); i++){
		for(auto ii = _vals.begin(); ii != _vals.end(); ii++){
			int pt = i->first + ii->first;
			if(auto j = out._vals.find(pt); j != out._vals.end()) j->second += (GF::Element)i->second * ii->second;
			else out._vals[pt] = (GF::Element)i->second * ii->second;

			if(out._vals[pt] == zero) out._vals.erase(pt);
		}
		if(out._vals.empty()) out._vals[0] = zero;
	}
	return out;
}

#define lst(x)x._vals.rbegin()
#define sz(x) lst(x)->first

PolynomGF PolynomGF::div(const PolynomGF& p){
	PolynomGF r(*this);
	PolynomGF k(_gf);

	while(sz(r) >= sz(p)){
		auto k1 = lst(r)->second / lst(p)->second;
		int p1 = sz(r) - sz(p);

		PolynomGF t(_gf);
		for(auto i = p._vals.begin(); i != p._vals.end(); i++)
			t._vals[p1 + i->first] = k1 * i->second;

		r -= t;
		k.append(k1, p1);
	}
	return k;
}

std::tuple<PolynomGF, PolynomGF> PolynomGF::div_with_rem(const PolynomGF& p){
	PolynomGF r(*this);
	PolynomGF k(_gf);

	while(sz(r) >= sz(p)){
		auto k1 = lst(r)->second / lst(p)->second;
		int p1 = sz(r) - sz(p);

		PolynomGF t(_gf);
		for(auto i = p._vals.begin(); i != p._vals.end(); i++)
			t._vals[p1 + i->first] = k1 * i->second;

		r -= t;
		k.append(k1, p1);
	}

	return {k, r};
}

PolynomGF PolynomGF::operator+(const PolynomGF& p){
	return add(p);
}

PolynomGF PolynomGF::operator+=(const PolynomGF& p){
	*this = *this + p;
	return *this;
}

PolynomGF PolynomGF::operator-(const PolynomGF& p){
	return sub(p);
}

PolynomGF PolynomGF::operator-=(const PolynomGF& p){
	*this = *this - p;
	return *this;
}

PolynomGF PolynomGF::operator*(const GF::Element& e){
	PolynomGF out(_gf);
	for(auto i = _vals.begin(); i != _vals.end(); i++) out._vals[i->first] = i->second * e;
	return out;
}

PolynomGF PolynomGF::operator*=(const GF::Element& e){
	for(auto i = _vals.begin(); i != _vals.end(); i++) i->second *= e;
	return *this;
}

PolynomGF PolynomGF::operator*(const PolynomGF& p){
	return mul(p);
}

PolynomGF PolynomGF::operator*=(const PolynomGF& p){
	*this = *this * p;
	return *this;
}

PolynomGF PolynomGF::operator/(const PolynomGF& p){
	return div(p);
}

PolynomGF PolynomGF::operator/=(const PolynomGF& p){
	*this = *this / p;
	return *this;
}

GF::Element PolynomGF::get_by_power(int n){
	return _vals[n];
}

GF::Element& PolynomGF::operator[](int n){
	return _vals[n];
}

#define _x(n) << (n >= 1 ? "x" : "") << (n >= 2 ? "^" + std::to_string(n) : "")

std::ostream& operator<<(std::ostream& out, const PolynomGF& p){
	auto zero = p._gf->get_element_by_power(-1);
	out << '(' << p._vals.rbegin()->second <<')' _x(p._vals.rbegin()->first);
	for(auto i = ++p._vals.rbegin(); i != p._vals.rend(); i++) out << " + (" << i->second << ')' _x(i->first);
	return out;
}
