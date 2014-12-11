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
		// Close output file
		OutFile.close();
	}

	// Return success
	return(0);
}