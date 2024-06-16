using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Text.Json;
using System.Windows.Media.Animation;

namespace ClientGUI
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            this.DataContext = this;

            try
            {
                //Communicator.Connect();
            }
            catch (Exception e)
            {
                MessageBox.Show("Connection failed: " + e.Message + "\nPlease try again later.");
                System.Windows.Application.Current.Shutdown();
            }
        }

        private void MainWindow_Loaded(object sender, RoutedEventArgs? e)
        {
            //this.MainFrame.NavigationService.Navigate(new HighscorePage(
            //[
            //    new HighscorePage.Highscore() { Username = "first", Score = 539 },
            //    new HighscorePage.Highscore() { Username = "SecONd", Score = 4 },
            //    new HighscorePage.Highscore() { Username = "tHIrD", Score = -2 }
            //]));

            this.MainFrame.NavigationService.Navigate(new LoginPage());
        }
    }
}
