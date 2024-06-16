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
    /// Interaction logic for AdminWaitingRoom.xaml
    /// </summary>
    public partial class AdminWaitingRoom : Page
    {
        public AdminWaitingRoom()
        {
            InitializeComponent();
        }

        private void CloseRoom_Click(object sender, RoutedEventArgs? e)
        {
            Helper.ResponseType status = Helper.SendCloseRoomRequest(); 

            switch (status)
            {
                case Helper.ResponseType.OK:
                    this.NavigationService.Navigate(new MenuPage());
                    break;
                default:
                    MessageBox.Show("Cannot Close room!");
                    break;
            }
        }

        private void StartGame_Click(object sender, RoutedEventArgs? e)
        {
            Helper.ResponseType status = Helper.SendStartGameRequest();

            switch (status)
            {
                case Helper.ResponseType.OK:
                    // this.NavigationService.Navigate(new AdminGamePage());
                    break;
                default:
                    MessageBox.Show("Cannot start room!");
                    break;
            }
        }
    }
}
