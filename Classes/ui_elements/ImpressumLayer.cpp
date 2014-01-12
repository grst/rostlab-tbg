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
#include "../ui_elements/cc-extensions/CCGestureRecognizer/CCSwipeGestureRecognizer.h"

using namespace cocos2d;

ImpressumLayer::ImpressumLayer() {
	// TODO Auto-generated constructor stub

}

ImpressumLayer::~ImpressumLayer() {
	this->ttfDesc->removeFromParent();
	this->ttfTitle->removeFromParent();
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
				ccp(
						winSize.width - pauseLabel->getContentSize().width / 2
								- winSize.width * 1 / 16,
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
				ccp(winSize.width * 1 / 5 + pCloseLayer->getContentSize().width,
						winSize.height - pCloseLayer->getContentSize().height));
		pCloseLayer->setTag(2);
		pCloseLayer->setScale(0.7);
		menuIcons->addObject(pCloseLayer);

		/*
		// next Button
		CCMenuItemImage *pNextButton = CCMenuItemImage::create(
				"grey/forward.png", "grey/forward.png", this,
				menu_selector(ImpressumLayer::OnMenu));
		pNextButton->setPosition(
				ccp(winSize.width * 3 / 5 + pNextButton->getContentSize().width,
						winSize.height - pNextButton->getContentSize().height));
		pNextButton->setTag(3);
		menuIcons->addObject(pNextButton);
		*/

		// Create a menu with our menu items
		levelMenu = CCMenu::createWithArray(menuIcons);
		levelMenu->setPosition(ccp(25, winSize.height - 25));
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
		textCounter = -1;
		getNextText();

		// add swipe
		CCSwipeGestureRecognizer * swipe = CCSwipeGestureRecognizer::create();
		swipe->setTarget(this, callfuncO_selector(ImpressumLayer::didSwipe));
		swipe->setDirection(
				kSwipeGestureRecognizerDirectionRight
						| kSwipeGestureRecognizerDirectionLeft);
		swipe->setCancelsTouchesInView(true);
		this->addChild(swipe, 13);

		//paging
		updatePaging(0);

		bRet = true;
	} while (0);

	return bRet;

}

void ImpressumLayer::getPreviousText() {
	if (textCounter > 0) {
		textCounter--;
		updateImg(textCounter, false);
	}
}

void ImpressumLayer::getNextText() {
	CCLog("get next text");
	textCounter++;
	updateImg(textCounter, true);
}

void ImpressumLayer::updatePaging(int counter){
	int PAGING_ELEMENTS = 6;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	// do clean up = removeAll
	CCLog("updating pager to %d", counter);
	if(pagings.size() > 0){
		for(int i=pagings.size()-1; i>= 0; i--){
			CCSprite *a = pagings[i];
			if(a != NULL){
				pagings.erase(pagings.end()-1);
				a->removeFromParent();
			}
		}
	}
	for(int i=0; i< PAGING_ELEMENTS; i++){
		CCSprite * indie;
		if(i == counter){
			indie =CCSprite::create("paging-active.png");
		}else{
			indie =CCSprite::create("paging-inactive.png");
		}
		indie->setPosition(ccp(winSize.width / 2 -(PAGING_ELEMENTS / 2 * 15) + i * 15,20));
		addChild(indie, 10);
		pagings.push_back(indie);
	}
}

void ImpressumLayer::updateImg(int count, bool direction) {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCLOG("Starting sliding remove");

	float slideTime = 0.25;
	float bondary = winSize.width * 1 / 2;

	// removing old image via swipe
	if (this->ttfDesc != NULL && (textCounter > 0  || (textCounter == 0 && direction == false))) {
		CCLOG("Doing a swipe");
		CCFiniteTimeAction* actionMoveTitle;
		CCFiniteTimeAction* actionMoveDesc;
		if (direction) {
			actionMoveDesc = CCMoveTo::create((float) slideTime,
					ccp(-bondary, winSize.height * 3 / 5));
			actionMoveTitle = CCMoveTo::create((float) slideTime,
								ccp(-bondary, winSize.height * 11 / 12));
		} else {
			actionMoveDesc = CCMoveTo::create((float) slideTime,
					ccp(winSize.width+  bondary, winSize.height * 3 / 5));
			actionMoveTitle = CCMoveTo::create((float) slideTime,
											ccp(winSize.width+bondary, winSize.height * 11 / 12));
		}
		CCSequence* seqDesc = CCSequence::create(actionMoveDesc,
				CCCallFunc::create(ttfDesc,
						callfunc_selector(CCSprite::removeFromParent)), NULL);
		CCSequence* seqTitle = CCSequence::create(actionMoveTitle,
				CCCallFunc::create(ttfTitle,
						callfunc_selector(CCSprite::removeFromParent)), NULL);
		ttfDesc->runAction(seqDesc);
		ttfTitle->runAction(seqTitle);
	}

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
		sDesc =
				"Structure and Sequence of the Proteins is taken from http://pdb.org.\n\n"
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
		sDesc = "[cocos2dx, box2d, ccblade, …?]";
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

	if (this->ttfDesc != NULL) {

		// set starting point for swip
		if (direction) {
			//next
			ttfTitle->setPosition(
					ccp(winSize.width + bondary, winSize.height * 11 / 12));
			ttfDesc->setPosition(
					ccp(winSize.width + bondary, winSize.height * 3 / 5));
		} else {
			//previous
			ttfTitle->setPosition(ccp(-bondary, winSize.height * 11 / 12));
			ttfDesc->setPosition(ccp(-bondary, winSize.height  * 3 / 5));
		}

		// slide in
		CCSequence * readySequenceTitle = CCSequence::create(
				CCMoveTo::create((float) slideTime,
						ccp(winSize.width * 3 / 6, winSize.height * 11 / 12)), NULL, NULL
		);
		ttfTitle->runAction(readySequenceTitle);

		CCSequence * readySequenceDesc = CCSequence::create(
				CCMoveTo::create((float) slideTime,
						ccp(winSize.width * 3 / 6, winSize.height * 3 / 5)), NULL, NULL
		);
		ttfDesc->runAction(readySequenceDesc);

		// update paging
		updatePaging(textCounter);

	}

}

void ImpressumLayer::didSwipe(CCObject * pSender) {
	CCSwipe * swipe = (CCSwipe *) pSender;
	// recognize swipe to the left
	CCLog("got swipe event");
	if (swipe->direction == kSwipeGestureRecognizerDirectionLeft) {
		if (textCounter == 5) {
			// set a delay
			this->runAction(
					CCSequence::create(CCDelayTime::create(0.1),
							CCCallFunc::create(this,
									callfunc_selector(
											ImpressumLayer::getNextText)),
							NULL));
		} else {
			getNextText();
		}
	} else if (swipe->direction == kSwipeGestureRecognizerDirectionRight) {
		getPreviousText();
	}
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
		//layer = (MainScreenLayer*) this->getParent();
	//	layer->keyBackClicked();
		textCounter = 5;
		this->runAction(
							CCSequence::create(CCDelayTime::create(0.1),
									CCCallFunc::create(this,
											callfunc_selector(
													ImpressumLayer::getNextText)),NULL));
		break;
	case 3:
		getNextText();
		break;
	}
}

