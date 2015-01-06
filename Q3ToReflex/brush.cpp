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
	// Make as many polygons as faces
	_rResult.resize(_krBrush.m_Faces.size());

	// Check our brush has atleast four sides as all 3d volumes should
	if(_krBrush.m_Faces.size() >= 4)
	{
		// For all plane combinations
		for(size_t i = 0; i < _krBrush.m_Faces.size()-2; ++i)
		{
			for(size_t j = i; j < _krBrush.m_Faces.size()-1; ++j)
			{
				for(size_t k = j; k < _krBrush.m_Faces.size(); ++k)
				{
					// Check we aren't trying to intersect a plane against itself
					if(		(i != j)
						&&	(j != k)
						&&	(k != i))
					{
						// Get brush planes
						const TPlaneD3TP kPA(	_krBrush.m_Vertices[_krBrush.m_Faces[i].m_Indices[2]],
												_krBrush.m_Vertices[_krBrush.m_Faces[i].m_Indices[1]],
												_krBrush.m_Vertices[_krBrush.m_Faces[i].m_Indices[0]]);

						const TPlaneD3TP kPB(	_krBrush.m_Vertices[_krBrush.m_Faces[j].m_Indices[2]],
												_krBrush.m_Vertices[_krBrush.m_Faces[j].m_Indices[1]],
												_krBrush.m_Vertices[_krBrush.m_Faces[j].m_Indices[0]]);

						const TPlaneD3TP kPC(	_krBrush.m_Vertices[_krBrush.m_Faces[k].m_Indices[2]],
												_krBrush.m_Vertices[_krBrush.m_Faces[k].m_Indices[1]],
												_krBrush.m_Vertices[_krBrush.m_Faces[k].m_Indices[0]]);

						// Find intersection point if one exists
						const TVectorD3 kNewVertex(math::GetIntersection(TVectorD3(), TPlaneD3DN(kPA), TPlaneD3DN(kPB), TPlaneD3DN(kPC)));

						// Check if planes actually intersect and gave valid intersection point
						bool bLegal = math::IsIntersection(TPlaneD3DN(kPA), TPlaneD3DN(kPB), TPlaneD3DN(kPC));
				
						if(bLegal)
						{
							for(size_t m = 0; m < _krBrush.m_Faces.size(); ++m)
							{
								const TPlaneD3DN kPM(	TPlaneD3TP(	_krBrush.m_Vertices[_krBrush.m_Faces[m].m_Indices[2]],
																	_krBrush.m_Vertices[_krBrush.m_Faces[m].m_Indices[1]],
																	_krBrush.m_Vertices[_krBrush.m_Faces[m].m_Indices[0]]));
								// Check vertex against exclusion plane to find if it lies outside brush and needs to be discarded
								if(math::DotProduct(kPM.m_Normal, kNewVertex) - kPM.m_dDistance > s_kdEpsilon)
								{
									bLegal = false;
								}
							}
						}

						// Add point to all three faces if valid
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
	}

	return(_rResult);
}

const std::vector<std::vector<TVectorD3>>& GetPolyFaces(	std::vector<std::vector<TVectorD3>>& _rResult,
															const TPlaneBrush& _krBrush)
{
	// Make as many polygons as faces
	_rResult.resize(_krBrush.m_Faces.size());

	// Check our brush has atleast four sides as all 3d volumes should
	if(_krBrush.m_Faces.size() >= 4)
	{
		// For all plane combinations
		for(size_t i = 0; i < _krBrush.m_Faces.size()-2; ++i)
		{
			for(size_t j = i; j < _krBrush.m_Faces.size()-1; ++j)
			{
				for(size_t k = j; k < _krBrush.m_Faces.size(); ++k)
				{
					// Check we aren't trying to intersect a plane against itself
					if(		(i != j)
						&&	(j != k)
						&&	(k != i))
					{
						// Get brush planes
						const TPlaneD3TP kPA = _krBrush.m_Faces[i].m_Plane;

						const TPlaneD3TP kPB = _krBrush.m_Faces[j].m_Plane;

						const TPlaneD3TP kPC = _krBrush.m_Faces[k].m_Plane;

						// Find intersection point if one exists
						const TVectorD3 kNewVertex(math::GetIntersection(TVectorD3(), TPlaneD3DN(kPA), TPlaneD3DN(kPB), TPlaneD3DN(kPC)));

						// Check if planes actually intersect and gave valid intersection point
						bool bLegal = math::IsIntersection(TPlaneD3DN(kPA), TPlaneD3DN(kPB), TPlaneD3DN(kPC));
				
						if(bLegal)
						{
							for(size_t m = 0; m < _krBrush.m_Faces.size(); ++m)
							{
								// Check vertex against exclusion plane to find if it lies outside brush and needs to be discarded
								const TPlaneD3DN kPM(_krBrush.m_Faces[m].m_Plane);
								const double kdExclusionDotProduct = math::DotProduct(kPM.m_Normal, kNewVertex);
								if(kdExclusionDotProduct - kPM.m_dDistance > s_kdEpsilon)
								{
									bLegal = false;
								}
							}
						}

						// Add point to all three faces if valid
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

		// Swap vertex order if necessary
		if(szNext < _rResult.size())
		{
			std::swap(_rResult[i+1], _rResult[szNext]);
		}
	}

	const TVectorD3 kNormal = math::Normalize(	TVectorD3(),
												math::CrossProduct(	TVectorD3(),
																	math::Subtract(TVectorD3(), _rResult[1], _rResult[2]),
																	math::Subtract(TVectorD3(), _rResult[1], _rResult[0])));

	// Reverse face winding if normal is facing the wrong direction.
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

const bool CheckForBrushCull(const std::string& _krInput)
{
	const std::array<std::string, 7> kCullStrings = { 
		"common/hint",
		"common/hintskip",
		"common/weapclip",
		"common/metalclip",
		"common/botclip",
		"common/clusterportal" };

	// Check input string against array of materials to cull
	for(const std::string& krCullString : kCullStrings)
	{
		if(strcmp(_krInput.c_str(), krCullString.c_str()) == 0)
		{
			return(true);
		}
	}

	return(false);
}

const TPolyBrush& ToPolyBrush(TPolyBrush& _rResult, const TPlaneBrush& _krInput)
{
	// Get unsorted polygons
	const std::vector<std::vector<TVectorD3>> kPolysUnsorted = GetPolyFaces(std::vector<std::vector<TVectorD3>>(), _krInput);

	// Sort polygons
	std::vector<std::vector<TVectorD3>> SortedPolys;
	// Sort each face...
	for(size_t i = 0; i < _krInput.m_Faces.size(); ++i)
	{
		// Check face has atleast three vertices
		if(kPolysUnsorted[i].size() >= 3)
		{
			const TVectorD3 kFaceNormal = GetFaceNormal(TVectorD3(), i, _krInput);
			// Get sorted vertices for face
			const std::vector<TVectorD3> kSortedFaceVerts = SortFaceVerts(std::vector<TVectorD3>(), kPolysUnsorted[i], kFaceNormal, false);
			// Add to our sorted polygons
			SortedPolys.push_back(kSortedFaceVerts);
		}
		else
		{
			// If face is invalid, push empty vector
			SortedPolys.push_back(std::vector<TVectorD3>());
		}
	}

	// Add all brush vertices to a single vector of unmerged vertices
	std::vector<TVectorD3> UnmergedFaceVerts;
	for(size_t n = 0; n < SortedPolys.size(); ++n)
	{
		for(size_t m = 0; m < SortedPolys[n].size(); ++m)
		{
			UnmergedFaceVerts.push_back(SortedPolys[n][m]);
		}
	}

	// Merge vertices so they are not duplicated across faces
	std::vector<TVectorD3> MergedFaceVerts;
	for(size_t n = 0; n < UnmergedFaceVerts.size(); ++n)
	{
		bool bUnique = true;
		for(size_t m = 0; m < MergedFaceVerts.size(); ++m)
		{
			// If a vertex already exists in our merged list, flag as non-unique
			if(math::Equal(UnmergedFaceVerts[n], MergedFaceVerts[m], s_kdEpsilon))
			{
				bUnique = false;
			}
		}
		// If a vertex doesn't yet exist in our list of merged vertices, add it.
		if(bUnique)
		{
			MergedFaceVerts.push_back(UnmergedFaceVerts[n]);
		}
	}

	// Assign our merged vertices to our corresponding polygonal brush
	_rResult.m_Vertices = MergedFaceVerts;
	// Set number of faces for brush
	_rResult.m_Faces.resize(_krInput.m_Faces.size());
	// For each brush face...
	for(size_t i = 0; i < _rResult.m_Faces.size(); ++i)
	{
		// Copy properties from out plane-defined brush
		_rResult.m_Faces[i].m_Material = _krInput.m_Faces[i].m_Material;

		_rResult.m_Faces[i].m_dTexCoordU = (double)_krInput.m_Faces[i].m_iTexCoordU;
		_rResult.m_Faces[i].m_dTexCoordV = (double)_krInput.m_Faces[i].m_iTexCoordV;

		_rResult.m_Faces[i].m_dTexScaleU = (double)_krInput.m_Faces[i].m_dTexScaleU;
		_rResult.m_Faces[i].m_dTexScaleV = (double)_krInput.m_Faces[i].m_dTexScaleV;

		_rResult.m_Faces[i].m_dTexRotation = (double)_krInput.m_Faces[i].m_dTexRotation;

		// Find new indices that correspond to our brushes merged vertices
		_rResult.m_Faces[i].m_Indices.resize(SortedPolys[i].size());
		for(size_t n = 0; n < _rResult.m_Faces[i].m_Indices.size(); ++n)
		{
			for(size_t m = 0; m < _rResult.m_Vertices.size(); ++m)
			{
				// If brush vertex matches polygon vertex, assign it's index to our faces indices
				if(math::Equal(SortedPolys[i][n], _rResult.m_Vertices[m], s_kdEpsilon))
				{
					_rResult.m_Faces[i].m_Indices[n] = m;
				}
			}
		}
	}

	return(_rResult);
}

const TPolyBrush& ExtrudeFaceToBrush(	TPolyBrush& _rResult, const std::vector<TVectorD3>& _krVertices, const std::string& _krMaterial,
										const double _kdTexCoordU, const double _kdTexCoordV,
										const double _kdTexScaleU, const double _kdTexScaleV,
										const double _kdTexRotation)
{
	const TVectorD3 kNormal = math::GetPolygonNormal(TVectorD3(), _krVertices);

	// Find center point
	TVectorD3 Center(0.0, 0.0, 0.0);
	for(size_t i = 0; i < _krVertices.size(); ++i)
	{
		Center = math::Add(Center, Center, _krVertices[i]);
	}
	Center = math::ScalarMultiply(Center, Center, (1.0 / (double)_krVertices.size()));

	// Find back point
	const TVectorD3 kBackPoint = math::Add(TVectorD3(), Center, math::ScalarMultiply(TVectorD3(), kNormal, -4.0f));

	// Add vertices to brush
	_rResult.m_Vertices.resize(_krVertices.size()+1);
	for(size_t i = 0; i < _rResult.m_Vertices.size()-1; ++i)
	{
		_rResult.m_Vertices[i] = _krVertices[i];
	}
	_rResult.m_Vertices[_rResult.m_Vertices.size()-1] = kBackPoint;

	// Add indices
	_rResult.m_Faces.resize(_krVertices.size()+1);
	for(size_t i = 0; i < _rResult.m_Vertices.size()-1; ++i)
	{
		_rResult.m_Faces[0].m_Indices.push_back(i);
	}
	std::reverse(_rResult.m_Faces[0].m_Indices.begin(), _rResult.m_Faces[0].m_Indices.end());
	for(size_t i = 1; i < _rResult.m_Faces.size(); ++i)
	{
		_rResult.m_Faces[i].m_Indices.push_back(i-1);
		_rResult.m_Faces[i].m_Indices.push_back(_rResult.m_Vertices.size()-1);
		_rResult.m_Faces[i].m_Indices.push_back(i % (_rResult.m_Faces.size()-1));
		std::reverse(_rResult.m_Faces[i].m_Indices.begin(), _rResult.m_Faces[i].m_Indices.end());
	}

	// Set face properties
	for(size_t i = 1; i < _rResult.m_Faces.size(); ++i)
	{
		_rResult.m_Faces[i].m_dTexCoordU = 0.0;
		_rResult.m_Faces[i].m_dTexCoordV = 0.0;
		_rResult.m_Faces[i].m_dTexScaleU = 0.0;
		_rResult.m_Faces[i].m_dTexScaleV = 0.0;
		_rResult.m_Faces[i].m_dTexRotation = 0.0;
		_rResult.m_Faces[i].m_Material = "internal/editor/textures/editor_nolight";
	}
	_rResult.m_Faces[0].m_dTexCoordU = _kdTexCoordU;
	_rResult.m_Faces[0].m_dTexCoordV = _kdTexCoordV;
	_rResult.m_Faces[0].m_dTexScaleU = _kdTexScaleU;
	_rResult.m_Faces[0].m_dTexScaleV = _kdTexScaleV;
	_rResult.m_Faces[0].m_dTexRotation = _kdTexRotation;
	_rResult.m_Faces[0].m_Material = _krMaterial;

	return(_rResult);
}

const std::vector<TPolyBrush>& BuildPatchBrushes(	std::vector<TPolyBrush>& _rResult, const TVectorD3 _Controls[9], const int _kiLevel,
													const std::string& _krMaterial,
													const double _kdTexCoordU, const double _kdTexCoordV,
													const double _kdTexScaleU, const double _kdTexScaleV,
													const double _kdTexRotation)
{
	const int kiL = _kiLevel;
	const int kiL1 = _kiLevel + 1;

	std::vector<TVectorD3> Verts;

	Verts.resize(kiL1 * kiL1);

	// Make Verts
	for(int i = 0; i <= kiL; ++i)
	{
		float a = (float)i / (float)kiL;
		float b = 1.0f - a;

		TVectorD3 P0, P1, P2, P3, PF;

		math::ScalarMultiply(P0, _Controls[0], (b * b));
		math::ScalarMultiply(P1, _Controls[3], (2 * b * a));
		math::ScalarMultiply(P2, _Controls[6], (a * a));

		math::Add(P3, P0, P1);
		math::Add(PF, P3, P2);

		Verts[i] = PF;
	}
	for(int i = 1; i <= kiL; ++i)
	{
		float a = (float)i / (float)kiL;
		float b = 1.0f - a;
		
		TVectorD3 temp[3];
		
		for(int j = 0; j < 3; ++j)
		{
			int k = 3 * j;

			TVectorD3 P0, P1, P2, P3, PF;

			math::ScalarMultiply(P0, _Controls[k+0], (b * b));
			math::ScalarMultiply(P1, _Controls[k+1], (2 * b * a));
			math::ScalarMultiply(P2, _Controls[k+2], (a * a));

			math::Add(P3, P0, P1);
			math::Add(PF, P3, P2);

			temp[j] = PF;
		}
		
		for(int j = 0; j <= kiL; ++j)
		{
			float a = (float)j / (float)kiL;
			float b = 1.0f - a;

			TVectorD3 P0, P1, P2, P3, PF;

			math::ScalarMultiply(P0, temp[0], (b * b));
			math::ScalarMultiply(P1, temp[1], (2 * b * a));
			math::ScalarMultiply(P2, temp[2], (a * a));

			math::Add(P3, P0, P1);
			math::Add(PF, P3, P2);
		
			Verts[i * kiL1 + j] = PF;
		}
	}

	// Make Faces
	for(int x = 0; x < kiL1-1; ++x)
	{
		for(int y = 0; y < kiL1-1; ++y)
		{
			const int ki3 = x + (y*kiL1);
			const int ki2 = x + 1 + (y*kiL1);
			const int ki1 = x + ((y + 1) * kiL1);
			const int ki0 = (x + 1) + ((y + 1)*kiL1);
			const std::vector<TVectorD3> kFaceVerts = { Verts[ki0], Verts[ki1], Verts[ki3], Verts[ki2] };
			_rResult.push_back(ExtrudeFaceToBrush(TPolyBrush(), kFaceVerts, _krMaterial, _kdTexCoordU, _kdTexCoordV, _kdTexScaleU, _kdTexScaleV, _kdTexRotation));
		}
	}

	return(_rResult);
}
