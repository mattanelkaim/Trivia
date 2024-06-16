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
    /// Interaction logic for WaitingRoomPage.xaml
    /// </summary>
    public partial class WaitingRoomPage : Page
    {
        public WaitingRoomPage()
        {
            InitializeComponent();
        }

        private void LeaveRoom_Click(object sender, RoutedEventArgs? e)
        {            
            Helper.ResponseType status = Helper.SendLeaveRoomRequest(); // Button name is the room id

            switch (status)
            {
                case Helper.ResponseType.OK:
                    this.NavigationService.Navigate(new MenuPage());
                    break;
                default:
                    MessageBox.Show("Cannot join room!");
                    break;
            }            
        }
    }
}
