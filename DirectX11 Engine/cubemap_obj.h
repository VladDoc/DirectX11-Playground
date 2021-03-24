#pragma once

#include "texture_obj.h"

struct faces_paths
{
	std::string posx, negx;
	std::string posy, negy;
	std::string posz, negz;
};

struct CubeMapObj : public TextureObj
{
	CubeMapObj() = default;
	CubeMapObj(CubeMapObj&&) = default;
	CubeMapObj(const CubeMapObj&) = default;
	CubeMapObj& operator=(const CubeMapObj&) = default;
	CubeMapObj& operator=(CubeMapObj&&) = default;

	CubeMapObj(const char* posx, const char* negx,
			   const char* posy, const char* negy,
			   const char* posz, const char* negz) : paths{posx, negx, posy, negy, posz, negz} {}

	CubeMapObj(faces_paths&& fp) : paths{std::move(fp)} {}

	// others methods are taken from the TextureObj class 
	virtual void init(RenderContext&, const GameState&) override;

protected:
	faces_paths paths;
};