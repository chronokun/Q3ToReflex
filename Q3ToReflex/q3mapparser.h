//
//	Author:	Michael Cameron
//	Email:	chronokun@hotmail.com
//

#pragma once

#ifndef __Q3MAPPARSER_H__
#define __Q3MAPPARSER_H__

// Prototypes
struct TPlaneBrush;
struct TPlaneBrushFace;

// Enums
enum EParserState
{
	PARSERSTATE_TOPLEVEL,
	PARSERSTATE_ENTITY,
	PARSERSTATE_BRUSH
};

class CQ3MapParser
{
	// Variables
public:
	std::vector<TPlaneBrush> m_Brushes;

	// Functions
public:
	CQ3MapParser();
	const bool LoadMap(const char* _kpcFileName);
	void ParseEntity(const std::vector<std::string>& _krTokens);
	void ParseBrush(const std::vector<std::vector<std::string>>& _krLines);
	const bool ParseBrushFace(TPlaneBrushFace& _rFace, const std::vector<std::string>& _krTokens);

	const bool ParseQ3Map(const char* _kpcFileName);
};

#endif