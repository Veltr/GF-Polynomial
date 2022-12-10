#ifndef GF_H
#define GF_H

// В векторе в начале стоит старшая степень
// x^3 + 2x + 3 ==> [1, 0, 2, 3]

#include "GF_polynom_global.h"
//#include "polynomgf.h"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <list>
#include <map>
#include <string>

class GF_POLYNOM_EXPORT GF{
public:
	class GF_POLYNOM_EXPORT Element{
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

		friend GF_POLYNOM_EXPORT std::ostream& operator<<(std::ostream&, const Element&);

	private:
		std::list<int> _value;
		GF* _base = NULL;
		int _power;
	};

	GF(int mode, int p);
	void build_GF(const std::list<int>& m);
	void build_GF();
	std::ostream& print_GF(std::ostream& out);

	Element get_element(const std::list<int>& value);
	Element get_element_by_power(int power);
	bool check_and_set_element(Element& e);
	Element get_rev(const Element& e);

	struct GF_POLYNOM_EXPORT BuildProgress {
		float first;
		float second;
		std::string msg;
	};

	//float get_build_progress(){ return _elements.size() / pow(_mode, _p); }
	BuildProgress get_build_progress();

	bool operator== (const GF&);
	bool operator!= (const GF&);
	friend GF_POLYNOM_EXPORT std::ostream& operator<<(std::ostream&, const GF&);

private:
	int _mode;
	size_t _p;

	std::vector<GF::Element> _elements;
	std::vector<size_t> _revs;
	std::list<int> _m;

	const int _build_steps_number = 3; // check pol/get gen, get els, get revs
	int _cur_build_step = 0;
	float _cur_progress = 0;

	void build_GF_n1(int m);
	bool check_polynom(const std::list<int>&);
	void get_revs();

	int get_power(Element& e);
	int mode_power(int a, int p, int m);
};

#include "polynomgf.h"

#endif // GF_H
