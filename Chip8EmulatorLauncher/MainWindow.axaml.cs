using Avalonia.Controls;
using Avalonia.Interactivity;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using static System.Runtime.InteropServices.JavaScript.JSType;

/*
IMPORTANT NOTICE:
Many codes in this file assumes the 4 element arrays to be structured as the following:
    Index 0: Demos
    Index 1: Games
    Index 2: Hires
    Index 3: Programs
Not following this order might cause weird bugs that might be hard to debug. The arrays that
follow this order include the dictionaries array, the comboboxes array and the folder names array.
*/

namespace Chip8EmulatorLauncher
{
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        //Handle OS dependent const variables
#if WINDOWS
        private const string EXECUTABLE_PATH = "..\\..\\..\\..\\x64\\Debug\\chip8emulator.exe";
        private const string ROMS_PATH = "..\\..\\..\\..\\chip8emulator\\roms";
#elif LINUX
        private const string EXECUTABLE_PATH = "chip8emulator/bin/chip8emulator";
        private const string ROMS_PATH = "chip8emulator/roms";
#endif

        //This boolean is used to distinguish between combobox items being changed via the user
        //vs the program. If the program changes the SelectedIndex (which it needs to do in order
        //to set the old combobox item to NONE), OnSelectionChanged events will be raised over and
        //over again, causing bugs. So if this boolean is false, that means the change must have come
        //from the user. In that case, the code will immediately set this variable to true, indicating
        //that SelectedIndex or SelectedItem will be changed and it will NOT be made by the user, so the
        //other comboboxes shouldn't bother executing the event function (the events will still be raised).
        //Once the required execution is complete, this boolean will be set to false again so that once the
        //user chooses a new ROM, it will be registered correctly.
        private bool codeWillChangeSelectedItem = false;
        private Process? currentProcess = null;
        private Dictionary<string, string>[] romName2romPath =
        {
            new Dictionary<string, string>(),
            new Dictionary<string, string>(),
            new Dictionary<string, string>(),
            new Dictionary<string, string>(),
        };
        public string? SelectedROMPath { get; set; }

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;
            ResetErrorText();

            //Load ROM paths and add ROM names to comboboxes
            ComboBox[] cbxs = GetAllComboBoxes();
            string[] folderNames = { "demos", "games", "hires", "programs" };
            for (int i = 0; i < cbxs.Length; i++)
            {
                cbxs[i].Items.Add("NONE");
                cbxs[i].SelectedIndex = 0;

                string directoryPath = Path.Combine(ROMS_PATH, folderNames[i]);
                if (Directory.Exists(directoryPath))
                {
                    string[] filePaths = Directory.GetFiles(directoryPath, "*.ch8", SearchOption.AllDirectories);
                    foreach (string filePath in filePaths)
                    {
                        string fileName = Path.GetFileName(filePath);
                        romName2romPath[i].Add(fileName, filePath);
                        cbxs[i].Items.Add(fileName);
                    }
                }
            }
            //Register events to comboboxes
            cbxDemos.SelectionChanged += cbxSelectionChanged;
            cbxGames.SelectionChanged += cbxSelectionChanged;
            cbxHires.SelectionChanged += cbxSelectionChanged;
            cbxPrograms.SelectionChanged += cbxSelectionChanged;
        }

        private void btnLaunchEmulator_Click(object? sender, RoutedEventArgs e)
        {
            if (SelectedROMPath == null || SelectedROMPath == string.Empty)
            {
                DisplayError("Invalid ROM path, did you choose a ROM?");
                return;
            }

            ProcessStartInfo psi = new ProcessStartInfo()
            {
                FileName = EXECUTABLE_PATH,
                UseShellExecute = true,
            };
            psi.ArgumentList.Add(SelectedROMPath);
            Process p = new Process()
            {
                StartInfo = psi,
                EnableRaisingEvents = true,
            };
            bool started = p.Start();
            if (started)
            {
                currentProcess = p;
                ResetErrorText();
            }
            else
            {
                DisplayError("Couldn't start process");
            }
        }

        private void btnKillInstance_Click(object? sender, RoutedEventArgs e)
        {
            if (currentProcess != null)
            {
                //If the process didn't already exit through normal means
                if (!currentProcess.HasExited)
                {
                    try
                    {
                        currentProcess.Kill(true);
                        ResetErrorText();
                    }
                    catch (Exception ex)
                    {
                        DisplayError(ex.Message);
                    }
                }
                currentProcess.Close();
                currentProcess.Dispose();
                currentProcess = null;
            }
            else
            {
                DisplayError("No working emulator instance found");
            }
        }

        private void btnOpenWorkingDir_Click(object? sender, RoutedEventArgs e)
        {
            string? path = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            if (path == null)
            {
                DisplayError("Failed to find executing assembly path");
                return;
            }
#if WINDOWS
            Process.Start("explorer.exe", path);
#elif LINUX
            Process.Start("xdg-open", path);
#endif
            ResetErrorText();
        }

        private void cbxSelectionChanged(object? sender, RoutedEventArgs e)
        {
            if (codeWillChangeSelectedItem || sender == null)
            {
                return;
            }
            codeWillChangeSelectedItem = true;
            ComboBox source = (ComboBox)sender;
            ComboBox[] cbxs = GetAllComboBoxes();
            bool romSelected = false;
            for (int i = 0; i < cbxs.Length; i++)
            {
                ComboBox cbx = cbxs[i];
                if (cbx.Name == source.Name) //This is the combobox that we changed the selection in
                {
                    //Update the ROM path
                    bool success = romName2romPath[i].TryGetValue(cbx.SelectedItem.ToString(), out string romPath);
                    if (success)
                    {
                        SelectedROMPath = romPath;
                        romSelected = true;
                    }
                }
                else
                {
                    //Set the selected item to NONE
                    cbx.SelectedIndex = 0;
                }
            }

            if (!romSelected)
            {
                SelectedROMPath = null;
            }
            codeWillChangeSelectedItem = false;
        }

        private ComboBox[] GetAllComboBoxes()
        {
            ComboBox[] cbxs = { cbxDemos, cbxGames, cbxHires, cbxPrograms };
            return cbxs;
        }

        private void DisplayError(string? error)
        {
            txtErrors.Text = error;
        }

        private void ResetErrorText()
        {
            txtErrors.Text = "No errors";
        }
    }
}
