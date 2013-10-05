/*
 * MatrixHelper.h
 *
 *  Created on: Oct 5, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#ifndef MATRIXHELPER_H_
#define MATRIXHELPER_H_

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "cocos2d.h"


class MatrixHelper {
public:
	MatrixHelper();
	virtual ~MatrixHelper();
	static const char * getImagePathForAcid(char c);
	static char getRandomAminoAcid();
};

#endif /* MATRIXHELPER_H_ */
