//
//	Author:	Michael Cameron
//	Email:	chronokun@hotmail.com, 
//

// Libraries Include
#include "libraries.h"
// Local Includes
#include "mathtypes.h"
#include "vector.h"
// This Include
#include "geometry.h"


TPlaneD3DN::TPlaneD3DN(const TPlaneD3TP& _krP)
{
	const TVectorD3 kAB = math::Subtract(TVectorD3(), _krP.m_B, _krP.m_A);
	const TVectorD3 kBC = math::Subtract(TVectorD3(), _krP.m_C, _krP.m_B);

	this->m_Normal = math::Normalize(TVectorD3(), math::CrossProduct(TVectorD3(), kBC, kAB));
	this->m_dDistance = math::DotProduct(_krP.m_B, this->m_Normal);
}

const bool math::IsIntersection(const TPlaneD3DN& _krA,
								const TPlaneD3DN& _krB,
								const TPlaneD3DN& _krC)
{
	const TVectorD3 kNAxNB = math::CrossProduct(TVectorD3(), _krA.m_Normal, _krB.m_Normal);
	return(abs(math::DotProduct(kNAxNB, _krC.m_Normal)) > 0.0);
}

const TVectorD3& math::GetIntersection(	TVectorD3& _rResult,
										const TPlaneD3DN& _krA,
										const TPlaneD3DN& _krB,
										const TPlaneD3DN& _krC)
{
	const TVectorD3 kNBxNC = math::CrossProduct(TVectorD3(), _krB.m_Normal, _krC.m_Normal);
	const TVectorD3 kNCxNA = math::CrossProduct(TVectorD3(), _krC.m_Normal, _krA.m_Normal);
	const TVectorD3 kNAxNB = math::CrossProduct(TVectorD3(), _krA.m_Normal, _krB.m_Normal);

	const TVectorD3 kDA_NBxNC = math::ScalarMultiply(TVectorD3(), kNBxNC, _krA.m_dDistance);
	const TVectorD3 kDB_NCxNA = math::ScalarMultiply(TVectorD3(), kNCxNA, _krB.m_dDistance);
	const TVectorD3 kDC_NAxNB = math::ScalarMultiply(TVectorD3(), kNAxNB, _krC.m_dDistance);

	const double kdDenom = math::DotProduct(kNAxNB, _krC.m_Normal);

	const TVectorD3 kNumer = math::Add(TVectorD3(), math::Add(TVectorD3(), kDA_NBxNC, kDB_NCxNA), kDC_NAxNB);

	_rResult = math::ScalarMultiply(TVectorD3(), kNumer, (1.0 / kdDenom));

	return(_rResult);
}

const TPlaneD3TP& math::ReversePlane(TPlaneD3TP& _rResult, const TPlaneD3TP& _krInput)
{
	_rResult.m_A = _krInput.m_C;
	_rResult.m_B = _krInput.m_B;
	_rResult.m_C = _krInput.m_A;
	return(_rResult);
}