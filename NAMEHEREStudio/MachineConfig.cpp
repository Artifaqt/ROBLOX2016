/**
 * MachineConfig.cpp
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "MachineConfig.h"

// NAMEHERE Headers
#include "MachineConfiguration.h"

// NAMEHERE Studio Headers
#include "NAMEHERESettings.h"

MachineConfig* MachineConfig::machineConfig = NULL;
////////////////////////////////////////////////////////////////////////////////////

void MachineConfig::PostMachineConfiguration(bool authSuccess)
{
	if(authSuccess)
	{
		disconnect(&AuthenticationHelper::Instance(), SIGNAL(authenticationDone(bool)), machineConfig, SLOT(PostMachineConfiguration(bool)));

        NAMEHERESettings settings;
        int lastGfxMode = settings.contains("lastGFXMode") ? settings.value("lastGFXMode").toInt() : 0;

		RBX::postMachineConfiguration(NAMEHERESettings::getBaseURL().toLocal8Bit().constData(), lastGfxMode);
	}
}
/////////////////////////////////////////////////////////////////////////////////
