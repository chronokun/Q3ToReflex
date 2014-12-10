//
//	Author:	Michael Cameron
//	Email:	chronokun@hotmail.com
//

// Libraries Include
#include "libraries.h"
// Local Includes
#include "brush.h"
// This Include
#include "q3mapparser.h"

CQ3MapParser::CQ3MapParser()
{
	//
}

const bool CQ3MapParser::ParseQ3Map(const char* _kpcFileName)
{
	std::ifstream InFile;

	std::vector<std::vector<std::string>> Lines;

	InFile.open(_kpcFileName, std::ios::in);
	if(InFile.is_open())
	{
		std::string Line;
		size_t szLevel = -1;
		while(std::getline(InFile, Line))
		{
			Lines.push_back(std::vector<std::string>());
			const size_t kszLineSize = 2048;
			char pcLine[kszLineSize];
			const char* kpcDelim  = " \t\n\r";
			char* pcToken;
			char* pcContext;

			strcpy_s(pcLine, Line.c_str());
			pcToken = strtok_s(pcLine, kpcDelim, &pcContext);

			while(pcToken != nullptr)
			{
				if(strcmp("//", pcToken) == 0)
				{
					pcToken = strtok_s(nullptr, kpcDelim, &pcContext);
					break;
				}
				else
				{
					Lines[Lines.size()-1].push_back(pcToken);
					pcToken = strtok_s(nullptr, kpcDelim, &pcContext);
				}
			}
		}
		InFile.close();

		EParserState eState = PARSERSTATE_TOPLEVEL;
		for(size_t i = 0; i < Lines.size(); ++i)
		{
			if(Lines[i].size() == 1)
			{
				if(strcmp("{", Lines[i][0].c_str()) == 0)
				{
					if(eState == PARSERSTATE_TOPLEVEL)
					{
						eState = PARSERSTATE_ENTITY;
					}
					else if(eState == PARSERSTATE_ENTITY)
					{
						eState = PARSERSTATE_BRUSH;
						this->m_Brushes.push_back(TPlaneBrush());
					}
				}
				else if(strcmp("}", Lines[i][0].c_str()) == 0)
				{
					if(eState == PARSERSTATE_BRUSH)
					{
						eState = PARSERSTATE_ENTITY;
					}
					else if(eState == PARSERSTATE_ENTITY)
					{
						eState = PARSERSTATE_TOPLEVEL;
					}
				}
			}
			else
			{
				if(eState == PARSERSTATE_ENTITY)
				{
					this->ParseEntity(Lines[i]);
				}
				else if(eState == PARSERSTATE_BRUSH)
				{
					this->m_Brushes[this->m_Brushes.size()-1].m_Faces.push_back(TPlaneBrushFace());
					this->ParseBrushFace(this->m_Brushes[this->m_Brushes.size()-1].m_Faces[this->m_Brushes[this->m_Brushes.size()-1].m_Faces.size()-1], Lines[i]);
				}
			}
		}
		//
	}
	else
	{
		return(false);
	}

	return(true);
}

void CQ3MapParser::ParseEntity(const std::vector<std::string>& _krTokens)
{
	//
}

void CQ3MapParser::ParseBrushFace(TPlaneBrushFace& _rFace, const std::vector<std::string>& _krTokens)
{
	_rFace.m_Plane.m_A.m_dX = std::stod(_krTokens[1]);
	_rFace.m_Plane.m_A.m_dY = std::stod(_krTokens[2]);
	_rFace.m_Plane.m_A.m_dZ = std::stod(_krTokens[3]);

	_rFace.m_Plane.m_B.m_dX = std::stod(_krTokens[6]);
	_rFace.m_Plane.m_B.m_dY = std::stod(_krTokens[7]);
	_rFace.m_Plane.m_B.m_dZ = std::stod(_krTokens[8]);

	_rFace.m_Plane.m_C.m_dX = std::stod(_krTokens[11]);
	_rFace.m_Plane.m_C.m_dY = std::stod(_krTokens[12]);
	_rFace.m_Plane.m_C.m_dZ = std::stod(_krTokens[13]);

	_rFace.m_Material = _krTokens[15];

	_rFace.m_iTexCoordU = std::stoi(_krTokens[16]);
	_rFace.m_iTexCoordV = std::stoi(_krTokens[17]);

	_rFace.m_dTexRotation = std::stod(_krTokens[18]);

	_rFace.m_dTexScaleU = std::stod(_krTokens[19]);
	_rFace.m_dTexScaleV = std::stod(_krTokens[20]);
}