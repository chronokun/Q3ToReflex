//
//	Author:	Michael Cameron
//	Email:	chronokun@hotmail.com
//

#pragma once

#ifndef __MATHTYPES_H__
#define __MATHTYPES_H__

// Class Prototypes

//
// Vector
//
struct TVectorD3
{
	union
	{
		struct
		{
			double m_dX;
			double m_dY;
			double m_dZ;
		};
		double m_dV[3];
	};

	TVectorD3() = default;
	TVectorD3(	const double _dX,
				const double _dY,
				const double _dZ)
	{
		this->m_dX = _dX;
		this->m_dY = _dY;
		this->m_dZ = _dZ;
	};

};

#endif