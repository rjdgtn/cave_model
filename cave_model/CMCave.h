//---------------------------------------------------------------------------
#pragma once
#include <vector>           
#include <map>
//#include "OgrePrerequisites.h" 
#include "CMHelpers.h"
#include <unordered_map>    
#include <unordered_set>      
#include "CMPiket.h"

namespace CM {

typedef std::vector<V3> vectorV3;

class Cave {
public:
	Cave();
	~Cave();
    bool setCaveViewPrefs(const CaveViewPrefs& prefs);

	void addVertice(const PiketInfo& piketInfo, int equatesVerticeId = 0);
	void addEdge(int verticeId0, int verticeId1);
	void addWall(const Wall& wall, int linkToVerticeId, int parentPiketId);

	void finishInit();

	const std::vector<OutputPoly>& getOutputPoly(OuputType type) { return outputPoly[type]; }
	const std::vector<OutputLine>& getOutputLine(OuputType type) { return outputLines[type]; }
	bool isOutputEnabled(OuputType type) { return outputLayers[type]; }

protected:
//    void buildPikets(); // ������� ���� ������� �� �������� P3D � W3D
    void buildFakeZSurveyPikets(); // ������������ ���������� ������ �������� �������������� ������ 
    void buildEquatesMap();
	void buildWallsObject(); // ��������� ����������� ������ ����               
    // ������������� ����������� ���� ����� ����� �������� ������� ��������� � ����������
	WallTriangles buildWallSegment(const Piket* piket, const Piket* nextPiket);
    // ������������� ����������� ���� ����� ����� �������� ������� �������������� 
    // ���� ����� ������� ������ ������ ����������� ��� ������
	WallTriangles buildWallSegmentCenterMode(const Piket* piket, const Piket* nextPiket);
    // ������������� ����������� ���� ����� ����� �������� ������� �������������� 
    // �� ����������� ������������ ������ �������� ���������������
	WallTriangles buildWallSegmentConvexPolyMode(const Piket* piket, const Piket* nextPiket);
    // ������������� ����������� ���� ����� ����� �������� ������� �������������� 
    // �� �������� �� ������� ������������ �����������������
	WallTriangles buildWallSegmentConvexQuadMode(const Piket* piket, const Piket* nextPiket);
    void buildThreadObject(); // ������ ����������� ������ ����� ����  
	void buildPointsObject(); // ������ ����������� ������ ����� �������
    void buildCutsObject(); // ������ ����������� ������ ����� �������
    void buildBoxObject();

    void updateWallsSurrfaceMode(); // �������� / ������ ����.������� � ������������ � ����������� 

    // ��������� ����� � ������ ��� ���� �� ������ ������� ������� �� �������
    // �������� genPiketsFakeWalls�hainSearch ��� ���� ��������� ������� �� �������                                
    void genPiketsFakeWalls(FakeWallsMode mode); 
    // ����������� ������� ������ ������� ���������� ������� ����������� ���������� ��������
	void genPiketsFakeWalls�hainSearch(FakeWallsMode mode, Piket* beginPiket, Piket* curEnd, int accumLength, std::vector<Piket*>& intermPikets);
    // ������ ��������������� ��������� ������� � ����� ���������� ���������� ������ ���������
	void genPiketsFakeWallsProcessChain(FakeWallsMode mode, Piket* beginPiket, std::vector<Piket*> intermPikets, Piket* endPiket);
	// ������������ ����������
	void genPiketsFakeWallsSkip(Piket* beginPiket, std::vector<Piket*> intermPikets, Piket* endPiket);
    // ��������� ����������
	void genPiketsFakeWallsTrail(Piket* beginPiket, std::vector<Piket*> intermPikets, Piket* endPiket);
    // ������ ����� ��������� � ������������
    void genPiketsFakeWallsBudge(Piket* beginPiket, std::vector<Piket*> intermPikets, Piket* endPiket);

    
    // ������� ���������� �����
//    const P3D* getP3D(int id) const;
//    const P3D* getP3DbyMet(int met) const;
    Piket* getPiketMut(int id);
    const Piket* getPiket(int id);
//    int getPiketId(const P3D* p3d, int met) const;
//    int getPiketId(const P3D* p3d) const;
//    int getPiketId(int met) const;
//    const std::string& getPiketLabel(int met);

    void logPikets();
    
	// ������� ��������� ����
    std::string getWallMaterial();            
//    const Color& getThreadColour(int color, P3DPriz priz = PRIZ_NONE) const;

    // ������� ��������� �� ���� ����� �� ������
    static int minAngleWallPoint(V2 point, const std::vector<V2>& rotWalls, bool abs, int skipPoint);
                              
    void buildWallTriangles();
    
    //  ��������� ������������ ���������������� ������������� � ����������� ��
    //  ����������� ����������� ����������� ������������� (�������� smooth)
    //  ����������� ������ ���� ������������ �� �������
    // ����������� ��������� - �����      
	void drawTriangleSmooth(int smooth, const Color& ca, const Color& cb, const Color& cc, V3 a, V3 an, V3 b, V3 bn, V3 c, V3 cn);
	// ���������� ���������� ����������� �����. �� ���������, �������� �������
	void drawTriangleSmooth2(int smooth, const Color& ca, const Color& cb, const Color& cc, V3 a, V3 an, V3 b, V3 bn, V3 c, V3 cn);
	// �� �������������������� ���������� ����������� �����. �� ���������
	void drawTriangleSmooth3(int smooth, const Color& ca, const Color& cb, const Color& cc, V3 a, V3 an, V3 b, V3 bn, V3 c, V3 cn);
	// ������������ �����. �� ������������� ��������. �� ���������
	void drawTriangleSmooth4(int smooth, const Color& ca, const Color& cb, const Color& cc, V3 a, V3 an, V3 b, V3 bn, V3 c, V3 cn);

    // �������� ����������� � walls ManualObject
	// ����������� ������ ���� ������������ �� �������
	void drawTriangle(const Color& ca, const Color& cb, const Color& cc, V3 a, V3 an, V3 b, V3 bn, V3 c, V3 cn);
    // �������� ����������� ��� ������������ ������������ ��������                                                              
    void addSmoothedTriangle(const Color& ca, const Color& cb, const Color& cc, bool clockwise, V3 a, V3 b, V3 c);
                                                    
    V3 normalisedEdgeCenter(V3 a, V3 an, V3 b, V3 bn);

    void debugDraw(V3 a, V3 b, Color col = Color(1, 1, 0, 1));

    void processZSurveyPiketsChain(const std::list<const Piket*>& chain);

//    int getFreePiketId() const;
	const Piket* addFakePiket(V3 pos, Color col, PiketMark priz, const std::vector<Wall>& walls, const std::vector<const Piket*>& adjPkets);
    
    static std::vector<std::pair<bool, int> > calcTriangulationOrdertConvexPolyMode(const std::vector<WallProj>& a, const std::vector<WallProj>& b, bool clockwise, bool force_convex);
    static std::vector<std::pair<bool, int> > calcTriangulationOrdertConvexPolyMode(const std::vector<WallProj>& a, int aStart, int aEnd, const std::vector<WallProj>& b, int bStart, int bEnd, bool clockwise, bool force_convex) ;

	Color getColorForPiket(const Piket* piket);
	Color getDepthColor(float depthRate);
    
    static Color getColorByRatio(const std::vector<std::pair<float, Color> >& colors, float rate);

	// output:
	void resetOutput(OuputType type);
	void addOutputPoly(OuputType type, V3 a, V3 b, V3 c, V3 an, V3 bn, V3 cn, const Color& ca, const Color& cb, const Color& cc);
	void addOutputPoly(OuputType type, V3 a, V3 b, V3 c, const Color& col);
	void addOutputLine(OuputType type, V3 a, V3 b, const Color& c);
	void addOutputLine(OuputType type, V3 a, V3 b, const Color& ca, const Color& cb);

protected: // �������������
//	std::vector<V3> calcCube(int i);
//	void addPolygon(OuputType type, Ogre::Vector3 p0, Ogre::Vector3 p1, Ogre::Vector3 p2, Ogre::Vector3 p3);

protected:                 
	bool wasInited;

	std::tr1::unordered_map<int, Piket> pikets;
	CaveViewPrefs caveViewPrefs;

	V3 boxMin;
	V3 boxMax;

    std::string curWallsMaterial;
    // ������� � ��������� ������������ � ���������.
    std::vector<SurfaceTriangle> lastTriangles;
    
    bool debugTriangulationAlgo;
    
    // ��������� ������
	// ����������� � ������� �� ������, ������������ ����� ������������ �����������
    // ������������ ������������ ����� � ���������� �� ����������                                                                   
    std::vector<VerticeTriangle> smoothedTriangles;
	std::map<Vertice, VerticeNormal> smoothedTrianglesVerticesNormals;

	std::tr1::unordered_map<OuputType, std::vector<OutputPoly> > outputPoly;
	std::tr1::unordered_map<OuputType, std::vector<OutputLine> > outputLines;
	std::tr1::unordered_map<OuputType, bool> outputLayers;

};

}

