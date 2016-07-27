#include "CMPiket.h"
//#include "OgreManualObject.h"
#include "CMAssertions.h"
#include "wykobi_wrap.h"
#include "CMDebug.h"

using namespace wykobi;
using namespace Ogre;
using namespace std;

namespace CM {

struct PreperedForClassifyWall {
	PreperedForClassifyWall(const Wall* raw, V3 rotRelPos);
	// является ли внешней для ассоциированного пикета
    bool fixed;
    const Wall* raw;
    V3 rotRelPos;
    // угол вдоль Z
    Radian alongZAngle;
    // угол вокруг Z (от X0)
    Radian crossZAngle;
};

bool compareWallsByRotatedZ(const PreperedForClassifyWall& w1, const PreperedForClassifyWall& w2) {
    return w1.rotRelPos.z < w2.rotRelPos.z;
}

PreperedForClassifyWall::PreperedForClassifyWall(const Wall* raw, V3 rotRelPos)
: raw(raw)
, rotRelPos(rotRelPos)
, fixed(false) {
    alongZAngle = rotRelPos.angleBetween(V3::UNIT_Z);
    crossZAngle = V2(rotRelPos.x, rotRelPos.y).angleTo(V2::UNIT_X);
}

void Piket::addP3D(const PiketInfo& piket) {
	allP3D.push_back(piket);
}

void Piket::addW3D(long long parentPiket, const Wall w3d) {
	for (int i = 0; i < allP3D.size(); i++) {
		if (allP3D[i].id == parentPiket) {
			allP3D[i].hasWalls ++;
            break;
        }
    }
    allWalls.push_back(w3d);
}


void Piket::preProcessWalls(const CaveViewPrefs& caveViewPrefs) {
    classifyWalls();
    recalcPosCenterDirrection();
    processPiketPosAsWall();
//    updateEffectivePos();
//    updateDirrection();
//    updateWallsCenter();
    propagateWalls(caveViewPrefs.wallsPropagateMode, caveViewPrefs.wallsBlowMode);
    adjFakePikets.clear();
}

void Piket::recalcPosCenterDirrection() {
    updateEffectivePos();
    updateDirrection();
    updateWallsCenter();
}

void Piket::updateEffectivePos() {
	V3 center(0, 0, 0);
	if (!classifiedWalls.empty()) {
        center =  V3(0, 0, 0);
        for (int i = 0; i < classifiedWalls.size(); i++) {
            center += classifiedWalls[i].pos;
        }
        center /= classifiedWalls.size();
    }
    // смещаем пикет немного в сторону чтобы не было стен прямо на пикете, откуда были бы проблемы с углами
    piketEffectivePos = pos + (center - pos).normalisedCopy() * 1.0f;
}

void Piket::updateWallsCenter() {
    if (!classifiedWalls.empty()) {
        wallsCenter =  V3(0, 0, 0);
        for (int i = 0; i < classifiedWalls.size(); i++) {
            wallsCenter += classifiedWalls[i].pos;
        }
        wallsCenter /= classifiedWalls.size();
    } else {
        wallsCenter = piketEffectivePos;
    }
//    wallsCenter = piketEffectivePos;
//    wallsMassCenter = getWallsMassCenter(dirrection);
}

void Piket::updateDirrection() {
    dirrection = V3::ZERO;

    if (adjPikets.size() >= 1 && adjPikets.size() <= 2) {
		dirrection += (adjPikets.front()->pos - pos).normalisedCopy();
    }
    if (adjPikets.size() == 2) {
		dirrection += (pos - adjPikets.back()->pos).normalisedCopy();
    }

    for (int i = 0; i < classifiedWalls.size(); i++) {
        for (int j = i + 1; j < classifiedWalls.size(); j++) {
            V3 ish = (classifiedWalls[i].pos - piketEffectivePos);
            V3 jsh = (classifiedWalls[j].pos - piketEffectivePos);
            V3 norm = ish.crossProduct(jsh);
            norm.normalise();

            if (!dirrection.isZeroLength() && norm.angleBetween(dirrection) > Radian(M_PI_2)) {
                norm = -norm;
            }
            dirrection += norm;
        }
    }
    dirrection.normalise();

}

//V3 OgrePiket::getWallsMassCenter(V3 dirrection) {
//    if (!dirrection.isZeroLength()) {
//        Quaternion dirrectionRotation = dirrection.getRotationTo(V3::UNIT_Z);
//        std::vector<WallProj> walls2d = getWalls2d(piketEffectivePos, dirrection, dirrection, classifiedWalls);
//
//        V2 polyCenter2d = polyCenter(walls2d);
//        V3 polyCenter3d = piketEffectivePos + dirrectionRotation.Inverse() * V3(polyCenter2d.x, polyCenter2d.y, 0);
//
//        return polyCenter3d;
//    } else {
//        return piketEffectivePos;
//    }
//}

void Piket::processPiketPosAsWall() {
    if (classifiedWalls.empty()) return;

    std::vector<WallProj> rotWalls = getWalls2d(wallsCenter, dirrection, dirrection, classifiedWalls);

    for (int i = 0; i < classifiedWalls.size(); i++) {
        if (classifiedWalls[i].pos.distance(pos) < 1) {
            return;
        }
    }

    Quaternion dirrectionRotation = dirrection.getRotationTo(V3::UNIT_Z);
	V3 rotPiketPos(pos);
    rotPiketPos -= wallsCenter;
    rotPiketPos = dirrectionRotation * rotPiketPos;
    point2d<float> rotPiketProjPos(rotPiketPos.x, rotPiketPos.y);

    std::vector<point2d<float> > wallsPolygon2d;
    for (int i = 0; i < rotWalls.size(); i++) {
        const V2& v2 = rotWalls[i].posBySelfDir;
        wallsPolygon2d.push_back(point2d<float>(v2.x, v2.y));

        //debugDraw(V3(rotPiketProjPos.x, rotPiketProjPos.y, -100), V3(v2.x, v2.y, -100));
    }
    polygon<float, 2> wallsPolygon = make_polygon(wallsPolygon2d);


    if (!point_in_polygon(rotPiketProjPos, wallsPolygon)) {
        classifiedWalls.push_back(PiketWall(pos));
        recalcPosCenterDirrection();
    }
}

void Piket::propagateWalls(WallsPropagateMode propMode, WallsBlowMode blowMode) {
    if (classifiedWalls.empty()) return;

    std::vector<WallProj> rotWalls = getWalls2d(wallsCenter, dirrection, dirrection, classifiedWalls);
    std::vector<PiketWall> newWalls;

    for (int i = 0; i < rotWalls.size(); i++) {
        int j = (i + 1) % rotWalls.size();

        V3 iPos = classifiedWalls[rotWalls[i].idx].pos - wallsCenter;
        V3 jPos = classifiedWalls[rotWalls[j].idx].pos - wallsCenter;

        int addWallsNum = 0;
        if (propMode == WPM_X2) addWallsNum = 1;
        else if (propMode == WPM_X4) addWallsNum = 2;
        else if (propMode == WPM_1M) addWallsNum = (jPos - iPos).length() / 100 / 1;
        else if (propMode == WPM_2M) addWallsNum = (jPos - iPos).length() / 100 / 2;
        else if (propMode == WPM_4M) addWallsNum = (jPos - iPos).length() / 100 / 4;
        else if (propMode == WPM_10D) addWallsNum = (jPos.angleBetween(iPos)).valueDegrees() / 5;
        else if (propMode == WPM_20D) addWallsNum = (jPos.angleBetween(iPos)).valueDegrees() / 20;
        else if (propMode == WPM_30D) addWallsNum = (jPos.angleBetween(iPos)).valueDegrees() / 30;

        std::vector<PiketWall> addinWalls;
        if (blowMode == WBM_NONE
         || blowMode == WBM_LINEAR
         || blowMode == WBM_COS2PI
         || blowMode == WBM_COSCOS2PI) {
            addinWalls = propagateWallAngleAbove(rotWalls[i].idx, rotWalls[j].idx, addWallsNum, blowMode);
        } else if (blowMode == WBM_BESIER3) {
            int wallsNum = rotWalls.size();
            int h = (wallsNum + i - 1) % wallsNum;
            int k = (wallsNum + j + 1) % wallsNum;
            addinWalls = propagateWallBesier3(rotWalls[h].idx, rotWalls[i].idx, rotWalls[j].idx, rotWalls[k].idx, addWallsNum);
        }

        newWalls.insert(newWalls.end(), addinWalls.begin(), addinWalls.end());
    }
    classifiedWalls.insert(classifiedWalls.end(), newWalls.begin(), newWalls.end());
}

std::vector<PiketWall> Piket::propagateWallAngleAbove(int wallId1, int wallId2, int addWallsNum, WallsBlowMode blowMode) {
    std::vector<PiketWall> result;

    AssertReturn(classifiedWalls.size() > wallId1, return result);
    AssertReturn(classifiedWalls.size() > wallId2, return result);

    V3 iPos = classifiedWalls[wallId1].pos - wallsCenter;
    V3 jPos = classifiedWalls[wallId2].pos - wallsCenter;

    for (int addWallIdx = 1; addWallIdx <= addWallsNum; addWallIdx++) {
        float offset = (float)addWallIdx / (addWallsNum + 1);
        V3 pos = iPos + (jPos - iPos) * offset;
        if (blowMode == WBM_NONE) {
            pos = wallsCenter + pos;
        } else {
            float sinOffset = 0.5f;

            if (blowMode == WBM_LINEAR) sinOffset = offset;
            else if (blowMode == WBM_COS2PI) sinOffset = sinusate(offset);
            else if (blowMode == WBM_COSCOS2PI) sinOffset = sinusate(sinusate(offset));

            pos = wallsCenter + pos.normalisedCopy() * (iPos.length()*(1.0f - sinOffset) + jPos.length()*sinOffset);
        }

        result.push_back(PiketWall(pos));
    }
    return result;
}

std::vector<PiketWall> Piket::propagateWallBesier3(int h, int i, int j, int k, int addWallsNum, float strong) {
    std::vector<PiketWall> result;
    int wallsNum = classifiedWalls.size();
   // addWallsNum = 20;

    AssertReturn(wallsNum > h, return result);
    AssertReturn(wallsNum > i, return result);
    AssertReturn(wallsNum > j, return result);
    AssertReturn(wallsNum > k, return result);

    V3 hPos = classifiedWalls[h].pos  ;
    V3 iPos = classifiedWalls[i].pos ;
    V3 jPos = classifiedWalls[j].pos;
    V3 kPos = classifiedWalls[k].pos  ;

    V3 icPos = (iPos + (iPos - hPos)) * 0.5f + jPos * 0.5f;
    V3 jcPos = (jPos + (jPos - kPos)) * 0.5f + iPos * 0.5f;

    float strongi = strong * pow(Math::Sin((hPos-iPos).angleBetween((jPos-iPos))/2), 0.5f);
    float strongj = strong * pow(Math::Sin((kPos-jPos).angleBetween((iPos-jPos))/2), 0.5f);

    icPos = iPos + (icPos - iPos).normalisedCopy() * (iPos - jPos).length() * strongi;
    jcPos = jPos + (jcPos - jPos).normalisedCopy() * (iPos - jPos).length() * strongj;

//    if (i != 4) return result;

	Debug::inst()->drawLine(iPos, icPos, Color::Red);
//	debugManualObject->position(iPos);
//	debugManualObject->colour();
//	debugManualObject->position(icPos);

	Debug::inst()->drawLine(jPos, jcPos, Color::Red);
//	debugManualObject->position(jPos);
//	debugManualObject->position(jcPos);

    for (int addWallIdx = 1; addWallIdx <= addWallsNum; addWallIdx++) {
        double t = (float)addWallIdx / (addWallsNum + 1);
        V3 pos = iPos * (1.0f-t)*(1.0f-t)*(1.0f-t) + 3.0f * icPos * t*(1.0f-t)*(1.0f-t) + 3.0f * jcPos * t*t*(1.0f-t) + jPos * t*t*t;
        result.push_back(PiketWall(pos));
    }
    return result;
}

void Piket::classifyWalls() {
    classifiedWalls.clear();
	for (int i = 0; i < allWalls.size(); i++) {
		//if (!allWalls.at(i)->ignoreAt3d) {
			classifiedWalls.push_back(PiketWall(allWalls.at(i).pos));
		//}
	}

//    piket->checkWallsCenter();
//
//    std::vector<int>::const_iterator assocPikIt = piket->adjPikets.begin();
//    while(true) {
//        if (assocPikIt == piket->adjPikets.end()) assocPikIt = piket->adjPikets.begin();
//        if (assocPikIt == piket->adjPikets.end()) break;
//
//        Piket* assocPiket = getPiket(*assocPikIt);
//        assocPiket->checkWallsCenter();
//
//        V3 dirrection = piket->wallsCenter - assocPiket->wallsCenter;
//        Quaternion dirrectionRotation = dirrection.getRotationTo(V3::UNIT_Z);
//
//        // массив повернутых относительных расстояний до стен
//        // чем больше Z тем дальше от assocPiket
//        std::vector<PreperedForClassifyWall> rotatedWalls;
//        for (int i = 0; i < piket->allWalls.size(); i++) {
//            V3 rotRelPos = dirrectionRotation * (piket->allWalls[i]->pos - piket->wallsCenter);
//            rotatedWalls.push_back(PreperedForClassifyWall(piket->allWalls[i], rotRelPos));
//        }
//
//        std::sort(rotatedWalls.begin(), rotatedWalls.end(), &compareWallsByRotatedZ);
//
//        // массив индексов стен уже ассоциированных с assocPiket;
////        vector<int> fixedWalls;
////        for (i = 0; i < rotatedWalls.size(); i++) {
////
////
////        }
//    }
}

bool Piket::isInactive() const {
	return !hasNoPriz(MARK_Z_SURVEY);
}

bool Piket::hasPriz(PiketMark priz) const {
    for (int i = 0; i < allP3D.size(); i++) {
        int res = allP3D[i].priz & priz;
        if (res > 0) return true;
    }

    return false;
}

bool Piket::hasNoPriz(PiketMark priz) const {
    for (int i = 0; i < allP3D.size(); i++) {
        int res = allP3D[i].priz & priz;
        if (res == 0) return true;
    }

    return false;
}

Color Piket::getColorOfP3DWithPriz(PiketMark priz) const {
    for (int i = 0; i < allP3D.size(); i++) {
        if(priz & allP3D[i].priz) {
            return allP3D[i].col;
        }
    }
    return Color::Green;
}

PiketMark Piket::getSumPriz() const {
    PiketMark res = MARK_NONE;
    for (int i = 0; i < allP3D.size(); i++) {
       res = (PiketMark)(res | allP3D[i].priz);
    }
    return res;
}

Color Piket::getPrevailWallColor() const {
    Color col = Color::Green;
    int walls = 0;
    for (int i = 0; i < allP3D.size(); i++) {
        if (allP3D[i].hasWalls >= walls) {
            walls = allP3D[i].hasWalls;
            col = allP3D[i].col;
        }
    }
    return col;
}

std::vector<const Piket*> Piket::getAdjPiketsWithPriz(PiketMark prz) const {
	std::vector<const Piket*> res;
    for (int i = 0; i < adjPikets.size(); i++) {
		const Piket* piket = adjPikets[i];
        if (piket && piket->hasPriz(MARK_Z_SURVEY)) res.push_back(piket);
    }
    return res;
}

std::vector<const Piket*> Piket::getAdjPiketsWithoutPriz(PiketMark prz) const {
	std::vector<const Piket*> res;
    for (int i = 0; i < adjPikets.size(); i++) {
        const Piket* piket = adjPikets[i];
		if (piket && !piket->hasPriz(MARK_Z_SURVEY)) res.push_back(piket);
    }
    return res;
}

//void Piket::debugDraw(V3 a, V3 b, Color col) {
//	DEBUG_DRAW(a, b col);
////	debugManualObject->position(a);
////	debugManualObject->colour(col);
////	debugManualObject->position(b);
//}

float Piket::getMaxDimension() const {
    float maxDist = 0;
    for (int i = 0; i < classifiedWalls.size(); i++) {
        for (int j = i + 1; j < classifiedWalls.size(); j++) {
            float dist = classifiedWalls[i].pos.distance(classifiedWalls[j].pos);
            maxDist = std::max(dist, maxDist);
        }
    }
    return maxDist;
}

std::string Piket::getLabel() const {
	if (!allP3D.empty()) {
    	return "";
	}
	return allP3D.front().label;
}

std::string Piket::getName() const {
	std::string name;
	for (int i = 0; i < allP3D.size(); i++) {
		name += allP3D[i].name + " ";
	}
	return name;
//	if (allP3D.empty()) {
//		return "";
//	}
//	return allP3D.front().name;
}
}
