﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ClientGUI
{
    /// <summary>
    /// Interaction logic for MenuPage.xaml
    /// </summary>
    public partial class MenuPage : Page
    {
        public MenuPage()
        {
            InitializeComponent();

            Helper.RemoveButtonHighlighting(CreateRoomButton);
            Helper.RemoveButtonHighlighting(JoinRoomButton);
            Helper.RemoveButtonHighlighting(StatisticsButton);
            Helper.RemoveButtonHighlighting(LeaderboardButton);
            Helper.RemoveButtonHighlighting(LogoutButton);
        }

        /*---------------------------------------
                       NAVIGATION
         *--------------------------------------*/

        private void CreateRoom_Click(object sender, RoutedEventArgs? e)
        {
            this.NavigationService.Navigate(new CreateRoomPage());
        }

        private void JoinRoom_Click(object sender, RoutedEventArgs? e)
        {
            this.NavigationService.Navigate(new JoinRoomPage());
        }

        private void ViewStats_Click(object sender, RoutedEventArgs? e)
        {
            this.NavigationService.Navigate(new PersonalStatsPage());
        }

        private void ViewLeaderboard_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new ScoreboardPage()); // TODO remove comment @MattanElkaim // TODO remove the previous comment @Gil-Yelkin
        }

        /*---------------------------------------
                       OTHER EVENTS
        *--------------------------------------*/

        private void Button_MouseEnter(object sender, MouseEventArgs? e)
        {
            Helper.ButtonGotHoverEffect((Button)sender);
        }

        private void Button_MouseLeave(object sender, MouseEventArgs? e)
        {
            Helper.ButtonLostHoverEffect((Button)sender);
        }

        private void Logout_Click(object sender, RoutedEventArgs e)
        {
            int randomNumber = new Random().Next(1, 5);

            switch (randomNumber)
            {
                case 1:
                    MessageBox.Show("Hahahahaha nice try babe");
                    break;
                case 2:
                    MessageBox.Show("You CANNOT leave :)");
                    break;
                case 3:
                    MessageBoxResult result;
                    do
                    {
                        result = MessageBox.Show("Are you sure you want to logout?", "Not today honey", MessageBoxButton.YesNo, MessageBoxImage.Stop);
                        if (result == MessageBoxResult.Yes)
                        {
                            MessageBox.Show("You can't leave me that easily");
                        }
                        else break;
                    }
                    while (true);

                    do
                    {
                        result = MessageBox.Show("That's right baby. Now whose your daddy?", "Not today honey", MessageBoxButton.YesNo, MessageBoxImage.Stop);
                        if (result == MessageBoxResult.No)
                        {
                            MessageBox.Show("Imagine being stuck in a washing machine...");
                        }
                        else break;
                    }
                    while (true);

                    break;
                case 4:
                    MessageBox.Show("An error occurred while logging out. Please contact support.");
                    break;
            }
        }
    }
}
