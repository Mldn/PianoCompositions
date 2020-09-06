#pragma once
#include "LinijskiSistem.h"

class Kompozicija
{
	//Trajanje trajanje_takta;
	LinijskiSistem desna;
	LinijskiSistem leva;
public:



	Kompozicija(LinijskiSistem d, LinijskiSistem l) : desna(d), leva(l) {}
	friend class LinijskiSistem;

	/*void SetTakt(Trajanje t) {
		trajanje_takta = t;
	}
	Trajanje GetTrajanje(Trajanje t) const  {
		return t;
	}
	*/
	void pisi(ostream& os) {
		list<Takt>::iterator it;
		list<Takt>::iterator ir;
		for ( it = this->leva.taktovi.begin() , ir = this->desna.taktovi.begin(); it != this->leva.taktovi.end(); it++, ir++) {
			os << *it << "\n" << *ir;
			os << "\n \n";
		}
	}

	LinijskiSistem& getLinijskiSistem(char c) {
		if (c == 'l') return this->leva;
		if (c == 'd') return this->desna;
	}

	void PromenaOktave(int p) {
		list<Takt>::iterator it;
		list<MuzickiSimbol*> pom;
		list<MuzickiSimbol*>::iterator ii , ij;
		list<Takt>::iterator ir;
		for (it = this->leva.taktovi.begin(), ir = this->desna.taktovi.begin(); it != this->leva.taktovi.end(); it++, ir++) {
			for (ii = it->getSimboli().begin(), ij = ir->getSimboli().begin(); ii != it->getSimboli().end(); ii++, ij++) {
				if((*ii)->jesteNota()&&((*ij)->jestePauza())) {
					int ok = (*ii)->GetOktava();
					(*ii)->PromeniOktavu(p);
					if (((ok > 3) && ((*ii)->GetOktava() < 4)) || ((ok < 4) && ((*ii)->GetOktava() > 3))) {
						auto pom = *ii;
						*ii = *ij;
						*ij = pom;
					}
				}
				if ((*ij)->jesteNota() && ((*ii)->jestePauza())) {
					int ok = (*ij)->GetOktava();
					(*ij)->PromeniOktavu(p);
					if (((ok > 3) && ((*ij)->GetOktava() < 4)) || ((ok < 4) && ((*ij)->GetOktava() > 3))) {
						auto pom = *ii;
						*ii = *ij;
						*ij = pom;
					}
				}
				if ((*ij)->jestePauza() && ((*ii)->jestePauza())) {
					continue;
				}
			}
		}



	}

	/*Takt IteracijaTakt(int i ,char s) {
		i++;
		int b;
		list<Takt>::iterator it;
		list<Takt>::iterator ir;
		for (it = this->leva.taktovi.begin(), it = this->desna.taktovi.begin(), b = 0; b < i, it != this->leva.taktovi.end(); it++, ir++, b++);
		
	}*/

	friend ostream& operator << (ostream& os, Kompozicija &k) {
		
		k.pisi(os);
		/*os << k.desna;
		os << "\n" << "\n";
		os << k.leva;
		return os; */
		return os;
	}


	//~Kompozicija();
};

