#pragma once
#include "MuzickiSimbol.h"
#include <iostream>

using namespace std;

class Nota : public MuzickiSimbol
{
	int oktava;
	char visina;
	bool povisena;
	const char static povisilica = '#';
public:
	Nota(Trajanje tr, int oktava, char v, bool p) :MuzickiSimbol(tr), oktava(oktava), visina(v) { povisena = p; }

	void povisi() override {
		if (this->visina == 'E' || this->visina == 'B') return;
		this->povisena = true;
	}
	bool jestePauza() override {
		return false;
	}

	bool jesteAkord() override {
		return false;
	}
	void podeli() override {
		this->t.div();
		this->spliter = true;
	}
	void PromeniVisinu(char c) override {
		if (c != 'A' && c != 'B' && c != 'C' && c != 'D' && c != 'E' && c != 'F' && c != 'G') { cout << "Ne postoji zadata visina!"; return; }
		this->visina = c;
	}
	bool jesteNota() override {
		return true;
	}

	void PromeniOktavu(int o) override {
		if ((this->oktava < 3 && o < 3) || (this->oktava > 3 && o > 3)) {
			this->oktava = o;
		} else cout << "Nije moguce promeniti oktavu note jer je nova oktava van njenog linijskog sistema!\n" ;
	}
	void pisi(ostream &os)const override {
		const Trajanje cetvrtina(4, 1);
		const Trajanje osmina(8, 1);

		if (this->getT() == cetvrtina) os << visina;
		else {
			os << (char)(visina - 'A' + 'a');
		}
			if (povisena) os << "#";
		os << oktava;
	}
	int GetOktava() const override {
		return this->oktava;
	}
	char GetVisina() const override {
		return this->visina;
	}

	bool GetPovisena() const override {
		return this->povisena;
	}

	virtual string MSXML(bool premesti, bool akord) override {
		int d;
		const Trajanje cetvrtina(4, 1);
		const Trajanje osmina(8, 1);
		if (this->t == osmina) {
			d = 1;
		}
		else {
			d = 2;
		}
		string s;
		if(akord) s += "       <chord/>";
		s += "    <note>\n";
		s += "     <pitch>\n";
		s += "      <step>";
		s += visina;
		s += "</step>\n";
		s += "      <octave>";
		s += '0' + oktava  ;
		s += "</octave>\n";
		if (povisena) {
			s += "      <alter>1</alter>\n";
		}
		s += "     </pitch>\n";
		s += "     <duration>";
		s += d + '0';
		s +="</duration>\n";
		if (premesti && this->GetSpliter()) {
			s += "     <tie type=\"start\"/>\n";
		}
		else if (!premesti && this->GetSpliter()) {
			s += "     <tie type=\"start\"/>\n";
			}

		s += "    </note>\n";
		return s;
	
		}


		
	

	//~Nota();
};

