#pragma once


#include <iterator>
#include <list>
#include "Takt.h"

using namespace std;

class LinijskiSistem
{
	list<Takt> taktovi;
	list<Takt>::iterator it;
	list<MuzickiSimbol*>::iterator i;
public:
	LinijskiSistem() {
		taktovi.clear();
	}
	LinijskiSistem(list<Takt> t){
		taktovi = t;
	}

	void dodaj(Takt t) {
		taktovi.push_back(t);
	}
	friend class Kompozicija;
	friend ostream& operator <<(ostream&os, const Kompozicija& k);

	LinijskiSistem& PromenaTakta(double t) {
		it = taktovi.begin();
		bool premesti = false, prosirivanje = (t > it->GetT() ? 1 : 0);
		MuzickiSimbol* deo = nullptr ;
		list<Takt> pom;
		it++;
		//pom.splice(it, taktovi);
		pom.swap(taktovi);
		taktovi.clear();
		it = pom.begin();
		Takt tak(t);
		bool p = false;
		for (it = pom.begin(); it != pom.end(); it++) {
			/*if (skracivanje) {
				tak.dodaj(deo);
				skracivanje = false;
			}*/
			for (i = it->simboli.begin(); i != it->simboli.end(); i++) {
				if (premesti) {
					i++;
					if (i == it->simboli.end()) break;
					premesti = false;
				}
				if (!tak.popujenjen()) {
					if (prosirivanje) {
						if ((*i)->GetSpliter()) {
							(*i)->povrati();
							premesti = true;
						}
						p = tak.dodaj(*i);
					}else{  p = tak.dodaj(*i); }
					
					if (p) {
						this->dodaj(tak);
						tak.brisi();
						tak.dodaj(*i);
						//if (premesti) premesti = false;
					}
				}
				else {
					this->dodaj(tak);
					tak.brisi();
					if (prosirivanje) {
						if ((*i)->GetSpliter()) {
							(*i)->povrati();
							premesti = true;
						}
						p = tak.dodaj(*i);
					}
					else { p = tak.dodaj(*i); }
					if (p) {
						this->dodaj(tak);
						tak.brisi();
						tak.dodaj(*i);
						//if (premesti) premesti = false;
					}
				}
				

			}

		}
		this->dodaj(tak);
		tak.brisi();

		return *this;
	}
	friend ostream& operator << (ostream&os, const LinijskiSistem& ls) {
		for (auto it = ls.taktovi.begin(); it != ls.taktovi.end(); it++) {
			os << *it;
		}
		return os;
	}

	auto GetTaktovi() {
		return taktovi;
	}


};

