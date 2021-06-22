#ifndef _MOLECULE_H_
#define _MOLECULE_H

#include <vector>
#include <map>
#include <string>
#include <OpenGL/GL.h>
#include <OpenGL/glu.h>

#include "Atom.h"
#include "Bond.h"
#include "Color.h"
#include "SecondaryStructure.h"

class Molecule
{
	public:
		Molecule(const char* filename);
		~Molecule();
		
		void render(int mode);
		
		// the final information
		std::string idCode;
		std::string title;
		std::string classification;
		std::string experiment;
		std::string resolution;
		std::string authors;
		std::string date;
		std::string organism;
		
		std::map<std::string, std::string> compounds;
		std::map<std::string, std::string> sequenceInfo;
		
		
		Color colorInfo;
		
	private:
		void readFile(std::string filename);
		void parseFile(std::vector<std::string>* fileLines);
		void findBonds();
		void processSecondaryStructure();
		void processAnnotation();
		void generateCallLists();
		Atom* findAtomBySerial(unsigned int serial);

		std::vector<Atom*> atomList;
		std::vector<Bond*> bondList;
		std::vector<SecondaryStructure*> ssList;
		
		GLuint moleculeCallList;
		GLuint cAlphaCallList;
		GLuint spaceFillCallList;
		GLuint sphereList;
		
		Point4 pcenter;
		float size;
		
		// temporary variables
		std::string compoundInfo;
		std::string sourceInfo;
		
		
		
};

#endif