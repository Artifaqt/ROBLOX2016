/* Copyright 2003-2007 NAMEHERE Corporation, All Rights Reserved */

#pragma once

#include "BevelMesh.h"

namespace RBX
{
	extern const char* const sBlockMesh;
	class BlockMesh 
		: public DescribedCreatable<BlockMesh, BevelMesh, sBlockMesh>
	{
	public:
		BlockMesh(){}
	};
}