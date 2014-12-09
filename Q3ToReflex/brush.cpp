//
//	Author:	Michael Cameron
//	Email:	chronokun@hotmail.com
//

// Libraries Include
#include "libraries.h"
// Local Includes
#include "mathtypes.h"
#include "constants.h"
// This Include
#include "brush.h"

const std::vector<std::vector<TVectorD3>>& GetPolyFaces(	std::vector<std::vector<TVectorD3>>& _rResult,
															const TPolyBrush& _krBrush)
{
	_rResult.resize(_krBrush.m_Faces.size());

	for(size_t i = 0; i < _krBrush.m_Faces.size()-2; ++i)
	{
		for(size_t j = i; j < _krBrush.m_Faces.size()-1; ++j)
		{
			for(size_t k = j; k < _krBrush.m_Faces.size(); ++k)
			{
				if(		(i != j)
					&&	(j != k)
					&&	(k != i))
				{
					const TPlaneD3TP kPA(	_krBrush.m_Vertices[_krBrush.m_Faces[i].m_Indices[2]],
											_krBrush.m_Vertices[_krBrush.m_Faces[i].m_Indices[1]],
											_krBrush.m_Vertices[_krBrush.m_Faces[i].m_Indices[0]]);

					const TPlaneD3TP kPB(	_krBrush.m_Vertices[_krBrush.m_Faces[j].m_Indices[2]],
											_krBrush.m_Vertices[_krBrush.m_Faces[j].m_Indices[1]],
											_krBrush.m_Vertices[_krBrush.m_Faces[j].m_Indices[0]]);

					const TPlaneD3TP kPC(	_krBrush.m_Vertices[_krBrush.m_Faces[k].m_Indices[2]],
											_krBrush.m_Vertices[_krBrush.m_Faces[k].m_Indices[1]],
											_krBrush.m_Vertices[_krBrush.m_Faces[k].m_Indices[0]]);

					const TVectorD3 kNewVertex(math::GetIntersection(TVectorD3(), TPlaneD3DN(kPA), TPlaneD3DN(kPB), TPlaneD3DN(kPC)));

					bool bLegal = math::IsIntersection(TPlaneD3DN(kPA), TPlaneD3DN(kPB), TPlaneD3DN(kPC));
			
					if(bLegal)
					{
						for(size_t m = 0; m < _krBrush.m_Faces.size(); ++m)
						{
							const TPlaneD3DN kPM(	TPlaneD3TP(	_krBrush.m_Vertices[_krBrush.m_Faces[m].m_Indices[2]],
																_krBrush.m_Vertices[_krBrush.m_Faces[m].m_Indices[1]],
																_krBrush.m_Vertices[_krBrush.m_Faces[m].m_Indices[0]]));
							if(math::DotProduct(kPM.m_Normal, kNewVertex) - kPM.m_dDistance > s_kdEpsilon)
							{
								bLegal = false;
							}
						}
					}

					if(bLegal)
					{
						_rResult[i].push_back(kNewVertex);
						_rResult[j].push_back(kNewVertex);
						_rResult[k].push_back(kNewVertex);
					}
				}
			}
		}
	}

	return(_rResult);
}

const std::vector<std::vector<TVectorD3>>& GetPolyFaces(	std::vector<std::vector<TVectorD3>>& _rResult,
															const TPlaneBrush& _krBrush)
{
	_rResult.resize(_krBrush.m_Faces.size());

	for(size_t i = 0; i < _krBrush.m_Faces.size()-2; ++i)
	{
		for(size_t j = i; j < _krBrush.m_Faces.size()-1; ++j)
		{
			for(size_t k = j; k < _krBrush.m_Faces.size(); ++k)
			{
				if(		(i != j)
					&&	(j != k)
					&&	(k != i))
				{
					const TPlaneD3TP kPA = _krBrush.m_Faces[i].m_Plane;

					const TPlaneD3TP kPB = _krBrush.m_Faces[j].m_Plane;

					const TPlaneD3TP kPC = _krBrush.m_Faces[k].m_Plane;

					const TVectorD3 kNewVertex(math::GetIntersection(TVectorD3(), TPlaneD3DN(kPA), TPlaneD3DN(kPB), TPlaneD3DN(kPC)));

					bool bLegal = math::IsIntersection(TPlaneD3DN(kPA), TPlaneD3DN(kPB), TPlaneD3DN(kPC));
			
					if(bLegal)
					{
						for(size_t m = 0; m < _krBrush.m_Faces.size(); ++m)
						{
							const TPlaneD3DN kPM(_krBrush.m_Faces[m].m_Plane);
							const double kdExclusionDotProduct = math::DotProduct(kPM.m_Normal, kNewVertex);
							if(kdExclusionDotProduct - kPM.m_dDistance > s_kdEpsilon)
							{
								bLegal = false;
							}
						}
					}

					if(bLegal)
					{
						_rResult[i].push_back(kNewVertex);
						_rResult[j].push_back(kNewVertex);
						_rResult[k].push_back(kNewVertex);
					}
				}
			}
		}
	}

	return(_rResult);
}

const std::vector<TVectorD3>& SortFaceVerts(	std::vector<TVectorD3>& _rResult,
												const std::vector<TVectorD3>& _krInput,
												const TVectorD3& _krFaceNormal,
												const bool _kbClockwise)
{
	_rResult = _krInput;

	// Find Center Point
	TVectorD3 Center(0.0, 0.0, 0.0);
	for(size_t i = 0; i < _rResult.size(); ++i)
	{
		Center = math::Add(Center, Center, _rResult[i]);
	}
	Center = math::ScalarMultiply(Center, Center, (1.0 / (double)_rResult.size()));

	// For all verts in face
	for(size_t i = 0; i < _rResult.size()-2; ++i)
	{
		double dLargestDotProduct = -1.0f;
		size_t szNext = -1;

		// Find vec CenterToA
		const TVectorD3 kToA = math::Normalize(TVectorD3(), math::Subtract(TVectorD3(), _rResult[i], Center));
		// Find Normal for Exclusion plane
		const TVectorD3 kExclusionNormal = math::Normalize(TVectorD3(), math::CrossProduct(TVectorD3(), _krFaceNormal, kToA));
		// Check all cantidates
		for(size_t j = i + 1; j < _rResult.size(); ++j)
		{	
			// Find vec CenterToB
			const TVectorD3 kToB = math::Normalize(TVectorD3(), math::Subtract(TVectorD3(), _rResult[j], Center));
			// If new point is not excluded by exclusion plane...
			const double kdExclusionDotProduct = math::DotProduct(kExclusionNormal, kToB);
			if(kdExclusionDotProduct > -s_kdEpsilon)
			{
				// Find its dot product with ToA (looking for minimum angle = max DP)
				const double kdNewDotProduct = math::DotProduct(kToA, kToB);
				// If new angle is closer (larger DP)
				if(kdNewDotProduct > dLargestDotProduct)
				{
					// Set as best cantidate
					dLargestDotProduct = kdNewDotProduct;
					szNext = j;
				}
			}
		}

		if(szNext < _rResult.size())
		{
			std::swap(_rResult[i+1], _rResult[szNext]);
		}
	}

	const TVectorD3 kNormal = math::Normalize(	TVectorD3(),
												math::CrossProduct(	TVectorD3(),
																	math::Subtract(TVectorD3(), _rResult[1], _rResult[2]),
																	math::Subtract(TVectorD3(), _rResult[1], _rResult[0])));
	if(math::DotProduct(kNormal, _krFaceNormal) > 0.0)
	{
		if(!_kbClockwise)
		{
			std::reverse(_rResult.begin(), _rResult.end());
		}
	}
	else
	{
		if(_kbClockwise)
		{
			std::reverse(_rResult.begin(), _rResult.end());
		}
	}

	return(_rResult);
}

const TVectorD3& GetFaceNormal(TVectorD3& _rResult, const size_t _kszFace, const TPolyBrush& _krBrush)
{
	const TPlaneD3DN kPlane = TPlaneD3DN(TPlaneD3TP(_krBrush.m_Vertices[_krBrush.m_Faces[_kszFace].m_Indices[2]],
													_krBrush.m_Vertices[_krBrush.m_Faces[_kszFace].m_Indices[1]],
													_krBrush.m_Vertices[_krBrush.m_Faces[_kszFace].m_Indices[0]]));

	_rResult = kPlane.m_Normal;
	return(_rResult);
}

const TVectorD3& GetFaceNormal(TVectorD3& _rResult, const size_t _kszFace, const TPlaneBrush& _krBrush)
{
	const TPlaneD3DN kPlane = TPlaneD3DN(_krBrush.m_Faces[_kszFace].m_Plane);

	_rResult = kPlane.m_Normal;
	return(_rResult);
}