#pragma once
#include "Nota.h"
#include <list>
#include "GreskaUTaktu.h"
#include <iostream>
#include <iterator>

using namespace std;

class Akord :
	public MuzickiSimbol
{
	list<Nota> note;
	
public:
	void podeli() override {
		this->t.div();
		this->spliter = true;
	}

	Akord(Trajanje tr) : MuzickiSimbol(tr) {}
	
	auto GetNote() {
		return this->note;
	}

	void dodajNotu(Nota& n) {
		if (n.getT() != this->t) throw GreskaUTaktu();
		else note.push_back(n);
	}

	void pisi(ostream& os) const override {
		os << "{";
		for (auto it = note.begin(); it != note.end(); it++) {
			os << *it << " ";
		}
		os << "}";
	}
	bool prazan() {
		return this->note.empty();
	}
	bool jesteNota() override {
		return false;
	}
	bool jestePauza() override {
		return false;
	}

	bool jesteAkord() override {
		return true;
	}

	virtual string MSXML(bool premesti, bool akord) override {

		string str, pom;
		pom = note.front().MSXML(premesti, false);

		str += pom;
		for (list<Nota>::iterator i = note.begin(); i != note.end(); i++) {
			if (i == note.begin()) continue;
			str += "    <note>\n";
			//str += "       <chord/>";
			str += i->MSXML(premesti, akord);
			str += "    </note>\n";
		}
		return str;
	}





	//~Akord();
};

