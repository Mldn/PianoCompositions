#pragma once
#include <iostream>

using namespace std;

class GreskaUTaktu
{
public:

	GreskaUTaktu() {}

	friend ostream& operator << (ostream & os, const GreskaUTaktu &g) {
		os << "Pogresan takt za akord/linijski sistem";
			return os;
	}
};

