/*
 * MatrixHelper.cpp
 *
 *  Created on: Oct 5, 2013
 *      Author: 'Sebastian Wilzbach <sebi@wilzbach.me>'
 */

#include "MatrixHelper.h"

MatrixHelper::MatrixHelper() {
	// TODO Auto-generated constructor stub

}

MatrixHelper::~MatrixHelper() {
	// TODO Auto-generated destructor stub
}

const  char * MatrixHelper::getImagePathForAcid(char c){
	const  char *  url;

	switch(c){
		case 'A': url="DAlanine.png"; break;
		case 'C': url="LArginine.png"; break;
		case 'D': url="LAsparagine.png"; break;
		case 'E': url="LAsparticAcid.png"; break;
		case 'F': url="LCysteine.png"; break;
		case 'G': url="LGlutamicAcid.png"; break;
		case 'H': url="LGlutamine.png"; break;
		case 'I': url="Glycine.png"; break;
		case 'K': url="DHistidine.png"; break;
		case 'L': url="LIsoleucine.png"; break;
		case 'M': url="LLeucine.png"; break;
		case 'N': url="LLysine.png"; break;
		case 'P': url="LMethione.png"; break;
		case 'Q': url="LPhenylalanine.png"; break;
		case 'R': url="DProline.png"; break;
		case 'S': url="LSerine.png"; break;
		case 'T': url="LThreonine.png"; break;
		case 'V': url="LTryptophan.png"; break;
		case 'W': url="LTyrosine.png"; break;
		case 'Y': url="LValine.png"; break;
		}

	//add subfolder - NOT possible
	//std::string  temp= url;
	//std::string result = "" + temp;

	//cocos2d::CCLog("Path: %s", url);

	return url;
}

 char MatrixHelper::getRandomAminoAcid(){
	int i= rand() % 20;
	char a= 'A';
	switch(i){
	case 0: a='A'; break;
	case 1: a='C'; break;
	case 2: a='D'; break;
	case 3: a='E'; break;
	case 4: a='F'; break;
	case 5: a='G'; break;
	case 6: a='H'; break;
	case 7: a='I'; break;
	case 8: a='K'; break;
	case 9: a='L'; break;
	case 10: a='M'; break;
	case 11: a='N'; break;
	case 12: a='P'; break;
	case 13: a='Q'; break;
	case 14: a='R'; break;
	case 15: a='S'; break;
	case 16: a='T'; break;
	case 17: a='V'; break;
	case 18: a='W'; break;
	case 19: a='Y'; break;
	}

	a = 'A';

	return a;
}


