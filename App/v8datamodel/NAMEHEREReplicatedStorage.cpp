#include "stdafx.h"

#include "v8datamodel/NAMEHEREReplicatedStorage.h"

using namespace RBX;

const char* const RBX::sNAMEHEREReplicatedStorage = "NAMEHEREReplicatedStorage";

NAMEHEREReplicatedStorage::NAMEHEREReplicatedStorage(void)
{
	setName(sNAMEHEREReplicatedStorage);
	setNAMEHERELocked(true);
}
