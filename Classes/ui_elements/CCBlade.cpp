/*
 * CCBlade
 *
 * Copyright (c) 2011 - Ngo Duc Hiep
 * Copyright (c) 2012 - YangLe
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "CCBlade.h"

USING_NS_CC;

static inline CBPoint cgpSub(const CBPoint& v1, const CBPoint& v2)
{
    return cgp(v1.x - v2.x, v1.y - v2.y);
}

static inline float cgpDot(const CBPoint& v1, const CBPoint& v2)
{
    return v1.x*v2.x + v1.y*v2.y;
}

static inline float cgpLengthSQ(const CBPoint& v)
{
    return cgpDot(v, v);
}

static inline float cgpLength(const CBPoint& v)
{
    return sqrtf(cgpLengthSQ(v));
}

static inline float cgpDistance(const CBPoint& v1, const CBPoint& v2)
{
    return cgpLength(cgpSub(v1, v2));
}

static inline CBPoint cgpMult(const CBPoint& v, const float s)
{
    return cgp(v.x*s, v.y*s);
}

static inline CBPoint cgpAdd(const CBPoint& v1, const CBPoint& v2)
{
    return cgp(v1.x + v2.x, v1.y + v2.y);
}

static inline CBPoint cgpRotateByAngle(const CBPoint& v, const CBPoint& pivot, float angle)
{
    CBPoint r = cgpSub(v, pivot);
    float cosa = cosf(angle), sina = sinf(angle);
    float t = r.x;
    r.x = t*cosa - r.y*sina + pivot.x;
    r.y = t*sina + r.y*cosa + pivot.y;
    return r;
}

static inline float angle(const CBPoint &vect)
{
    if (vect.x == 0.0 && vect.y == 0.0) {
        return 0;
    }
    if (vect.x == 0.0) {
        return vect.y > 0 ? M_PI/2 : -M_PI/2;
    }
    if (vect.y == 0.0 && vect.x < 0) {
        return -M_PI;
    }
    float angle = atan(vect.y / vect.x);
    
    return vect.x < 0 ? angle + M_PI : angle;
}

static inline void populateBorderVertices(const CBPoint &p1, const CBPoint &p2, float d, CBPoint *o1, CBPoint *o2)
{
    float l = cgpDistance(p1, p2);
    float a = angle(cgpSub(p2, p1));
    *o1 = cgpRotateByAngle(cgp(p1.x+l, p1.y+d), p1, a);
    *o2 = cgpRotateByAngle(cgp(p1.x+l, p1.y-d), p1, a);
}

static inline void CBPointSet(CBPoint *p, float x, float y)
{
    p->x = x;
    p->y = y;
}

float CCBlade::getStroke()
{
    return _stroke;
}

void CCBlade::setStroke(float stroke)
{
    _stroke = stroke * CC_CONTENT_SCALE_FACTOR();
}

CCBlade::~CCBlade()
{
    CC_SAFE_RELEASE(_texture);
    free(_vertices);
    free(_texCoords);
}

#define POINT_LIMIT_MIN 3

CCBlade::CCBlade(const char *filePath, float stroke, int pointLimit)
: _texture(NULL)
{
    setColor(ccWHITE);
    setOpacity(255);
    setTexture(CCTextureCache::sharedTextureCache()->addImage(filePath));
    setStroke(stroke);
    CCAssert(pointLimit >= POINT_LIMIT_MIN, "point limit should be >= POINT_LIMIT_MIN");
    _pointLimit = pointLimit;
    
    // head point => 1 vertex, body point => 2 vertices, tail point => 1 vertex
    int vertexCount = 2*pointLimit-2;
    _vertices = (CBPoint *)calloc(vertexCount, sizeof(CBPoint));
    _texCoords = (CBPoint *)calloc(vertexCount, sizeof(CBPoint));
    _autoCleanup = false;
    
    // shader stuff
    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture_uColor));
    _uniformColor = glGetUniformLocation( getShaderProgram()->getProgram(), "u_color");
}

CCBlade* CCBlade::create(const char *filePath, float stroke, int pointLimit)
{
    CCBlade* pRet = new CCBlade(filePath, stroke, pointLimit);
        pRet->autorelease();
        return pRet;
}

void CCBlade::populateVertices()
{
    // head
    int i = 0;
    _vertices[0] = _path[i];
    CBPointSet(&_texCoords[0], 0.0, 0.5);
    i++;
    
    // body
    float strokeFactor = _stroke / _path.size();
    CBPoint previous = _vertices[0];
    while (i < _path.size()-1) {
        CBPoint current = _path[i];
        populateBorderVertices(previous, current, _stroke-i*strokeFactor, &_vertices[2*i-1], &_vertices[2*i]);
        CBPointSet(&_texCoords[2*i-1], 0.5, 1.0);
        CBPointSet(&_texCoords[2*i], 0.5, 0.0);
        
        previous = current;
        i++;
    }
    
    // modified for head
    CBPointSet(&_texCoords[1], 0.25, 1.0);
    CBPointSet(&_texCoords[2], 0.25, 0.0);
    
    // tail
    _vertices[2*i-1] = _path[i];
    CBPointSet(&_texCoords[2*i-1], 0.75, 0.5);
}

#define DISTANCE_TO_INTERPOLATE 10

void CCBlade::push(const CCPoint &point)
{
    CBPoint p = cgp(point.x, point.y);
    if (CC_CONTENT_SCALE_FACTOR() != 1.0) {
    	// Sebi: this doesn't work on Android , maybe we will need it in future for iPhone
        // p = cgpMult(p, CC_CONTENT_SCALE_FACTOR());
    }
    
    if (_path.size() == 0) {
        _path.insert(_path.begin(), p);
        return;
    }
    
    //cocos2d::CCLog("CCBlade point x:%d, y:%d", int(p.x), int(p.y));

    CBPoint first = _path[0];
    float distance = cgpDistance(p, first);
    if (distance < DISTANCE_TO_INTERPOLATE) {
        _path.insert(_path.begin(), p);
    } else {
        int count = distance / DISTANCE_TO_INTERPOLATE + 1;
        const CBPoint &interval = cgpMult(cgpSub(p, first), 1.0/count);
        for (int i = 1; i <= count; i++) {
            _path.insert(_path.begin(), cgpAdd(first, cgpMult(interval, i)));
        }
    }
    while (_path.size() > _pointLimit) {
        _path.pop_back();
    }
    
    populateVertices();
}

void CCBlade::pop(int count)
{
    while (_path.size() > 0 && count > 0) {
        _path.pop_back();
        count--;
    }
    
    if (_path.size() >= POINT_LIMIT_MIN) {
        populateVertices();
    }
}

void CCBlade::popLastOne(float delta)
{
    pop(1);
    if (_autoCleanup && _path.size() < POINT_LIMIT_MIN) {
        _path.clear();
        removeFromParentAndCleanup(true);
    }
}

float CCBlade::getDrainInterval()
{
    return _drainInterval;
}

void CCBlade::setDrainInterval(float drainInterval)
{
    _drainInterval = drainInterval;
    if (drainInterval > 0) {
        schedule(schedule_selector(CCBlade::popLastOne), drainInterval);
    } else {
        unschedule(schedule_selector(CCBlade::popLastOne));
    }
}

void CCBlade::autoCleanup()
{
    _autoCleanup = true;
    if (_drainInterval <= 0) {
        setDrainInterval(1.0/60);
    }
}

void CCBlade::draw()
{
    if (_path.size() < POINT_LIMIT_MIN) {
        return;
    }
    
    CC_NODE_DRAW_SETUP();
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
    
    const ccColor3B &realColor = getColor();
    GLfloat color[4] = { realColor.r/255.0f, realColor.g/255.0f, realColor.b/255.0f, getOpacity()/255.0f };
    getShaderProgram()->setUniformLocationWith4fv(_uniformColor, color, 1);
    ccGLBindTexture2D(_texture->getName());
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, _vertices);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, _texCoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*_path.size()-2);
    
    CC_INCREMENT_GL_DRAWS(1);
}
