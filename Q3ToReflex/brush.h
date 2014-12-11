//
//	Author:	Michael Cameron
//	Email:	chronokun@hotmail.com
//

#pragma once

#ifndef __BRUSH_H__
#define __BRUSH_H__

// Local Includes
#include "vector.h"
#include "geometry.h"

// Types
struct TPolyBrushFace
{
	double m_dTexCoordU;
	double m_dTexCoordV;
	double m_dTexScaleU;
	double m_dTexScaleV;
	double m_dTexRotation;
	std::vector<unsigned int> m_Indices;
	std::string m_Material;
};

struct TPolyBrush
{
	std::vector<TVectorD3> m_Vertices;
	std::vector<TPolyBrushFace> m_Faces;
};

struct TPlaneBrushFace
{
	int m_iTexCoordU;
	int m_iTexCoordV;
	double m_dTexScaleU;
	double m_dTexScaleV;
	double m_dTexRotation;
	TPlaneD3TP m_Plane;
	std::string m_Material;
};

struct TPlaneBrush
{
	std::vector<TPlaneBrushFace> m_Faces;
};

const std::vector<std::vector<TVectorD3>>& GetPolyFaces(	std::vector<std::vector<TVectorD3>>& _rResult,
															const TPolyBrush& _krBrush);

const std::vector<std::vector<TVectorD3>>& GetPolyFaces(	std::vector<std::vector<TVectorD3>>& _rResult,
															const TPlaneBrush& _krBrush);

const std::vector<TVectorD3>& SortFaceVerts(	std::vector<TVectorD3>& _rResult,
												const std::vector<TVectorD3>& _krInput,
												const TVectorD3& _krFaceNormal,
												const bool _kbClockwise);

const TVectorD3& GetFaceNormal(TVectorD3& _rResult, const size_t _kszFace, const TPolyBrush& _krBrush);

const TVectorD3& GetFaceNormal(TVectorD3& _rResult, const size_t _kszFace, const TPlaneBrush& _krBrush);

const bool CheckForBrushCull(const std::string& _krInput);

#endif