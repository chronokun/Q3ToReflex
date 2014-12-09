//
//	Author:	Michael Cameron
//	Email:	chronokun@hotmail.com, 
//

// Libraries Include
#include "libraries.h"
// This Include
#include "vector.h"
// Local Includes
#include "mathtypes.h"


const bool math::Equal(		const TVectorD3& _krA,
							const TVectorD3& _krB,
							const double _kdEpsilon)
{
	const bool kbEqual =	(abs(_krA.m_dX - _krB.m_dX) < _kdEpsilon)
						&&	(abs(_krA.m_dY - _krB.m_dY) < _kdEpsilon)
						&&	(abs(_krA.m_dZ - _krB.m_dZ) < _kdEpsilon);
	return(kbEqual);
}

const TVectorD3& math::Add(	TVectorD3& _rResult,
							const TVectorD3& _krA, 
							const TVectorD3& _krB)
{
	_rResult.m_dX = _krA.m_dX + _krB.m_dX;
	_rResult.m_dY = _krA.m_dY + _krB.m_dY;
	_rResult.m_dZ = _krA.m_dZ + _krB.m_dZ;
	return(_rResult);
}

const TVectorD3& math::Subtract(	TVectorD3& _rResult,
								const TVectorD3& _krA,
								const TVectorD3& _krB)
{
	_rResult.m_dX = _krA.m_dX - _krB.m_dX;
	_rResult.m_dY = _krA.m_dY - _krB.m_dY;
	_rResult.m_dZ = _krA.m_dZ - _krB.m_dZ;
	return(_rResult);
}

const TVectorD3& math::ScalarMultiply(	TVectorD3& _rResult,
										const TVectorD3& _krV,
										const double _kdS)
{
	_rResult.m_dX = _krV.m_dX * _kdS;
	_rResult.m_dY = _krV.m_dY * _kdS;
	_rResult.m_dZ = _krV.m_dZ * _kdS;

	return(_rResult);
}

const double math::Magnitude(const TVectorD3& _krV)
{
	return(sqrt(	(_krV.m_dX * _krV.m_dX)
				+	(_krV.m_dY * _krV.m_dY)
				+	(_krV.m_dZ * _krV.m_dZ)));
}

const double math::DotProduct(	const TVectorD3& _krA,
								const TVectorD3& _krB)
{
	return(		(_krA.m_dX * _krB.m_dX)
			+	(_krA.m_dY * _krB.m_dY)
			+	(_krA.m_dZ * _krB.m_dZ));
}

const TVectorD3& math::CrossProduct(	TVectorD3& _rResult,
									const TVectorD3& _krA,
									const TVectorD3& _krB)
{
	_rResult.m_dX = (_krA.m_dY * _krB.m_dZ) - ( _krA.m_dZ * _krB.m_dY);
	_rResult.m_dY = (_krA.m_dZ * _krB.m_dX) - ( _krA.m_dX * _krB.m_dZ);
	_rResult.m_dZ = (_krA.m_dX * _krB.m_dY) - ( _krA.m_dY * _krB.m_dX);
	return(_rResult);
}

const TVectorD3& math::Normalize(	TVectorD3& _rResult,
									const TVectorD3& _krV)
{
	math::ScalarMultiply(_rResult, _krV, (1.0 / math::Magnitude(_krV)) );
	return(_rResult);
}