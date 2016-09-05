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
	void addEdge(const EdgeInfo& info);
	void addWall(const Wall& wall, int linkToVerticeId, int parentPiketId = 0);

	void finishInit();

	const std::vector<OutputPoly>& getOutputPoly(OuputType type) { return outputPoly[type]; }
	const std::vector<OutputLine>& getOutputLine(OuputType type) { return outputLines[type]; }
	bool isOutputEnabled(OuputType type) { return outputLayers[type]; }

protected:
	
    void buildFakeZSurveyPikets(); // обрабатывает зигзаговую съемку создавая дополнительные пикеты 
    void buildEquatesMap();
	void buildWallsObject(); // заполняет графический объект стен               
    // триангулирует поверхность стен между двумя пикетами методом указанном в настройках
	WallTriangles buildWallSegment(const Piket* piket, const Piket* nextPiket);
    // триангулирует поверхность стен между двумя пикетами методом основывающимся 
    // угле между стенами вокруг прямой соединующий два пикета
	WallTriangles buildWallSegmentCenterMode(const Piket* piket, const Piket* nextPiket);
    // триангулирует поверхность стен между двумя пикетами методом основывающимся 
    // на рекурсивной триангуляции строго выпуклых многоугольников
	WallTriangles buildWallSegmentConvexPolyMode(const Piket* piket, const Piket* nextPiket);
    // триангулирует поверхность стен между двумя пикетами методом основывающимся 
    // на выпуклой по часовой треангуляции четырехугольников
	WallTriangles buildWallSegmentConvexQuadMode(const Piket* piket, const Piket* nextPiket);
    void buildThreadObject(); // строит графический объект нитки хода  
	void buildPointsObject(); // строит графический объект точек пикетов
    void buildCutsObject(); // строит графический объект точек пикетов
    void buildBoxObject();

    void updateWallsSurrfaceMode(); // показать / скрыть граф.объекты в соответствии с настройками 

    // добавляет стены в пикеты без стен на основе крайних пикетов со стенами
    // вызывает genPiketsFakeWallsСhainSearch для всех известных пикетов со стенами                                
    void genPiketsFakeWalls(FakeWallsMode mode); 
    // рекурсивная функция поиска цепочек бесстенных пикетов обрамленных состенными пикетами
	void genPiketsFakeWallsСhainSearch(FakeWallsMode mode, Piket* beginPiket, Piket* curEnd, int accumLength, std::vector<Piket*>& intermPikets);
    // фунция предварительной обработки цепочки и выбор конкретной реализации режима достоения
	void genPiketsFakeWallsProcessChain(FakeWallsMode mode, Piket* beginPiket, std::vector<Piket*> intermPikets, Piket* endPiket);
	// пропускающая реализация
	void genPiketsFakeWallsSkip(Piket* beginPiket, std::vector<Piket*> intermPikets, Piket* endPiket);
    // петляющая реализация
	void genPiketsFakeWallsTrail(Piket* beginPiket, std::vector<Piket*> intermPikets, Piket* endPiket);
    // гибрид между петляющий и пропускающей
    void genPiketsFakeWallsBudge(Piket* beginPiket, std::vector<Piket*> intermPikets, Piket* endPiket);

    
    // функции построения графа
//    const P3D* getP3D(int id) const;
//    const P3D* getP3DbyMet(int met) const;
    Piket* getPiketMut(int id);
    const Piket* getPiket(int id);
//    int getPiketId(const P3D* p3d, int met) const;
//    int getPiketId(const P3D* p3d) const;
//    int getPiketId(int met) const;
//    const std::string& getPiketLabel(int met);

    void logPikets();
    
	// функции раскраски стен
    std::string getWallMaterial();            
//    const Color& getThreadColour(int color, P3DPriz priz = PRIZ_NONE) const;

    // рассчет ближайшей по углу точки из списка
    static int minAngleWallPoint(V2 point, const std::vector<V2>& rotWalls, bool abs, int skipPoint);
                              
    void buildWallTriangles();
    
    //  алгоритым апроксимации параметрическими поверхностями с последующей их
    //  тесселяцией управляемым количеством треугольников (параметр smooth)
    //  треугольник должен быть ориентирован по часовой
    // рекурсивное разбиение - фигня      
	void drawTriangleSmooth(int smooth, const Color& ca, const Color& cb, const Color& cc, V3 a, V3 an, V3 b, V3 bn, V3 c, V3 cn);
	// построчное построение поверхности безье. не безшовная, неверные нормали
	void drawTriangleSmooth2(int smooth, const Color& ca, const Color& cb, const Color& cc, V3 a, V3 an, V3 b, V3 bn, V3 c, V3 cn);
	// не построчноепострочное построение поверхности безье. не безшовная
	void drawTriangleSmooth3(int smooth, const Color& ca, const Color& cb, const Color& cc, V3 a, V3 an, V3 b, V3 bn, V3 c, V3 cn);
	// триангуляция Фонга. не настраивается кривизна. не безшовная
	void drawTriangleSmooth4(int smooth, const Color& ca, const Color& cb, const Color& cc, V3 a, V3 an, V3 b, V3 bn, V3 c, V3 cn);

    // добавить треугольник в walls ManualObject
	// треугольник должен быть ориентирован по часовой
	void drawTriangle(const Color& ca, const Color& cb, const Color& cc, V3 a, V3 an, V3 b, V3 bn, V3 c, V3 cn);
    // добавить треугольник для последующего смешниквания нормалей                                                              
    void addSmoothedTriangle(const Color& ca, const Color& cb, const Color& cc, bool clockwise, V3 a, V3 b, V3 c);
                                                    
    V3 normalisedEdgeCenter(V3 a, V3 an, V3 b, V3 bn);

    void debugDraw(V3 a, V3 b, Color col = Color(1, 1, 0, 1));

    void processZSurveyPiketsChain(const std::list<const Piket*>& chain);

//    int getFreePiketId() const;
	const Piket* addFakePiket(V3 pos, Color col, PiketMark priz, const std::vector<Wall>& walls, const std::vector<EdgeInfo>& edges);
    
    static std::vector<std::pair<bool, int> > calcTriangulationOrdertConvexPolyMode(const std::vector<WallProj>& a, const std::vector<WallProj>& b, bool clockwise, bool force_convex);
    static std::vector<std::pair<bool, int> > calcTriangulationOrdertConvexPolyMode(const std::vector<WallProj>& a, int aStart, int aEnd, const std::vector<WallProj>& b, int bStart, int bEnd, bool clockwise, bool force_convex) ;

	Color getColorForPiket(const Piket* piket);
	Color getColorForPiketByEdges(const Piket* piket);
	const Color& getColorForEdge(const Piket* from, const Piket* to);
	Color getDepthColor(float depthRate);
    
    static Color getColorByRatio(const std::vector<std::pair<float, Color> >& colors, float rate);

	const EdgeInfo& getEdgeInfo(const Piket* from, const Piket* to) const { return getEdgeInfo(from->id, to->id); }
	const EdgeInfo& getEdgeInfo(int from, int to) const;

	std::vector<const Piket*> getZSurveyEdges(const Piket* from) const;

	// output:
	void resetOutput(OuputType type);
	void addOutputPoly(OuputType type, V3 a, V3 b, V3 c, V3 an, V3 bn, V3 cn, const Color& ca, const Color& cb, const Color& cc);
	void addOutputPoly(OuputType type, V3 a, V3 b, V3 c, const Color& col);
	void addOutputLine(OuputType type, V3 a, V3 b, const Color& c);
	void addOutputLine(OuputType type, V3 a, V3 b, const Color& ca, const Color& cb);
	

protected: // неиспольземое
//	std::vector<V3> calcCube(int i);
//	void addPolygon(OuputType type, Ogre::Vector3 p0, Ogre::Vector3 p1, Ogre::Vector3 p2, Ogre::Vector3 p3);

protected:                 
	bool wasInited;

	std::tr1::unordered_map<int, Piket> pikets;
	std::map<std::pair<int, int>, EdgeInfo> edges;
	CaveViewPrefs caveViewPrefs;

	V3 boxMin;
	V3 boxMax;

    std::string curWallsMaterial;
    // готовые к отрисовке треугольники с нормалями.
    std::vector<SurfaceTriangle> lastTriangles;
    
    bool debugTriangulationAlgo;
    
    // временные данные
	// треугольнки и нормали их вершин, получившиеся после апроксимации результатов
    // триангуляции поверхностью безье и посделющей ее тесселяции                                                                   
    std::vector<VerticeTriangle> smoothedTriangles;
	std::map<Vertice, VerticeNormal> smoothedTrianglesVerticesNormals;

	std::tr1::unordered_map<OuputType, std::vector<OutputPoly> > outputPoly;
	std::tr1::unordered_map<OuputType, std::vector<OutputLine> > outputLines;
	std::tr1::unordered_map<OuputType, bool> outputLayers;
	float colourMult;
};

}

