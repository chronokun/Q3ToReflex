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
	// Cull any brushes we don't wish to import, eg. visibility hints etc.
	bool bCullBrush = false;
	for(const TPolyBrushFace& krFace : _krBrush.m_Faces)
	{
		if(CheckForBrushCull(krFace.m_Material))
		{
			bCullBrush = true;
		}
	}

	// Check brush wasn't culled and has a valid number of faces
	if(_krBrush.m_Faces.size() >= 4 && !bCullBrush)
	{
		std::stringstream ssOutput;

		ssOutput << std::fixed;
		ssOutput << "brush" << std::endl;
		ssOutput << "\tvertices" << std::endl;
		for(size_t i = 0; i < _krBrush.m_Vertices.size(); ++i)
		{
			// Coordinate space needs to be transformed from q3 coordinate space to reflex coordinate space (RightHanded +Z Up -> LeftHanded +Y Up)
			ssOutput << "\t\t" << _krBrush.m_Vertices[i].m_dX << " " << _krBrush.m_Vertices[i].m_dZ << " " << _krBrush.m_Vertices[i].m_dY << " " << std::endl;
		}
		ssOutput << "\tfaces" << std::endl;
		for(size_t i = 0; i < _krBrush.m_Faces.size(); ++i)
		{
			ssOutput << "\t\t" << _krBrush.m_Faces[i].m_dTexCoordU << " "
				<< _krBrush.m_Faces[i].m_dTexCoordV << " "
				<< _krBrush.m_Faces[i].m_dTexScaleU * 2.0 << " "
				<< _krBrush.m_Faces[i].m_dTexScaleV * 2.0 << " "
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
	// Check we have correct number of parameters
	if(_kiArgC != 3)
	{
		return(3);
	}

	// Parse input file
	CQ3MapParser Parser;
	const bool kbSuccess = Parser.ParseQ3Map(_kppcArgv[1]);

	if(!kbSuccess)
	{
		return(1);
	}
	else
	{
		// Open output file
		std::ofstream OutFile;
		OutFile.open(_kppcArgv[2]);
		if(!OutFile.is_open())
		{
			return(2);
		}

		std::vector<TPolyBrush> PolyBrushes(Parser.m_Brushes.size());

		// For all quake style plane brushes get reflex style polygon brush equivalent
		for(size_t i = 0; i < Parser.m_Brushes.size(); ++i)
		{
			PolyBrushes[i] = ToPolyBrush(TPolyBrush(), Parser.m_Brushes[i]);
		}

		std::vector<TPolyBrush> PatchBrushes;
		for(size_t i = 0; i < Parser.m_PatchDefs.size(); ++i)
		{
			if(		(Parser.m_PatchDefs[i].m_ControlPoints.size() > 0)
				&&	(Parser.m_PatchDefs[i].m_ControlPoints[0].size() == Parser.m_PatchDefs[i].m_szRows)
				&&	(Parser.m_PatchDefs[i].m_ControlPoints.size() == Parser.m_PatchDefs[i].m_szColumns))
			{
				TVectorD3 Controls[9];
				TVectorD3 ControlsM[3][3];
				for(size_t y = 0; y < (Parser.m_PatchDefs[i].m_szRows-2); y++)
				{
					for(size_t x = 0; x < (Parser.m_PatchDefs[i].m_szColumns-2); x++)
					{
						// Controls
						ControlsM[0][0] = Parser.m_PatchDefs[i].m_ControlPoints[x+0][y+0];
						ControlsM[0][1] = Parser.m_PatchDefs[i].m_ControlPoints[x+0][y+1];
						ControlsM[0][2] = Parser.m_PatchDefs[i].m_ControlPoints[x+0][y+2];
						
						ControlsM[1][0] = Parser.m_PatchDefs[i].m_ControlPoints[x+1][y+0];
						ControlsM[1][1] = Parser.m_PatchDefs[i].m_ControlPoints[x+1][y+1];
						ControlsM[1][2] = Parser.m_PatchDefs[i].m_ControlPoints[x+1][y+2];
						
						ControlsM[2][0] = Parser.m_PatchDefs[i].m_ControlPoints[x+2][y+0];
						ControlsM[2][1] = Parser.m_PatchDefs[i].m_ControlPoints[x+2][y+1];
						ControlsM[2][2] = Parser.m_PatchDefs[i].m_ControlPoints[x+2][y+2];

						for(int j = 0; j < 9; ++j)
						{
							Controls[j] = ControlsM[j/3][j%3];
							//Controls[j] = Parser.m_PatchDefs[i].m_ControlPoints[j/3][j%3];
						}

						const std::vector<TPolyBrush> kBrushes = BuildPatchBrushes(std::vector<TPolyBrush>(), Controls, 5, Parser.m_PatchDefs[i].m_Material, 0.0, 0.0, 1.0, 1.0, 0.0);
						for(size_t k = 0; k < kBrushes.size(); ++k)
						{
							PatchBrushes.push_back(kBrushes[k]);
						}
					}
				}
			}
		}

		// Add map header and WorldSpawn entity
		OutFile << "reflex map version 6\n"
				<< "entity\n"
				<< "\ttype WorldSpawn\n"
				<< "\tColourXRGB32 clearColor ffffff\n"
				<< "\tColourXRGB32 worldColor0 0\n"
				<< "\tColourXRGB32 worldColor1 0\n";
		// Add brushes
		for(size_t i = 0; i < PolyBrushes.size(); ++i)
		{
			const std::string BrushString = GetBrushString(std::string(), PolyBrushes[i]);
			OutFile << BrushString;
		}
		// Add patches
		for(size_t i = 0; i < PatchBrushes.size(); ++i)
		{
			const std::string PatchString = GetBrushString(std::string(), PatchBrushes[i]);
			OutFile << PatchString;
		}
		// Close output file
		OutFile.close();
	}

	// Return success
	return(0);
}