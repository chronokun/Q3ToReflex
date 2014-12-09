#pragma once

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

// Local Includes
#include "mathtypes.h"

// Prototypes
struct TPlaneD3DN;
struct TPlaneD3TP;

// Types
struct TPlaneD3DN
{
	double m_dDistance;
	TVectorD3 m_Normal;
	TPlaneD3DN() = default;
	TPlaneD3DN(const TVectorD3& _krNormal, const double _kdDistance)
	{
		this->m_dDistance = _kdDistance;
		this->m_Normal = _krNormal;
	}
	TPlaneD3DN(const TPlaneD3TP& _krP);
};

struct TPlaneD3TP
{
	TVectorD3 m_A;
	TVectorD3 m_B;
	TVectorD3 m_C;
	TPlaneD3TP() = default;
	TPlaneD3TP(const TVectorD3& _krA, const TVectorD3& _krB, const TVectorD3& _krC)
	{
		this->m_A = _krA;
		this->m_B = _krB;
		this->m_C = _krC;
	}
};

namespace math
{
	const bool IsIntersection(	const TPlaneD3DN& _krA,
								const TPlaneD3DN& _krB,
								const TPlaneD3DN& _krC);

	const TVectorD3& GetIntersection(	TVectorD3& _rResult,
										const TPlaneD3DN& _krA,
										const TPlaneD3DN& _krB,
										const TPlaneD3DN& _krC);

	const TPlaneD3TP& ReversePlane(TPlaneD3TP& _rResult, const TPlaneD3TP& _krInput);
};

#endif