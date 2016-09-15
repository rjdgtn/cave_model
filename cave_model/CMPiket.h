#pragma once
#include "CMTypes.h"
#include "CMHelpers.h"

namespace CM {

struct Piket {
public:
	Piket(PiketInfo info/*V3 pos*/) :
	id(info.id),
	pos(info.pos),
	wallsCenter(0, 0, 0),
	//wallsMassCenter(0, 0, 0),
	piketEffectivePos(0, 0, 0),
	dirrection(0, 0, 0) {
		allP3D.push_back(info);
	}

    void preProcessWalls(const CaveViewPrefs& caveViewPrefs);
    void recalcPosCenterDirrection();
	bool isInactive() const;

	void addP3D(const PiketInfo& piket);
	void addW3D(long long parentPiket, const Wall w3d);

	// ВНИМАНИЕ
    // hasPriz(priz) != !hasNoPriz(priz) в общем случае !!!
    // ВНИМАНИЕ
	bool hasPriz(PiketMark priz) const;
	bool hasNoPriz(PiketMark priz) const;

	std::vector<const Piket*> getAdjPiketsWithPriz(PiketMark prz) const;
	std::vector<const Piket*> getAdjPiketsWithoutPriz(PiketMark prz) const;
	PiketMark getSumPriz() const;

	Color getPrevailWallColor() const;
	Color getColorOfP3DWithPriz(PiketMark priz) const;

	float getMaxDimension() const;
	std::string getName() const;
	std::string getLabel() const;
  //	const V3& pos() const { return allP3D.front().pos; }

	struct LeftRight { V3 left; V3 right; };
	LeftRight getCornerCutPoints(V3 lookDirection, V3 orientation) const;
	std::vector<LineBesier3> getCutBezier3() const;

protected:
    void processPiketPosAsWall();
    void updateEffectivePos(); // рассчитать центр стен
    void updateWallsCenter(); // рассчитать центр стен
    void updateDirrection(); // рассчитать нормаль к сечению хода образованному стенами
    void propagateWalls(WallsPropagateMode propMode, WallsBlowMode blowMode); // размножить стены
    std::vector<PiketWall> propagateWallAngleAbove(int wallId1, int wallId2, int num, WallsBlowMode blowMode ) const; // алгорим размножения стен на основе угламежду ими и центром
    std::vector<PiketWall> propagateWallBesier3(int h, int i, int j, int k, int addWallsNum, float strong = 0.4f) const; // алгоритм размножения стен на основе безье 3 порядка
	LineBesier3 getCutSegmentBesier3(int h, int i, int j, int k, float strong = 0.4f) const; // строит безье для сегмента стены

    void classifyWalls(); // разделить на внутренние и внешние стены
    //V3 getWallsMassCenter(V3 dirrection); // РАБОТАЕТ НЕПРАВИЛЬНО получить центр масс многоугольника стен

//    void debugDraw(V3 a, V3 b, Ogre::ColourValue col = Ogre::ColourValue(1, 1, 0, 1));

public:
	int id;
	V3 pos;
	std::vector<const Piket*> adjPikets;
	std::vector<const Piket*> adjFakePikets;
	std::vector<Wall> allWalls;
	std::vector<PiketInfo> allP3D;
    V3 wallsCenter;
    //V3 wallsMassCenter;
    V3 piketEffectivePos;
    V3 dirrection;

	std::vector<PiketWall> classifiedWalls;
};

}

