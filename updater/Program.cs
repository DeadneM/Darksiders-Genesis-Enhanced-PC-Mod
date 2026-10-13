using System;
using System.IO;
using System.Threading;
using System.Windows.Forms;

namespace DarksidersGenesisUpdater {
    internal static class Program {
        [STAThread]
        private static int Main(string[] args) {
            if (args.Length > 0 && string.Equals(args[0], "--install-on-exit", StringComparison.Ordinal)) {
                return RunQuietInstall(args);
            }
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new UpdaterForm());
            return 0;
        }

        // Called by the in-game ASI updater tab. Never writes game files until
        // the game executable has exited and the exact package is reverified.
        private static int RunQuietInstall(string[] args) {
            if (args.Length != 5) return 2;
            string folder=args[1], channel=args[2], expectedName=args[3], expectedSha=args[4];
            if (channel!="main" && channel!="debug") return 2;
            string result="Updater failed before checking the package.";
            int code=1;
            try {
                if (!Directory.Exists(folder)) throw new DirectoryNotFoundException(folder);
                // Wait independently of the ASI process. A running game cannot
                // have its loaded DLLs replaced safely.
                while(UpdateEngine.Running()) Thread.Sleep(2000);
                Thread.Sleep(1800);
                Package remote=UpdateEngine.Latest(channel);
                if (!string.Equals(remote.FileName,expectedName,StringComparison.Ordinal) ||
                    !string.Equals(remote.GitBlobSha,expectedSha,StringComparison.OrdinalIgnoreCase))
                    throw new InvalidDataException("Requested package changed after confirmation; update cancelled.");
                result=UpdateEngine.Install(remote,folder,delegate(string line){});
                code=0;
            } catch(Exception ex) {
                result="Updater FAILED: "+ex.Message;
            }
            try {
                string state=UpdateEngine.StateFolder(folder);
                Directory.CreateDirectory(state);
                File.WriteAllText(Path.Combine(state,"last-result.txt"),result);
            } catch {}
            return code;
        }
    }
}
