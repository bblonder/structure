#ifndef _ATOM_H_
#define _ATOM_H_

#include <string>
#include "Point4.h"

class Atom
{
	public:
		Point4 pos;
		std::string name;
		std::string name4;
		std::string resName;
		int serial;
		std::string chainID;
		int resSeq;
		
		bool helix, sheet, hetatm;
};

bool compareAtomsBySerial(const Atom* a1, const Atom* a2);
bool compareAtomsByPosX(const Atom* a1, const Atom* a2);
bool compareAtomsByResSeq(const Atom* a1, const Atom* a2);

#endif