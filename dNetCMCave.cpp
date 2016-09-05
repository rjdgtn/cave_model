// This is the main DLL file.

#include <msclr/marshal_cppstd.h>
#include "dNetCMCave.h"
#include "CMCave.h"
#include <stdlib.h>
#include <string.h>

namespace DotNetCaveModel {

	using namespace CM;
	using namespace msclr::interop;
	using namespace System;
	using namespace System::Collections::Generic;

	DNetCMCave::DNetCMCave() {
		CM::PointsInMeter = 1.0f;

		cmCave = new Cave();
		caveViewPrefs = new CaveViewPrefs();
	}



	DNetCMCave::~DNetCMCave()
	{
		delete cmCave;
		cmCave = nullptr;

		delete caveViewPrefs;
		caveViewPrefs = nullptr;
	}

	bool DNetCMCave::setMode(RenderMode mode)
	{
		switch (mode)
		{
		case DotNetCaveModel::RenderMode::SM_ROUGH_WALLS:
			updateForRoughWallsMode(caveViewPrefs);
			break;
		case DotNetCaveModel::RenderMode::SM_SMOOTH_WALLS:
			updateForSmoothWallsMode(caveViewPrefs);
			break;
		case DotNetCaveModel::RenderMode::SM_CUTS:
			updateForSectionsWallsMode(caveViewPrefs);
			break;
		}

		return cmCave->setCaveViewPrefs(*caveViewPrefs);
	}

	bool DNetCMCave::setColoringMode(ColoringMode mode) {
		return setColoringMode(mode, false);
	}

	bool DNetCMCave::setColoringMode(ColoringMode mode, bool grayscale)
	{
		switch (mode)
		{
		case DotNetCaveModel::ColoringMode::CM_CAVEBRANCH:
			caveViewPrefs->wallColoringMode = WCM_SMOOTH;
			break;
		case DotNetCaveModel::ColoringMode::CM_TIGHTNESS_SMOOTH:
			caveViewPrefs->wallColoringMode = WCM_TIGHTNESS_SMOOTH;
			break;
		case DotNetCaveModel::ColoringMode::CM_DEPTH_SMOOTH:
			caveViewPrefs->wallColoringMode = WCM_DEPTH_SMOOTH;
			break;
		}

		caveViewPrefs->grayscale = grayscale;

		return cmCave->setCaveViewPrefs(*caveViewPrefs);
	}

	void DNetCMCave::updateForRoughWallsMode(CaveViewPrefs* caveViewPrefs) {
		caveViewPrefs->wallsPropagateMode = CM::WPM_NONE;
		caveViewPrefs->wallsShadowMode = CM::WSM_ROUGH;
		caveViewPrefs->wallsBlowMode = CM::WBM_NONE;
		caveViewPrefs->wallsSurfaceMode = CM::WSFM_SURF;
		caveViewPrefs->wallsTrianglesBlowMode = CM::WTBM_NONE;
		caveViewPrefs->wallsTrianglesBlowStrength = CM::WTBS_LOW;
		caveViewPrefs->showSections = false;
		caveViewPrefs->showWallLines = false;
	}

	void DNetCMCave::updateForSmoothWallsMode(CaveViewPrefs* caveViewPrefs) {
		caveViewPrefs->wallsPropagateMode = CM::WPM_NONE;
		caveViewPrefs->wallsShadowMode = CM::WSM_ROUGH;
		caveViewPrefs->wallsBlowMode = CM::WBM_NONE;
		caveViewPrefs->wallsSurfaceMode = CM::WSFM_SURF;
		caveViewPrefs->wallsTrianglesBlowMode = CM::WTBM_9;
		caveViewPrefs->wallsTrianglesBlowStrength = CM::WTBS_LOW;
		caveViewPrefs->showSections = false;
		caveViewPrefs->showWallLines = false;
	}

	void DNetCMCave::updateForSectionsWallsMode(CaveViewPrefs* caveViewPrefs) {
		caveViewPrefs->wallsPropagateMode = CM::WPM_NONE;
		caveViewPrefs->wallsShadowMode = CM::WSM_ROUGH;
		caveViewPrefs->wallsBlowMode = CM::WBM_NONE;
		caveViewPrefs->wallsSurfaceMode = CM::WSFM_NONE;
		caveViewPrefs->wallsTrianglesBlowMode = CM::WTBM_NONE;
		caveViewPrefs->wallsTrianglesBlowStrength = CM::WTBS_LOW;
		caveViewPrefs->showSections = true;
		caveViewPrefs->showWallLines = true;
	}

	CM::PiketInfo DNPiketInfo::toPiketMark()
	{
		PiketInfo res;
		res.id = id;
		String^ temp = name;
		res.name = marshal_as< std::string>(temp);
		temp = label;
		res.label = marshal_as< std::string>(temp);
		res.pos = V3(x, y, z);
		res.col = Color(r, g, b);
		res.priz = (PiketMark)priz;
		//res.hasWalls = hasWalls;

		return res;
	
	}
	void DNetCMCave::addVertice(DNPiketInfo^ piketInfo) {
		cmCave->addVertice(piketInfo->toPiketMark());
	}

	void DNetCMCave::addVertice(DNPiketInfo^ piketInfo, int equatesVerticeId) {
		cmCave->addVertice(piketInfo->toPiketMark(), equatesVerticeId);
	}

	void DNetCMCave::addEdge(int verticeId0, int verticeId1)
	{
		cmCave->addEdge(verticeId0, verticeId1);
	}

	void DNetCMCave::addEdge(int verticeId0, int verticeId1, bool zSurvey, float r, float g, float b)
	{
		EdgeInfo info(verticeId0, verticeId1);
		info.col = Color(r, g, b, 1.0f) ;
		info.zsurvey = zSurvey;
		cmCave->addEdge(info);
	}

	void DNetCMCave::addWall(DNWall ^ wall, int linkToVerticeId, int parentPiketId)
	{
		cmCave->addWall(wall->toWall(), linkToVerticeId, parentPiketId);
	}

	void DNetCMCave::addWall(float x, float y, float z, int linkToVerticeId, int parentPiketId)
	{
		DNWall ^ wall = gcnew DNWall;
		wall->x = x;
		wall->y = y;
		wall->z = z;
		addWall(wall, linkToVerticeId, parentPiketId);
	}

	void DNetCMCave::finishInit()
	{
		cmCave->finishInit();
	}

	System::Collections::Generic::List<DNOutputPoly^>^ DNetCMCave::getOutputPoly(DMOuputType type)
	{
		const auto& outputPoly = cmCave->getOutputPoly((OuputType)type);
		List<DNOutputPoly^>^ res = gcnew List<DNOutputPoly^>(outputPoly.size());
		for (const auto& poly : outputPoly) {
			res->Add(gcnew DNOutputPoly(poly));
		}
		return res;
	}

	List<DMOutputLine^>^ DNetCMCave::getOutputLine(DMOuputType type)
	{
		const auto& outputLine = cmCave->getOutputLine((OuputType)type);
		List<DMOutputLine^>^ res = gcnew List<DMOutputLine^>(outputLine.size());
		for (const auto& line : outputLine) {
			res->Add(gcnew DMOutputLine(line));
		}
		return res;
	}

	bool DNetCMCave::isOutputEnabled(DMOuputType type)
	{
		return cmCave->isOutputEnabled((OuputType)type);
	}


	CM::Wall DNWall::toWall()
	{
		return Wall(V3(x, y, z));
	}

	DNOutputPoly::DNOutputPoly(const CM::OutputPoly poly)
	{
		vertice = gcnew array<float, 2>{ {poly.a.x, poly.a.y, poly.a.z}, { poly.b.x, poly.b.y, poly.b.z }, { poly.c.x, poly.c.y, poly.c.z } };
		color = gcnew array<float, 2> { {poly.ca.r, poly.ca.g, poly.ca.b, poly.ca.a}, { poly.cb.r, poly.cb.g, poly.cb.b, poly.cb.a }, { poly.cc.r, poly.cc.g, poly.cc.b, poly.cc.a } };
	}

	DMOutputLine::DMOutputLine(const CM::OutputLine line)
	{
		List<DMOutputLine^>^ arr = gcnew List<DMOutputLine^>(100);

		vertice = gcnew array<float, 2>{ {line.a.x, line.a.y, line.a.z}, { line.b.x, line.b.y, line.b.z } };
		color = gcnew array<float, 2> { {line.ca.r, line.ca.g, line.ca.b, line.ca.a}, { line.cb.r, line.cb.g, line.cb.b, line.cb.a } };
	}



}