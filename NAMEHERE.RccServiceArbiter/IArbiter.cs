using System;
using System.Collections.Generic;
using System.Text;
using System.ServiceModel;

namespace NAMEHERE.RccServiceArbiter
{
    [ServiceContract(Namespace = "http://namehere.com/", Name = "Arbiter", ConfigurationName = "IArbiter")]
    interface IArbiter
    {
        [OperationContract]
        string GetStatsEx(bool clearExceptions);

        [OperationContract]
        string GetStats();

        [OperationContract]
        void SetMultiProcess(bool value);

        [OperationContract]
        void SetRecycleProcess(bool value);

        [OperationContract]
        void SetThreadConfigScript(string script, bool broadcast);

        [OperationContract]
        void SetRecycleQueueSize(int value);
    };
}
