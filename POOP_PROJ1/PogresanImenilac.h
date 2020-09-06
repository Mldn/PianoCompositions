#pragma once
#include <iostream>

using namespace std;
class PogresanImenilac: public exception
{
public:

	friend ostream& operator << (ostream & os , const PogresanImenilac &p) {
		os << "Imenilac je pogresno unet za trajanje takta, mora biti 8 ili  4 !";
			return os;
	}
};

