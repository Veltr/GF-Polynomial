#include "gf.h"

//=================================================
// GF::Element

#define set_power _base != NULL && power == -2 ? _base->get_power(*this) : power

GF::Element::Element(std::initializer_list<int> value, GF* base, int power) : _value(*new std::list<int>(value)), _base(base), _power(set_power) {}
GF::Element::Element(std::list<int>& value, GF* base, int power) : _value(value), _base(base), _power(set_power) {}
GF::Element::Element(const Element& other) : _value(other._value), _base(other._base), _power(other._power) {}

GF::Element GF::Element::add(const Element& e){
//    if()
//    if(!e._base){}
	if(*e._base != *_base) throw "Not equal bases";

	const Element* te;
	auto i = --e._value.end(), end = --e._value.begin();
	if(e._value.size() > _value.size()){
		te = &e;
		i = --_value.end();
		end = --_value.begin();
	}
	else te = this;
	Element out(*te);

	for(auto ii = --out._value.end(); i != end; i--, ii--){
		//        std::cout << *ii << " + " << *i << '\n';
		*ii = (*ii + *i) % _base->_mode;
	}

	while(!(*out._value.begin()) && out._value.size() > 1) out._value.pop_front();
	if(!out._value.size()) out._value.push_back(0);

	out._power = out._base->get_power(out);
	return out;
}

GF::Element GF::Element::sub(const Element& e){
	if(*e._base != *_base) throw "Not equal bases";
	if(e._power < 0) return *this;

	return add(_base->get_rev(e));
}

GF::Element GF::Element::mul(const Element& e){
	if(*e._base != *_base) throw "Not equal bases";
	if(e._power < 0 || _power < 0) return _base->get_element_by_power(-1);

	return _base->get_element_by_power(_power + e._power);
}

GF::Element GF::Element::div(const Element& e){
	if(*e._base != *_base) throw "Not equal bases";
	if(_power < 0) return _base->get_element_by_power(-1);

	int p = _power - e._power;
	//    if(p < 0){ p %= _base->_elements.size() - 1; p += _base->_elements.size() - 1; }
	for(int t = _base->_elements.size() - 1; p < 0;) p += t;
	return _base->get_element_by_power(p);
}

GF::Element GF::Element::operator+(const Element& e){
	return add(e);
}

GF::Element GF::Element::operator+=(const Element& e){
	*this = add(e);
	return *this;
}

GF::Element GF::Element::operator-(){
	return _base->get_rev(*this);
}

GF::Element GF::Element::operator-(const Element& e){
	return sub(e);
}

GF::Element GF::Element::operator-=(const Element& e){
	*this = sub(e);
	return *this;
}

GF::Element GF::Element::operator*(const Element& e){
	return mul(e);
}

GF::Element GF::Element::operator*=(const Element& e){
	*this = mul(e);
	return *this;
}

GF::Element GF::Element::operator/(const Element& e){
	return div(e);
}

GF::Element GF::Element::operator/=(const Element& e){
	*this = div(e);
	return *this;
}

bool GF::Element::operator==(const Element& e){
	if(e._base != _base || e._power != _power) return false;
	return true;
}

bool GF::Element::operator!=(const Element& e){
	return !(*this == e);
}

std::ostream& GF::Element::print(std::ostream& out){
	return out << *this;
}

std::ostream& GF::Element::print_as_an(std::ostream& out){
	if(_power == -1) out << 0;
	else if(!_power) out << 1;
	else if(_power == 1) out << 'a';
	else out << "a^" << _power;
	return out;
}

std::ostream& operator<<(std::ostream& out, const GF::Element& e){
	size_t ip = e._value.size() - 1;
	std::string t = "";
	for(auto ii = e._value.begin(); ii != e._value.end(); ii++, ip--)
		if(*ii){
			if((*ii > 1) || (ip < 1)) t += std::to_string(*ii);
			if(ip > 0){
				t += "a";
				if(ip > 1) t += '^' + std::to_string(ip);
			}
			t += " + ";
		}
	if(t == "") t = "0 + ";
	t[t.size() - 3] = '\0';
	//fprintf(out, "%s", t.c_str());
	out << t.c_str();
	return out;
}

//=================================================
// GF

GF::GF(int mode, int p) : _mode(mode), _p(p){}

void GF::build_GF(const std::list<int>& m0){
	if(m0.size() < 2) { build_GF(); return; }
	if(m0.size() == 2){
		build_GF_n1(_mode-*(--m0.end()));
		return;
	}

	if(!check_polynom(m0)){ throw std::runtime_error("Polynom isn't primitive"); return; }

	for(auto i = (++m0.begin()); i != m0.end(); i++) {
		int t = (-(*i)) % _mode;
		if(t < 0) t += _mode;
		_m.push_back(t);
	}

	_cur_build_step++;
	_elements.push_back(Element({0}, this, -1));
	_elements.push_back(Element({1}, this, 0));
	_elements.push_back(Element({1, 0}, this, 1));

	for(size_t i = 3, size = pow(_mode, _p); i < size; i++){
		_cur_progress = (float)i / size;
		Element cur(_elements[i - 1]);
		cur._power = i - 1;
		cur._value.push_back(0);

		if(cur._value.size() > _p){
			int k1 = *cur._value.begin();
			cur._value.pop_front();
			for(auto ii = _m.begin(), iii = cur._value.begin(); ii != _m.end(); ii++, iii++)
				*iii = (*iii + *ii * k1) % _mode;

			while(!(*cur._value.begin())) cur._value.pop_front();
		}
		_elements.push_back(cur);
	}
	get_revs();
}

void GF::build_GF(){
	if(_p != 1) throw std::runtime_error("power != 1");

	if(_mode == 2){
		_elements.push_back(Element({0}, this, -1));
		_elements.push_back(Element({1}, this, 0));
		_m.push_back(1);
		_cur_progress = 1;
		get_revs();
		return;
	}

	if(_mode == 3){
		_elements.push_back(Element({0}, this, -1));
		_elements.push_back(Element({1}, this, 0));
		_elements.push_back(Element({2}, this, 1));
		_m.push_back(2);
		_cur_progress = 1;
		get_revs();
		return;
	}

	for(int i = 2; i < _mode - 1;){
		_cur_progress = (float)i / (_mode - 1);
		if(mode_power(i, _mode - 1, _mode) == 1){
			for(int ii = i, k = 1; k < _mode - 2; ii = (ii * i) % _mode, k++) if(ii == 1) goto skip1;
			build_GF_n1(i);
			return;
		}
skip1: i++;
	}
	throw std::runtime_error("No generators found");
}

bool GF::operator==(const GF& g){
	return (g._mode == _mode) && (g._p == _p) && (g._m == _m);
}

bool GF::operator!=(const GF& g){
	return !(*this == g);
}

std::ostream& GF::print_GF(std::ostream& out){
	return out << *this;
}

std::ostream& operator<<(std::ostream& out, const GF& e){
	if(e._elements.size() < 2) return out;
	out << "0) 0\n1) 1\n";
	for(size_t i = 2; i < e._elements.size(); i++){
		out << "a^" << i - 1 << ") ";
		out << e._elements[i] << '\n';
	}
	return out;
}

int GF::get_power(Element& e){
	for(size_t i = 0; i < _elements.size(); i++)
		if(_elements[i]._value == e._value) return _elements[i]._power;
	return -2;
}

void GF::get_revs(){
	_cur_build_step++;
	_revs.resize(_elements.size(), 0);
	for(size_t i = 1; i < _revs.size(); i++){
		if(!_revs[i])
			for(size_t ii = i; ii < _elements.size(); ii++)
				if((_elements[i] + _elements[ii])._power == -1){
					_cur_progress = (float)i / _revs.size() * 2;
					_revs[i] = ii;
					_revs[ii] = i;
					break;
				}
	}
	_cur_progress = 1;
	_cur_build_step++;
	return;
}

bool GF::check_and_set_element(Element& e){
	int p = get_power(e);
	if(p < -1) return false;
	e._base = this;
	e._power = p;
	return true;
}

void GF::build_GF_n1(int m){
	_cur_build_step++;
	_m.push_back(m);
	_elements.push_back(Element({0}, this, -1));
	_elements.push_back(Element({1}, this, 0));

	for(int i = 2, t = m; i < _mode; i++){
		_cur_progress = (float)i / _mode;
		_elements.push_back(Element({t}, this, i - 1));
		t = (t * m) % _mode;
	}
	get_revs();
}

bool GF::check_polynom(const std::list<int>& list){
	GF g1(_mode, 1); g1.build_GF();
	int pp = list.size() - 1;
	std::map<int, GF::Element> mapa;
	for(auto i = list.begin(); i != list.end(); i++, pp--){
		if(*i != 0)
			mapa[pp] = g1.get_element(std::list<int>{*i});
	}
	_cur_progress = 0.1;
	PolynomGF p1((GF*)&g1, mapa);

	pp = pow(_mode, _p) - 1;
	GF::Element mOne = g1.get_element({_mode - 1});
	GF::Element one = g1.get_element_by_power(0);
	PolynomGF ppn((GF*)&g1, {{pp, one}, {0, mOne}});
	auto[k0, r] = ppn.div_with_rem(p1);
	if(!r.is_zero()) return false;

	for(int k = 1; k < pp; k++){
		_cur_progress = (k + 1) / pp;
		PolynomGF pk((GF*)&g1, {{k, one}, {0, mOne}});
		auto[k0, r] = p1.div_with_rem(pk);
		if(r.is_zero()) return false;
		auto[k1, r2] = pk.div_with_rem(p1);
		if(r2.is_zero()) return false;
	}

	_cur_build_step++;
	return true;
}

GF::Element GF::get_rev(const Element& e){
	return _elements[_revs[e._power + 1]];
}

GF::BuildProgress GF::get_build_progress(){
	BuildProgress out;
	out.first = (float)_cur_build_step / _build_steps_number;
	out.second = _cur_progress;

	if(!_cur_build_step){
		if(_p > 1) out.msg = "Checking is polynom is primitive...";
		else out.msg = "Finding generator...";
	}
	else if(_cur_build_step == 1) out.msg = "Building elements...";
	else out.msg = "Getting reverse elements...";

	return out;
}


GF::Element GF::get_element(const std::list<int>& value){
	return *std::find_if(_elements.begin(), _elements.end(), [value](const GF::Element& e){return e._value == value;});
}

GF::Element GF::get_element_by_power(int power){
	if(_mode == 2 && _p == 1) return _elements[power + 1];
	return _elements[power % ((int)pow(_mode, _p) - 1) + 1];
}

int GF::mode_power(int a, int p, int m){
	int out = 1;

	while(p > 0){
		if(p % 2) out = (out * a) % m;
		p >>= 1;
		a = (a * a) % m;
	}

	return out;
}
