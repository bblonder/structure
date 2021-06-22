#ifndef _SECONDARY_STRUCTURE_H
#define _SECONDARY_STRUCTURE_H

#include <string>

class SecondaryStructure
{
	public:
		std::string initChainID, endChainID;
		int initSeqNum, endSeqNum;
		bool helix, sheet;
};

#endif