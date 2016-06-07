#pragma once
#include "ogre/OgreVector3.h"

namespace CM {

typedef Ogre::Vector3 V3;
typedef Ogre::Vector2 V2;


enum GenerateWallsForNoWallsPiketsMode { //���� ����� ����� �������� �� ������� ���� ������ ��� ����
    GWNWPM_NONE,     // ������ �� ������� ����� ����� �� �� �����������
    GWNWPM_SKIP,     // ������ �� ������� ����������� ����� ����� ��� ��� ����� ��� ��������
    GWNWPM_TRAIL,    // ����� ��������������� �� ������� ��� ���� � ������� ����� �� ����
    GWNWPM_BUDGE,    // ����� ��������������� �� ���������� ���, ����� ��� ������������� ������ ���� ������ ������
    GWNWPM_NUM
};

typedef GenerateWallsForNoWallsPiketsMode FakeWallsMode;

enum WallsPropagateMode {
    WPM_NONE,
    WPM_4M, // add wall every 4 meters
    WPM_2M, // add wall every 2 meters
    WPM_1M, // add wall every 1 meters
    WPM_NUM,
    WPM_X2, // twice walls num
    WPM_X4, // x4 walls num
    WPM_10D, // add wall every 10 degree
    WPM_20D, // add wall every 20 degree
    WPM_30D, // add wall every 30 degree
};

enum WallsShadowMode {
    WSM_ROUGH,   // ������� ���� �� ������
	WSM_SMOOTH,  // �������(��������) �������� ����
    WSM_NUM
};

enum WallsBlowMode {
    WBM_NONE,      // ��� �������� ������, ���, ����� ����� �� ������ ����� ��������
    WBM_BESIER3,   // ������������� ����� 2 �������
    WBM_NUM,
    WBM_LINEAR,    // ���������� ��������������� ��������
    WBM_COS2PI,    // ����������� � ������ ����������
    WBM_COSCOS2PI, // ������ ����������� � ������ ���������� (������ �������� �����)
};

enum WallsTrianglesBlowMode {
    WTBM_NONE,      // ��� �������� �������������
    WTBM_4,         // ������ ����������� ���������� 4��
    WTBM_9,         // ������ ����������� ���������� 9�
    WTBM_16,        // ������ ����������� ���������� x16
    WTBM_24,        // ������ ����������� ���������� x24
    WTBM_33,        // ������ ����������� ���������� x22
    WTBM_44,        // ������ ����������� ���������� x44
    WTBM_57,        // ������ ����������� ���������� x57
    WTBM_NUM,
};

enum WallsTrianglesBlowStrength {
    WTBS_LOW,         // ������ �������
    WTBS_MEDIUM,      // ������� �������
    WTBS_STRONG,      // ������� �������
    WTBS_HUDGE,       // �������� �������
    WTBS_NUM,
};

enum WallsSurfaceMode {
    WSFM_NONE,     // ����������� �� ��������
    WSFM_SURF,     // ����������� ��������
    WSFM_NUM,
};

enum WallsSegmentTriangulationMode {
    WSTM_CONVEX_POLY, // ����������� ������� �� ������ ������ �������� ���������������
    WSTM_CONVEX_QUAD, // ������� �� ������ �������� �����������������
    WSTM_NUM,
    WSTM_ANGLE,     // ������� �� ������w ������������ ����
};

enum AmbientLightStrength {
    ALS_100,
    ALS_90,
    ALS_75,
    ALS_50,
    ALS_30,
    ALS_10,
    ALS_00,
    ALS_NUM
};

enum WallColoringMode {
    WCM_SMOOTH,
    WCM_TIGHTNESS_SMOOTH,
    WCM_DEPTH_SMOOTH,
    WCM_NUM,
    WCM_ROUGE
};

enum RenderSimpleMode {
    SM_ORIGINAL, // ������������ ����� ������, ������ ��������, ������� ������������
    SM_ROUGH_WALLS, // ����� ��������, ��� ������������� ���������
	SM_SMOOTH_WALLS, // ����� ��������, "������" ���������, "����������" ���������
	SM_SECTIONS_WALLS, // ����� ��������, �������� ������, ����� ������������ ����������
	SM_NUM
};

struct CaveViewPrefs {
    CaveViewPrefs()
    : surfColorId(-1)
    , dupColorId(-1)
    , stretchWallsOverPiketNumMax(8)
    , stretchWallsOverPiketLengthMax(50)
    , wallsPropagateMode(WPM_NONE)//(WPM_1M)
    , wallsShadowMode(WSM_SMOOTH)
    , wallsBlowMode(WBM_NONE)
    , wallsSurfaceMode(WSFM_NONE)//(WSFM_SURF)
    , generateWallsForNoWallsPiketsMode(GWNWPM_BUDGE)
    , wallsSegmentTriangulationMode(WSTM_CONVEX_POLY)
    , wallsTrianglesBlowMode(/*WTBM_16*/WTBM_NONE)
    , wallsTrianglesBlowStrength(WTBS_MEDIUM)
    , ambientLightStrength(ALS_50)
    , wallColoringMode(WCM_TIGHTNESS_SMOOTH)
    , showThread(true)
    , fillRate(1.0f)
	, showDebug(false)
	, showWallLines(false)
	, showBox(false)
	, showSections(false)
    , skipNum(0)
    { }

    bool operator== (const CaveViewPrefs& o2) {
        return surfColorId == o2.surfColorId
        && dupColorId == o2.dupColorId
        && stretchWallsOverPiketNumMax == o2.stretchWallsOverPiketNumMax
        && stretchWallsOverPiketLengthMax == o2.stretchWallsOverPiketLengthMax
        && wallsPropagateMode == o2.wallsPropagateMode
        && wallsShadowMode == o2.wallsShadowMode
        && wallsBlowMode == o2.wallsBlowMode
        && wallsSurfaceMode == o2.wallsSurfaceMode
        && showDebug == o2.showDebug
        && generateWallsForNoWallsPiketsMode == o2.generateWallsForNoWallsPiketsMode
        && wallsSegmentTriangulationMode == o2.wallsSegmentTriangulationMode
        && wallsTrianglesBlowMode == o2.wallsTrianglesBlowMode
        && wallsTrianglesBlowStrength == o2.wallsTrianglesBlowStrength
        && ambientLightStrength == o2.ambientLightStrength
        && wallColoringMode == o2.wallColoringMode
        && showThread == o2.showThread
		&& showSections == o2.showSections
		&& showWallLines == o2.showWallLines
		&& showBox == o2.showBox
		&& fillRate == o2.fillRate
        && skipNum == o2.skipNum;
    }

    bool operator!= (const CaveViewPrefs& o2) { return !(*this == o2); };

    int surfColorId;
    int dupColorId;
    int stretchWallsOverPiketNumMax;
    int stretchWallsOverPiketLengthMax;
    WallsPropagateMode wallsPropagateMode;
    WallsShadowMode wallsShadowMode;
    WallsBlowMode wallsBlowMode;
    WallsSurfaceMode wallsSurfaceMode;
    GenerateWallsForNoWallsPiketsMode generateWallsForNoWallsPiketsMode;
    WallsSegmentTriangulationMode wallsSegmentTriangulationMode;
    WallsTrianglesBlowMode wallsTrianglesBlowMode;
    WallsTrianglesBlowStrength wallsTrianglesBlowStrength;
    AmbientLightStrength ambientLightStrength;
    WallColoringMode wallColoringMode;
	bool showThread;
	bool showSections;
	bool showWallLines;
	bool showBox;
	float fillRate; // 0..1
	bool showDebug;
    int skipNum;
};

struct Color {
	Color(): r(1), g(1), b(1), a(1) {}
	Color(float r, float g,	float b, float a = 1.0f): r(r), g(g), b(b), a(a) {}

	bool operator== (const Color& c1) const {
		return r == c1.r && g == c1.g && b == c1.b && a == c1.a;
	}

	Color operator+ (const Color& c1) const {
		return Color(r + c1.r, g + c1.g, b + c1.b, a + c1.a);
	}

	Color operator/ (float f) const {
		return Color(r / f, g / f, b / f, a / f);
	}

	Color operator* (float f) const {
		return Color(r * f, g * f, b * f, a * f);
	}

	static Color None;
	static Color White;
	static Color Red;
	static Color Blue;
	static Color Green;

	float r;
	float g;
	float b;
	float a;
};

inline Color operator* (float f, const Color& c) {
	return c*f;
}

enum OuputType {
	OT_UNKNOW,
//	OT_STATION,
	OT_THREAD,
//	OT_DIMENSION,
//	OT_CUBE,
	OT_WALL,
	OT_WALL_CUTS,
//	OT_WALL_LINES,
//	OT_LINE,
	OT_DEBUG,
    OT_DEBUG2,
    OT_BOX,
};

struct OutputPoly {
	V3 a;
	V3 an;
	V3 b;
	V3 bn;
	V3 c;
	V3 cn;
	Color ca;
	Color cb;
	Color cc;
};

struct OutputLine {
	V3 a;
	V3 b;
	Color ca;
	Color cb;
};

struct Wall {
	Wall(V3 p): pos(p)  {}
	V3 pos;
};

enum PiketMark {
	MARK_NONE = 0,
	MARK_Z_SURVEY = 1,
	MARK_Z_TURN = 2,
	MARK_Z_SURVEY_FAKE = 4, // ����� - �������������� � ���������� ���-��� ������
	MARK_ONLY_CONVEX_WALLS = 8 //����������� ������������ ������������� �������� �����
};

struct PiketInfo {
	PiketInfo(): id(getUniqId()), pos(0,0,0), priz(MARK_NONE), hasWalls(0) { }
	long long id;
	std::string name;
	std::string label;
	V3 pos;
	Color col;
	PiketMark priz;
	int hasWalls;

	static int getUniqId();
};

}
