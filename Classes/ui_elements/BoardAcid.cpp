/*
 * BoardAcid.cpp
 *
 *  Created on: Oct 6, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#include "BoardAcid.h"

BoardAcid::BoardAcid() {
}

BoardAcid::~BoardAcid() {
	if(_label){
		_label->release();
		_label = NULL;
	}
}

void BoardAcid::setAcid(char c){
	acid = c;
}

