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

const bool CQ3MapParser::LoadMap(const char* _kpcFileName)
{
	std::ifstream InFile;

	std::vector<std::string> Tokens;
	InFile.open(_kpcFileName, std::ios::in);
	if(InFile.is_open())
	{
		std::string Line;
		size_t szLevel = -1;
		while(std::getline(InFile, Line))
		{
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
					Tokens.push_back(pcToken);
					pcToken = strtok_s(nullptr, kpcDelim, &pcContext);
				}
			}
		}
		InFile.close();

		for(size_t i = 0; i < Tokens.size(); ++i)
		{
			if(strcmp("{", Tokens[i].c_str()) == 0)
			{
				i = this->ParseEntity(Tokens, i+1);
				break;
			}
		}
	}
	else
	{
		return(false);
	}

	return(true);
}

const size_t CQ3MapParser::ParseEntity(const std::vector<std::string>& _krTokens, const size_t _kszIndex)
{
	for(size_t i = _kszIndex; i < _krTokens.size(); ++i)
	{
		if(strcmp("{", _krTokens[i].c_str()) == 0)
		{
			i = this->ParseBrush(_krTokens, i+1);
		}
		else if(strcmp("}", _krTokens[i].c_str()) == 0)
		{
			return(i+1);
		}
	}
	return(_krTokens.size());
}

const size_t CQ3MapParser::ParseBrush(const std::vector<std::string>& _krTokens, const size_t _kszIndex)
{
	TPlaneBrush Brush;
	for(size_t i = _kszIndex; i < _krTokens.size(); ++i)
	{
		if(strcmp("}", _krTokens[i].c_str()) == 0)
		{
			if(Brush.m_Faces.size() > 0)
			{
				this->m_Brushes.push_back(Brush);
			}
			return(i);
		}
		else if(strcmp("patchDef2", _krTokens[i].c_str()) == 0)
		{
			for(size_t j = i; j < _krTokens.size(); ++j)
			{
				if(strcmp("}", _krTokens[j].c_str()) == 0)
				{
					i = j;
					break;
				}
			}
		}
		else
		{
			Brush.m_Faces.push_back(TPlaneBrushFace());
			i = this->ParseBrushFace(Brush.m_Faces[Brush.m_Faces.size()-1], _krTokens, i);
		}
	}
	return(_krTokens.size());
}

const size_t CQ3MapParser::ParseBrushFace(TPlaneBrushFace& _rFace, const std::vector<std::string>& _krTokens, const size_t _kszIndex)
{
	std::vector<std::string> DebugShit;
	if(_kszIndex > 15)
	{
		for(size_t i = _kszIndex-10; i < _kszIndex+10; ++i)
		{
			DebugShit.push_back(_krTokens[i]);
		}
	}


	_rFace.m_Plane.m_A.m_dX = std::stod(_krTokens[_kszIndex+1]);
	_rFace.m_Plane.m_A.m_dY = std::stod(_krTokens[_kszIndex+2]);
	_rFace.m_Plane.m_A.m_dZ = std::stod(_krTokens[_kszIndex+3]);

	_rFace.m_Plane.m_B.m_dX = std::stod(_krTokens[_kszIndex+6]);
	_rFace.m_Plane.m_B.m_dY = std::stod(_krTokens[_kszIndex+7]);
	_rFace.m_Plane.m_B.m_dZ = std::stod(_krTokens[_kszIndex+8]);

	_rFace.m_Plane.m_C.m_dX = std::stod(_krTokens[_kszIndex+11]);
	_rFace.m_Plane.m_C.m_dY = std::stod(_krTokens[_kszIndex+12]);
	_rFace.m_Plane.m_C.m_dZ = std::stod(_krTokens[_kszIndex+13]);

	_rFace.m_Material = _krTokens[_kszIndex+15];

	_rFace.m_iTexCoordU = std::stoi(_krTokens[_kszIndex+16]);
	_rFace.m_iTexCoordV = std::stoi(_krTokens[_kszIndex+17]);

	_rFace.m_dTexRotation = std::stod(_krTokens[_kszIndex+18]);

	_rFace.m_dTexScaleU = std::stod(_krTokens[_kszIndex+19]);
	_rFace.m_dTexScaleV = std::stod(_krTokens[_kszIndex+20]);

	return(_kszIndex+23);
}