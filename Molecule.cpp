#include <fstream>
#include <iostream>

#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>

#include <algorithm>

#include "Molecule.h"
#include <GLUT/glut.h>
#include "StringHelper.h"

Molecule::Molecule(const char* filename)
{
	readFile(filename);
}

Molecule::~Molecule()
{
	for (std::vector<Atom*>::iterator it = atomList.begin(); it != atomList.end(); ++it)
	{
		delete *it;
	}
	
	for (std::vector<Bond*>::iterator it = bondList.begin(); it != bondList.end(); ++it)
	{
		delete *it;
	}
	
	for (std::vector<SecondaryStructure*>::iterator it = ssList.begin(); it != ssList.end(); ++it)
	{
		delete *it;
	}
	
	glDeleteLists (moleculeCallList, 1);
	glDeleteLists (cAlphaCallList, 1);
}

Atom* Molecule::findAtomBySerial(unsigned int serial)
{
	for (int i=0; i<atomList.size(); i++)
	{
		Atom *tempAtom = atomList[i];
		if (tempAtom->serial == serial)
		{
			return tempAtom;
		}
	}
	return NULL;
}

void Molecule::parseFile(std::vector<std::string>* fileLines)
{
	for (int i=0; i<fileLines->size(); i++)
	{
		std::string text = fileLines->at(i);

		if (text.find("ENDMDL") == 0) // only use the first model
		{
			break;
		}
		else if ((text.find("ATOM  ") == 0) || (text.find("HETATM") == 0))
		{
			float x, y, z;
			std::string name;
			std::string name4;
			std::string resName;
			int serial;
			std::string chainID;
			int resSeq;
			bool hetatm;
			
			hetatm = (text.find("HETATM") == 0);

			x = getFloat(text, 31, 38);
			y = getFloat(text, 39, 46);
			z = getFloat(text, 47, 54);
			
			name = getString(text, 77, 78);
			if (name == "  ") name = getString(text, 13, 14);
			
			name4  = getString(text, 13, 16);
			resName = getString(text, 18, 20);
			serial = getInt(text, 7, 11);
			chainID = getString(text, 22, 22);
			resSeq = getInt(text, 23, 26);
			
			Atom *a = new Atom();
			a->serial = serial;
			a->pos = Point4(x,y,z);
			a->chainID = chainID;
			a->name = name;
			a->name4 = name4;
			a->resName = resName;
			a->resSeq = resSeq;
			a->hetatm = hetatm;
			
			if (!(resName == "HOH")) // ditch the waters
			{
				atomList.push_back(a);
			}
		}
		else if (text.find("CONECT") == 0)
		{
			int connect0 = getInt(text, 7, 11);
			int connect1 = getInt(text, 12, 16);
			int connect2 = getInt(text, 17, 21);
			int connect3 = getInt(text, 22, 26);
			int connect4 = getInt(text, 27, 31);

			Atom *aStart, *aEnd;
			
			if (connect0 > 0)
			{
				aStart = findAtomBySerial(connect0);

				if (connect1 > 0)
				{
					aEnd = findAtomBySerial(connect1);
					if (aStart != NULL && aEnd != NULL)
					{
						bondList.push_back(new Bond(aStart, aEnd));
					}
					else
					{
						//std::cout << connect0 << " " << connect1 << "eep1" << std::endl;
					}
				}
				if (connect2 > 0)
				{
					aEnd = findAtomBySerial(connect2);
					if (aStart != NULL && aEnd != NULL)
					{
						bondList.push_back(new Bond(aStart, aEnd));
					}
					else
					{
						//std::cout << "eep2" << std::endl;
					}
				}
				if (connect3 > 0)
				{
					aEnd = findAtomBySerial(connect3);
					if (aStart != NULL && aEnd != NULL)
					{
						bondList.push_back(new Bond(aStart, aEnd));
					}
					else
					{
						//std::cout << "eep3" << std::endl;
					}
				}
				if (connect4 > 0)
				{
					aEnd = findAtomBySerial(connect4);
					if (aStart != NULL && aEnd != NULL)
					{
						bondList.push_back(new Bond(aStart, aEnd));
					}
					else
					{
						//std::cout << "eep4" << std::endl;
					}
				}
			}
		}
		else if (text.find("HELIX ") == 0)
		{
			std::string initChainID, endChainID;
			int initSeqNum, endSeqNum;
			
			initChainID = getString(text, 20, 20);
			initSeqNum = getInt(text, 22, 25);
			endChainID = getString(text, 32, 32);
			endSeqNum = getInt(text, 34, 37);
			
			SecondaryStructure *ss = new SecondaryStructure();
			ss->initChainID = initChainID;
			ss->endChainID = endChainID;
			ss->initSeqNum = initSeqNum;
			ss->endSeqNum = endSeqNum;
			ss->helix = true;
			
			ssList.push_back(ss);
		}
		else if (text.find("SHEET ") == 0)
		{
			std::string initChainID, endChainID;
			int initSeqNum, endSeqNum;
			
			initChainID = getString(text, 22, 22);
			initSeqNum = getInt(text, 23, 26);
			endChainID = getString(text, 33, 33);
			endSeqNum = getInt(text, 34, 37);
			
			SecondaryStructure *ss = new SecondaryStructure();
			ss->initChainID = initChainID;
			ss->endChainID = endChainID;
			ss->initSeqNum = initSeqNum;
			ss->endSeqNum = endSeqNum;
			ss->sheet = true;
			

			ssList.push_back(ss);
		}
		
		// now all the annotation stuff
		else if (text.find("HEADER") == 0)
		{
			classification = getString(text, 11, 50);
			idCode = getString(text, 63, 66); 
			date = getString(text, 51, 59);
		}
		else if (text.find("TITLE ") == 0)
		{
			title += getString(text, 11, 70);
		}
		else if (text.find("COMPND") == 0)
		{
			compoundInfo += getString(text, 11, 70);
		}
		else if (text.find("SOURCE") == 0)
		{
			sourceInfo += getString(text, 11, 70);
		}
		else if (text.find("EXPDTA") == 0)
		{
			experiment += getString(text, 11, 70);
		}
		else if (text.find("REMARK") == 0)
		{
			std::string candidate = "RESOLUTION. ";
			int pos = std::string(text).find(candidate);
			if (pos == 11)
			{
				resolution += getString(text, 12 + candidate.size(), 70);
			}
		}
		else if (text.find("AUTHOR") == 0)
		{
			authors += getString(text, 11, 70);
		}
		else if (text.find("SEQRES") == 0)
		{
			std::string chainID = getString(text, 12, 12);
			sequenceInfo[chainID] += getString(text, 20, 70);
			sequenceInfo[chainID] += " ";
		}
		else if (text.find("HETNAM") == 0)
		{
			std::string key = getString(text, 12, 14);
			compounds[key] = getString(text, 16, 70);
			compounds[key] += " ";
		}
		else
		{
			//printf("Rejecting line '%s'\n", text.c_str());
		}
	}
}

void Molecule::processAnnotation()
{
	// remove extra spaces
	classification = removeWhitespace(classification, " ", " ");
	idCode = removeWhitespace(idCode, " ", " ");
	title = removeWhitespace(title, " ", " ");
	compoundInfo = removeWhitespace(compoundInfo, " ", " ");
	sourceInfo = removeWhitespace(sourceInfo, " ", " ");
	experiment = removeWhitespace(experiment, " ", " ");
	resolution = removeWhitespace(resolution, " ", " ");
	authors = removeWhitespace(authors, ", ", " ");
	date = removeWhitespace(date, " ", " ");
	
	for (std::map<std::string,std::string>::iterator it = sequenceInfo.begin(); it != sequenceInfo.end(); ++it)
	{
		it->second = removeWhitespace(it->second, " ", " ");
	}
	for (std::map<std::string,std::string>::iterator it = compounds.begin(); it != compounds.end(); ++it)
	{
		it->second = removeWhitespace(it->second, " ", " ");
	}
	
	// fix up the date
	if (date.size() >= 8)
	{
		std::string day = date.substr(0,2);
		std::string month = date.substr(3,3);
		std::string year = date.substr(7,2);
		
		if (month == "JAN") month = "January";
		else if (month == "FEB") month = "February";
		else if (month == "MAR") month = "March";
		else if (month == "APR") month = "April";
		else if (month == "MAY") month = "May";
		else if (month == "JUN") month = "June";
		else if (month == "JUL") month = "July";
		else if (month == "AUG") month = "August";
		else if (month == "SEP") month = "September";
		else if (month == "OCT") month = "October";
		else if (month == "NOV") month = "November";
		else if (month == "DEC") month = "December";
		
		if (day[0] == '0') day = day.substr(1,1);
		
		date = day + " " + month;

		if (atoi(year.c_str()) < 50) // so this breaks in 2050
		{
			date +=" 20" + year; 
		}
		else
		{
			date +=" 19" + year; 
		}
	}
	
	// clean up text more
	if (resolution.size() > 0)
	{
		if (resolution[resolution.size() - 1] == '.')
		{
			resolution = resolution.substr(0, resolution.size() - 1);
		}
	}
	
	// make a list of the names of each chain
	std::string strMolID, strChain, strMolecule;
	std::map<std::string, std::string> tempComponentInfo;
	std::vector<std::string> compoundTokens = tokenize(compoundInfo, ";");
	for (int i=0; i<compoundTokens.size(); i++)
	{
		// process the section	
		std::string strMolIDTemp = extractValue(compoundTokens.at(i), "MOL_ID: ");
		std::string strMoleculeTemp = extractValue(compoundTokens.at(i), "MOLECULE: ");
		std::string strChainTemp = extractValue(compoundTokens.at(i), "CHAIN: ");
		
		if (strMolIDTemp != "")
		{
			strMolID = strMolIDTemp;
		}
		if (strMoleculeTemp != "")
		{
			strMolecule = strMoleculeTemp;
		}
		if (strChainTemp != "")
		{
			strChain = strChainTemp;
		}
		
		if ((strMolID != "") && (strMolecule != "") && (strChain != ""))
		{
			tempComponentInfo[strChain] = strMolecule;
			
			strMolecule = "";
			strChain = "";
			strMolID = "";
		}	
	}
	
	for (std::map<std::string, std::string>::iterator it = tempComponentInfo.begin(); it != tempComponentInfo.end(); ++it)
	{		
		std::string newtext = (*it).first;
		std::vector<std::string> tempTokens = tokenize(newtext, ", ");
		for (int i=0; i<tempTokens.size(); i++)
		{
			compounds[tempTokens.at(i)] = (*it).second;
		}
	}
	
	// make a list of the organisms used
	std::vector<std::string> sourceTokens = tokenize(sourceInfo, ";");	
	std::string strOrgSci;
	std::map<std::string, std::string> soi;
	
	for (int i=0; i<sourceTokens.size(); i++)
	{
		// process the section		
		std::string strMolIDTemp = extractValue(sourceTokens.at(i), "MOL_ID: ");
		std::string strOrgSciTemp = extractValue(sourceTokens.at(i), "ORGANISM_SCIENTIFIC: ");
		
		
		if (strMolIDTemp != "")
		{
			strMolID = strMolIDTemp;
		}
		if (strOrgSciTemp != "")
		{
			strOrgSci = strOrgSciTemp;
		}
		
		if ((strMolID != "") && (strOrgSci != ""))
		{
			soi[strOrgSci] += "";
			
			strOrgSci = "";
			strMolID = "";
		}
	}
	
	std::string delim = ", ";
	for (std::map<std::string, std::string>::iterator it = soi.begin(); it != soi.end(); ++it)
	{
		if (it != soi.begin())
		{
			organism += delim;
		}
		organism += (*it).first;
	}

}

void Molecule::processSecondaryStructure()
{
	std::sort(atomList.begin(), atomList.end(), compareAtomsBySerial);

	for (int i=0; i<ssList.size(); i++)
	{
		SecondaryStructure *ss = ssList[i];
		for (int j=0; j<atomList.size(); j++)
		{
			bool withinChainRange = (atomList[j]->chainID == ss->initChainID) && (atomList[j]->chainID == ss->endChainID);
			bool withinResSeqRange = (atomList[j]->resSeq >= ss->initSeqNum) && (atomList[j]->resSeq <= ss->endSeqNum);
			if (withinChainRange && withinResSeqRange)
			{
				atomList[j]->helix = ss->helix;
				atomList[j]->sheet = ss->sheet;
			}
		}
	}
}

void Molecule::findBonds()
{
	std::sort(atomList.begin(), atomList.end(), compareAtomsByPosX);
	
	int startIndex = 0;
	
	for (int i=0; i<atomList.size(); i++)
	{
		Atom *ai = atomList[i];
		float currentPosX = ai->pos.x;
		for (int j=startIndex; j<atomList.size(); j++)
		{
			Atom *aj = atomList[j];
			if (aj->pos.x < currentPosX - 1.9)
			{
				startIndex = j;
			}
			else if (aj->pos.x > currentPosX)
			{
				break;
			}
			else
			{
				float calculatedDistance2 = (ai->pos - aj->pos).mag2();
				float proposedDistance = Bond::bondDistance(ai->name, aj->name);
				
				bool a1h = (ai->name == " H");
				bool a2h = (aj->name == " H");
				
				if ((a1h && !a2h) || (!a1h && a2h)) proposedDistance = 1.2;
				if (!(a1h && a2h))
				{				
					if (calculatedDistance2 < (proposedDistance*proposedDistance) && calculatedDistance2 > (0.4*0.4))
					{
						// add a bond				
						bondList.push_back(new Bond(ai, aj));
					}
				}
			}
		}
	}

	// get everyone back into order
	std::sort(atomList.begin(), atomList.end(), compareAtomsBySerial);
}



void Molecule::readFile(std::string filename)
{
	std::vector<std::string> textLines;
	std::string tempString;
	
	std::ifstream ifs;
	ifs.open(filename.c_str(),  std::ifstream::in);
	while(getline(ifs,tempString))
	{
		textLines.push_back(tempString);
	}
	ifs.close();
	
	// get the content out
	parseFile(&textLines);
	
	// parse multiline annotation
	processAnnotation();
	
	// write in secondary structure information to atoms
	processSecondaryStructure();

	// generate bonds
	findBonds();
	
	// determine bounding box of molecule
	Point4 mean;
	for (int i=0; i<atomList.size(); i++) 
	{
		mean = mean + atomList.at(i)->pos;
	}
	mean = mean / atomList.size();
	float stdev = 0;
	for (int i=0; i<atomList.size(); i++) 
	{
		stdev += (atomList.at(i)->pos - mean).mag2();
	}
	stdev = sqrt(stdev/atomList.size());
	
	pcenter = mean;
	size = 5*stdev;

	generateCallLists();

}

void Molecule::generateCallLists()
{
	// move all the hetatms to the end

	colorInfo.initializeChainColors(compounds);

	moleculeCallList = glGenLists(1);
	glNewList(moleculeCallList, GL_COMPILE);
	
	std::vector<Atom*> hetAtmList;

	glPointSize(2.0);
	glDisable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	for (std::vector<Atom*>::iterator it = atomList.begin(); it!=atomList.end(); ++it) 
	{
		Point4 p = (*it)->pos;
		Point4 c = colorInfo.getColor(*it);
		
		if ((*it)->hetatm)
		{
			hetAtmList.push_back(*it);
		}
		else
		{
			glColor4f(c.x, c.y, c.z, c.a);
			glVertex3f(p.x, p.y, p.z);
		}
	}
	glEnd();
	
	//we have to split it up so the callList is efficient...
	glPointSize(4.0);
	glBegin(GL_POINTS);
	for (int i=0; i<hetAtmList.size(); i++)
	{
		Atom *ta = hetAtmList[i];
		Point4 c = colorInfo.getColor(ta);
		Point4 p = ta->pos;
		
		bool isIon = (compounds[ta->resName].find(" ION") != std::string::npos);
		
		if (isIon)
		{
			glEnd();
			glPointSize(8.0);
			glBegin(GL_POINTS);
		}

		glColor4f(c.x, c.y, c.z, c.a);
		glVertex3f(p.x, p.y, p.z);
		
		if (isIon)
		{
			glEnd();
			glPointSize(4.0);
			glBegin(GL_POINTS);
		}
	}
	glEnd();
	glPointSize(1.0);

	
	glLineWidth(2.0);
	glBegin(GL_LINES);
	for (std::vector<Bond*>::iterator it = bondList.begin(); it!=bondList.end(); ++it) 
	{
		Atom *a1 = (*it)->atomStart;
		Atom *a2 = (*it)->atomEnd;
		
		if ((a1->hetatm && a2->hetatm) || (!a1->hetatm && !a2->hetatm))
		{			
			Point4 c1 = colorInfo.getColor(a1);
			Point4 c2 = colorInfo.getColor(a2);
			
			// find the bond midPoint4
			Point4 p1 = a1->pos;
			Point4 p2 = a2->pos;
			
			float r1 = Bond::bondDistance(a1->name);
			float r2 = Bond::bondDistance(a2->name);
			Point4 pm = p1 + r1/(r1+r2)*(p2-p1);
			
			// draw!
			glColor4f(c1.x, c1.y, c1.z, c1.a);
			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(pm.x, pm.y, pm.z);
			
			glColor4f(c2.x, c2.y, c2.z, c2.a);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(pm.x, pm.y, pm.z);
		}
	}
	glEnd();
	glLineWidth(1.0);
	
	glEndList();
	
	// now do the second one
	
	cAlphaCallList = glGenLists(1);
	glNewList(cAlphaCallList, GL_COMPILE);
	
	std::map<std::string, std::vector<Atom*> > chainMap;
	glPointSize(4.0);
	glBegin(GL_POINTS);
	for (std::vector<Atom*>::iterator it = atomList.begin(); it!=atomList.end(); ++it) 
	{
		if (((*it)->name4 == " CA ") || ((*it)->name4 == " C1 ") || ((*it)->name4 == " C1'"))
		{
			chainMap[(*it)->chainID].push_back(*it);
		}
		
		else if ((*it)->hetatm)
		{
			Point4 p = (*it)->pos;
			Point4 c = colorInfo.getColor(*it);
			bool isIon = (compounds[(*it)->resName].find(" ION") != std::string::npos);
			
			if (isIon)
			{
				glEnd();
				glPointSize(8.0);
				glBegin(GL_POINTS);
			}

			glColor4f(c.x, c.y, c.z, c.a);
			glVertex3f(p.x, p.y, p.z);
			
			if (isIon)
			{
				glEnd();
				glPointSize(4.0);
				glBegin(GL_POINTS);
			}
		}
	}
	glEnd();
	glPointSize(1.0);
	
	glLineWidth(2.0);
	for (std::map<std::string, std::vector<Atom*> >::iterator it = chainMap.begin(); it != chainMap.end(); ++it)
	{
		std::vector<Atom*> caList = (*it).second;
		std::sort(caList.begin(), caList.end(), compareAtomsByResSeq);
		
		Point4 cc = colorInfo.getChainColor((*it).first);
		glColor4f(cc.x, cc.y, cc.z, 1.0);
		
		glBegin(GL_LINES);
		Point4 lastPos;
		for (int i=0; i<caList.size(); i++) 
		{
			Point4 caPos = caList[i]->pos;
			
			if (i != 0)
			{
				//if ((caList[i]->resSeq - caList[i-1]->resSeq) == 1)
				if ((caPos - lastPos).mag() < 10)
				{
					glVertex3f(lastPos.x, lastPos.y, lastPos.z);
					glVertex3f(caPos.x, caPos.y, caPos.z);
				}
			}
			
			lastPos = caPos;
		}
		glEnd();
	}
	glLineWidth(1.0);
	
	glLineWidth(2.0);
	glBegin(GL_LINES);
	for (std::vector<Bond*>::iterator it = bondList.begin(); it!=bondList.end(); ++it) 
	{
		Atom *a1 = (*it)->atomStart;
		Atom *a2 = (*it)->atomEnd;
		
		if (a1->hetatm && a2->hetatm)
		{
			Point4 c1 = colorInfo.getColor(a1);
			Point4 c2 = colorInfo.getColor(a2);
			Point4 p1 = a1->pos;
			Point4 p2 = a2->pos;

			float r1 = Bond::bondDistance(a1->name);
			float r2 = Bond::bondDistance(a2->name);
			Point4 pm = p1 + r1/(r1+r2)*(p2-p1);

			// draw!
			glColor4f(c1.x, c1.y, c1.z, c1.a);
			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(pm.x, pm.y, pm.z);

			glColor4f(c2.x, c2.y, c2.z, c2.a);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(pm.x, pm.y, pm.z);
		}
	}
	glEnd();
	glLineWidth(1.0);
	glEndList();
	
	
	/*sphereList = glGenLists(1);
	glNewList(sphereList, GL_COMPILE);
		glutWireCube(1, 4, 4);
	glEndList();*/
	
	spaceFillCallList = glGenLists(1);
	glNewList(spaceFillCallList, GL_COMPILE);
	
	
	glDisable(GL_POINT_SMOOTH);
	
	glBegin(GL_POINTS);
	for (std::vector<Atom*>::iterator it = atomList.begin(); it!=atomList.end(); ++it) 
	{
		Point4 p = (*it)->pos;
		Point4 c = colorInfo.getColor(*it);
		//float r = Bond::bondDistance((*it)->resName);
		
		/*glPushMatrix();
			glColor4f(c.x, c.y, c.z, c.a);
			glTranslatef(p.x, p.y, p.z);
			glScalef(r, r, r);
			glCallList(sphereList);
		glPopMatrix();*/
		
		glColor4f(c.x, c.y, c.z, c.a);
		glVertex3f(p.x, p.y, p.z);
	}
	glEnd();

	
	glEndList();
	
}

void Molecule::render(int mode)
{
	glPushMatrix();
	glScalef(1.0/size, 1.0/size, 1.0/size);
	glTranslatef(-1*pcenter.x, -1*pcenter.y, -1*pcenter.z);
	if (mode == 0)
	{
		glCallList(cAlphaCallList);
	}
	else if (mode == 1)
	{
		glCallList(moleculeCallList);
	}
	else if (mode == 2)
	{
		glCallList(spaceFillCallList);
	}
	else
	{
	
	}
	glPopMatrix();
}