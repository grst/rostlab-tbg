/*
 * ImpressumLayer.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#include "ImpressumLayer.h"
#include "../helper/HelperFunctions.h"
#include "../screens/MainScreenScene.h"
#include "../screens/SettingsScreenScene.h"
#include "../Globals.h"

using namespace cocos2d;

ImpressumLayer::ImpressumLayer() {
	// TODO Auto-generated constructor stub

}

ImpressumLayer::~ImpressumLayer() {
	// TODO Auto-generated destructor stub
}

bool ImpressumLayer::init() {

	bool bRet = false;
	do {
		CC_BREAK_IF(!CCLayer::init());
		this->setTouchEnabled(true);

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCLabelTTF* pauseLabel = CCLabelTTF::create("Imprint", "carrois", 24,
				CCSize(winSize.width / 4, 50), kCCTextAlignmentRight);
		CC_BREAK_IF(!pauseLabel);


		pauseLabel->setPosition(
				ccp(winSize.width  - pauseLabel->getContentSize().width / 2 - winSize.width * 1/ 16 ,
						winSize.height
								- pauseLabel->getContentSize().height / 2));

		// add this to the layer
		this->addChild(pauseLabel, 11);

		CCArray * menuIcons = CCArray::create();

		// close layer
		CCMenuItemImage *pCloseLayer = CCMenuItemImage::create(
				"grey/exit.png", "grey/exit.png", this,
				menu_selector(ImpressumLayer::OnMenu));
		pCloseLayer->setPosition(
				ccp(winSize.width * 2 / 5 + pCloseLayer->getContentSize().width,
						winSize.height - pCloseLayer->getContentSize().height));
		pCloseLayer->setTag(2);
		menuIcons->addObject(pCloseLayer);

		// next Button
		CCMenuItemImage *pNextButton = CCMenuItemImage::create(
				"grey/forward.png", "grey/forward.png", this,
				menu_selector(ImpressumLayer::OnMenu));
		pNextButton->setPosition(
				ccp(winSize.width * 3 / 5 + pNextButton->getContentSize().width,
						winSize.height - pNextButton->getContentSize().height));
		pNextButton->setTag(3);
		menuIcons->addObject(pNextButton);

		// Create a menu with our menu items
		levelMenu = CCMenu::createWithArray(menuIcons);
		levelMenu->setPosition(ccp(100, winSize.height - 25));
		levelMenu->alignItemsHorizontallyWithPadding(15);

		// Add the menu to TestWorld layer as a child layer.
		this->addChild(levelMenu, 11);

		CCLayerGradient* layer3 = CCLayerGradient::create();
		layer3->setContentSize(CCSizeMake(winSize.width, winSize.height));
		layer3->setPosition(ccp(0, 0));
		layer3->setStartColor(ccc3(100, 100, 100));
		layer3->setEndColor(ccc3(120, 120, 120));
		layer3->setStartOpacity(255);
		layer3->setEndOpacity(255);
		ccBlendFunc blend;
		blend.src = GL_SRC_ALPHA;
		blend.dst = GL_ONE_MINUS_SRC_ALPHA;
		layer3->setBlendFunc(blend);
		addChild(layer3, 10);
		textCounter = 0;
		getNextText();

		bRet = true;
	} while (0);

	return bRet;

}

void ImpressumLayer::getNextText() {

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCLOG("Starting remove");
	if (this->ttfDesc != NULL && textCounter > 0) {
		this->ttfDesc->removeFromParent();
		this->ttfTitle->removeFromParent();
	}
	CCLOG("Remove survided");

	std::string sDesc;
	MainScreenLayer *layer;
	std::string title;

	switch (textCounter) {

	case 0:
		title = "Realization";
		sDesc =
				"Proudly presented by Gregor Sturm & Sebastian Wilzbach.\n"
						"For questions, suggestions or support please write to tbg@rostlab.org [TODO!]";
		this->ttfDesc = CCLabelTTF::create(sDesc.c_str(), "carrois", 18,
				CCSizeMake(winSize.width * 4 / 6, winSize.height * 1 / 2),
				kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		break;

	case 1:
		title = "Publisher";
		sDesc = "rostlab - Unit XII\n"
				"TUM Fakultät für Informatik\n"
				"Boltzmannstraße 3\n"
				"85748 Garching";
		this->ttfDesc = CCLabelTTF::create(sDesc.c_str(), "carrois", 18,
				CCSizeMake(winSize.width * 4 / 6, winSize.height * 1 / 2),
				kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
		break;

	case 2:
			title = "Proteins";
			sDesc = "Structure and Sequence of the Proteins is taken from http://pdb.org.\n\n"
					"The descriptions texts are adapted from the PDB-101 \"Molecule of the Month\"-articles "
					"(http://pdb.org/pdb/101/motm_archive.do)";
			this->ttfDesc = CCLabelTTF::create(sDesc.c_str(), "carrois", 18,
					CCSizeMake(winSize.width * 5 / 6, winSize.height * 2 / 3),
					kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
			break;

	case 3:
		title = "Graphics";
		sDesc = "[CC-Icons, Google-Icons]";
		this->ttfDesc = CCLabelTTF::create(sDesc.c_str(), "carrois", 18,
				CCSizeMake(winSize.width * 4 / 6, winSize.height * 1 / 2),
				kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		break;

	case 4:
		title = "Music";
		sDesc = "Freesound.org";
		this->ttfDesc = CCLabelTTF::create(sDesc.c_str(), "carrois", 18,
				CCSizeMake(winSize.width * 4 / 6, winSize.height * 1 / 2),
				kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		break;

	case 5:
		title = "Libraries";
		sDesc =				"[cocos2dx, box2d, ccblade, …?]";
		this->ttfDesc = CCLabelTTF::create(sDesc.c_str(), "carrois", 18,
				CCSizeMake(winSize.width * 4 / 6, winSize.height * 1 / 2),
				kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		break;

	case 6:
		// go back to main menu
		layer = (MainScreenLayer*) this->getParent();
		layer->keyBackClicked();
		return;

	default:
		break;

	}

	if (this->ttfDesc != NULL) {

		//	ttfDesc->retain();
		this->ttfDesc->setColor(ccc3(0, 0, 0));
		this->ttfDesc->setPosition(
				ccp(winSize.width * 3 / 6, winSize.height * 3 / 5));
		this->addChild(ttfDesc, 12);

		this->ttfTitle = CCLabelTTF::create(title.c_str(), "carrois", 20,
				CCSizeMake(winSize.width * 4 / 6, 30), kCCTextAlignmentCenter,
				kCCVerticalTextAlignmentTop);
		this->ttfTitle->setColor(ccc3(0, 0, 0));
		this->ttfTitle->setPosition(
				ccp(winSize.width * 3 / 6, winSize.height * 11 / 12));
		this->addChild(ttfTitle, 12);

	}

	textCounter++;
}

void ImpressumLayer::OnMenu(CCObject* pSender) {

	CCMenuItem* pMenuItem = (CCMenuItemImage *) (pSender);
	int tag = (int) pMenuItem->getTag();

	CCLOG("ImpressumItem  %d", tag);
	SoundEffectHelper::playClickSound();

	CCScene *pScene1;
	MainScreenLayer *layer;
	switch (tag) {
	case 2:
		// simulate close Button clicked
		layer = (MainScreenLayer*) this->getParent();
		layer->keyBackClicked();
		break;
	case 3:
		getNextText();
		break;
	}
}

