#include <map>
#include "Nota.h"
#include "Pauza.h"
#include "Akord.h"
#include "Kompozicija.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include "PogresanImenilac.h"
#include "MidiFile.h"



using namespace std;
using namespace smf;

struct RGB {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};



struct nota_info {
	int oktava;
	char visina, opis;
	string nota;
	bool povisena;
	int midi_br;
	Trajanje t;
	nota_info(string o, string n, string mb  ) {
		opis = o[0];
		nota = n;
		midi_br = stoi(mb);
	}
	nota_info() {}

};

int main(int argc, char** argv) {
	std::map<char, nota_info> mapa;
	const Trajanje cetvrtina(4, 1);
	const Trajanje osmina(8, 1);
	std::map<string, int> midi_mapa;

	string directory = "map.csv";
	ifstream file(directory);
	string textl, karakter, Opis_note, midi_broj;
	vector<nota_info> ni;

	regex rx("([^,]+),([A-Z]#?[2-6]),([0-9][0-9])");
	//int i = 0;
	while (getline(file, textl)) {
		smatch result;
		if (regex_match(textl, result, rx)) {
			karakter = result.str(1);
			Opis_note = result.str(2);
			midi_broj = result.str(3);
		}
		nota_info nn = nota_info(karakter, Opis_note, midi_broj);
		nn.visina = Opis_note[0];
		if (Opis_note[1] == '#') {
			nn.povisena = true;
			nn.oktava = (int)(Opis_note[2] - '0');
		}
		else {
			nn.povisena = false;
			nn.oktava = (int)(Opis_note[1] - '0');
		}
		mapa[nn.opis] = nn;
		string zapis = "";
		zapis += nn.visina;
		if (nn.povisena) zapis += '#';
		zapis += nn.oktava + '0';
		if(zapis != "" ) midi_mapa[zapis] = nn.midi_br;
		ni.push_back(nn);
		//i++;
	}
	//cout << mapa['1'] << "\n" << mapa['q'] << "\n";
	try {
		bool eksportovano = false;
		Kompozicija* komp = nullptr;
		int q = 1;
		while (q) {
			system("CLS");
			cout << "IZABERITE OPCIJU MENIJA:\n";
			cout << "1.Unos podataka kompozicije iz datoteke. \n2.Citaj podatke iz kompozicije. \n3.Iteriraj kroz kompoziciju. \n4.Promena takta kompozicije. \n5.Promena oktave kompozicije. \n6.Eksportuj kompoziciju\n7.Napustanje programa.\n ";
			cin >> q;
			system("CLS");

			switch (q) {

			case 1: {
				int im, br;
				string dir;
				cout << "Unesite falj iz kod zelite da citate podatke o kompoziciji:\n";
				cin >> dir;
				system("CLS");
				cout << "Unesite zeljenu duzinu taktova :";
				cin >> br >> im;
				system("CLS");
				if (im != 4 && im != 8) throw PogresanImenilac();
				Trajanje trajanjeT(im, br);
				nota_info np;
				LinijskiSistem right, left;
				Takt taktr(trajanjeT), taktl(trajanjeT);
				bool p;

				ifstream file(dir);
				string str;
				getline(file, str);
				regex rex("(\\[[^\\]]*\\])?([^\\[]*)?");
				smatch res;
				sregex_iterator currM(str.begin(), str.end(), rex);
				sregex_iterator lstM;

				while (currM != lstM) {
					smatch match = *currM;
					//cout << match.str() << "\n";
					if (match.str().length() != 0) {
						if (match.str(1).length() != 0) {
							string pom = match.str(1);
							if (pom[2] == ' ') {
								for (char c : pom) {
									if (c == '[' || c == ']') continue;
									if (c != ' ') {
										np = mapa[c];
										Nota *n = new Nota(osmina, np.oktava, np.visina, np.povisena);
										Pauza *pa = new Pauza(osmina);
										if (np.oktava > 3) {
											if (!taktr.popujenjen()) {
												p = taktr.dodaj(n);
												taktl.dodaj(pa);
												if (p) {
													right.dodaj(taktr);
													left.dodaj(taktl);
													taktl.brisi();
													taktr.brisi();
													taktr.dodaj(n);
													taktl.dodaj(pa);
												}
											}
											else {
												right.dodaj(taktr);
												left.dodaj(taktl);
												taktr.brisi();
												taktl.brisi();
												taktr.dodaj(n);
												taktl.dodaj(pa);
											}
										}
										else {
											if (!taktl.popujenjen()) {
												p = taktl.dodaj(n);
												taktr.dodaj(pa);
												if (p) {
													right.dodaj(taktr);
													left.dodaj(taktl);
													taktl.brisi();
													taktr.brisi();
													taktl.dodaj(n);
													taktr.dodaj(pa);
												}
											}
											else {
												right.dodaj(taktr);
												left.dodaj(taktl);
												taktl.brisi();
												taktr.brisi();
												taktl.dodaj(n);
												taktr.dodaj(pa);
											}
										}
									}
									else {
										Pauza *pa = new Pauza(osmina);
										if (!taktl.popujenjen()) {
											p = taktl.dodaj(pa);
											taktr.dodaj(pa);
											if (p) {
												right.dodaj(taktr);
												left.dodaj(taktl);
												taktl.brisi();
												taktr.brisi();
												taktl.dodaj(pa);
												taktr.dodaj(pa);
											}
										}
										else {
											right.dodaj(taktr);
											left.dodaj(taktl);
											taktl.brisi();
											taktr.brisi();
											taktl.dodaj(pa);
											taktr.dodaj(pa);
										}
									
									}
								}

							}
							else {
								Akord* zalevo = new Akord(cetvrtina);
								Akord *zadesno = new Akord(cetvrtina);
								Pauza *pak = new Pauza(cetvrtina);
								bool p = false;
								for (char c : pom) {
									if (c == '[' || c == ']') continue;
									if (c == ' ') continue;
									np = mapa[c];
									Nota *n = new Nota(cetvrtina, np.oktava, np.visina, np.povisena);
									if (np.oktava > 3)
										zadesno->dodajNotu(*n);
									else zalevo->dodajNotu(*n);
								}

								if (!taktl.popujenjen()) {
									if (!zalevo->prazan())p = taktl.dodaj(zalevo); else p = taktl.dodaj(pak);
									if (!zadesno->prazan()) taktr.dodaj(zadesno);  else taktr.dodaj(pak);
									if (p) {
										right.dodaj(taktr);
										left.dodaj(taktl);
										taktl.brisi();
										taktr.brisi();
										if (!zalevo->prazan())p = taktl.dodaj(zalevo); else p = taktl.dodaj(pak);
										if (!zadesno->prazan()) taktr.dodaj(zadesno);  else taktr.dodaj(pak);
									}
								}
								else {
									right.dodaj(taktr);
									left.dodaj(taktl);
									taktl.brisi();
									taktr.brisi();
									if (!zalevo->prazan())p = taktl.dodaj(zalevo); else p = taktl.dodaj(pak);
									if (!zadesno->prazan()) taktr.dodaj(zadesno);  else taktr.dodaj(pak);
								}
							}
						}
						if (match.str(2).length() != 0) {
							string pom = match.str(2);
							for (char c : pom) {
								if (c != ' ' && c != '|') {
									np = mapa[c];
									Nota *n = new Nota(cetvrtina, np.oktava, np.visina, np.povisena);
									Pauza *pa = new Pauza(cetvrtina);
									if (np.oktava > 3) {
										if (!taktr.popujenjen()) {
											p = taktr.dodaj(n);
											taktl.dodaj(pa);
											if (p) {
												right.dodaj(taktr);
												left.dodaj(taktl);
												taktl.brisi();
												taktr.brisi();
												taktr.dodaj(n);
												taktl.dodaj(pa);
											}
										}
										else {
											right.dodaj(taktr);
											left.dodaj(taktl);
											taktr.brisi();
											taktl.brisi();
											taktr.dodaj(n);
											taktl.dodaj(pa);
										}
									}
									else {
										if (!taktl.popujenjen()) {
											p = taktl.dodaj(n);
											taktr.dodaj(pa);
											if (p) {
												right.dodaj(taktr);
												left.dodaj(taktl);
												taktl.brisi();
												taktr.brisi();
												taktl.dodaj(n);
												taktr.dodaj(pa);
											}
										}
										else {
											right.dodaj(taktr);
											left.dodaj(taktl);
											taktl.brisi();
											taktr.brisi();
											taktl.dodaj(n);
											taktr.dodaj(pa);
										}
									}
								}
								else {
									if (c == ' ') {
										Pauza *pa = new Pauza(osmina);
										if (!taktl.popujenjen()) {
											p = taktl.dodaj(pa);
											taktr.dodaj(pa);
											if (p) {
												right.dodaj(taktr);
												left.dodaj(taktl);
												taktl.brisi();
												taktr.brisi();
												taktl.dodaj(pa);
												taktr.dodaj(pa);
											}
										}
										else {
											right.dodaj(taktr);
											left.dodaj(taktl);
											taktl.brisi();
											taktr.brisi();
											taktl.dodaj(pa);
											taktr.dodaj(pa);
										}
									}
									else {
										Pauza *pa = new Pauza(cetvrtina);
										if (!taktl.popujenjen()) {
											p = taktl.dodaj(pa);
											taktr.dodaj(pa);
											if (p) {
												right.dodaj(taktr);
												left.dodaj(taktl);
												taktl.brisi();
												taktr.brisi();
												taktl.dodaj(pa);
												taktr.dodaj(pa);
											}
										}
										else {
											right.dodaj(taktr);
											left.dodaj(taktl);
											taktl.brisi();
											taktr.brisi();
											taktl.dodaj(pa);
											taktr.dodaj(pa);
										}
									}

								}
							}
						}
					}
					currM++;
				}
				
				while (!taktr.popujenjen()) {
					Pauza *pp = new Pauza(osmina);
					taktr.dodaj(pp);
					taktl.dodaj(pp);
				}

				right.dodaj(taktr);
				left.dodaj(taktl);
				taktl.brisi();
				taktr.brisi();
				komp = new Kompozicija(right, left);
				//cout << *komp;
			}

				break;
			case 2:
			{
				if (komp) cout << *komp;
				else "Kompozicija nije uneta !\n";
				cout << "\n____Pritisnite Enter da nastavite____";
				char e;
				cin.ignore();
				cin.get();
				system("CLS");
			}
				break;
			case 3:
			{
				list<Takt>::iterator it;
				list<Takt>::iterator ir;
				char s;
				//for (it = this->leva.taktovi.begin(), it = this->desna.taktovi.begin(), b = 0; b < i, it != this->leva.taktovi.end(); it++, ir++, b++);

				cout << "Zelite li da iterirate po taktovima levog ili desnog linijskog sistema? (l za levi, d za desni)\n";
				cin >> s;
				system("CLS");
				if (s != 'd' && s != 'l') exit(1); //GRESKA!!!!!
				//Takt tren = komp->IteracijaTakt(i, s);
				LinijskiSistem ls = komp->getLinijskiSistem(s);
				int i = 1, j = 1;
				list<Takt> tren = ls.GetTaktovi();
				for (it = tren.begin(); it != tren.end(); it++) {
					cout << "Trenutni takt:\n" << *it;
					int qt = 1;
					while (qt) {
						cout << "IZABERITE OPCIJU ZA ITERIRANJE KROZ TAKTOVE:(Unesite 0 za prekid) \n1.Predji na sledeci takt. \n2.Vrati se na prethodni takt. \n3.Zapocni iteriranje po notama.\n";
						cin >> qt;
						switch (qt) {
						case 1:
							if (it != tren.end()) {
								it++; i++;
							}
							else {
								cout << "Stigli ste do poslednjeg takta.\n"; break;
							}
							cout << "Trenutni takt:\n" << i << ":" << " \n" << *it;
							break;
						case 2:
							if (it != tren.begin()) {
								it--; i--;
							}
							else {
								cout << "Nalazite se na prvom taktu.\n";
								break;
							}
							cout << "Trenutni takt:\n" << i << ":" << " \n" << *it;
							break;
						case 3:
							system("CLS");
							list<MuzickiSimbol*>::iterator ii;
							list<MuzickiSimbol*>msp = it->getSimboli();
							for (ii = msp.begin(); ii != msp.end(); ii++) {
								cout << "Trenutna nota:\n" << **ii << "\n";
								int qtn = 1;
								while (qtn) {
									cout << "IZABERITE OPCIJU ZA ITERIRANJE KROZ NOTE:(Unesite 0 za prekid) \n1.Predji na sledecu notu. \n2.Vrati se na prethodnu notu. \n3.Promena oktave note \n4.Promena visine note \n5.Dodati povisenje noti \n";
									cin >> qtn;
									switch (qtn) {
									case 1:
										if (*ii != msp.back()) {
											ii++;
											j++;
										}
										else {
											cout << "Stigli ste do poslednje note u taktu.\n";
											break;
										}
										cout << "Trenutna nota:\n" << j << ":" << "\n" << **ii << "\n";
										break;
									case 2:
										if (ii != msp.begin()) { ii--; j--; }
										else { cout << "Ne postoji vise nota u taktu.\n"; break; }
										cout << "Trenutna nota:\n" << j << ":" << "\n" << **ii << "\n";
										break;
									case 3:
										cout << "Unesite novi broj oktave u kojoj zelite da svirate datu notu:\n";
										int o; cin >> o; cout << "\n";
										(*ii)->PromeniOktavu(o);
										cout << "Trenutna nota:\n" << j << ":" << "\n" << **ii << "\n";
										break;
									case 4:
										cout << "Unesite novu visinu note koju zelite da promenite:\n";
										char k; cin >> k; cout << "\n";
										(*ii)->PromeniVisinu(k);
										cout << "Trenutna nota:\n" << j << ":" << "\n" << **ii << "\n";
										break;
									case 5:
										(*ii)->povisi();
										cout << "Trenutna nota:\n" << j << ":" << "\n" << **ii << "\n";
										break;

									}
								}
								break;
							}

						}
					}
					break;
				}
				//for(it = komp->getLinijskiSistem(s).GetTaktovi().begin() ; )
			}
				break;
			case 4:
			{
				cout << "Unesite novo trajanje kompozicije:\n";
				int imenilac, brojilac;
				cin >> brojilac >> imenilac;
				double tra =1.0 * brojilac / imenilac;

				komp->getLinijskiSistem('l').PromenaTakta(tra);
				komp->getLinijskiSistem('d').PromenaTakta(tra);
			}
				break;
			case 5:
			{
				cout << "Unesite za koliko oktava zelite da pomerite kompoziciju:\n";
				int p;
				cin >> p;
				komp->PromenaOktave(p);

			}
			break;
			case 6:
			{
				eksportovano = true;
				int quet;
				cout << "IZABERITE U KOJI FORMAT ZELITE DA EKSPORTUJETE KOMPOZICIJU: \n1.MIDI \n2.XML \n3.BMP \n";
				cin >> quet;
				system("CLS");
				switch (quet) {
				case 1:
				{
					string fajl;
					cout << "Unesite ime fajla u koji zelite da eksportujete format:\n";
					cin >> fajl;
					MidiFile outputfile;
					outputfile.absoluteTicks();
					vector<uchar> midievent;
					midievent.resize(3);
					int tpq = 48;
					int q;

					outputfile.setTicksPerQuarterNote(2*tpq);
					outputfile.addTrack(1);
					const Trajanje cetvrtina(4, 1);
					const Trajanje osmina(8, 1);

					//int i = 0;
					int actiontime = 0;
					midievent[2] = 64;
					list<Takt>::iterator it;
					list<Takt>::iterator ir;
					bool premesti = false;
					LinijskiSistem ls = komp->getLinijskiSistem('l');

					list<Takt> tren = ls.GetTaktovi();

					for (it = tren.begin(); it != tren.end(); it++) {


						list<MuzickiSimbol*>::iterator ii;
						list<MuzickiSimbol*> msp = it->getSimboli();
						for (ii = msp.begin(); ii != msp.end(); ii++) {


							if (premesti) {
								ii++;
								premesti = false;
								if (ii == msp.end()) break;
								premesti = false;
							}


							if ((*ii)->jesteNota()) {
								string zapis = "";
								zapis += (*ii)->GetVisina();
								if ((*ii)->GetPovisena()) zapis += '#';
								zapis += (*ii)->GetOktava() + '0';
								midievent[0] = 0x90;
								midievent[1] = midi_mapa[zapis];
								if ((*ii)->GetSpliter()) {
									(*ii)->povrati();
									premesti = true;
								}

								outputfile.addEvent(1, actiontime, midievent);
								actiontime += tpq / 2 * ((*ii)->getT() == cetvrtina ? 2 : 1);

								midievent[0] = 0x80;
								outputfile.addEvent(1, actiontime, midievent);
							}
							if ((*ii)->jestePauza()) {
								actiontime += tpq / 2 * ((*ii)->getT() == cetvrtina ? 2 : 1);
							}
							if ((*ii)->jesteAkord()) {
								int at = actiontime;
								if ((*ii)->GetSpliter()) {
									(*ii)->povrati();
									premesti = true;
								}
								list<Nota>::iterator v;
								list<Nota> ak = ((Akord*)(*ii))->GetNote();
								for (v = ak.begin(); v != ak.end(); v++) {
									string zapis = "";
									zapis += v->GetVisina();
									if ( v->GetPovisena()) zapis += '#';
									zapis += v->GetOktava() + '0';
									midievent[0] = 0x90;
									midievent[1] = midi_mapa[zapis];
									outputfile.addEvent(1, actiontime, midievent);
									//at += tpq / 2 * ((v)->getT() == cetvrtina ? 2 : 1);

								}
								actiontime = at + tpq;
								midievent[0] = 0x80;
								outputfile.addEvent(1, actiontime, midievent);

							}


						}
					}
					//i = 0;
					actiontime = 0;
					LinijskiSistem ds = komp->getLinijskiSistem('d');
					premesti = false;
					list<Takt> trend = ds.GetTaktovi();

					for (it = trend.begin(); it != trend.end(); it++) {
						list<MuzickiSimbol*>::iterator ii;
						list<MuzickiSimbol*> msp = it->getSimboli();
						for (ii = msp.begin(); ii != msp.end(); ii++) {
							if (premesti) {
								ii++;
								premesti = false;
								if (ii == msp.end()) break;
								premesti = false;
							}



							if ((*ii)->jesteNota()) {
								if ((*ii)->GetSpliter()) {
									(*ii)->povrati();
									premesti = true;
								}
								string zapis = "";
								zapis += (*ii)->GetVisina();
								if ((*ii)->GetPovisena()) zapis += '#';
								zapis += (*ii)->GetOktava() + '0';
								midievent[0] = 0x90;
								midievent[1] = midi_mapa[zapis];
								outputfile.addEvent(0, actiontime, midievent);
								actiontime += tpq / 2 * ((*ii)->getT() == cetvrtina ? 2 : 1);

								midievent[0] = 0x80;
								outputfile.addEvent(0, actiontime, midievent);
							}
							if ((*ii)->jestePauza()) {
								actiontime += tpq / 2 * ((*ii)->getT() == cetvrtina ? 2 : 1);
							}
							if ((*ii)->jesteAkord()) {
								if ((*ii)->GetSpliter()) {
									(*ii)->povrati();
									premesti = true;
								}

								int at = actiontime;

								list<Nota>::iterator v;

								list<Nota> ak = ((Akord*)(*ii))->GetNote();
								for (v = ak.begin(); v != ak.end(); v++) {
									string zapis = "";
									zapis += v->GetVisina();
									if (v->GetPovisena()) zapis += '#';
									zapis += v->GetOktava() + '0';
									midievent[0] = 0x90;
									midievent[1] = midi_mapa[zapis];
									outputfile.addEvent(0, actiontime, midievent);
									//at += tpq / 2 * ((v)->getT() == cetvrtina ? 2 : 1);

								}
								actiontime = at + tpq;
								midievent[0] = 0x80;
								outputfile.addEvent(0, actiontime, midievent);
							}


						}
					}
					outputfile.sortTracks();
					outputfile.write(fajl);
				}
					break;
				case 2:
				{
					cout << "Unesite ime fajla u koji zelite da eksportujete format:\n";
					string imef;
					cin >> imef;
					ofstream os;
					LinijskiSistem levi = komp->getLinijskiSistem('l');
					LinijskiSistem desni = komp->getLinijskiSistem('d');
					//bool split = false;
					auto l = levi.GetTaktovi();
					auto d = desni.GetTaktovi();
					os.open(imef);

					os << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << endl;
					os << "<!DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 3.1 Partwise//EN\" \"http://www.musicxml.org/dtds/partwise.dtd\">" << endl;
					os << "<score-partwise version=\"3.1\">" << endl;
					os << "  <part-list>" << endl;
					os << "    <score-part id=\"Right\"></score-part>" << endl;
					os << "    <score-part id=\"Left\"></score-part>" << endl;
					os << "  </part-list>" << endl;
					os << "  <part id=\"Right\">" << endl;
					os << "   <measure>" << endl;
					os << "    <attributes>" << endl;
					os << "     <divisions>2</divisions>" << endl;

					list<Takt> pom = desni.GetTaktovi();
					list<Takt>::iterator ipom = pom.begin();

					os << ipom->GetT().TXML();
					os << "     <clef>" << endl;
					os << "      <sign>G</sign>" << endl;
					os << "      <line>2</line>" << endl;
					os << "     </clef>" << endl;
					os << "    </attributes>" << endl << endl;
					bool premesti = false;
					bool akord = false;
					for (auto rit = d.begin(); rit != d.end(); rit++) {
						if (rit != d.begin())
							os << "   <measure>" << endl;
						auto sim = (*rit).getSimboli();
						for (auto it = sim.begin(); it != sim.end(); it++) {
							if ((*it)->jesteAkord()) akord = true;
							if ((*it)->GetSpliter()) {
								if (premesti) premesti = false;
								else premesti = true;
							}
							os << (*it)->MSXML(premesti, akord);

						}
						os << "   </measure>" << endl;

					}
					os << "  <part id=\"Left\">" << endl;
					os << "  </part>" << endl;
					os << "   <measure>" << endl;
					os << "    <attributes>" << endl;
					os << "     <divisions>2</divisions>" << endl;

					pom = levi.GetTaktovi();
					ipom = pom.begin();

					os << ipom->GetT().TXML();
					os << "     <clef>" << endl;
					os << "      <sign>F</sign>" << endl;
					os << "      <line>4</line>" << endl;
					os << "     </clef>" << endl;
					os << "    </attributes>" << endl << endl;
					premesti = false; akord = false;
					for (auto rit = l.begin(); rit != l.end(); rit++) {
						if (rit != l.begin())
							os << "   <measure>" << endl;
						auto sim = (*rit).getSimboli();
						for (auto it = sim.begin(); it != sim.end(); it++) {
							if ((*it)->jesteAkord()) akord = true;
							if ((*it)->GetSpliter()) {
								if (premesti) premesti = false;
								else premesti = true;
							}
							os << (*it)->MSXML(premesti, akord);

						}
						os << "   </measure>" << endl;

					}
					os << "  </part>" << endl;
					os << "</score-partwise>" << endl;
					os.close();
				}
					break;
				case 3:
				{
					std::map<string, RGB> bmp_mapa;
					RGB pom;
					pom.blue = 0;
					pom.green = 0;
					pom.red = 255;
					bmp_mapa["C4"] = pom;
					pom.green = 127;
					bmp_mapa["C#4"] = pom;
					pom.green = 255;
					bmp_mapa["D4"] = pom;
					pom.red = 127;
					bmp_mapa["D#4"] = pom;
					pom.red = 0;
					bmp_mapa["E4"] = pom;
					pom.blue = 127;
					bmp_mapa["F4"] = pom;
					pom.blue = 255;
					bmp_mapa["F#4"] = pom;
					pom.green = 127;
					bmp_mapa["G4"] = pom;
					pom.green = 0;
					bmp_mapa["G#4"] = pom;
					pom.red = 127;
					bmp_mapa["A4"] = pom;
					pom.red = 255;
					bmp_mapa["A#4"] = pom;
					pom.blue = 127;
					bmp_mapa["B4"] = pom;

					for (int i = 2; i < 7; i++) {
						if (i == 4) continue;
						char cp = i + '0';
						RGB pom1;
						string pomocni = "";

						pom = bmp_mapa["C4"];
						switch (i) {
						case 2:
							pom1.blue = pom.blue - (pom.blue / 8) * 6;
							pom1.red = pom.red - (pom.red / 8 )* 6;
							pom1.green = pom.green - (pom.green / 8) * 6;
							break;
						case 3:
							pom1.blue = pom.blue - (pom.blue / 8) * 3;
							pom1.red = pom.red - (pom.red / 8) * 3;
							pom1.green = pom.green - (pom.green / 8) * 3;
							break;
						case 5:
							pom1.blue = pom.blue + ((255 - pom.blue) / 8) * 3;
							pom1.red = pom.red + ((255 - pom.red) / 8) * 3;
							pom1.green = pom.green + ((255 - pom.green) / 8) * 3;
							break;
						case 6:
							pom1.blue = pom.blue - (255 - pom.blue) / 8 * 6;
							pom1.red = pom.red - (255 - pom.red) / 8 * 6;
							pom1.green = pom.green - (255 - pom.green) / 8 * 6;
							break;
						}
						pomocni += 'C';
						pomocni += cp;
						bmp_mapa[pomocni] = pom1;
						pomocni = "";

						pom = bmp_mapa["C#4"];
						switch (i) {
						case 2:
							pom1.blue = pom.blue - (pom.blue / 8) * 6;
							pom1.red = pom.red - (pom.red / 8) * 6;
							pom1.green = pom.green - (pom.green / 8) * 6;
							break;
						case 3:
							pom1.blue = pom.blue - (pom.blue / 8) * 3;
							pom1.red = pom.red - (pom.red / 8) * 3;
							pom1.green = pom.green - (pom.green / 8) * 3;
							break;
						case 5:
							pom1.blue = pom.blue + ((255 - pom.blue) / 8) * 3;
							pom1.red = pom.red + ((255 - pom.red) / 8) * 3;
							pom1.green = pom.green + ((255 - pom.green) / 8) * 3;
							break;
						case 6:
							pom1.blue = pom.blue - (255 - pom.blue) / 8 * 6;
							pom1.red = pom.red - (255 - pom.red) / 8 * 6;
							pom1.green = pom.green - (255 - pom.green) / 8 * 6;
							break;
						}
						pomocni += 'C';
						pomocni += '#';
						pomocni += cp;
						bmp_mapa[pomocni] = pom1;
						pomocni = "";

						pom = bmp_mapa["D4"];
						switch (i) {
						case 2:
							pom1.blue = pom.blue - (pom.blue / 8) * 6;
							pom1.red = pom.red - (pom.red / 8) * 6;
							pom1.green = pom.green - (pom.green / 8) * 6;
							break;
						case 3:
							pom1.blue = pom.blue - (pom.blue / 8) * 3;
							pom1.red = pom.red - (pom.red / 8) * 3;
							pom1.green = pom.green - (pom.green / 8) * 3;
							break;
						case 5:
							pom1.blue = pom.blue + ((255 - pom.blue) / 8) * 3;
							pom1.red = pom.red + ((255 - pom.red) / 8) * 3;
							pom1.green = pom.green + ((255 - pom.green) / 8) * 3;
							break;
						case 6:
							pom1.blue = pom.blue - (255 - pom.blue) / 8 * 6;
							pom1.red = pom.red - (255 - pom.red) / 8 * 6;
							pom1.green = pom.green - (255 - pom.green) / 8 * 6;
							break;
						}
						pomocni += 'D';
						pomocni += cp;
						bmp_mapa[pomocni] = pom1;
						pomocni = "";

						pom = bmp_mapa["D#4"];
						switch (i) {
						case 2:
							pom1.blue = pom.blue - (pom.blue / 8) * 6;
							pom1.red = pom.red - (pom.red / 8) * 6;
							pom1.green = pom.green - (pom.green / 8) * 6;
							break;
						case 3:
							pom1.blue = pom.blue - (pom.blue / 8) * 3;
							pom1.red = pom.red - (pom.red / 8) * 3;
							pom1.green = pom.green - (pom.green / 8) * 3;
							break;
						case 5:
							pom1.blue = pom.blue + ((255 - pom.blue) / 8) * 3;
							pom1.red = pom.red + ((255 - pom.red) / 8) * 3;
							pom1.green = pom.green + ((255 - pom.green) / 8) * 3;
							break;
						case 6:
							pom1.blue = pom.blue - (255 - pom.blue) / 8 * 6;
							pom1.red = pom.red - (255 - pom.red) / 8 * 6;
							pom1.green = pom.green - (255 - pom.green) / 8 * 6;
							break;
						}
						pomocni += 'D';
						pomocni += '#';
						pomocni += cp;
						bmp_mapa[pomocni] = pom1;
						pomocni = "";

						pom = bmp_mapa["E4"];
						switch (i) {
						case 2:
							pom1.blue = pom.blue - (pom.blue / 8) * 6;
							pom1.red = pom.red - (pom.red / 8) * 6;
							pom1.green = pom.green - (pom.green / 8) * 6;
							break;
						case 3:
							pom1.blue = pom.blue - (pom.blue / 8) * 3;
							pom1.red = pom.red - (pom.red / 8) * 3;
							pom1.green = pom.green - (pom.green / 8) * 3;
							break;
						case 5:
							pom1.blue = pom.blue + ((255 - pom.blue) / 8) * 3;
							pom1.red = pom.red + ((255 - pom.red) / 8) * 3;
							pom1.green = pom.green + ((255 - pom.green) / 8) * 3;
							break;
						case 6:
							pom1.blue = pom.blue - (255 - pom.blue) / 8 * 6;
							pom1.red = pom.red - (255 - pom.red) / 8 * 6;
							pom1.green = pom.green - (255 - pom.green) / 8 * 6;
							break;
						}
						pomocni += 'E';
						pomocni += cp;
						bmp_mapa[pomocni] = pom1;
						pomocni = "";

						pom = bmp_mapa["F4"];
						switch (i) {
						case 2:
							pom1.blue = pom.blue - (pom.blue / 8) * 6;
							pom1.red = pom.red - (pom.red / 8) * 6;
							pom1.green = pom.green - (pom.green / 8) * 6;
							break;
						case 3:
							pom1.blue = pom.blue - (pom.blue / 8) * 3;
							pom1.red = pom.red - (pom.red / 8) * 3;
							pom1.green = pom.green - (pom.green / 8) * 3;
							break;
						case 5:
							pom1.blue = pom.blue + ((255 - pom.blue) / 8) * 3;
							pom1.red = pom.red + ((255 - pom.red) / 8) * 3;
							pom1.green = pom.green + ((255 - pom.green) / 8) * 3;
							break;
						case 6:
							pom1.blue = pom.blue - (255 - pom.blue) / 8 * 6;
							pom1.red = pom.red - (255 - pom.red) / 8 * 6;
							pom1.green = pom.green - (255 - pom.green) / 8 * 6;
							break;
						}
						pomocni += 'F';
						pomocni += cp;
						bmp_mapa[pomocni] = pom1;
						//bmp_mapa["F" + cp] = pom1;
						pomocni = "";

						pom = bmp_mapa["F#4"];
						switch (i) {
						case 2:
							pom1.blue = pom.blue - (pom.blue / 8) * 6;
							pom1.red = pom.red - (pom.red / 8) * 6;
							pom1.green = pom.green - (pom.green / 8) * 6;
							break;
						case 3:
							pom1.blue = pom.blue - (pom.blue / 8) * 3;
							pom1.red = pom.red - (pom.red / 8) * 3;
							pom1.green = pom.green - (pom.green / 8) * 3;
							break;
						case 5:
							pom1.blue = pom.blue + ((255 - pom.blue) / 8) * 3;
							pom1.red = pom.red + ((255 - pom.red) / 8) * 3;
							pom1.green = pom.green + ((255 - pom.green) / 8) * 3;
							break;
						case 6:
							pom1.blue = pom.blue - (255 - pom.blue) / 8 * 6;
							pom1.red = pom.red - (255 - pom.red) / 8 * 6;
							pom1.green = pom.green - (255 - pom.green) / 8 * 6;
							break;
						}
						pomocni += 'F';
						pomocni += '#';
						pomocni += cp;
						bmp_mapa[pomocni] = pom1;
						//bmp_mapa["F#" + cp] = pom1;
						pomocni = "";

						pom = bmp_mapa["G4"];
						switch (i) {
						case 2:
							pom1.blue = pom.blue - (pom.blue / 8) * 6;
							pom1.red = pom.red - (pom.red / 8) * 6;
							pom1.green = pom.green - (pom.green / 8) * 6;
							break;
						case 3:
							pom1.blue = pom.blue - (pom.blue / 8) * 3;
							pom1.red = pom.red - (pom.red / 8) * 3;
							pom1.green = pom.green - (pom.green / 8) * 3;
							break;
						case 5:
							pom1.blue = pom.blue + ((255 - pom.blue) / 8) * 3;
							pom1.red = pom.red + ((255 - pom.red) / 8) * 3;
							pom1.green = pom.green + ((255 - pom.green) / 8) * 3;
							break;
						case 6:
							pom1.blue = pom.blue - (255 - pom.blue) / 8 * 6;
							pom1.red = pom.red - (255 - pom.red) / 8 * 6;
							pom1.green = pom.green - (255 - pom.green) / 8 * 6;
							break;
						}
						pomocni += 'G';
						pomocni += cp;
						bmp_mapa[pomocni] = pom1;
						//bmp_mapa["G" + cp] = pom1;
						pomocni = "";

						pom = bmp_mapa["G#4"];
						switch (i) {
						case 2:
							pom1.blue = pom.blue - (pom.blue / 8) * 6;
							pom1.red = pom.red - (pom.red / 8) * 6;
							pom1.green = pom.green - (pom.green / 8) * 6;
							break;
						case 3:
							pom1.blue = pom.blue - (pom.blue / 8) * 3;
							pom1.red = pom.red - (pom.red / 8) * 3;
							pom1.green = pom.green - (pom.green / 8) * 3;
							break;
						case 5:
							pom1.blue = pom.blue + ((255 - pom.blue) / 8) * 3;
							pom1.red = pom.red + ((255 - pom.red) / 8) * 3;
							pom1.green = pom.green + ((255 - pom.green) / 8) * 3;
							break;
						case 6:
							pom1.blue = pom.blue - (255 - pom.blue) / 8 * 6;
							pom1.red = pom.red - (255 - pom.red) / 8 * 6;
							pom1.green = pom.green - (255 - pom.green) / 8 * 6;
							break;
						}
						pomocni += 'G';
						pomocni += '#';
						pomocni += cp;
						bmp_mapa[pomocni] = pom1;
						//bmp_mapa["G#" + cp] = pom1;
						pomocni = "";

						pom = bmp_mapa["A4"];
						switch (i) {
						case 2:
							pom1.blue = pom.blue - (pom.blue / 8) * 6;
							pom1.red = pom.red - (pom.red / 8) * 6;
							pom1.green = pom.green - (pom.green / 8) * 6;
							break;
						case 3:
							pom1.blue = pom.blue - (pom.blue / 8) * 3;
							pom1.red = pom.red - (pom.red / 8) * 3;
							pom1.green = pom.green - (pom.green / 8) * 3;
							break;
						case 5:
							pom1.blue = pom.blue + ((255 - pom.blue) / 8) * 3;
							pom1.red = pom.red + ((255 - pom.red) / 8) * 3;
							pom1.green = pom.green + ((255 - pom.green) / 8) * 3;
							break;
						case 6:
							pom1.blue = pom.blue - (255 - pom.blue) / 8 * 6;
							pom1.red = pom.red - (255 - pom.red) / 8 * 6;
							pom1.green = pom.green - (255 - pom.green) / 8 * 6;
							break;
						}
						pomocni += 'A';
						pomocni += cp;
						bmp_mapa[pomocni] = pom1;
						//bmp_mapa["A" + cp] = pom1;
						pomocni = "";

						pom = bmp_mapa["A#4"];
						switch (i) {
						case 2:
							pom1.blue = pom.blue - (pom.blue / 8) * 6;
							pom1.red = pom.red - (pom.red / 8) * 6;
							pom1.green = pom.green - (pom.green / 8) * 6;
							break;
						case 3:
							pom1.blue = pom.blue - (pom.blue / 8) * 3;
							pom1.red = pom.red - (pom.red / 8) * 3;
							pom1.green = pom.green - (pom.green / 8) * 3;
							break;
						case 5:
							pom1.blue = pom.blue + ((255 - pom.blue) / 8) * 3;
							pom1.red = pom.red + ((255 - pom.red) / 8) * 3;
							pom1.green = pom.green + ((255 - pom.green) / 8) * 3;
							break;
						case 6:
							pom1.blue = pom.blue - (255 - pom.blue) / 8 * 6;
							pom1.red = pom.red - (255 - pom.red) / 8 * 6;
							pom1.green = pom.green - (255 - pom.green) / 8 * 6;
							break;
						}
						pomocni += 'A';
						pomocni += '#';
						pomocni += cp;
						bmp_mapa[pomocni] = pom1;
						//bmp_mapa["A#" + cp] = pom1;
						pomocni = "";

						pom = bmp_mapa["B4"];
						switch (i) {
						case 2:
							pom1.blue = pom.blue - (pom.blue / 8) * 6;
							pom1.red = pom.red - (pom.red / 8) * 6;
							pom1.green = pom.green - (pom.green / 8) * 6;
							break;
						case 3:
							pom1.blue = pom.blue - (pom.blue / 8) * 3;
							pom1.red = pom.red - (pom.red / 8) * 3;
							pom1.green = pom.green - (pom.green / 8) * 3;
							break;
						case 5:
							pom1.blue = pom.blue + ((255 - pom.blue) / 8) * 3;
							pom1.red = pom.red + ((255 - pom.red) / 8) * 3;
							pom1.green = pom.green + ((255 - pom.green) / 8) * 3;
							break;
						case 6:
							pom1.blue = pom.blue - (255 - pom.blue) / 8 * 6;
							pom1.red = pom.red - (255 - pom.red) / 8 * 6;
							pom1.green = pom.green - (255 - pom.green) / 8 * 6;
							break;
						}
						pomocni += 'B';
						pomocni += cp;
						bmp_mapa[pomocni] = pom1;
						//bmp_mapa["B" + cp] = pom1;
						pomocni = "";
					}
					cout << "Unesite ime fajla u koji zelite da eksportujete kompoziciju:\n";
					string fajl;
					cin >> fajl;
					int sirina;
					cout << "Unesite broj piksela:\n";
					cin >> sirina;
					int visina;
					
					ofstream f(fajl);
					list<Takt>::iterator it;
					list<Takt>::iterator ir;

					LinijskiSistem ls = komp->getLinijskiSistem('l');
					LinijskiSistem ds = komp->getLinijskiSistem('d');

					
					list<Takt> trenl = ls.GetTaktovi();
					list<Takt> trend = ds.GetTaktovi();
					Trajanje traj = trenl.begin()->GetT();
					int broj = trend.size()*traj.GetT() * 8;
					visina = (int)ceil((double)broj / sirina);
					int vel_red = (((24 * sirina) + 31) / 32)*4;
					int dopuna = vel_red-3*sirina;
					RGB ** matrica = new RGB*[visina];
					for (int i = 0; i < visina; i++) {
						matrica[i] = new RGB[sirina];
					}
					int i = visina - 1;
					int j = 0;
					for (it = trenl.begin(), ir = trend.begin(); it != trenl.end(); it++, ir++) {
						list<MuzickiSimbol*>::iterator il;
						list<MuzickiSimbol*>::iterator id;

						list<MuzickiSimbol*> dt = (*it).getSimboli();
						list<MuzickiSimbol*> lt = (*ir).getSimboli();
						for (il = lt.begin(), id = dt.begin(); il != lt.end(); il++, id++) {
							if ((*il)->jesteNota() && (*id)->jestePauza()) {
								string str = "";
								str += (*il)->GetVisina();
								if ((*il)->GetPovisena()) str += "#";
								str += (*il)->GetOktava() + '0';
								RGB polje = bmp_mapa[str];
								if ((*il)->getT() == cetvrtina) {
									matrica[i][j++] = polje;
									if (j == sirina) {
										j = 0;
										i--;
									}
								}
								else
								{
									matrica[i][j++] = polje;
								}
							}
							if ((*il)->jestePauza() && (*id)->jesteNota()) {
								string str = "";
								str += (*id)->GetVisina();
								if ((*id)->GetPovisena()) str += "#";
								str += (*id)->GetOktava() + '0';
								RGB polje = bmp_mapa[str];
								if ((*il)->getT() == cetvrtina) {
									matrica[i][j++] = polje;
									if (j == sirina) {
										j = 0;
										i--;
									}
								}
								else
								{
									matrica[i][j++] = polje;
								}
							}
							if ((*il)->jesteAkord() || (*id)->jesteAkord()) {
								list<Nota> notee = ((Akord*)(*il))->GetNote();
								string str = "";
								str += notee.front().GetVisina();
								if (notee.front().GetPovisena()) str += "#";
								str += notee.front().GetOktava() + '0';
								RGB polje = bmp_mapa[str];
								if (notee.front().getT() == cetvrtina) {
									matrica[i][j++] = polje;
									if (j == sirina) {
										j = 0;
										i--;
									}
								}
								else
								{
									matrica[i][j++] = polje;
								}

							}
							if ((*il)->jestePauza() && (*id)->jestePauza()) {
								Trajanje d = (*il)->getT();
								RGB polje;
								polje.blue = 255;
								polje.green = 255;
								polje.red = 255;
								if (d == cetvrtina) { matrica[i][j++] = polje;
									if (j == sirina) {
										j = 0;
										i--;
									}
								}
								else { matrica[i][j++] = polje; }
							}
							if (j == sirina) {
								j = 0;
								i--;
							}

						}
						

					}
					while (i >= 0) {
							RGB polje;
							polje.blue = 255;
							polje.green = 255;
							polje.red = 255;
							matrica[i][j++] = polje;
							if (j == sirina) {
								j = 0;
								i--;
							}





					}
					
					int velicina = vel_red * visina+54;

					f << (unsigned char)'B';
					f << (unsigned char)'M';
					f << (unsigned char)velicina;
					f << (unsigned char)(velicina >> 8);
					f << (unsigned char)(velicina >> 16);
					f << (unsigned char)(velicina >> 24);
					for (int i = 0; i < 4; i++) f << (unsigned char)0;
					f << (unsigned char)54;
					for (int i = 0; i < 3; i++) f << (unsigned char)0;
					f << (unsigned char)40;
					for (int i = 0; i < 3; i++) f << (unsigned char)0;
					f << (unsigned char)sirina;
					f << (unsigned char)(sirina >> 8);
					f << (unsigned char)(sirina >> 16);
					f << (unsigned char)(sirina >> 24);
					f << (unsigned char)visina;
					f << (unsigned char)(visina >> 8);
					f << (unsigned char)(visina >> 16);
					f << (unsigned char)(visina >> 24);
					f << (unsigned char)1;
					f << (unsigned char)0;
					f << (unsigned char)24;
					for (int i = 0; i < 5; i++) {
						f << (unsigned char)0;
					}
					f << (unsigned char)16;
					for (int i = 0; i < 3; i++) {
						f << (unsigned char)0;
					}
					f <<(unsigned char) 0x13;
					f << (unsigned char)0x0B;
					f << (unsigned char)0;
					f << (unsigned char)0;
					f << (unsigned char)0x13;
					f << (unsigned char)0x0B;
					f << (unsigned char)0;
					f << (unsigned char)0;
					for (int i = 0; i < 8; i++) {
						f << (unsigned char)0;
					}

					for (int i = 0; i < visina; i++) {
						for (int j = 0; j < sirina; j++) {
							f << (unsigned char)matrica[i][j].red;
							f << (unsigned char)matrica[i][j].green;
							f << (unsigned char)matrica[i][j].blue;
						}
						for (int j = 0; j < dopuna; j++) {
							f << (unsigned char)0;
						}
					}
					f.close();
					


				}	
				break;
				}
			}
			break;
			case 7:
			{
				if (!eksportovano) {
					cout << "Niste eksportovali kompoziciju!";
					cout << "Unesite 1 ako zelite da se predomislite:"; int op = 0; cin >> op;
					if (op == 1) break;
				}
					q = 0;
			}
				break;
			}

		}
	}
	catch (PogresanImenilac& p) {
		cout << p;
	}
	catch (GreskaUTaktu &g) {
		cout << g;
	}

}