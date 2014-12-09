#pragma once

#ifndef __VECTOR_H__
#define __VECTOR_H__

// Local Includes
#include "mathtypes.h"

// Vector Function Prototypes
namespace math
{
	const bool Equal(	const TVectorD3& _krA,
						const TVectorD3& _krB,
						const double _kdEpsilon);
	
	const TVectorD3& Add(	TVectorD3& _rResult,
							const TVectorD3& _krA, 
							const TVectorD3& _krB);
	
	const TVectorD3& Subtract(	TVectorD3& _rResult,
								const TVectorD3& _krA,
								const TVectorD3& _krB);
	
	const TVectorD3& ScalarMultiply(TVectorD3& _rResult,
									const TVectorD3& _krV,
									const double _kdS);
	
	const double Magnitude(const TVectorD3& _krV);
	
	const double DotProduct(	const TVectorD3& _krA,
								const TVectorD3& _krB);
	
	const TVectorD3& CrossProduct(	TVectorD3& _rResult,
									const TVectorD3& _krA,
									const TVectorD3& _krB);
	
	const TVectorD3& Normalize(	TVectorD3& _rResult,
								const TVectorD3& _krV);	
}

#endif