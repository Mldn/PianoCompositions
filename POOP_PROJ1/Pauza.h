#pragma once
#include "MuzickiSimbol.h"
#include <iostream>

using namespace std;

class Pauza :
	public MuzickiSimbol
{
public:
	Pauza(double tr) : MuzickiSimbol(tr) {}
	Pauza(Trajanje t) : MuzickiSimbol(t){}
	
	void podeli() override {
		this->t.div();
		this->spliter = true;
	}
	bool jesteNota() override {
		return false;
	}
	bool jestePauza() override {
		return true;
	}

	bool jesteAkord() override {
		return false;
	}
	void pisi(ostream& os) const override {
		static const Trajanje cetvrtina(4, 1);
		static const Trajanje osmina(8, 1);
		if (t == osmina) os << "p_1/8";
		if (t == cetvrtina) os << "P_1/4";
	}
	virtual string MSXML(bool premesti, bool akord) override {
		static const Trajanje cetvrtina(4, 1);
		static const Trajanje osmina(8, 1);
		string str;
		int d ;
		if (this->t == cetvrtina)
			d = 2;
		else {
			d = 1;
		}

		str += "    <note>\n";
		str += "     <rest/>\n";
		str += "    <duration>";
		str += '0' + d ;
		str += "</duration>\n";
		str += "    </note>\n";
		return str;
	}


	~Pauza();

};

