using System;
using System.Collections.Generic;
using System.ComponentModel;

namespace NAMEHERE.RccServiceArbiter
{
    [RunInstaller(true)]
    public class ArbiterInstaller : NAMEHERE.ServiceProcess.ServiceHostInstaller
    {
        public override string ServiceName
        {
            get { return "NAMEHERE.RccServiceArbiter"; }
        }

        public override string DisplayName
        {
            get { return "NAMEHERE RccService Arbiter"; }
        }

        public override string Description
        {
            get { return "Manages a handful of processes running the RCC web service."; }
        }
    }
}
