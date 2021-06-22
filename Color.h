#ifndef _COLOR_H_
#define _COLOR_H_

#include <string>
#include <map>

#include "Atom.h"

class Color
{
	public:
		Point4 getColor(Atom *a);
		Point4 getChainColor(std::string chainID);
		Point4 getCPKColor(std::string name);

		void initializeChainColors(std::map<std::string, std::string> compounds);
		
	private:
		//void initializeColorMap();
		
		std::map<std::string,Point4> ColorMap;
		std::map<std::string, Point4> ChainColorMap;
};

#endif