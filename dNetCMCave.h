#pragma once
using namespace System;
#include "CMTypes.h"


namespace CM {
	class Cave; 
	struct CaveViewPrefs; 
}

namespace DotNetCaveModel{

	using namespace System::Collections::Generic;

	public enum class ColoringMode
	{
		CM_EXTERNAL,
		CM_TIGHTNESS_SMOOTH,
		CM_DEPTH_SMOOTH
	};

	public enum class RenderMode
	{
		SM_ORIGINAL,
		SM_ROUGH_WALLS,
		SM_SMOOTH_WALLS,
		SM_CUTS,
		SM_NUM
	};

	public enum class DNPiketMark {
		MARK_NONE = 0,
		MARK_Z_SURVEY = 1,
		MARK_Z_TURN = 2,
		MARK_Z_SURVEY_FAKE = 4, // ����� - �������������� � ���������� ���-��� ������
		MARK_ONLY_CONVEX_WALLS = 8 //����������� ������������ ������������� �������� �����
	};

	public ref struct DNPiketInfo {
		DNPiketInfo() : id(CM::PiketInfo::getUniqId()) { }
		int id;
		String^ name {""};
		String^ label {""};
		float x {0};
		float y {0};
		float z {0};
		float r {0};
		float g {0};
		float b {0};
		DNPiketMark priz { DNPiketMark::MARK_NONE};
		//int hasWalls {0};
				
		virtual CM::PiketInfo toPiketMark();
	};

	public ref struct DNWall {
		float x{ 0 };
		float y{ 0 };
		float z{ 0 };

		virtual CM::Wall toWall();
	};
	
	public enum class DMOuputType {
		OT_UNKNOW,
		OT_THREAD,
		OT_WALL,
		OT_WALL_CUTS,
		OT_DEBUG,
		OT_DEBUG2,
		OT_BOX,
		OT_NUM
	};

	public ref struct DNOutputPoly {
		DNOutputPoly(const CM::OutputPoly poly);
		array<float, 2>^ vertice; // { {ax, ay, az}, { bx, by, bz }, { cx, cy, cz } };
		array<float, 2>^ color; // { {ar, ag, ab, aa}, { br, bg, bb, ba }, { cr, cg, cb, ca } };
	};

	public ref struct DMOutputLine {
		DMOutputLine(const CM::OutputLine line);
		array<float, 2>^ vertice; // { {ax, ay, az}, { bx, by, bz } };
		array<float, 2>^ color; // { {ar, ag, ab, aa}, { br, bg, bb, ba } };
	};


	public ref class DNetCMCave {
	public:
		DNetCMCave();
		~DNetCMCave();
		
		bool setMode(RenderMode mode);
		bool setColoringMode(ColoringMode mode);
		
		void addVertice(DNPiketInfo^ piketInfo, int equatesVerticeId);
		void addVertice(DNPiketInfo^ piketInfo);
		void addEdge(int verticeId0, int verticeId1);
		void addEdge(int verticeId0, int verticeId1, float r, float g, float b);
		void addWall(DNWall^ wall, int linkToVerticeId, int parentPiketId);
		void addWall(float x, float y, float z, int linkToVerticeId, int parentPiketId);
		void addWall(DNWall^ wall, int linkToVerticeId) { addWall(wall, linkToVerticeId, 0); };
		void addWall(float x, float y, float z, int linkToVerticeId) { addWall(x, y, z, linkToVerticeId, 0); };
		void finishInit();

		List<DNOutputPoly^>^ getOutputPoly(DMOuputType type);
		List<DMOutputLine^>^ getOutputLine(DMOuputType type);
		bool isOutputEnabled(DMOuputType type);

	protected:
		void updateForRoughWallsMode(CM::CaveViewPrefs* caveViewPrefs);
		void updateForSmoothWallsMode(CM::CaveViewPrefs* caveViewPrefs);
		void updateForSectionsWallsMode(CM::CaveViewPrefs* caveViewPrefs);

	protected:
		CM::CaveViewPrefs* caveViewPrefs;
		CM::Cave* cmCave;
	};
}