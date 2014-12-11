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

		bool bIsWorldSpawn = false;
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
						if(bIsWorldSpawn)
						{
							this->m_Brushes.push_back(TPlaneBrush());
						}
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
						bIsWorldSpawn = false;
					}
					else if(eState == PARSERSTATE_PATCH)
					{
						eState = PARSERSTATE_BRUSH;
					}
				}
				else if(strcmp("patchDef2", Lines[i][0].c_str()) == 0)
				{
					eState = PARSERSTATE_PATCH;
				}
			}
			else
			{
				if(eState == PARSERSTATE_ENTITY)
				{
					if((Lines[i].size() == 2) && (strcmp("\"classname\"", Lines[i][0].c_str()) == 0))
					{
						if(strcmp("\"worldspawn\"", Lines[i][1].c_str()) == 0)
						{
							bIsWorldSpawn = true;
						}
						else
						{
							bIsWorldSpawn = false;
						}
					}
					//this->ParseEntity(Lines[i]);
				}
				else if(eState == PARSERSTATE_BRUSH)
				{
					if(bIsWorldSpawn)
					{
						TPlaneBrushFace BrushFace;
						//this->m_Brushes[this->m_Brushes.size()-1].m_Faces.push_back(TPlaneBrushFace());
						if(this->ParseBrushFace(BrushFace, Lines[i]))
						{
							this->m_Brushes[this->m_Brushes.size()-1].m_Faces.push_back(BrushFace);
						}
					}
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

const bool CQ3MapParser::ParseBrushFace(TPlaneBrushFace& _rFace, const std::vector<std::string>& _krTokens)
{
	if((strcmp(_krTokens[0].c_str(), "(") == 0) && (strcmp(_krTokens[4].c_str(), ")") == 0))
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

		_rFace.m_Material = this->SubstituteMaterial(std::string(), _krTokens[15]);

		_rFace.m_iTexCoordU = std::stoi(_krTokens[16]);
		_rFace.m_iTexCoordV = std::stoi(_krTokens[17]);

		_rFace.m_dTexRotation = std::stod(_krTokens[18]);

		_rFace.m_dTexScaleU = std::stod(_krTokens[19]);
		_rFace.m_dTexScaleV = std::stod(_krTokens[20]);
		return(true);
	}
	else
	{
		return(false);
	}
}

const std::string& CQ3MapParser::SubstituteMaterial(std::string& _rResult, const std::string& _krInput)
{
	_rResult = _krInput;

	if(strcmp(_krInput.c_str(), "common/clip") == 0)
	{
		_rResult = "internal/editor/textures/editor_clip";
	}

	return(_krInput);
}
