using System;
using System.Collections.Generic;
using System.ServiceModel;
using System.Text;
using System.Diagnostics;

namespace NAMEHERE.RccServiceArbiter
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.Single, ConcurrencyMode = ConcurrencyMode.Multiple)]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.ServiceModel", "3.0.0.0")]
    [System.ServiceModel.ServiceContractAttribute(Namespace = "http://namehere.com/", ConfigurationName = "RCCServiceSoap")]
    [System.ServiceModel.XmlSerializerFormat(Style=OperationFormatStyle.Document, Use=OperationFormatUse.Literal)]
    class RccService : NAMEHERE.Grid.Rcc.Server.RCCServiceSoap
    {
        private JobManager jobManager;

        public RccService()
        {
            jobManager = new JobManager();
        }
        public void initializeJobManager()
        {
            jobManager.initialize();
        }
        #region Converters


        private static NAMEHERE.Grid.Rcc.Job convert(NAMEHERE.Grid.Rcc.Server.Job job)
        {
            NAMEHERE.Grid.Rcc.Job result = new NAMEHERE.Grid.Rcc.Job();
            result.id = job.id;
            result.category = job.category;
            result.cores = job.cores;
            result.expirationInSeconds = job.expirationInSeconds;
            return result;
        }
        private static NAMEHERE.Grid.Rcc.Server.Job convert(NAMEHERE.Grid.Rcc.Job job)
        {
            NAMEHERE.Grid.Rcc.Server.Job result = new NAMEHERE.Grid.Rcc.Server.Job();
            result.id = job.id;
            result.category = job.category;
            result.cores = job.cores;
            result.expirationInSeconds = job.expirationInSeconds;
            return result;
        }
        private static NAMEHERE.Grid.Rcc.LuaType convert(NAMEHERE.Grid.Rcc.Server.LuaType luaType)
        {
            switch (luaType)
            {
                case NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TBOOLEAN: return NAMEHERE.Grid.Rcc.LuaType.LUA_TBOOLEAN;
                case NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TNIL: return NAMEHERE.Grid.Rcc.LuaType.LUA_TNIL;
                case NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TNUMBER: return NAMEHERE.Grid.Rcc.LuaType.LUA_TNUMBER;
                case NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TSTRING: return NAMEHERE.Grid.Rcc.LuaType.LUA_TSTRING;
                case NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TTABLE: return NAMEHERE.Grid.Rcc.LuaType.LUA_TTABLE;
            }
            throw new Exception("Unknown LuaType");
        }
        private static NAMEHERE.Grid.Rcc.Server.LuaType convert(NAMEHERE.Grid.Rcc.LuaType luaType)
        {
            switch (luaType)
            {
                case NAMEHERE.Grid.Rcc.LuaType.LUA_TBOOLEAN: return NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TBOOLEAN;
                case NAMEHERE.Grid.Rcc.LuaType.LUA_TNIL: return NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TNIL;
                case NAMEHERE.Grid.Rcc.LuaType.LUA_TNUMBER: return NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TNUMBER;
                case NAMEHERE.Grid.Rcc.LuaType.LUA_TSTRING: return NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TSTRING;
                case NAMEHERE.Grid.Rcc.LuaType.LUA_TTABLE: return NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TTABLE;
            }
            throw new Exception("Unknown LuaType");
        }
        private static NAMEHERE.Grid.Rcc.LuaValue[] convert(NAMEHERE.Grid.Rcc.Server.LuaValue[] luaValues)
        {
            if (luaValues == null) return null;

            NAMEHERE.Grid.Rcc.LuaValue[] result = new NAMEHERE.Grid.Rcc.LuaValue[luaValues.Length];
            int i = 0;
            foreach (NAMEHERE.Grid.Rcc.Server.LuaValue luaValue in luaValues)
            {
                result[i++] = convert(luaValue);
            }
            return result;
        }
        private static NAMEHERE.Grid.Rcc.Server.LuaValue[] convert(NAMEHERE.Grid.Rcc.LuaValue[] luaValues)
        {
            if (luaValues == null) return null;

            NAMEHERE.Grid.Rcc.Server.LuaValue[] result = new NAMEHERE.Grid.Rcc.Server.LuaValue[luaValues.Length];
            int i = 0;
            foreach (NAMEHERE.Grid.Rcc.LuaValue luaValue in luaValues)
            {
                result[i++] = convert(luaValue);
            }
            return result;
        }


        private static NAMEHERE.Grid.Rcc.LuaValue convert(NAMEHERE.Grid.Rcc.Server.LuaValue luaValue)
        {
            NAMEHERE.Grid.Rcc.LuaValue result = new NAMEHERE.Grid.Rcc.LuaValue();
            result.type = convert(luaValue.type);
            result.value = luaValue.value;
            result.table = convert(luaValue.table);
            return result;
        }

        private static NAMEHERE.Grid.Rcc.Server.LuaValue convert(NAMEHERE.Grid.Rcc.LuaValue luaValue)
        {
            NAMEHERE.Grid.Rcc.Server.LuaValue result = new NAMEHERE.Grid.Rcc.Server.LuaValue();
            result.type = convert(luaValue.type);
            result.value = luaValue.value;
            result.table = convert(luaValue.table);
            return result;
        }


        private static NAMEHERE.Grid.Rcc.ScriptExecution convert(NAMEHERE.Grid.Rcc.Server.ScriptExecution script)
        {
            NAMEHERE.Grid.Rcc.ScriptExecution result = new NAMEHERE.Grid.Rcc.ScriptExecution();
            result.name = script.name;
            result.script = script.script;
            result.arguments = convert(script.arguments);
            return result;
        }

        #endregion 

        private string ToString(NAMEHERE.Grid.Rcc.Server.LuaValue[] values)
        {
            string result = "";
            foreach (NAMEHERE.Grid.Rcc.Server.LuaValue value in values)
            {
                switch (value.type)
                {
                    case NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TBOOLEAN:
                    case NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TNUMBER:
                    case NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TSTRING:
                        result += value.value + "\n";
                        break;
                    case NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TNIL:
                        result += "[NIL]";
                        break;
                    case NAMEHERE.Grid.Rcc.Server.LuaType.LUA_TTABLE:
                        result += "{\n" + ToString(value.table) + "}\n";
                        break;
                }
            }
            return result;
        }
        public string GetStats(bool clearExceptions)
        {
            return jobManager.GetStats(clearExceptions);
        }

        public void SetMultiProcess(bool value)
        {
            jobManager.SetMultiProcess(value);
        }

        public void SetRecycleProcess(bool value)
        {
            jobManager.SetRecycleProcess(value);
        }
        public void SetRecycleProcessCount(int value)
        {
            jobManager.SetRecycleProcessCount(value);
        }



        public void SetThreadConfigScript(string script, bool broadcast)
        {
            jobManager.SetStartScript(script, broadcast);
        }

        #region RCCServiceSoap Members

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/HelloWorld", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override string HelloWorld()
        {
            if (Properties.Settings.Default.Verbose) Console.WriteLine("HelloWorld()");

            try
            {
                string result;
                using (NAMEHERE.Grid.Rcc.RCCServiceSoap rccService = jobManager.AnyJob(30))
                {
                    result = rccService.HelloWorld();
                }
                Log.Event("HelloWorld - Success");
                return result;
            }
            catch (Exception e)
            {
                Log.Event(String.Format("HelloWorld - Exception - {0}", e.Message));
                throw;
            }
        }

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/GetVersion", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override string GetVersion()
        {
            //if (Properties.Settings.Default.Verbose) Console.WriteLine("GetVersion()");
            using (NAMEHERE.Grid.Rcc.RCCServiceSoap rccService = jobManager.AnyJob(30))
            {
                return rccService.GetVersion();
            }
        }

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/OpenJob", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormat(Style = OperationFormatStyle.Document, Use = OperationFormatUse.Literal)]
        public override NAMEHERE.Grid.Rcc.Server.LuaValue[] OpenJob(NAMEHERE.Grid.Rcc.Server.Job job, NAMEHERE.Grid.Rcc.Server.ScriptExecution script)
        {
            System.Threading.ThreadPool.QueueUserWorkItem((dummy) =>
            {
                try
                {
                    OpenJobEx(job, script); 
                }
                catch (Exception ex)
                {
                    ExceptionHandler.LogException(ex);
                }
            });
            return null; 
        }

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/OpenJobEx", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormat(Style = OperationFormatStyle.Document, Use = OperationFormatUse.Literal)]
        public override NAMEHERE.Grid.Rcc.Server.LuaValue[] OpenJobEx(NAMEHERE.Grid.Rcc.Server.Job job, NAMEHERE.Grid.Rcc.Server.ScriptExecution script)
        {
            try
            {
                if (Properties.Settings.Default.Verbose) Console.WriteLine("OpenJob(" + job.ToString() + ")");

                Console.WriteLine("OpenJobEx, script name: " + script.name);
                NAMEHERE.Grid.Rcc.Server.LuaValue[] finalResult;
                using (NAMEHERE.Grid.Rcc.RCCServiceSoap rccService = jobManager.NewJob(job.id, job.expirationInSeconds, "OpenJobEx"))
                {
                    //Create a new process, and have it execute this Job
                    NAMEHERE.Grid.Rcc.LuaValue[] result = rccService.OpenJob(convert(job), convert(script));
                    finalResult = convert(result);
                }

                Log.Event(String.Format("OpenJobEx('{0}', {1}, {2}) - Success", job.id, job.category, job.expirationInSeconds));
                return finalResult;
            }
            catch (Exception e)
            {
                Log.Event(String.Format("OpenJobEx('{0}', {1}, {2}) - Exception - {3}", job.id, job.category, job.expirationInSeconds, e.Message));
                throw;
            }
        }

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/BatchJob", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override NAMEHERE.Grid.Rcc.Server.LuaValue[] BatchJob(NAMEHERE.Grid.Rcc.Server.Job job, NAMEHERE.Grid.Rcc.Server.ScriptExecution script)
        {
            throw new Exception("BatchJob is deprecated, use BatchJobEx");
        }

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/BatchJobEx", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override NAMEHERE.Grid.Rcc.Server.LuaValue[] BatchJobEx(NAMEHERE.Grid.Rcc.Server.Job job, NAMEHERE.Grid.Rcc.Server.ScriptExecution script)
        {
            try
            {
                if (Properties.Settings.Default.Verbose) Console.WriteLine("BatchJob(" + job.ToString() + ")");
                string jobId = "Batch" + job.id;
                NAMEHERE.Grid.Rcc.Server.LuaValue[] result;
                using (NAMEHERE.Grid.Rcc.RCCServiceSoap rccService = jobManager.NewJob(jobId, job.expirationInSeconds, "BatchJob"))
                {
                    try
                    {
                        result = convert(rccService.BatchJob(convert(job), convert(script)));
                    }
                    finally
                    {
                        jobManager.CloseJob(jobId);
                    }
                }
                Log.Event(String.Format("BatchJobEx('{0}', {1}, {2}) - Success", job.id, job.category, job.expirationInSeconds));
                return result;

            }
            catch (Exception e)
            {
                Log.Event(String.Format("BatchJobEx('{0}', {1}, {2}) - Exception - {3}", job.id, job.category, job.expirationInSeconds, e.Message));
                throw;
            }
        }

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/RenewLease", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override double RenewLease(string jobID, double expirationInSeconds)
        {
            System.Threading.ThreadPool.QueueUserWorkItem((dummy) =>
            {
                try
                {
                    RenewLeaseSync(jobID, expirationInSeconds);                    
                }
                catch (Exception ex)
                {
                    ExceptionHandler.LogException(ex);                        
                }
            });
            return 0;
        }
        private void RenewLeaseSync(string jobID, double expirationInSeconds)
        {
            try
            {
                if (Properties.Settings.Default.Verbose) Console.WriteLine("RenewLease(" + jobID + "," + expirationInSeconds + ")");
                jobManager.RenewLease(jobID, expirationInSeconds);
                double result;
                using (NAMEHERE.Grid.Rcc.RCCServiceSoap rccService = jobManager.GetJob(jobID, "RenewLease", ""))
                {
                    result = rccService.RenewLease(jobID, expirationInSeconds);
                }
                Log.Event(String.Format("RenewLease('{0}', {1}) - Success", jobID, expirationInSeconds));                
            }
            catch (Exception e)
            {
                Log.Event(String.Format("RenewLease('{0}', {1}) - Exception - {2}", jobID, expirationInSeconds, e.Message));
                throw;
            }
        }

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/Execute", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override NAMEHERE.Grid.Rcc.Server.LuaValue[] Execute(string jobID, NAMEHERE.Grid.Rcc.Server.ScriptExecution script)
        {
            throw new Exception("Execute is deprecated, use ExecuteEx");
        }
       
        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/ExecuteEx", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override NAMEHERE.Grid.Rcc.Server.LuaValue[] ExecuteEx(string jobID, NAMEHERE.Grid.Rcc.Server.ScriptExecution script)
        {
            try
            {
                if (Properties.Settings.Default.Verbose) Console.WriteLine("Execute(" + jobID + ")");

                NAMEHERE.Grid.Rcc.Server.LuaValue[] finalResult;
                using (NAMEHERE.Grid.Rcc.RCCServiceSoap rccService = jobManager.GetJob(jobID, "ExecuteEx", script.script.Substring(0, Math.Min(30, script.script.Length))))
                {
                    NAMEHERE.Grid.Rcc.LuaValue[] result = rccService.Execute(jobID, convert(script));
                    finalResult = convert(result);
                }
                Log.Event(String.Format("ExecuteEx('{0}', {1}) - Success", jobID, script.name));
                return finalResult;
            }
            catch (Exception e)
            {
                Log.Event(String.Format("ExecuteEx('{0}', {1}) - Exception - {2}", jobID, script.name, e.Message));
                throw;
            }
        }

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/CloseJob", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override void CloseJob(string jobID)
        {
            System.Threading.ThreadPool.QueueUserWorkItem((dummy) =>
            {
                try
                {
                    CloseJobSync(jobID);
                }
                catch (Exception ex)
                {
                    ExceptionHandler.LogException(ex);
                }
            });            
        }
        
        public void CloseJobSync(string jobID)
        {
            try
            {
                if (Properties.Settings.Default.Verbose) Console.WriteLine("CloseJob(" + jobID + ")");
                using (NAMEHERE.Grid.Rcc.RCCServiceSoap rccService = jobManager.GetJob(jobID, "CloseJob", ""))
                {
                    rccService.CloseJob(jobID);
                }
            }
            catch (Exception e)
            {
                Log.Event(String.Format("CloseJob('{0}') - Exception - {1}", jobID, e.Message));
                throw;
            }
            finally
            {
                jobManager.CloseJob(jobID);
            }
            Log.Event(String.Format("CloseJob('{0}') - Success", jobID));
        }


        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/GetExpiration", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override double GetExpiration(string jobID)
        {
            if (Properties.Settings.Default.Verbose) Console.WriteLine("GetExpiration(" + jobID + ")");
            using (NAMEHERE.Grid.Rcc.RCCServiceSoap rccService = jobManager.GetJob(jobID, "GetExpiration", ""))
            {
                return rccService.GetExpiration(jobID);
            }
        }

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/Diag", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override NAMEHERE.Grid.Rcc.Server.LuaValue[] Diag(int type, string jobID)
        {
            throw new Exception("Diag is deprecated, use DiagEx");
        }

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/DiagEx", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override NAMEHERE.Grid.Rcc.Server.LuaValue[] DiagEx(int type, string jobID)
        {

            if (Properties.Settings.Default.Verbose) Console.WriteLine("Diag(" +    type + "," + jobID + ")");
            if (jobID != "")
            {
                using (NAMEHERE.Grid.Rcc.RCCServiceSoap rccService = jobManager.GetJob(jobID, "DiagEx", ""))
                {
                    return convert(rccService.Diag(type, jobID));
                }
            }
            else
            {
                using (NAMEHERE.Grid.Rcc.RCCServiceSoap rccService = jobManager.AnyJob(60))
                {
                    return convert(rccService.Diag(type, jobID));
                }
            }
        }

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/GetStatus", ReplyAction = "http://namehere.com/GetStatusResponse")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override NAMEHERE.Grid.Rcc.Server.Status GetStatus()
        {
            //if (Properties.Settings.Default.Verbose) Console.WriteLine("GetStatus()");
            NAMEHERE.Grid.Rcc.Server.Status result = new NAMEHERE.Grid.Rcc.Server.Status();
            result.environmentCount = 0;

            //Pick version from 1
            result.version = GetVersion();

            jobManager.DispatchRequest(delegate(NAMEHERE.Grid.Rcc.RCCServiceSoap rccService)
                {
                    //Call GetStatus on all dependent processes
                    NAMEHERE.Grid.Rcc.Status status = rccService.GetStatus();

                    //Ensure all have same version
                    Debug.Assert(status.version != result.version);

                    //Sum environmentCount from each process
                    result.environmentCount += status.environmentCount;
                });
            return result;
        }

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/GetAllJobs", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override NAMEHERE.Grid.Rcc.Server.Job[] GetAllJobs()
        {
            throw new Exception("GetAllJobs is deprecated, use GetAllJobsEx");
        }


        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/GetAllJobsEx", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override NAMEHERE.Grid.Rcc.Server.Job[] GetAllJobsEx()
        {
            if (Properties.Settings.Default.Verbose) Console.WriteLine("GetAllJobs()");
            System.Collections.Generic.List<NAMEHERE.Grid.Rcc.Server.Job> result = new List<NAMEHERE.Grid.Rcc.Server.Job>();

            jobManager.DispatchRequest(delegate(NAMEHERE.Grid.Rcc.RCCServiceSoap rccService)
            {
                NAMEHERE.Grid.Rcc.Job[] jobs = rccService.GetAllJobs();
                foreach (NAMEHERE.Grid.Rcc.Job job in jobs)
                    result.Add(convert(job));
            });
            return result.ToArray();
    }

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/CloseExpiredJobs", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override int CloseExpiredJobs()
        {
            if (Properties.Settings.Default.Verbose) Console.WriteLine("CloseExpiredJobs()");
            int result = 0;
            //Forward to all processes
            jobManager.DispatchRequest(delegate(NAMEHERE.Grid.Rcc.RCCServiceSoap rccService)
                {
                    result += rccService.CloseExpiredJobs();
                });

            return result;
        }

        [System.ServiceModel.OperationContractAttribute(Action = "http://namehere.com/CloseAllJobs", ReplyAction = "*")]
        [System.ServiceModel.XmlSerializerFormatAttribute()]
        public override int CloseAllJobs()
        {
            if (Properties.Settings.Default.Verbose) Console.WriteLine("CloseAllJobs()");
            int result = 0;
            //Forward to all processes
            jobManager.DispatchRequest(delegate(NAMEHERE.Grid.Rcc.RCCServiceSoap rccService)
                {
                    result += rccService.CloseAllJobs();
                });

            return result;
        }        
        #endregion
    }
}
