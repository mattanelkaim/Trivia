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

        public Highscore[] Highscores { get; } = new Highscore[3];


        public HighscorePage(Highscore[] sortedHighscores)
        {
            if (sortedHighscores.Length != this.Highscores.Length)
            {
                throw new ArgumentException("Invalid array length. Expected an array with " + this.Highscores.Length + " elements.");
            }
            DataContext = this;
            this.Highscores = sortedHighscores;
            InitializeComponent();
        }

        private void Unblur_Text(object sender, RoutedEventArgs? e)
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
