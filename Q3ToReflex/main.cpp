//
//	Author:	Michael Cameron
//	Email:	chronokun@hotmail.com
//

// Libraries Include
#include "libraries.h"
// Local Includes
#include "constants.h"
#include "mathtypes.h"
#include "vector.h"
#include "geometry.h"
#include "brush.h"
#include "q3mapparser.h"

const std::string& GetBrushString(std::string& _rOutput, const TPolyBrush& _krBrush)
{
	if(_krBrush.m_Faces.size() > 0)
	{
		std::stringstream ssOutput;

		ssOutput << std::fixed;
		ssOutput << "brush" << std::endl;
		ssOutput << "\tvertices" << std::endl;
		for(size_t i = 0; i < _krBrush.m_Vertices.size(); ++i)
		{
			ssOutput << "\t\t" << _krBrush.m_Vertices[i].m_dX << " " << _krBrush.m_Vertices[i].m_dZ << " " << _krBrush.m_Vertices[i].m_dY << " " << std::endl;
		}
		ssOutput << "\tfaces" << std::endl;
		for(size_t i = 0; i < _krBrush.m_Faces.size(); ++i)
		{
			ssOutput << "\t\t" << _krBrush.m_Faces[i].m_dTexCoordU << " "
				<< _krBrush.m_Faces[i].m_dTexCoordV << " "
				<< _krBrush.m_Faces[i].m_dTexScaleU << " "
				<< _krBrush.m_Faces[i].m_dTexScaleV << " "
				<< _krBrush.m_Faces[i].m_dTexRotation;
			for(size_t j = 0; j < _krBrush.m_Faces[i].m_Indices.size(); ++j)
			{
				ssOutput << " " << _krBrush.m_Faces[i].m_Indices[j];
			}
			ssOutput << " " << _krBrush.m_Faces[i].m_Material << std::endl;
		}

		_rOutput = ssOutput.str();
	}
	return(_rOutput);
}

int main(const int _kiArgC, const char** _kppcArgv)
{
	if(_kiArgC != 3)
	{
		return(3);
	}

	CQ3MapParser Parser;
	const bool kbSuccess = Parser.ParseQ3Map(_kppcArgv[1]);

	if(!kbSuccess)
	{
		return(1);
	}
	else
	{
		std::ofstream OutFile;
		OutFile.open(_kppcArgv[2]);
		if(!OutFile.is_open())
		{
			return(2);
		}

		std::vector<TPolyBrush> PolyBrushes(Parser.m_Brushes.size());

		for(size_t i = 0; i < Parser.m_Brushes.size(); ++i)
		{
			const std::vector<std::vector<TVectorD3>> kPolysUnsorted = GetPolyFaces(std::vector<std::vector<TVectorD3>>(), Parser.m_Brushes[i]);

			std::vector<std::vector<TVectorD3>> SortedPolys;
			for(size_t j = 0; j < Parser.m_Brushes[i].m_Faces.size(); ++j)
			{
				if(kPolysUnsorted[j].size() >= 3)
				{
					const TVectorD3 kFaceNormal = GetFaceNormal(TVectorD3(), j, Parser.m_Brushes[i]);
					const std::vector<TVectorD3> kSortedFaceVerts = SortFaceVerts(std::vector<TVectorD3>(), kPolysUnsorted[j], kFaceNormal, false);
					SortedPolys.push_back(kSortedFaceVerts);
				}
				else
				{
					SortedPolys.push_back(std::vector<TVectorD3>());
				}
			}

			std::vector<TVectorD3> UnmergedFaceVerts;
			for(size_t n = 0; n < SortedPolys.size(); ++n)
			{
				for(size_t m = 0; m < SortedPolys[n].size(); ++m)
				{
					UnmergedFaceVerts.push_back(SortedPolys[n][m]);
				}
			}

			std::vector<TVectorD3> MergedFaceVerts;
			for(size_t n = 0; n < UnmergedFaceVerts.size(); ++n)
			{
				bool bUnique = true;
				for(size_t m = 0; m < MergedFaceVerts.size(); ++m)
				{
					if(math::Equal(UnmergedFaceVerts[n], MergedFaceVerts[m], s_kdEpsilon))
					{
						bUnique = false;
					}
				}
				if(bUnique)
				{
					MergedFaceVerts.push_back(UnmergedFaceVerts[n]);
				}
			}

			PolyBrushes[i].m_Vertices = MergedFaceVerts;
			PolyBrushes[i].m_Faces.resize(Parser.m_Brushes[i].m_Faces.size());
			for(size_t j = 0; j < PolyBrushes[i].m_Faces.size(); ++j)
			{
				PolyBrushes[i].m_Faces[j].m_Material = Parser.m_Brushes[i].m_Faces[j].m_Material;

				PolyBrushes[i].m_Faces[j].m_dTexCoordU = (double)Parser.m_Brushes[i].m_Faces[j].m_iTexCoordU;
				PolyBrushes[i].m_Faces[j].m_dTexCoordV = (double)Parser.m_Brushes[i].m_Faces[j].m_iTexCoordV;

				PolyBrushes[i].m_Faces[j].m_dTexScaleU = (double)Parser.m_Brushes[i].m_Faces[j].m_dTexScaleU;
				PolyBrushes[i].m_Faces[j].m_dTexScaleV = (double)Parser.m_Brushes[i].m_Faces[j].m_dTexScaleV;

				PolyBrushes[i].m_Faces[j].m_dTexRotation = (double)Parser.m_Brushes[i].m_Faces[j].m_dTexRotation;

				PolyBrushes[i].m_Faces[j].m_Indices.resize(SortedPolys[j].size());
				for(size_t n = 0; n < PolyBrushes[i].m_Faces[j].m_Indices.size(); ++n)
				{
					for(size_t m = 0; m < PolyBrushes[i].m_Vertices.size(); ++m)
					{
						if(math::Equal(SortedPolys[j][n], PolyBrushes[i].m_Vertices[m], s_kdEpsilon))
						{
							PolyBrushes[i].m_Faces[j].m_Indices[n] = m;
						}
					}
				}
			}
		}

		OutFile << "reflex map version 6\n"
				<< "entity\n"
				<< "\ttype WorldSpawn\n"
				<< "\tColourXRGB32 clearColor ffffff\n"
				<< "\tColourXRGB32 worldColor0 0\n"
				<< "\tColourXRGB32 worldColor1 0\n";
		for(size_t i = 0; i < PolyBrushes.size(); ++i)
		{
			const std::string BrushString = GetBrushString(std::string(), PolyBrushes[i]);
			OutFile << BrushString;
		}

		OutFile.close();
	}

	return(0);
}