
#include "CCSlider.h"

namespace cocos2dExt
{

	CCSlider::CCSlider():m_minValue(0),m_maxValue(0),m_value(0),m_target(NULL),m_selector(NULL),m_trackingTouch(false),m_trackTouchOutsideContent(false),m_evaluateFirstTouch(false),m_enabled(true),
		                 m_width(0),m_height(0),m_track(NULL),m_debug(false),m_knob(NULL)
    {
        // do nothing
    }

	CCSlider* CCSlider::SliderWithFiles(const char* trackFile, const char* knobFile, CCObject* target, SEL_MenuHandler selector)
	{
		CCSlider* slider = new CCSlider();
		if (slider->InitWithFiles(trackFile, knobFile, target, selector))
		{
			slider->autorelease();
			return slider;
		}
		else
		{
			CC_SAFE_DELETE(slider);
			return NULL;
		}
	}

	CCSlider* CCSlider::SliderWithSprites(CCSprite* trackSprite, CCSprite* knobSprite, CCObject* target, SEL_MenuHandler selector)
	{
		CCSlider* slider = new CCSlider();
		if (slider->InitWithSprites(trackSprite, knobSprite, target, selector))
		{
			slider->autorelease();
			return slider;
		}
		else
		{
			CC_SAFE_DELETE(slider);
			return NULL;
		}
	}

	//! init from files
	bool CCSlider::InitWithFiles(const char* trackFile, const char* knobFile, CCObject* target, SEL_MenuHandler selector)
	{
		//if (CCNode::init())
		{
			m_track = CCSprite::create(trackFile);
			m_knob = CCSprite::create(knobFile);
			if (!m_track || !m_knob) return false;

			m_target = target;
			m_selector = selector;

			m_minValue = 0;
			m_maxValue = 100;

			// NOTE: Attention the addChild sequence
			addChild(m_track); 
			addChild(m_knob);

			m_width = m_track->getContentSize().width;

			m_height = 45;

			m_enabled = false;

			return true;
		}
		//return false;
	}

	void CCSlider::draw(){
		if(m_debug){
		ccDrawRect(ccp(-m_width *0.5f, -20),
						ccp(m_width *0.5f , 20));
		}
	}

	//! init from sprites
	bool CCSlider::InitWithSprites(CCSprite* trackSprite, CCSprite* knobSprite, CCObject* target, SEL_MenuHandler selector)
	{
		//if (CCLayer::init())
		{
			m_track = trackSprite;
			m_knob = knobSprite;
			if (!m_track || !m_knob) return false;

			m_target = target;
			m_selector = selector;

			m_minValue = 0;
			m_maxValue = 100;

			// NOTE: Attention the addChild sequence
			addChild(m_track); 
			addChild(m_knob);

			m_width = m_track->getContentSize().width;
			m_height = 45;

			m_enabled = false;

			return true;
		}
		//return false;
	}

	void CCSlider::SetValue(float value)
	{
		if (!m_enabled) return;

		// clamp values
		value = value < m_minValue ? m_minValue : value;
		value = value > m_maxValue ? m_maxValue : value;

		m_value = value;
		float x = (m_value - m_minValue) / (m_maxValue - m_minValue) * m_width;
		m_knob->setPosition(ccp(x - m_width * 0.5f, m_knob->getPosition().y));

		// do the callback
		if (m_target) 
		{
			(m_target->*m_selector)(this);
		}
	}




	CCPoint& CCSlider::getRelative(CCPoint& p){

		CCPoint r =  CCPoint(this->getPositionX()-p.x,	this->getPositionY()-p.y);

		CCLog("Relative x:%f, y:%f", r.x, r.y);
		return r;

	}

	bool CCSlider::ccTouchBegan(CCTouch* pTouch, CCEvent *pEvent)
	{
		if (!this->isVisible() || !m_enabled) {
			return false;
		}

		CCPoint touchPoint = getTouchPoint(pTouch);

		if (containsPoint(touchPoint))
		{
			if (m_evaluateFirstTouch) 
			{
				setValueByX(getRelative(touchPoint).x);
				m_trackingTouch = true;
				return true;
			}
			else 
			{
				m_trackingTouch = knobTouched(touchPoint);
				return m_trackingTouch;
			}

		}

		return false;
	}

	void CCSlider::ccTouchMoved(CCTouch* pTouch, CCEvent *pEvent)
	{
		if (! this->isVisible() || !m_enabled) return;

		if (m_trackingTouch)
		{
			CCPoint touchPos = getTouchPoint(pTouch);

			if (m_trackTouchOutsideContent) 
			{
				// setValueByX(getRelative(touchPos).x);
			}
			else 
			{
				if(containsPoint(touchPos))
				{
					setValueByX(getRelative(touchPos).x);
				}
				else 
				{
					m_trackingTouch = false;
				}
			}
		}
	}

	void CCSlider::ccTouchEnded(CCTouch* pTouch, CCEvent *pEvent)
	{
		m_trackingTouch = false;
	}

	void CCSlider::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		m_trackingTouch = false;
	}

	void CCSlider::onEnter()
	{
		CCNode::onEnter();
		// haha is this hacky :P
		cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	}
	
	void CCSlider::onExit()
	{
		cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
		CCNode::onExit();
	}

	// get touch point
	CCPoint CCSlider::getTouchPoint(CCTouch* pTouch)
	{
		CCPoint location = pTouch->getLocationInView();
		location = CCDirector::sharedDirector()->convertToGL(location);
		return location;
	}

	// check contain given point
	bool CCSlider::containsPoint(const CCPoint& point)
	{
		CCPoint pos = convertToNodeSpace(point);
		//  plus/minus 30 for normal hands
		CCRect rect = CCRectMake(-m_width * 0.5f  , -m_height * 0.5f , m_width +50, m_height +50 ); // NOTE: attention here

		rect = CCRectMake(-m_width *0.5f, -20,
				m_width *0.5f , 20);


		CCPoint origin =  ccp(-m_width *0.5f, -20);
		CCPoint from =	ccp(m_width *0.5f , 20);

		CCLog("To x: %f, y:%f ", origin.x , origin.y);
		CCLog("From x: %f, y:%f ", from.x, from.y);

		if ( origin.x <= pos.x &&  pos.x <= from.x  &&
				origin.y <= pos.y && pos.y <= from.y) {
			CCLOG("Pos x: %f, y:%f included", pos.x, pos.y);
			return true;
		}
		else{
			CCLOG("Pos x: %f, y:%f NOT included", pos.x, pos.y);
		}
		return false;
	}

	// set value by x coordinate
	void CCSlider::setValueByX(float xpos)
	{

		CCLog("Setting value to %d", xpos);
		//xpos = -xpos;
		xpos = xpos < -m_width * 0.5f ? - m_width * 0.5f : xpos;
		xpos = xpos > m_width * 0.5f ? m_width * 0.5f : xpos;

		CCLog("Recab %d", xpos);

		m_knob->setPosition(ccp(xpos, m_knob->getPosition().y));
		m_value = (xpos + m_width * 0.5f) / m_width * (m_maxValue - m_minValue) + m_minValue;
		// do callback
		if (m_target) 
		{
			(m_target->*m_selector)(this);
		}
	}

	bool CCSlider::knobTouched(const CCPoint& point)
	{
		if (containsPoint(point)) 
		{
			CCPoint pos = convertToNodeSpace(point);
			//return  fabs(m_knob->getPosition().x - pos.x) < m_knob->getContentSize().width * 0.5f;
			return true;
		}
		return false;
	}

}
