using System;
using System.Collections.Generic;
using System.Text;

namespace NAMEHERE.RccServiceArbiter
{
    class Program
    {
        static void Main(string[] args)
        {
            Log.Event("Starting");
            //Cleanup old RCCService instances
            System.Diagnostics.Process[] processlist = System.Diagnostics.Process.GetProcesses();
            foreach(System.Diagnostics.Process theprocess in processlist)
            {
                if (theprocess.ProcessName == "RCCService")
                {
                    theprocess.Kill();
                }
            }

            RccService rccService = new RccService();
            RccServiceMonitor rccServiceMonitor = new RccServiceMonitor(rccService);
            NAMEHERE.ServiceProcess.ServiceBasePublic[] otherHosts = new NAMEHERE.ServiceProcess.ServiceBasePublic[1];
            otherHosts[0] = new NAMEHERE.ServiceProcess.ServiceHostApp<RccServiceMonitor>(rccServiceMonitor);
            var app = new NAMEHERE.ServiceProcess.ServiceHostApp<RccService>(rccService, otherHosts);
            app.HostOpened += new EventHandler(delegate(object a,EventArgs b) { rccService.initializeJobManager(); });
            //var app = new NAMEHERE.ServiceProcess.ServiceHostApp<RccServiceMonitor>(rccServiceMonitor);
            app.Process(args, delegate() { NAMEHERE.Grid.Arbiter.Common.ArbiterStats stats = new NAMEHERE.Grid.Arbiter.Common.ArbiterStats(rccService.GetStats(false));
                                           Console.Out.Write(stats.ToString());
            });
        }
    }
}
