#pragma once
#include <list>
#include <iostream>
#include "MuzickiSimbol.h"
#include <iterator>

using namespace std;

class Takt
{
	Trajanje t;
	list<MuzickiSimbol*> simboli;
	
	
	Trajanje pop = 0;
public:
	
	Takt(double tr): t(tr) {}
	Takt(Trajanje t) {
		this->t = t;
	}

	auto getSimboli() {
		return simboli;
	}

	friend class LinijskiSistem;
	bool dodaj(MuzickiSimbol *m) {
		if (pop + m->getT() > t ) {
			m->podeli();
			pop += m->getT();
			simboli.push_back(m);
			return true;
		}
		pop += m->getT();
		simboli.push_back(m);
		return false;
	}
	Trajanje GetT() const {
		return t;
	}
	bool popujenjen() {
		return pop == t;
	}

	void brisi() {
		simboli.clear();
		pop = 0;
	}
	friend ostream& operator << (ostream& os, const Takt&t) {
		os << "|";
		for (auto i = t.simboli.begin(); i != t.simboli.end(); ++i) {
			os << **i << " ";
		}
		os << "|" << "\n";
		return os;
	}
	Takt& promeniT(Trajanje t) {
		this->t = t;
		return *this;
	}
	~Takt() {
		brisi();
	}
};

