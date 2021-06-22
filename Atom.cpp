#include "Atom.h"

bool compareAtomsByPosX(const Atom* a1, const Atom* a2)
{
	return a1->pos.x < a2->pos.x;
}

bool compareAtomsBySerial(const Atom* a1, const Atom* a2)
{
	return a1->serial < a2->serial;
}

bool compareAtomsByResSeq(const Atom* a1, const Atom* a2)
{
	return a1->resSeq < a2->resSeq;
}