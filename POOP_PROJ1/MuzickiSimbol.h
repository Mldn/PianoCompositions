#ifndef _MUZICKISIMBOL_H_
#define _MUZICKISIMBOL_H_

#include "Trajanje.h"
#include <iostream>

using namespace std;

class MuzickiSimbol
{
protected:
	bool spliter = false;
	
	Trajanje t;
public:
	MuzickiSimbol(Trajanje tr): t(tr.GetT()) {}
	MuzickiSimbol() {};

	//virtual MuzickiSimbol* kopija() = 0;

	virtual void podeli()=0;
	MuzickiSimbol& povrati() {
		this->t.mul();
		this->spliter = false;
		return *this;
	}

	virtual bool jesteNota() = 0;
	virtual bool jestePauza() = 0;
	virtual bool jesteAkord() = 0;

	virtual string MSXML(bool premesti, bool akord) = 0;

	const Trajanje& getT() const { return t; }

	virtual void PromeniOktavu(int o) {
		return;
	}
	virtual void povisi() {
		return;
	}
	virtual void PromeniVisinu(char c) {
		return;
		//if (c != 'A' && c != 'B' && c != 'C' && c != 'D' && c != 'E' && c != 'F' && c != 'G') cout << "Ne postoji zadata visina!";
	}

	bool GetSpliter() const {
		return spliter;
	}
	MuzickiSimbol& operator = (const MuzickiSimbol & m) {
		this->spliter = m.spliter;
		this->t = m.t;
		return *this;
	}
	virtual void pisi(ostream& os) const {
		os << t;
	}

	friend ostream& operator<<(ostream& os,const MuzickiSimbol& ms) {
		ms.pisi(os);
		return os;
	}
	virtual int GetOktava() const {
		return 0;
	}
	virtual char GetVisina() const {
		return 0;
	}

	virtual bool GetPovisena() const {
		return false;
	}

};

#endif // _MUZICKISIMBOL_H_