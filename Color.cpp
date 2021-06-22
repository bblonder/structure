#include <map>
#include <math.h>
#include <time.h>

#include "RandomUtil.h"
#include "Color.h"

Point4 HSVtoRGB(float h, float s, float v) {
	float m, n, f;
	int i;
	
	h *= 6;

	i = floor(h);
	f = h - i;
	if ( !(i&1) ) f = 1 - f; // if i is even
	m = v * (1 - s);
	n = v * (1 - s * f);
	
	switch (i) {
		case 0: 
			return Point4(v, n, m);
		case 1: 
			return Point4(n, v, m);
		case 2: 
			return Point4(m, v, n);
		case 3: 
			return Point4(m, n, v);
		case 4: 
			return Point4(n, m, v);
		case 5: 
			return Point4(v, m, n);
		default: 
			return Point4(1, 1, 1);
	}
}

/*void Color::initializeColorMap()
{
	if (ColorMap.size() == 0)
	{
		ColorMap[" H"] = Point4(255,255,255);
		ColorMap["HE"] = Point4(217,255,255);
		ColorMap["LI"] = Point4(204,128,255);
		ColorMap["BE"] = Point4(194,255,0);
		ColorMap[" B"] = Point4(255,181,181);
		ColorMap[" C"] = Point4(144,144,144);
		ColorMap[" N"] = Point4(48,80,248);
		ColorMap[" O"] = Point4(255,13,13);
		ColorMap[" F"] = Point4(144,224,80);
		ColorMap["NE"] = Point4(179,227,245);
		ColorMap["NA"] = Point4(171,92,242);
		ColorMap["MG"] = Point4(138,255,0);
		ColorMap["AL"] = Point4(191,166,166);
		ColorMap["SI"] = Point4(240,200,160);
		ColorMap[" P"] = Point4(255,128,0);
		ColorMap[" S"] = Point4(255,255,48);
		ColorMap["CL"] = Point4(31,240,31);
		ColorMap["AR"] = Point4(128,209,227);
		ColorMap[" K"] = Point4(143,64,212);
		ColorMap["CA"] = Point4(61,255,0);
		ColorMap["SC"] = Point4(230,230,230);
		ColorMap["TI"] = Point4(191,194,199);
		ColorMap[" V"] = Point4(166,166,171);
		ColorMap["CR"] = Point4(138,153,199);
		ColorMap["MN"] = Point4(156,122,199);
		ColorMap["FE"] = Point4(224,102,51);
		ColorMap["CO"] = Point4(240,144,160);
		ColorMap["NI"] = Point4(80,208,80);
		ColorMap["CU"] = Point4(200,128,51);
		ColorMap["ZN"] = Point4(125,128,176);
		ColorMap["GA"] = Point4(194,143,143);
		ColorMap["GE"] = Point4(102,143,143);
		ColorMap["AS"] = Point4(189,128,227);
		ColorMap["SE"] = Point4(255,161,0);
		ColorMap["BR"] = Point4(166,41,41);
		ColorMap["KR"] = Point4(92,184,209);
		ColorMap["RB"] = Point4(112,46,176);
		ColorMap["SR"] = Point4(0,255,0);
		ColorMap[" Y"] = Point4(148,255,255);
		ColorMap["ZR"] = Point4(148,224,224);
		ColorMap["NB"] = Point4(115,194,201);
		ColorMap["MO"] = Point4(84,181,181);
		ColorMap["TC"] = Point4(59,158,158);
		ColorMap["RU"] = Point4(36,143,143);
		ColorMap["RH"] = Point4(10,125,140);
		ColorMap["PD"] = Point4(0,105,133);
		ColorMap["AG"] = Point4(192,192,192);
		ColorMap["CD"] = Point4(255,217,143);
		ColorMap["IN"] = Point4(166,117,115);
		ColorMap["SN"] = Point4(102,128,128);
		ColorMap["SB"] = Point4(158,99,181);
		ColorMap["TE"] = Point4(212,122,0);
		ColorMap[" I"] = Point4(148,0,148);
		ColorMap["XE"] = Point4(66,158,176);
		ColorMap["CS"] = Point4(87,23,143);
		ColorMap["BA"] = Point4(0,201,0);
		ColorMap["LA"] = Point4(112,212,255);
		ColorMap["CE"] = Point4(255,255,199);
		ColorMap["PR"] = Point4(217,255,199);
		ColorMap["ND"] = Point4(199,255,199);
		ColorMap["PM"] = Point4(163,255,199);
		ColorMap["SM"] = Point4(143,255,199);
		ColorMap["EU"] = Point4(97,255,199);
		ColorMap["GD"] = Point4(69,255,199);
		ColorMap["TB"] = Point4(48,255,199);
		ColorMap["DY"] = Point4(31,255,199);
		ColorMap["HO"] = Point4(0,255,156);
		ColorMap["ER"] = Point4(0,230,117);
		ColorMap["TM"] = Point4(0,212,82);
		ColorMap["YB"] = Point4(0,191,56);
		ColorMap["LU"] = Point4(0,171,36);
		ColorMap["HF"] = Point4(77,194,255);
		ColorMap["TA"] = Point4(77,166,255);
		ColorMap[" W"] = Point4(33,148,214);
		ColorMap["RE"] = Point4(38,125,171);
		ColorMap["OS"] = Point4(38,102,150);
		ColorMap["IR"] = Point4(23,84,135);
		ColorMap["PT"] = Point4(208,208,224);
		ColorMap["AU"] = Point4(255,209,35);
		ColorMap["HG"] = Point4(184,184,208);
		ColorMap["TL"] = Point4(166,84,77);
		ColorMap["PB"] = Point4(87,89,97);
		ColorMap["BI"] = Point4(158,79,181);
		ColorMap["PO"] = Point4(171,92,0);
		ColorMap["AT"] = Point4(117,79,69);
		ColorMap["RN"] = Point4(66,130,150);
		ColorMap["FR"] = Point4(66,0,102);
		ColorMap["RA"] = Point4(0,125,0);
		ColorMap["AC"] = Point4(112,171,250);
		ColorMap["TH"] = Point4(0,186,255);
		ColorMap["PA"] = Point4(0,161,255);
		ColorMap[" U"] = Point4(0,143,255);
		ColorMap["NP"] = Point4(0,128,255);
		ColorMap["PU"] = Point4(0,107,255);
		ColorMap["AM"] = Point4(84,92,242);
		ColorMap["CM"] = Point4(120,92,227);
		ColorMap["BK"] = Point4(138,79,227);
		ColorMap["CF"] = Point4(161,54,212);
		ColorMap["ES"] = Point4(179,31,212);
		ColorMap["FM"] = Point4(179,31,186);
		ColorMap["MD"] = Point4(179,13,166);
		ColorMap["NO"] = Point4(189,13,135);
		ColorMap["LR"] = Point4(199,0,102);
		ColorMap["RF"] = Point4(204,0,89);
		ColorMap["DB"] = Point4(209,0,79);
		ColorMap["SG"] = Point4(217,0,69);
		ColorMap["BH"] = Point4(224,0,56);
		ColorMap["HS"] = Point4(230,0,46);
		ColorMap["MT"] = Point4(235,0,38);
	}
}*/

Point4 ColorLookup(std::string atmName)
{
	// common ones first
	if (atmName == " C") return Point4(144,144,144);
	if (atmName == " N") return Point4(48,80,248);
	if (atmName == " O") return Point4(255,13,13);
	if (atmName == " H") return Point4(255,255,255);
	if (atmName == " P") return Point4(255,128,0);
	if (atmName == " S") return Point4(255,255,48);
	if (atmName == " K") return Point4(143,64,212);
	
	
	if (atmName == "HE") return Point4(217,255,255);
	if (atmName == "LI") return Point4(204,128,255);
	if (atmName == "BE") return Point4(194,255,0);
	if (atmName == " B") return Point4(255,181,181);
	if (atmName == " F") return Point4(144,224,80);
	if (atmName == "NE") return Point4(179,227,245);
	if (atmName == "NA") return Point4(171,92,242);
	if (atmName == "MG") return Point4(138,255,0);
	if (atmName == "AL") return Point4(191,166,166);
	if (atmName == "SI") return Point4(240,200,160);
	if (atmName == "CL") return Point4(31,240,31);
	if (atmName == "AR") return Point4(128,209,227);
	if (atmName == "CA") return Point4(61,255,0);
	if (atmName == "SC") return Point4(230,230,230);
	if (atmName == "TI") return Point4(191,194,199);
	if (atmName == " V") return Point4(166,166,171);
	if (atmName == "CR") return Point4(138,153,199);
	if (atmName == "MN") return Point4(156,122,199);
	if (atmName == "FE") return Point4(224,102,51);
	if (atmName == "CO") return Point4(240,144,160);
	if (atmName == "NI") return Point4(80,208,80);
	if (atmName == "CU") return Point4(200,128,51);
	if (atmName == "ZN") return Point4(125,128,176);
	if (atmName == "GA") return Point4(194,143,143);
	if (atmName == "GE") return Point4(102,143,143);
	if (atmName == "AS") return Point4(189,128,227);
	if (atmName == "SE") return Point4(255,161,0);
	if (atmName == "BR") return Point4(166,41,41);
	if (atmName == "KR") return Point4(92,184,209);
	if (atmName == "RB") return Point4(112,46,176);
	if (atmName == "SR") return Point4(0,255,0);
	if (atmName == " Y") return Point4(148,255,255);
	if (atmName == "ZR") return Point4(148,224,224);
	if (atmName == "NB") return Point4(115,194,201);
	if (atmName == "MO") return Point4(84,181,181);
	if (atmName == "TC") return Point4(59,158,158);
	if (atmName == "RU") return Point4(36,143,143);
	if (atmName == "RH") return Point4(10,125,140);
	if (atmName == "PD") return Point4(0,105,133);
	if (atmName == "AG") return Point4(192,192,192);
	if (atmName == "CD") return Point4(255,217,143);
	if (atmName == "IN") return Point4(166,117,115);
	if (atmName == "SN") return Point4(102,128,128);
	if (atmName == "SB") return Point4(158,99,181);
	if (atmName == "TE") return Point4(212,122,0);
	if (atmName == " I") return Point4(148,0,148);
	if (atmName == "XE") return Point4(66,158,176);
	if (atmName == "CS") return Point4(87,23,143);
	if (atmName == "BA") return Point4(0,201,0);
	if (atmName == "LA") return Point4(112,212,255);
	if (atmName == "CE") return Point4(255,255,199);
	if (atmName == "PR") return Point4(217,255,199);
	if (atmName == "ND") return Point4(199,255,199);
	if (atmName == "PM") return Point4(163,255,199);
	if (atmName == "SM") return Point4(143,255,199);
	if (atmName == "EU") return Point4(97,255,199);
	if (atmName == "GD") return Point4(69,255,199);
	if (atmName == "TB") return Point4(48,255,199);
	if (atmName == "DY") return Point4(31,255,199);
	if (atmName == "HO") return Point4(0,255,156);
	if (atmName == "ER") return Point4(0,230,117);
	if (atmName == "TM") return Point4(0,212,82);
	if (atmName == "YB") return Point4(0,191,56);
	if (atmName == "LU") return Point4(0,171,36);
	if (atmName == "HF") return Point4(77,194,255);
	if (atmName == "TA") return Point4(77,166,255);
	if (atmName == " W") return Point4(33,148,214);
	if (atmName == "RE") return Point4(38,125,171);
	if (atmName == "OS") return Point4(38,102,150);
	if (atmName == "IR") return Point4(23,84,135);
	if (atmName == "PT") return Point4(208,208,224);
	if (atmName == "AU") return Point4(255,209,35);
	if (atmName == "HG") return Point4(184,184,208);
	if (atmName == "TL") return Point4(166,84,77);
	if (atmName == "PB") return Point4(87,89,97);
	if (atmName == "BI") return Point4(158,79,181);
	if (atmName == "PO") return Point4(171,92,0);
	if (atmName == "AT") return Point4(117,79,69);
	if (atmName == "RN") return Point4(66,130,150);
	if (atmName == "FR") return Point4(66,0,102);
	if (atmName == "RA") return Point4(0,125,0);
	if (atmName == "AC") return Point4(112,171,250);
	if (atmName == "TH") return Point4(0,186,255);
	if (atmName == "PA") return Point4(0,161,255);
	if (atmName == " U") return Point4(0,143,255);
	if (atmName == "NP") return Point4(0,128,255);
	if (atmName == "PU") return Point4(0,107,255);
	if (atmName == "AM") return Point4(84,92,242);
	if (atmName == "CM") return Point4(120,92,227);
	if (atmName == "BK") return Point4(138,79,227);
	if (atmName == "CF") return Point4(161,54,212);
	if (atmName == "ES") return Point4(179,31,212);
	if (atmName == "FM") return Point4(179,31,186);
	if (atmName == "MD") return Point4(179,13,166);
	if (atmName == "NO") return Point4(189,13,135);
	if (atmName == "LR") return Point4(199,0,102);
	if (atmName == "RF") return Point4(204,0,89);
	if (atmName == "DB") return Point4(209,0,79);
	if (atmName == "SG") return Point4(217,0,69);
	if (atmName == "BH") return Point4(224,0,56);
	if (atmName == "HS") return Point4(230,0,46);
	if (atmName == "MT") return Point4(235,0,38);
	
	return Point4(255, 255, 255);
}

Point4 Color::getCPKColor(std::string name)
{
	// initializeColorMap();
	
	Point4 color = ColorLookup(name);
	
	// color = ColorMap[name];
	
	if (color.x == 0 && color.y == 0 && color.z == 0) 
	{
		return Point4(1, 1, 1);
	}
	else 
	{
		return color/255.0;
	}
}

void Color::initializeChainColors(std::map<std::string, std::string> compounds)
{
	ChainColorMap.clear();
	
	float oldHue = randomFloat(0,1);
	
	for (std::map<std::string, std::string>::iterator it = compounds.begin(); it != compounds.end(); ++it)
	{
		float newHue = oldHue + 0.2 + 0.1 * randomFloat(0,1);
		newHue = newHue - floor(newHue);
		oldHue = newHue;
		ChainColorMap[(*it).first] = HSVtoRGB(newHue, 1, 1);
	}
}

Point4 Color::getChainColor(std::string chainID)
{
	return ChainColorMap[chainID];
}

Point4 Color::getColor(Atom *a)
{
	Point4 c = getCPKColor(a->name);
	if ((a->name == std::string(" C")) && (!a->hetatm))
	{
		c = getChainColor(a->chainID);
	}
	if (a->hetatm)
	{
		float w1 = 2;
		float w2 = 1;
		float w3 = 0;
		c = getChainColor(a->resName);
		c = (w1*c + w2*getCPKColor(a->name) + w3*Point4(1,1,1))/(w1+w2+w3);
		c.a = 1.0;
	}
	else
	{
		//float hdelta = -0.15;
		//float sdelta = -0.30;
		float trans = 1.0;
		
		c.a = trans;
		//c.a += (a->helix ? hdelta : 0);
		//c.a += (a->sheet ? sdelta : 0);
	}
	
	return c;
}
