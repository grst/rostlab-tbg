/*
 * MatrixHelper.cpp
 *
 *  Created on: Oct 5, 2013
 *      Author: 'Sebastian Wilzbach <sebi@wilzbach.me>'
 */

#include "MatrixHelper.h"
#include "HelperFunctions.h"

int ** MatrixHelper::scoringMatrix = new int*[20];
bool MatrixHelper::firstTime = false;

MatrixHelper::MatrixHelper() {
	// TODO Auto-generated constructor stub

}

MatrixHelper::~MatrixHelper() {
	// TODO Auto-generated destructor stub
	for (int i = 0; i < 20; ++i) {
		delete[] MatrixHelper::scoringMatrix[i];
	}
	delete[] MatrixHelper::scoringMatrix;
}

const char * MatrixHelper::getImagePathForAcid(char c) {
	const char * url;

	switch (c) {
        case 'A':
            url = "DAlanine.png";
            break;
        case 'C':
            url = "LCysteine.png";
            break;
        case 'D':
            url = "LAsparticAcid.png";
            break;
        case 'E':
            url = "LGlutamicAcid.png";
            break;
        case 'F':
            url = "LPhenylalanine.png";
            break;
        case 'G':
            url = "Glycine.png";
            break;
        case 'H':
            url = "DHistidine.png";
            break;
        case 'I':
            url = "LIsoleucine.png";
            break;
        case 'K':
            url = "LLysine.png";
            break;
        case 'L':
            url = "LLeucine.png";
            break;
        case 'M':
            url = "LMethionine.png";
            break;
        case 'N':
            url = "LAsparagine.png";
            break;
        case 'P':
            url = "DProline.png";
            break;
        case 'Q':
            url = "LGlutamine.png";
            break;
        case 'R':
            url = "LArginine.png";
            break;
        case 'S':
            url = "LSerine.png";
            break;
        case 'T':
            url = "LThreonine.png";
            break;
        case 'V':
            url = "LValine.png";
            break;
        case 'W':
            url = "LTryptophan.png";
            break;
        case 'Y':
            url = "LTyrosine.png";
            break;
        default:
        	url = "DAlanine.png";
	}

	//add subfolder - NOT possible
	//std::string  temp= url;
	//std::string result = "" + temp;

	//cocos2d::CCLog("Path: %s", url);

	return url;
}

/**
 * returns the vertice data for the box2d body 
 * for the specified amino acid.
 *
 * Remember that the vertices need to be counterclockwise and not more than 8!
 * the vertices are drawn on the 256px images. Adjust the scale-ratio appropriately.
 * 
 * @param c the amino acid one-letter-code
 * @param scaleRatio every vector is multiplied with it (--> 1/PTM_RATIO)
 */
b2Vec2* MatrixHelper::getVerticeData(char c, float scaleRatio) {
    const int n = MatrixHelper::getVerticeNum(c);
    if(n < 0) {
        return NULL;
    }
    b2Vec2 *verts = new b2Vec2[n];


    // TODO add vertix data for other  AA

    switch(c) {
        case 'A':
            verts[0].Set(-53.0f * scaleRatio, 55.0f * scaleRatio);
            verts[1].Set(-79.0f * scaleRatio, -38.0f * scaleRatio);
            verts[2].Set(-2.0f * scaleRatio, -76.0f * scaleRatio);
            verts[3].Set(79.0f * scaleRatio, -15.0f * scaleRatio);
            verts[4].Set(71.0f * scaleRatio, 52.0f * scaleRatio);
            verts[5].Set(3.0f * scaleRatio, 87.0f * scaleRatio);
            break;
        case 'C':
            verts[0].Set(-5.0f * scaleRatio, 48.0f * scaleRatio);
            verts[1].Set(-66.0f * scaleRatio, 69.0f * scaleRatio);
            verts[2].Set(-98.0f * scaleRatio, -20.0f * scaleRatio);
            verts[3].Set(-31.0f * scaleRatio, -83.0f * scaleRatio);
            verts[4].Set(28.0f * scaleRatio, -70.0f * scaleRatio);
            verts[5].Set(42.0f * scaleRatio, -34.0f * scaleRatio);
            verts[6].Set(86.0f * scaleRatio, 44.0f * scaleRatio);
            verts[7].Set(52.0f * scaleRatio, 70.0f * scaleRatio);
            break;
        case 'D':
            verts[0].Set(1.0f * scaleRatio, 81.0f * scaleRatio);
            verts[1].Set(-100.0f * scaleRatio, -29.0f * scaleRatio);
            verts[2].Set(-19.0f * scaleRatio, -87.0f * scaleRatio);
            verts[3].Set(22.0f * scaleRatio, -45.0f * scaleRatio);
            verts[4].Set(76.0f * scaleRatio, -65.0f * scaleRatio);
            verts[5].Set(100.0f * scaleRatio, -33.0f * scaleRatio);
            verts[6].Set(59.0f * scaleRatio, 25.0f * scaleRatio);
            verts[7].Set(71.0f * scaleRatio, 57.0f * scaleRatio);
            break;
        case 'E':
            verts[0].Set(-2.0f * scaleRatio, 93.0f * scaleRatio);
            verts[1].Set(-30.0f * scaleRatio, 34.0f * scaleRatio);
            verts[2].Set(-54.0f * scaleRatio, 23.0f * scaleRatio);
            verts[3].Set(-55.0f * scaleRatio, -63.0f * scaleRatio);
            verts[4].Set(-21.0f * scaleRatio, -68.0f * scaleRatio);
            verts[5].Set(-4.0f * scaleRatio, -92.0f * scaleRatio);
            verts[6].Set(41.0f * scaleRatio, -55.0f * scaleRatio);
            verts[7].Set(42.0f * scaleRatio, 89.0f * scaleRatio);	
            break;
        case 'F':
            verts[0].Set(-11.0f * scaleRatio, 52.0f * scaleRatio);
            verts[1].Set(-94.0f * scaleRatio, 10.0f * scaleRatio);
            verts[2].Set(-63.0f * scaleRatio, -27.0f * scaleRatio);
            verts[3].Set(-29.0f * scaleRatio, -47.0f * scaleRatio);
            verts[4].Set(50.0f * scaleRatio, -44.0f * scaleRatio);
            verts[5].Set(95.0f * scaleRatio, -9.0f * scaleRatio);
            verts[6].Set(77.0f * scaleRatio, 32.0f * scaleRatio);
            verts[7].Set(11.0f * scaleRatio, 24.0f * scaleRatio);
            break;
        case 'G':
            verts[0].Set(-31.0f * scaleRatio, 46.0f * scaleRatio);
            verts[1].Set(-86.0f * scaleRatio, 7.0f * scaleRatio);
            verts[2].Set(-36.0f * scaleRatio, -58.0f * scaleRatio);
            verts[3].Set(-1.0f * scaleRatio, -47.0f * scaleRatio);
            verts[4].Set(77.0f * scaleRatio, -41.0f * scaleRatio);
            verts[5].Set(90.0f * scaleRatio, 9.0f * scaleRatio);
            verts[6].Set(13.0f * scaleRatio, 37.0f * scaleRatio);
            break;
        case 'H':
            verts[0].Set(-15.0f * scaleRatio, 29.0f * scaleRatio);
            verts[1].Set(-49.0f * scaleRatio, 43.0f * scaleRatio);
            verts[2].Set(-86.0f * scaleRatio, 29.0f * scaleRatio);
            verts[3].Set(-83.0f * scaleRatio, -12.0f * scaleRatio);
            verts[4].Set(6.0f * scaleRatio, -56.0f * scaleRatio);
            verts[5].Set(81.0f * scaleRatio, -40.0f * scaleRatio);
            verts[6].Set(94.0f * scaleRatio, -6.0f * scaleRatio);
            verts[7].Set(21.0f * scaleRatio, 56.0f * scaleRatio);
            break;
        case 'I':
            verts[0].Set(-15.0f * scaleRatio, 82.0f * scaleRatio);
            verts[1].Set(-99.0f * scaleRatio, -24.0f * scaleRatio);
            verts[2].Set(-87.0f * scaleRatio, -44.0f * scaleRatio);
            verts[3].Set(7.0f * scaleRatio, -74.0f * scaleRatio);
            verts[4].Set(39.0f * scaleRatio, -55.0f * scaleRatio);
            verts[5].Set(92.0f * scaleRatio, 34.0f * scaleRatio);
            verts[6].Set(71.0f * scaleRatio, 55.0f * scaleRatio);
            verts[7].Set(27.0f * scaleRatio, 59.0f * scaleRatio);
            break;
        case 'K':
            verts[0].Set(-87.0f * scaleRatio, 15.0f * scaleRatio);
            verts[1].Set(-71.0f * scaleRatio, -78.0f * scaleRatio);
            verts[2].Set(-45.0f * scaleRatio, -88.0f * scaleRatio);
            verts[3].Set(-13.0f * scaleRatio, -44.0f * scaleRatio);
            verts[4].Set(56.0f * scaleRatio, -15.0f * scaleRatio);
            verts[5].Set(95.0f * scaleRatio, 31.0f * scaleRatio);
            verts[6].Set(72.0f * scaleRatio, 65.0f * scaleRatio);
            verts[7].Set(8.0f * scaleRatio, 64.0f * scaleRatio);
            break;
        case 'L':
            verts[0].Set(-63.0f * scaleRatio, 70.0f * scaleRatio);
            verts[1].Set(-79.0f * scaleRatio, 41.0f * scaleRatio);
            verts[2].Set(-76.0f * scaleRatio, 2.0f * scaleRatio);
            verts[3].Set(-2.0f * scaleRatio, -57.0f * scaleRatio);
            verts[4].Set(66.0f * scaleRatio, -57.0f * scaleRatio);
            verts[5].Set(74.0f * scaleRatio, -10.0f * scaleRatio);
            verts[6].Set(37.0f * scaleRatio, 40.0f * scaleRatio);
            verts[7].Set(-5.0f * scaleRatio, 68.0f * scaleRatio);
            break;
        case 'M':
            verts[0].Set(-76.0f * scaleRatio, 35.0f * scaleRatio);
            verts[1].Set(-72.0f * scaleRatio, -58.0f * scaleRatio);
            verts[2].Set(-42.0f * scaleRatio, -68.0f * scaleRatio);
            verts[3].Set(77.0f * scaleRatio, -30.0f * scaleRatio);
            verts[4].Set(76.0f * scaleRatio, 29.0f * scaleRatio);
            verts[5].Set(45.0f * scaleRatio, 45.0f * scaleRatio);
            verts[6].Set(24.0f * scaleRatio, 81.0f * scaleRatio);
            verts[7].Set(-45.0f * scaleRatio, 70.0f * scaleRatio);
            break;
        case 'N':
            verts[0].Set(-24.0f * scaleRatio, 53.0f * scaleRatio);
            verts[1].Set(-77.0f * scaleRatio, 34.0f * scaleRatio);
            verts[2].Set(-82.0f * scaleRatio, -30.0f * scaleRatio);
            verts[3].Set(-25.0f * scaleRatio, -74.0f * scaleRatio);
            verts[4].Set(33.0f * scaleRatio, -70.0f * scaleRatio);
            verts[5].Set(69.0f * scaleRatio, 22.0f * scaleRatio);
            verts[6].Set(40.0f * scaleRatio, 90.0f * scaleRatio);
            verts[7].Set(-21.0f * scaleRatio, 86.0f * scaleRatio);
            break;
        case 'P':
            verts[0].Set(-79.0f * scaleRatio, 32.0f * scaleRatio);
            verts[1].Set(-73.0f * scaleRatio, -12.0f * scaleRatio);
            verts[2].Set(-14.0f * scaleRatio, -54.0f * scaleRatio);
            verts[3].Set(49.0f * scaleRatio, -53.0f * scaleRatio);
            verts[4].Set(90.0f * scaleRatio, 13.0f * scaleRatio);
            verts[5].Set(29.0f * scaleRatio, 69.0f * scaleRatio);
            verts[6].Set(-30.0f * scaleRatio, 61.0f * scaleRatio);
            verts[7].Set(-70.0f * scaleRatio, 46.0f * scaleRatio);
            break;
        case 'Q':
            verts[0].Set(-43.0f * scaleRatio, 72.0f * scaleRatio);
            verts[1].Set(-80.0f * scaleRatio, 34.0f * scaleRatio);
            verts[2].Set(-60.0f * scaleRatio, -45.0f * scaleRatio);
            verts[3].Set(-10.0f * scaleRatio, -79.0f * scaleRatio);
            verts[4].Set(19.0f * scaleRatio, -35.0f * scaleRatio);
            verts[5].Set(57.0f * scaleRatio, -39.0f * scaleRatio);
            verts[6].Set(87.0f * scaleRatio, 43.0f * scaleRatio);
            verts[7].Set(38.0f * scaleRatio, 77.0f * scaleRatio);
            break;
        case 'R':
            verts[0].Set(-3.0f * scaleRatio, 40.0f * scaleRatio);
            verts[1].Set(-85.0f * scaleRatio, 32.0f * scaleRatio);
            verts[2].Set(-94.0f * scaleRatio, -10.0f * scaleRatio);
            verts[3].Set(32.0f * scaleRatio, -43.0f * scaleRatio);
            verts[4].Set(60.0f * scaleRatio, -33.0f * scaleRatio);
            verts[5].Set(86.0f * scaleRatio, -33.0f * scaleRatio);
            verts[6].Set(94.0f * scaleRatio, -3.0f * scaleRatio);
            verts[7].Set(45.0f * scaleRatio, 36.0f * scaleRatio);
            break;
        case 'S':
            verts[0].Set(-75.0f * scaleRatio, 21.0f * scaleRatio);
            verts[1].Set(-74.0f * scaleRatio, -37.0f * scaleRatio);
            verts[2].Set(-12.0f * scaleRatio, -62.0f * scaleRatio);
            verts[3].Set(87.0f * scaleRatio, -30.0f * scaleRatio);
            verts[4].Set(93.0f * scaleRatio, 9.0f * scaleRatio);
            verts[5].Set(55.0f * scaleRatio, 39.0f * scaleRatio);
            verts[6].Set(-2.0f * scaleRatio, 61.0f * scaleRatio);
            verts[7].Set(-59.0f * scaleRatio, 44.0f * scaleRatio);
            break;
        case 'T':
            verts[0].Set(-87.0f * scaleRatio, 3.0f * scaleRatio);
            verts[1].Set(-54.0f * scaleRatio, -36.0f * scaleRatio);
            verts[2].Set(13.0f * scaleRatio, -73.0f * scaleRatio);
            verts[3].Set(80.0f * scaleRatio, -35.0f * scaleRatio);
            verts[4].Set(85.0f * scaleRatio, 0.0f * scaleRatio);
            verts[5].Set(42.0f * scaleRatio, 50.0f * scaleRatio);
            verts[6].Set(-26.0f * scaleRatio, 60.0f * scaleRatio);
            verts[7].Set(-76.0f * scaleRatio, 29.0f * scaleRatio);
            break;
        case 'V':
            verts[0].Set(-90.0f * scaleRatio, 36.0f * scaleRatio);
            verts[1].Set(-33.0f * scaleRatio, -58.0f * scaleRatio);
            verts[2].Set(16.0f * scaleRatio, -64.0f * scaleRatio);
            verts[3].Set(57.0f * scaleRatio, -36.0f * scaleRatio);
            verts[4].Set(79.0f * scaleRatio, 29.0f * scaleRatio);
            verts[5].Set(1.0f * scaleRatio, 64.0f * scaleRatio);
            verts[6].Set(-65.0f * scaleRatio, 60.0f * scaleRatio);
            break;
        case 'W':
            verts[0].Set(-87.0f * scaleRatio, 32.0f * scaleRatio);
            verts[1].Set(-37.0f * scaleRatio, -42.0f * scaleRatio);
            verts[2].Set(48.0f * scaleRatio, -46.0f * scaleRatio);
            verts[3].Set(88.0f * scaleRatio, -20.0f * scaleRatio);
            verts[4].Set(89.0f * scaleRatio, 18.0f * scaleRatio);
            verts[5].Set(41.0f * scaleRatio, 35.0f * scaleRatio);
            verts[6].Set(-24.0f * scaleRatio, 30.0f * scaleRatio);
            verts[7].Set(-70.0f * scaleRatio, 51.0f * scaleRatio);
            break;
        case 'Y':
            verts[0].Set(-92.0f * scaleRatio, 34.0f * scaleRatio);
            verts[1].Set(-27.0f * scaleRatio, -65.0f * scaleRatio);
            verts[2].Set(16.0f * scaleRatio, -64.0f * scaleRatio);
            verts[3].Set(54.0f * scaleRatio, -36.0f * scaleRatio);
            verts[4].Set(80.0f * scaleRatio, 18.0f * scaleRatio);
            verts[5].Set(54.0f * scaleRatio, 47.0f * scaleRatio);
            verts[6].Set(-2.0f * scaleRatio, 62.0f * scaleRatio);
            verts[7].Set(-77.0f * scaleRatio, 57.0f * scaleRatio);
            break;

            
            
    }
    return verts;
    
}

/**
 * returns the vertice number for the
 * specified amino acid.
 */
float MatrixHelper::getRelativeScaleFactor(char c) {
    //base scale.
    float n = 1.0f;
    switch(c) {
        case 'A':
            n *= 1;
            break;
        case 'C':
            n *= 1;
            break;
        case 'D':
            n *= 1;
            break;
        case 'E':
            n *= 1.1;
            break;
        case 'F':
            n *= 1.25;
            break;
        case 'G':
            n *= 1.15;
            break;
        case 'H':
            n *= 1.1;
            break;
        case 'I':
            n *= 1.1;
            break;
        case 'K':
            n *= 1.3;
            break;
        case 'L':
           n *= 1;
            break;
        case 'M':
            n *= 1;
            break;
        case 'N':
            n *= 1;
            break;
        case 'P':
            n *= 1;
            break;
        case 'Q':
            n *= 1;
            break;
        case 'R':
            n *= 1.25;
            break;
        case 'S':
            n *= 1;
            break;
        case 'T':
            n *= 1;
            break;
        case 'V':
            n *= 1;
            break;
        case 'W':
            n *= 1.2;
            break;
        case 'Y':
            n *= 1.1;
            break;
    }
    return n;
}

/**
 * returns the vertice number for the 
 * specified amino acid. 
 */
int MatrixHelper::getVerticeNum(char c) {
    int n = -1;
    switch(c) {
        case 'A':
            n = 6;
            break;
        case 'C':
            n = 8;
            break;
        case 'D':
            n = 8;
            break;
        case 'E':
            n = 8;
            break;
        case 'F':
            n = 8;
            break;
        case 'G':
            n = 7;
            break;
        case 'H':
            n = 8;
            break;
        case 'I':
            n = 8;
            break;
        case 'K':
            n = 8;
            break;
        case 'L':
            n = 8;
            break;
        case 'M':
            n = 8;
            break;
        case 'N':
            n = 8;
            break;
        case 'P':
            n = 8;
            break;
        case 'Q':
            n = 8;
            break;
        case 'R':
            n = 8;	
            break;
        case 'S':
            n = 8;
            break;
        case 'T':
            n = 8;
            break;
        case 'V':
            n = 7;
            break;
        case 'W':
            n = 8;	
            break;
        case 'Y':
            n = 8;
            break;
    }
    return n;
}

char MatrixHelper::getRandomAminoAcid() {
	int i = arc4random() % 20;
	char a;
	switch (i) {
	case 0:
		a = 'A';
		break;
	case 1:
		a = 'C';
		break;
	case 2:
		a = 'D';
		break;
	case 3:
		a = 'E';
		break;
	case 4:
		a = 'F';
		break;
	case 5:
		a = 'G';
		break;
	case 6:
		a = 'H';
		break;
	case 7:
		a = 'I';
		break;
	case 8:
		a = 'K';
		break;
	case 9:
		a = 'L';
		break;
	case 10:
		a = 'M';
		break;
	case 11:
		a = 'N';
		break;
	case 12:
		a = 'P';
		break;
	case 13:
		a = 'Q';
		break;
	case 14:
		a = 'R';
		break;
	case 15:
		a = 'S';
		break;
	case 16:
		a = 'T';
		break;
	case 17:
		a = 'V';
		break;
	case 18:
		a = 'W';
		break;
	case 19:
		a = 'Y';
		break;
    default:
        cocos2d::CCLog("Error creating AA");
        a = 'A';
	}

	return a;
}

int MatrixHelper::getScoreForAminoAcid(char c, char d) {
	int i = MatrixHelper::getAcidInt(c);
	int j = MatrixHelper::getAcidInt(d);
	if (j >= 0 && j <= 19 && i >= 0 && i <= 19) {
		return scoringMatrix[i][j];
	} else {
		return -1;
	}

}

/*
 * this method reads a matrix puts the data into an array
 * DO NOT MODIFY the reading part as long as it works
 * tested by Sebi
 * use the BLOSUM format as input
 */
void MatrixHelper::loadMatrix(std::string name) {

//	scoringMatrix = new int*[20];
	if (firstTime == false) {
		for (int i = 0; i < 20; i++) {
			/*if(scoringMatrix[i]){
			 delete[] scoringMatrix[i];
			 }*/
			scoringMatrix[i] = new int[20];
		}
		firstTime = true;
	}

    //on iOS only file name required, doesn't work with relative path. 
	std::string pFileName = name;

	// Create input file stream
	// Initialize variables needed
	unsigned long fileSize = 0;
	unsigned char * fileContents = NULL;
	std::string thisLine, result, fullPath, contents;

	// Get absolute path of file
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(
			pFileName.c_str());

	cocos2d::CCLog("[ NOTICE ] Starting to read opening file: %s", pFileName.c_str());

	// Get data of file
	fileContents = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(
			fullPath.c_str(), "r", &fileSize);
    
    if(fileContents == NULL) {
        cocos2d::CCLog("Error reading Matrix");
    }
    //cocos2d::CCLog((char *)fileContents);
	contents.append((char *) fileContents);

	// Create a string stream so that we can use getline( ) on it
	std::istringstream fileStringStream(contents);


	bool header = true;
	std::vector <int > acidOrder;
	acidOrder.resize(20);

	// Get file contents line by line
	while (getline(fileStringStream, thisLine)) {
		// Put all lines in vector
		//	mFileContents.push_back(thisLine);
		if (thisLine.at(0) == '#' ) {
			continue;
		}
		if(thisLine.at(0) == ' ' && header){
			std::vector < std::string > x = HelperFunctions::split(thisLine.c_str(),
							' ');

			int i=0;
			std::string  out ="";
			for (std::vector<std::string>::iterator it = x.begin() ; it != x.end(); ++it){
				if(i < 20 && (*it).size() > 0 ){
					char cd= (*it).at(0);
					acidOrder[i] = getAcidInt(cd);
					std::string str4 = static_cast<std::ostringstream*>(&(std::ostringstream()
												<< acidOrder[i]))->str();
					out = out +" "+  str4;
					i++;
				}
			}
			cocos2d::CCLog("Order of AA: %s", out.c_str());
			header = false;
		}

		int i = 0;
		int acidA = MatrixHelper::getAcidInt(thisLine.at(0));

		//check if AA is valid
		if(acidA < 0){
			continue;
		}

		//cocos2d::CCLog("AA: %c", thisLine.at(0));

		std::vector < std::string > x = HelperFunctions::split(thisLine.c_str(),
				' ');
		for (std::vector<std::string>::iterator it = x.begin(); it != x.end();
				++it) {


			std::string z = *it;
			/*	function to remove whitespace but not needed
			 * for (int i = 0; i < z.length(); i++) {
			 if (z[i] == ' ')
			 z.erase(i, 1);
			 } */

			if(z.size() > 0 ){
				int numb = atoi(z.c_str());
				// cocos2d::CCLog("%d", numb);
				if (i < 21 && i > 0) {
					scoringMatrix[acidA][acidOrder[i-1]] = numb;
				}
				i++;
			}
		}

	}

	// Delete buffer created by fileContents. This part is required.
	if (fileContents) {
		delete[] fileContents;
		fileContents = NULL;
	}

	for (int i = 0; i < 20; i++) {
		std::string timeStr = "";
		for (int j = 0; j < 20; j++) {
			timeStr = timeStr + " "
					+ static_cast<std::ostringstream*>(&(std::ostringstream()
							<< scoringMatrix[i][j]))->str();
		}
		cocos2d::CCLog("%s", timeStr.c_str());
	}

	// For testing purposes
	cocos2d::CCLog("[ NOTICE ] Finished opening file: %s", pFileName.c_str());

}

int MatrixHelper::getAcidInt(char c) {

	int n = -1;

	switch (c) {
	case 'A':
		n = 0;
		break;
	case 'C':
		n = 1;
		break;
	case 'D':
		n = 2;
		break;
	case 'E':
		n = 3;
		break;
	case 'F':
		n = 4;
		break;
	case 'G':
		n = 5;
		break;
	case 'H':
		n = 6;
		break;
	case 'I':
		n = 7;
		break;
	case 'K':
		n = 8;
		break;
	case 'L':
		n = 9;
		break;
	case 'M':
		n = 10;
		break;
	case 'N':
		n = 11;
		break;
	case 'P':
		n = 12;
		break;
	case 'Q':
		n = 13;
		break;
	case 'R':
		n = 14;
		break;
	case 'S':
		n = 15;
		break;
	case 'T':
		n = 16;
		break;
	case 'V':
		n = 17;
		break;
	case 'W':
		n = 18;
		break;
	case 'Y':
		n = 19;
		break;
	}
	return n;
}

