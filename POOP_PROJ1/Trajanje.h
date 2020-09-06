#pragma once
#include <iostream>
#include "PogresanImenilac.h"

using namespace std;

class Trajanje
{
	double trajanje;
public:
	Trajanje() {};
	Trajanje(double t) : trajanje(t) {}
	Trajanje(int i, int b) {
		if (i != 8 && i != 4) throw new PogresanImenilac();
		trajanje = 1.0 *b / i;
	}

	void div() {
		this->trajanje /= 2;
	}
	void mul() {
		this->trajanje *= 2;
	}
	double GetT() const {
		return trajanje;
	}
	friend bool operator!= (const Trajanje& t1,const Trajanje& t2) {
		if (t2.trajanje != t1.trajanje) return true;
		else return false;
	}
	friend bool operator == (const Trajanje& t1, const Trajanje &t2) {
		return !(t1 != t2);
	}
	Trajanje operator += (const Trajanje& t) {
		return this->trajanje=this->trajanje + t.trajanje;
	}
	friend Trajanje operator + (const Trajanje& t1,const Trajanje& t2) {
		Trajanje tn;
		tn.trajanje = t1.trajanje + t2.trajanje;
		return tn;
	}
	friend bool operator > (const Trajanje& t1, const Trajanje& t2) {
		return t1.trajanje > t2.trajanje;
	}
	friend bool operator < (const Trajanje& t1,const Trajanje& t2) {
		return t1.trajanje < t2.trajanje;
	}
	Trajanje& operator= (const Trajanje& t) {
		if (this != &t) {
			this->trajanje = t.trajanje;
		}
		return *this;
	}

	string TXML() const {
		string s;
		int b = trajanje * 8;
		int i = 8 ;
		s = "     <time>\n      <beats>";
		s += b + '0';
		s.append("</beats>\n      <beat-type>");
		s += i + '0';
		s.append("</beat-type>\n     </time>");
		return s;
	}


	friend ostream& operator << (ostream& os, const Trajanje t) {
		os << t.trajanje;
		return os;
	}
};

