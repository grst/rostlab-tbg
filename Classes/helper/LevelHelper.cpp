/*
 * LevelHelper.cpp
 *
 *  Created on: Nov 21, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#include "LevelHelper.h"

std::string LevelHelper::getDescriptionForLevel(int level){

	std::string description ="";
	switch(level){
	case 0:
		description = "While recovering from the excesses of New Year's Eve, we might\n"
				"ponder the enzyme that ceaselessly battles the champagne that we consume. \n"
				"Alcohol dehydrogenase is our primary defense against alcohol, \n"
				"a toxic molecule\n that compromises the function of our nervous system.\n\n"
				"Alcohol dehydrogenase (EC:1.1.1.1) (ADH) catalyzes the reversible oxidation\n"
				"of alcohols to their corresponding acetaldehyde or ketone with the concomitant reduction of NAD"; break;
	}

	return description;
}

std::string LevelHelper::getSequenceForLevel(int level){
	std::string sequence = "";
	switch(level){
	case 0:
			sequence = "STAGKVIKCKAAVLWEEKKPFSIEEVEVAPPKAHEVRIKMVATGICRSDDHVVSGTLVTPLPVIAGHEAAGIVESIGEGVTTVRPGDKVIPLFTPQCGKCRVCKHPEGNFCLKNDLSMPRGTMQDGTSRFTCRGKPIHHFLGTSTFSQYTVVDEISVAKIDAASPLEKVCLIGCGFSTGYGSAVKVAKVTQGSTCAVFGLGGVGLSVIMGCKAAGAARIIGVDINKDKFAKAKEVGATECVNPQDYKKPIQEVLTEMSNGGVDFSFEVIGRLDTMVTALSCCQEAYGVSVIVGVPPDSQNLSMNPMLLLSGRTWKGAIFGGFKSKDSVPKLVADFMAKKFALDPLITHVLPFEKINEGFDLLRSGESIRTILTF";
			break;
	default:
		sequence = "AAAAAAAAAAAAAAAAAAAAAAAAAA";
	}
	return sequence;
}

int LevelHelper::getMaxSpeed(int level){
	return 6;
}

int LevelHelper::getMinSpeed(int level){
	return 4;
}

int LevelHelper::getMaxAA(int level){
	return 9;
}

int LevelHelper::getMinAA(int level){
	return 6;
}

//Every n calls of tick, one additional amino acid is added (if withing MIN and MAX)
int LevelHelper::getAAAddProb(int level){
	return 230;
}

//Every n wall collisions, remove one amino acid.
int LevelHelper::getAARemProb(int level){
	return 15;
}




