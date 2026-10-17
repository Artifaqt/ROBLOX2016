/* Copyright 2003-2007 NAMEHERE Corporation, All Rights Reserved */

#pragma once

#include "BevelMesh.h"

namespace RBX
{
	extern const char* const sCylinderMesh;
	class CylinderMesh 
		: public DescribedCreatable<CylinderMesh, BevelMesh, sCylinderMesh>
	{
	public:
		CylinderMesh(){}
	};
}