using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ClientGUI
{
    /// <summary>
    /// Interaction logic for HighscorePage.xaml
    /// </summary>
    public partial class HighscorePage : Page
    {
        public struct Highscore
        {
            public string Username { get; set; }
            public int Score { get; set; }
        }

        public Highscore[] highscores { get; } = new Highscore[3];

        //this.MainFrame.NavigationService.Navigate(new HighscorePage(
        //[
        //    new HighscorePage.Highscore() { Username = "first", Score = 539 },
        //    new HighscorePage.Highscore() { Username = "SecONd", Score = 4 },
        //    new HighscorePage.Highscore() { Username = "tHIrD", Score = -2 }
        //]));
        public HighscorePage(Highscore[] sortedHighscores)
        {
            if (sortedHighscores.Length != this.highscores.Length)
            {
                throw new ArgumentException("Invalid array length. Expected an array with " + this.highscores.Length + " elements.");
            }
            DataContext = this;
            this.highscores = sortedHighscores;
            InitializeComponent();
        }

        private void Unblur_Text(object sender, RoutedEventArgs e)
        {
            DoubleAnimation unblurAnimation = new ()
            {
                To = 0,
                Duration = TimeSpan.FromMilliseconds(1000),
                EasingFunction = new QuarticEase()
            };
            ((TextBlock)sender).Effect.BeginAnimation(BlurEffect.RadiusProperty, unblurAnimation);
        }
    }
}
