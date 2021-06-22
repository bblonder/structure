#ifndef _BOND_H_
#define _BOND_H_

#include "Point4.h"
#include "Atom.h"

class Bond
{
	public:
		Bond(Atom *a1, Atom *b1);
		
		Atom *atomStart, *atomEnd;
		
		static float bondDistance(std::string a1, std::string a2);
		static float bondDistance(std::string atomName);
};



#endif