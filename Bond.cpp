#include "Bond.h"
#include <map>

Bond::Bond(Atom *a1, Atom *b1)
{
	atomStart = a1;
	atomEnd = b1;
}

static std::map<std::string, float> distanceMap;

float Bond::bondDistance(std::string a1, std::string a2)
{
	return bondDistance(a1) + bondDistance(a2) + 0.56;
}

float Bond::bondDistance(std::string atomName)
{

	
	// try the most common ones first
	if (atomName == " C") return 0.68;
	if (atomName == " O") return 0.68;
	if (atomName == " N") return 0.68;
	if (atomName == " S") return 1.02;
	if (atomName == " B") return 0.83;
	if (atomName == "CL") return 0.99;
	if (atomName == " K") return 1.33;
	if (atomName == " P") return 1.05;
	if (atomName == " H") return 0.23;

	// now the rest
	if (atomName == "AC") return 1.88;
	if (atomName == "AG") return 1.59;
	if (atomName == "AL") return 1.35;
	if (atomName == "AM") return 1.51;
	if (atomName == "AR") return 1.51;
	if (atomName == "AS") return 1.21;
	if (atomName == "AT") return 1.21;
	if (atomName == "AU") return 1.5;
	if (atomName == "BA") return 1.34;
	if (atomName == "BE") return 0.35;
	if (atomName == "BH") return 1.5;
	if (atomName == "BI") return 1.54;
	if (atomName == "BK") return 1.54;
	if (atomName == "BR") return 1.21;
	if (atomName == "CA") return 0.99;
	if (atomName == "CD") return 1.69;
	if (atomName == "CE") return 1.83;
	if (atomName == "CF") return 1.83;
	if (atomName == "CM") return 0.99;
	if (atomName == "CO") return 1.33;
	if (atomName == "CR") return 1.35;
	if (atomName == "CS") return 1.67;
	if (atomName == "CU") return 1.52;
	if (atomName == "DB") return 1.5;
	if (atomName == "DS") return 1.5;
	if (atomName == "DY") return 1.75;
	if (atomName == "ER") return 1.73;
	if (atomName == "ES") return 1.5;
	if (atomName == "EU") return 1.99;
	if (atomName == " F") return 0.64;
	if (atomName == "FE") return 1.34;
	if (atomName == "FM") return 1.5;
	if (atomName == "FR") return 1.5;
	if (atomName == "GA") return 1.22;
	if (atomName == "GD") return 1.79;
	if (atomName == "GE") return 1.17;
	if (atomName == "HE") return 1.5;
	if (atomName == "HF") return 1.57;
	if (atomName == "HG") return 1.7;
	if (atomName == "HO") return 1.74;
	if (atomName == "HS") return 1.5;
	if (atomName == " I") return 1.4;
	if (atomName == "IN") return 1.63;
	if (atomName == "IR") return 1.32;
	if (atomName == "KR") return 1.5;
	if (atomName == "LA") return 1.87;
	if (atomName == "LI") return 0.68;
	if (atomName == "LR") return 1.5;
	if (atomName == "LU") return 1.72;
	if (atomName == "MD") return 1.5;
	if (atomName == "MG") return 1.1;
	if (atomName == "MN") return 1.35;
	if (atomName == "MO") return 1.47;
	if (atomName == "MT") return 1.5;
	if (atomName == "NA") return 0.97;
	if (atomName == "NB") return 1.48;
	if (atomName == "ND") return 1.81;
	if (atomName == "NE") return 1.5;
	if (atomName == "NI") return 1.5;
	if (atomName == "NO") return 1.5;
	if (atomName == "NP") return 1.55;
	if (atomName == "OS") return 1.37;
	if (atomName == "PA") return 1.61;
	if (atomName == "PB") return 1.54;
	if (atomName == "PD") return 1.5;
	if (atomName == "PM") return 1.8;
	if (atomName == "PO") return 1.68;
	if (atomName == "PR") return 1.82;
	if (atomName == "PT") return 1.5;
	if (atomName == "PU") return 1.53;
	if (atomName == "RA") return 1.9;
	if (atomName == "RB") return 1.47;
	if (atomName == "RE") return 1.35;
	if (atomName == "RF") return 1.5;
	if (atomName == "RH") return 1.45;
	if (atomName == "RN") return 1.5;
	if (atomName == "RU") return 1.4;
	if (atomName == "SB") return 1.46;
	if (atomName == "SC") return 1.44;
	if (atomName == "SE") return 1.22;
	if (atomName == "SG") return 1.5;
	if (atomName == "SI") return 1.2;
	if (atomName == "SM") return 1.8;
	if (atomName == "SN") return 1.46;
	if (atomName == "SR") return 1.12;
	if (atomName == "TA") return 1.43;
	if (atomName == "TB") return 1.76;
	if (atomName == "TC") return 1.35;
	if (atomName == "TE") return 1.47;
	if (atomName == "TH") return 1.79;
	if (atomName == "TI") return 1.47;
	if (atomName == "TL") return 1.55;
	if (atomName == "TM") return 1.72;
	if (atomName == " U") return 1.58;
	if (atomName == " V") return 1.33;
	if (atomName == " W") return 1.37;
	if (atomName == "XE") return 1.5;
	if (atomName == " Y") return 1.78;
	if (atomName == "YB") return 1.94;
	if (atomName == "ZN") return 1.45;
	if (atomName == "ZR") return 1.56;
	
	else return 1.0;
}