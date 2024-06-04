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
        public float Score { get; set; }
        public uint Games { get; set; }
        public uint TotalAnswers { get; set; }
        public uint CorrectAnswers { get; set; }
        public uint WrongAnswers => TotalAnswers - CorrectAnswers;
        public float AccuracyStat { get; set; } // Accuracy is a reserved keyword
        public string AccuracyStr => AccuracyStat.ToString("P2"); // To percentage with max 2 decimal places

        public PersonalStatsPage()
        {
            DataContext = this;
            this.Score = 0;
            this.Games = 0;
            this.TotalAnswers = 11;
            this.CorrectAnswers = 7;

            if (TotalAnswers != 0)
                this.AccuracyStat = (float)CorrectAnswers / TotalAnswers;
            else
                this.AccuracyStat = 0;

            InitializeComponent();
        }
        private void HomeButton_Click(object sender, RoutedEventArgs? e)
        {
            this.NavigationService.Navigate(new MenuPage());
        }
    }
}
