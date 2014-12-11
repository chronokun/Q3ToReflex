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
	PARSERSTATE_BRUSH,
	PARSERSTATE_PATCH
};

struct TPatchDef
{
	size_t m_szRows;
	size_t m_szColumns;
	std::vector<std::vector<TVectorD3>> m_ControlPoints;
	std::string m_Material;
};

class CQ3MapParser
{
	// Variables
public:
	std::vector<TPlaneBrush> m_Brushes;
	std::vector<TPatchDef> m_PatchDefs;

	// Functions
public:
	CQ3MapParser();
	const bool LoadMap(const char* _kpcFileName);
	void ParseEntity(const std::vector<std::string>& _krTokens);
	void ParseBrush(const std::vector<std::vector<std::string>>& _krLines);
	const bool ParseBrushFace(TPlaneBrushFace& _rFace, const std::vector<std::string>& _krTokens);

	const bool ParseQ3Map(const char* _kpcFileName);

	const std::string& SubstituteMaterial(std::string& _rResult, const std::string& _krInput);

	const TPatchDef& CreatePatchDef(TPatchDef& _rResult, const std::vector<std::vector<std::string>>& _krLines);
};

#endif