#pragma once

#ifndef __Q3MAPPARSER_H__
#define __Q3MAPPARSER_H__

// Prototypes
struct TPlaneBrush;
struct TPlaneBrushFace;

// Enums
enum EEntityType
{
	ENTITYTYPE_UNKNOWN,
	ENTITYTYPE_WORLDSPAWN
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
	const size_t ParseEntity(const std::vector<std::string>& _krTokens, const size_t _kszIndex);
	const size_t ParseBrush(const std::vector<std::string>& _krTokens, const size_t _kszIndex);
	const size_t ParseBrushFace(TPlaneBrushFace& _rFace, const std::vector<std::string>& _krTokens, const size_t _kszIndex);

};

#endif