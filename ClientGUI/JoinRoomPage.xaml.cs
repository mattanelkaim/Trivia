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
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ClientGUI
{
    /// <summary>
    /// Interaction logic for JoinRoomPage.xaml
    /// </summary>
    public partial class JoinRoomPage : Page
    {
        public JoinRoomPage()
        {
            InitializeComponent();
        }

        private void HomeButton_Click(object sender, RoutedEventArgs? e)
        {
            this.NavigationService.Navigate(new MenuPage());
        }

        private void HighlightRoom_Hover(object sender, MouseEventArgs e)
        {
            if (sender is Grid roomRow) // Every row is a grid by itself
            {
                ColorAnimation animation = new ()
                {
                    To = Color.FromArgb(128, 173, 216, 255),
                    Duration = TimeSpan.FromMilliseconds(400)
                };
                roomRow.Background.BeginAnimation(SolidColorBrush.ColorProperty, animation);
            }
        }
    }
}
