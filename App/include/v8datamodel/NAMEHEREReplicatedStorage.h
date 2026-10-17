#pragma once

#include "V8Tree/Service.h"

namespace RBX {

	extern const char* const sNAMEHEREReplicatedStorage;

	class NAMEHEREReplicatedStorage
		: public DescribedCreatable<NAMEHEREReplicatedStorage, Instance, sNAMEHEREReplicatedStorage, Reflection::ClassDescriptor::INTERNAL, Security::NAMEHEREScript>
		, public Service
	{
	public:
		NAMEHEREReplicatedStorage();
	};

}
