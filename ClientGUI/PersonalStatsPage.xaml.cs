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
    /// Interaction logic for PersonalStatsPage.xaml
    /// </summary>
    public partial class PersonalStatsPage : Page
    {
        public struct PersonalStats
        {
            public float score { get; set; }
            public uint games { get; set; }
            public uint totalAnswers { get; set; }
            public uint correctAnswers { get; set; }
        }

        public PersonalStats DBStats { get; set; }
        // All of these are manually calculated based on DBStats
        public uint WrongAnswers => DBStats.totalAnswers - DBStats.correctAnswers;
        public float AccuracyStat { get; set; } // Accuracy is a reserved keyword

        public PersonalStatsPage()
        {
            DataContext = this;

            this.DBStats = FetchStatsFromDB();

            if (DBStats.totalAnswers != 0)
                this.AccuracyStat = (float)DBStats.correctAnswers / DBStats.totalAnswers;
            else
                this.AccuracyStat = 0;

            InitializeComponent();
            this.AccuracyBox.ToolTip = "Correct answers: " + DBStats.correctAnswers + "\nWrong answers: " + WrongAnswers;
        }

        private void HomeButton_Click(object sender, RoutedEventArgs? e)
        {
            this.NavigationService.Navigate(new MenuPage());
        }

        private static PersonalStats FetchStatsFromDB()
        {
            PersonalStats stats = Helper.SendPersonalStatsRequest().userStatistics;
            return stats;
        }
    }
}
