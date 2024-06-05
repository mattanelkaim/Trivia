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
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ClientGUI
{
    /// <summary>
    /// Interaction logic for ScoreboardPage.xaml
    /// </summary>
    public partial class ScoreboardPage : Page
    {
        public struct Highscore
        {
            public string Username { get; set; }
            public int Score { get; set; }
        }

        public Highscore[] TopPlayers { get; } = new Highscore[5];

        public ScoreboardPage()
        {
            DataContext = this;

            this.TopPlayers = FetchTopPlayersFromDB();

            InitializeComponent();
        }

        private void HomeButton_Click(object sender, RoutedEventArgs? e)
        {
            this.NavigationService.Navigate(new MenuPage());
        }

        private static Highscore[] FetchTopPlayersFromDB()
        {
            Dictionary<string, string> highscores = Helper.SendScoreboardRequest();
            Highscore[] Highscores = new Highscore[5];

            int i = 0;
            foreach (string username in highscores.Values)
            {
                Highscores[i] = new Highscore() { Username = username, Score = -1 }; // TODO score isn't supported yet
                if (++i >= 5)
                    break;
            }

            return Highscores;
        }
    }
}
