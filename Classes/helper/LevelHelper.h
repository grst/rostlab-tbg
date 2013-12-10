/*
 * LevelHelper.h
 *
 *  Created on: Nov 21, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#ifndef LEVELHELPER_H_
#define LEVELHELPER_H_

#include <string>

class LevelHelper {
public:
	static std::string getNameForLevel(int level);
	static std::string getCodeForLevel(int level);
	static std::string getSequenceForLevel(int level);
	static std::string getPathForLevel(int level);
	static std::string getDescriptionForLevel(int level);
	static int getMinSpeed(int level);
	static int getMaxSpeed(int level);
	static int getMinAA(int level);
	static int getMaxAA(int level);
	static int getAARemProb(int level);
	static int getAAAddProb(int level);

};

#endif /* LEVELHELPER_H_ */
