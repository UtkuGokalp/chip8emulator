using Avalonia.Controls;
using System.ComponentModel;
using Avalonia.Interactivity;
using System.Collections.ObjectModel;

namespace Chip8EmulatorLauncher
{
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
#if WINDOWS
        private const string EXECUTABLE_PATH = "..\\..\\..\\..\\x64\\Debug\\chip8emulator.exe";
#elif LINUX
        private const string EXECUTABLE_PATH = "..\\..\\..\\..\\x64\\Debug\\chip8emulator.exe";
#endif

        public string? SelectedItem { get; set; }
        public ObservableCollection<string> Demos { get; } = new ObservableCollection<string>();
        public ObservableCollection<string> Games { get; } = new ObservableCollection<string>();
        public ObservableCollection<string> Hires { get; } = new ObservableCollection<string>();
        public ObservableCollection<string> Programs { get; } = new ObservableCollection<string>();


        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;
            cbxDemos.SelectionChanged += cbxSelectionChanged;
            cbxGames.SelectionChanged += cbxSelectionChanged;
            cbxHires.SelectionChanged += cbxSelectionChanged;
            cbxPrograms.SelectionChanged += cbxSelectionChanged;
        }

        private void btnLaunchEmulator_Click(object? sender, RoutedEventArgs e)
        {
            
        }

        private void btnKillInstance_Click(object? sender, RoutedEventArgs e)
        {
        }

        private void btnOpenWorkingDir_Click(object? sender, RoutedEventArgs e)
        {
            
        }

        private void cbxSelectionChanged(object? sender, SelectionChangedEventArgs e)
        {

        }
    }
}