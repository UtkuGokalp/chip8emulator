using System.Diagnostics;

namespace Chip8EmulatorLauncher
{
    public partial class Form1 : Form
    {
        private const string EXECUTABLE_PATH = "..\\..\\..\\..\\x64\\Debug\\chip8emulator.exe";
        private Dictionary<string, string> demosNameToPath = new Dictionary<string, string>();
        private Dictionary<string, string> gamesNameToPath = new Dictionary<string, string>();
        private Dictionary<string, string> hiresNameToPath = new Dictionary<string, string>();
        private Dictionary<string, string> programsNameToPath = new Dictionary<string, string>();
        private volatile Process? currentProcessInstance = null;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            const string ROOT_PATH = "..\\..\\..\\..\\chip8emulator\\roms";
            string[] folderNames = { "demos", "games", "hires", "programs" };
            ComboBox[] cbxs = { cbxDemos, cbxGames, cbxHires, cbxPrograms };
            Dictionary<string, string>[] dictionaries = { demosNameToPath, gamesNameToPath, hiresNameToPath, programsNameToPath };
            for (int i = 0; i < cbxs.Length; i++)
            {
                cbxs[i].Items.Add("NONE");
                cbxs[i].SelectedIndex = 0;
                string directoryPath = Path.Combine(ROOT_PATH, folderNames[i]);
                if (Directory.Exists(directoryPath))
                {
                    string[] filePaths = Directory.GetFiles(directoryPath, "*.ch8", SearchOption.AllDirectories);
                    foreach (string filePath in filePaths)
                    {
                        string fileName = Path.GetFileName(filePath);
                        cbxs[i].Items.Add(fileName);
                        dictionaries[i].Add(fileName, filePath);
                    }
                }
            }
        }

        private void btnLaunch_Click(object sender, EventArgs e)
        {
            string filePath = string.Empty;

            if (cbxDemos.Text != "NONE")
            {
                filePath = demosNameToPath[cbxDemos.Text];
            }
            else if (cbxGames.Text != "NONE")
            {
                filePath = gamesNameToPath[cbxGames.Text];
            }
            else if (cbxHires.Text != "NONE")
            {
                filePath = hiresNameToPath[cbxHires.Text];
            }
            else if (cbxPrograms.Text != "NONE")
            {
                filePath = programsNameToPath[cbxPrograms.Text];
            }

            if (filePath == string.Empty)
            {
                MessageBox.Show("Invalid file path! " + filePath);
                return;
            }

            ProcessStartInfo startInfo = new ProcessStartInfo()
            {
                FileName = EXECUTABLE_PATH,
                Arguments = filePath,
                UseShellExecute = false,
                CreateNoWindow = true,
            };
            Process process = new Process
            {
                StartInfo = startInfo,
                EnableRaisingEvents = true,
            };
            bool started = process.Start();
            if (started)
            {
                currentProcessInstance = process;
            }
        }

        private void cbxDemos_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cbxGames.Items.Count > 0)
            {
                cbxGames.SelectedIndex = 0;
            }
            if (cbxHires.Items.Count > 0)
            {
                cbxHires.SelectedIndex = 0;
            }
            if (cbxPrograms.Items.Count > 0)
            {
                cbxPrograms.SelectedIndex = 0;
            }
        }

        private void cbxGames_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cbxDemos.Items.Count > 0)
            {
                cbxDemos.SelectedIndex = 0;
            }
            if (cbxHires.Items.Count > 0)
            {
                cbxHires.SelectedIndex = 0;
            }
            if (cbxPrograms.Items.Count > 0)
            {
                cbxPrograms.SelectedIndex = 0;
            }
        }

        private void cbxHires_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cbxDemos.Items.Count > 0)
            {
                cbxDemos.SelectedIndex = 0;
            }
            if (cbxGames.Items.Count > 0)
            {
                cbxGames.SelectedIndex = 0;
            }
            if (cbxPrograms.Items.Count > 0)
            {
                cbxPrograms.SelectedIndex = 0;
            }
        }

        private void cbxPrograms_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cbxDemos.Items.Count > 0)
            {
                cbxDemos.SelectedIndex = 0;
            }
            if (cbxGames.Items.Count > 0)
            {
                cbxGames.SelectedIndex = 0;
            }
            if (cbxHires.Items.Count > 0)
            {
                cbxHires.SelectedIndex = 0;
            }
        }

        private void btnKillInstance_Click(object sender, EventArgs e)
        {
            if (currentProcessInstance != null)
            {
                //If the process didn't already exit through normal means
                if (!currentProcessInstance.HasExited)
                {
                    try
                    {
                        currentProcessInstance.Kill(true);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Error killing process: " + ex.Message);
                    }
                }
                currentProcessInstance.Close();
                currentProcessInstance.Dispose();
                currentProcessInstance = null;
            }
        }

        private void btnOpenWorkingDirectory_Click(object sender, EventArgs e)
        {
            string? path = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            if (path == null)
            {
                MessageBox.Show("Working directory couldn't be found.");
                return;
            }
            Process.Start("explorer.exe", path);
        }
    }
}
