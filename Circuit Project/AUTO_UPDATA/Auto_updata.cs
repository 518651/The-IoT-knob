using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AutoUpdaterDotNET;
using System.Windows.Threading;
using System.Threading;
using System.Globalization;

namespace Wallet_Payment.AUTO_UPDATA
{
    public class Auto_updata
    {
        public static void Applist_Updata()
        {
            Thread.CurrentThread.CurrentCulture =
                   Thread.CurrentThread.CurrentUICulture = CultureInfo.CreateSpecificCulture("zh");
            AutoUpdater.ReportErrors = true;
            AutoUpdater.Start("http://d1w53g8s1ef5.tk/IoT/Applist_config.xml");
            AutoUpdater.RunUpdateAsAdmin = false;
        }
    }
}
