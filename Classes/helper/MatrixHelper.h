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
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include "cocos2d.h"
#include "Box2D/Box2D.h"

USING_NS_CC;


class MatrixHelper {
public:
	MatrixHelper();
	virtual ~MatrixHelper();
	static const char * getImagePathForAcid(char c);
	static char getRandomAminoAcid();
	static int getScoreForAminoAcid(char c, char d);
	static int getAcidInt(char c);
    static ccColor3B getAcidColor(char c);
    static float getRelativeScaleFactor(char c);
    static b2Vec2* getVerticeData(char c, float scaleRatio);
    static int getVerticeNum(char c);
	static void loadMatrix(std::string name);
private:
	static int** scoringMatrix;
	static bool firstTime;
};

#endif /* MATRIXHELPER_H_ */
