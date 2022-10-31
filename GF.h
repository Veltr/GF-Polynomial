#ifndef GF_H
#define GF_H

// В векторе в начале стоит старшая степень
// x^3 + 2x + 3 ==> [1, 0, 2, 3]

#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <string>

class GF{
public:
    class Element{
        friend class GF;
    public:
        Element(){};
        Element(std::initializer_list<int> value, GF* base = NULL, int power = -2);
        Element(std::list<int>& value, GF* base = NULL, int power = -2);
        Element(const Element& other);
        ~Element(){ _value.clear(); }

        Element add(const Element& e);
        Element sub(const Element& e);
        Element mul(const Element& e);
        Element div(const Element& e);

        Element get_rev(){ return -(*this); }

        Element operator+(const Element& e);
        Element operator+=(const Element& e);

        Element operator-();
        Element operator-(const Element& e);
        Element operator-=(const Element& e);

        Element operator*(const Element& e);
        Element operator*=(const Element& e);

        Element operator/(const Element& e);
        Element operator/=(const Element& e);

        bool operator==(const Element& e);
        bool operator!=(const Element& e);

        std::ostream& print(std::ostream& out);
        std::ostream& print_as_an(std::ostream& out);
        int getPower(){ return _power; }
        std::list<int> getValue(){ return _value; }

        friend std::ostream& operator<<(std::ostream&, const Element&);

    private:
        std::list<int> _value;
        GF* _base;
        int _power;
    };
    //    friend class Element;

    GF(int mode, int p);
    void build_GF(const std::list<int>& m);
    std::ostream& print_GF(std::ostream& out);

    Element get_element_by_power(int power);
    bool check_and_set_element(Element& e);
    Element get_rev(const Element& e);

    bool operator== (const GF&);
    bool operator!= (const GF&);
    friend std::ostream& operator<<(std::ostream&, const GF&);

private:
    int _mode;
    size_t _p;

    std::vector<GF::Element> _elements;
    std::vector<size_t> _revs;
    std::list<int> _m;

    int get_power(Element& e);
    void get_revs();
};

//=================================================
// GF::Element

#define set_power _base != NULL && power == -2 ? _base->get_power(*this) : power

GF::Element::Element(std::initializer_list<int> value, GF* base, int power) : _value(*new std::list<int>(value)), _base(base), _power(set_power) {}
GF::Element::Element(std::list<int>& value, GF* base, int power) : _value(value), _base(base), _power(set_power) {}
GF::Element::Element(const Element& other) : _value(other._value), _base(other._base), _power(other._power) {}

GF::Element GF::Element::add(const Element& e){
    //if(e._base != _base) throw "Not equal bases";
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
    //if(e._base != _base) throw "Not equal bases";
    if(*e._base != *_base) throw "Not equal bases";
    if(e._power < 0) return *this;

    return add(_base->get_rev(e));
}

GF::Element GF::Element::mul(const Element& e){
    //if(e._base != _base) throw "Not equal bases";
    if(*e._base != *_base) throw "Not equal bases";
    if(e._power < 0 || _power < 0) return _base->get_element_by_power(-1);

    return _base->get_element_by_power(_power + e._power);
}

GF::Element GF::Element::div(const Element& e){
    //if(e._base != _base) throw "Not equal bases";
    if(*e._base != *_base) throw "Not equal bases";
    //if(e._power < 0 || _power < 0) return _base->get_element_by_power(-1);
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
    for(auto i = (++m0.begin()); i != m0.end(); i++) {
        int t = (-(*i)) % _mode;
        if(t < 0) t += _mode;
        _m.push_back(t);
    }

    _elements.push_back(Element({0}, this, -1));
    _elements.push_back(Element({1}, this, 0));
    _elements.push_back(Element({1, 0}, this, 1));

    for(size_t i = 3, size = pow(_mode, _p); i < size; i++){
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

bool GF::operator==(const GF& g){
    return (g._mode == _mode) && (g._p == _p) && (g._m == _m);
}

bool GF::operator!=(const GF& g){
    return !(*this == g);
}

std::ostream& GF::print_GF(std::ostream& out){
    /*//fprintf(out, "0) 0\n1) 1\na) a\n");
    out << "0) 0\n1) 1\na) a\n";
    for(size_t i = 3; i < _elements.size(); i++){
        //fprintf(out, "a^%d) ", i - 1);
        //_elements[i].print(out);
        //fputc('\n', out);
        _elements[i].print_as_an(out);
        out << _elements[i] << '\n';
    }*/
    return out << *this;
}

std::ostream& operator<<(std::ostream& out, const GF& e){
    //fprintf(out, "0) 0\n1) 1\na) a\n");
    out << "0) 0\n1) 1\na) a\n";
    for(size_t i = 3; i < e._elements.size(); i++){
        //fprintf(out, "a^%d) ", i - 1);
        //_elements[i].print(out);
        //fputc('\n', out);

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
    _revs.resize(_elements.size(), 0);
    for(size_t i = 1; i < _revs.size(); i++){
        if(!_revs[i])
            for(size_t ii = i + 1; ii < _elements.size(); ii++)
                if((_elements[i] + _elements[ii])._power == -1){
                    _revs[i] = ii;
                    _revs[ii] = i;
                    break;
                }
    }
    return;
}

bool GF::check_and_set_element(Element& e){
    int p = get_power(e);
    if(p < -1) return false;
    e._base = this;
    e._power = p;
    return true;
}

GF::Element GF::get_rev(const Element& e){
    return _elements[_revs[e._power + 1]];
}

GF::Element GF::get_element_by_power(int power){
    return _elements[power % ((int)pow(_mode, _p) - 1) + 1];
}

#endif // GF_H
