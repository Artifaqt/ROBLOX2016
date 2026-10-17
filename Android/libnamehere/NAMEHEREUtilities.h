#ifndef NAMEHEREUTILITIES_H
#define NAMEHEREUTILITIES_H

#include "rbx/rbxTime.h"

class NAMEHEREUtilities
{

public:

    static NAMEHEREUtilities& getNAMEHEREUtilities()
    {

        static NAMEHEREUtilities namehereUtilities;
        return namehereUtilities;
    }

    RBX::Timer<RBX::Time::Fast>& getGameTimer() { return gameTimer; };


private:
    NAMEHEREUtilities() {};
    NAMEHEREUtilities(NAMEHEREUtilities const&);
    void operator=(NAMEHEREUtilities const&);

    RBX::Timer<RBX::Time::Fast> gameTimer;
};



#endif // NAMEHEREUTILITIES_H
