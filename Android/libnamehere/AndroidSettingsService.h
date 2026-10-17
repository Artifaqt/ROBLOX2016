//
//  Copyright (c) 2012 NAMEHERE. All rights reserved.
//

#ifndef __NAMEHEREMobile__AndroidSettingsService__
#define __NAMEHEREMobile__AndroidSettingsService__

#include "v8datamodel/FastLogSettings.h"
#include "util/Statistics.h"

#include <iostream>



class AndroidSettingsService : public RBX::FastLogJSON
{
    
public:
    START_DATA_MAP(AndroidSettingsService)
//    DECLARE_DATA_INT(iPadMinimumVersion)
    END_DATA_MAP();
};

#endif /* __NAMEHEREMobile__AndroidSettingsService__ */
