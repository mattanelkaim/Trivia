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
            this.LoginSubmit.Click += new RoutedEventHandler(LoginSubmit_Click);


            try
            {
                Communicator.Connect();
            }
            catch (Exception e)
            {
                MessageBox.Show("Connection failed: " + e.Message);
            }
        }

        private void LoginSubmit_Click(object sender, RoutedEventArgs e)
        {
            string json = JsonSerializer.Serialize(new { username = this.UsernameField.Text, password = this.PasswordField.Password });

            string msg = Helper.Serialize(json, Helper.MessageType.Login);
            MessageBox.Show("[Sending]: " + msg);
            Communicator.Send(msg);

            string response = Communicator.Receive();
            MessageBox.Show("[Received]: " + response);
        }

        private void Username_GotFocus(object sender, RoutedEventArgs e)
        {
            ThicknessAnimation animation = new()
            {
                To = new Thickness(2, 0, 0, 0),
                Duration = TimeSpan.FromMilliseconds(200),
                EasingFunction = new QuarticEase()
            };
            this.UsernameTextBlock.BeginAnimation(MarginProperty, animation);
        }

        private void Username_LostFocus(object sender, RoutedEventArgs e)
        {
            if (this.UsernameField.Text.Length == 0)
            {
                ThicknessAnimation animation = new()
                {
                    To = new Thickness(23, 17, 0, 0),
                    Duration = TimeSpan.FromMilliseconds(200),
                    EasingFunction = new QuarticEase()
                };
                this.UsernameTextBlock.BeginAnimation(MarginProperty, animation);
            }
        }

        private void Password_GotFocus(object sender, RoutedEventArgs e)
        {
            ThicknessAnimation animation = new()
            {
                To = new Thickness(2, 47, 0, 0),
                Duration = TimeSpan.FromMilliseconds(200),
                EasingFunction = new QuarticEase()
            };
            this.PasswordTextBlock.BeginAnimation(MarginProperty, animation);
        }

        private void Password_LostFocus(object sender, RoutedEventArgs e)
        {
            if (this.PasswordField.Password.Length == 0)
            {
                ThicknessAnimation animation = new()
                {
                    To = new Thickness(23, 64, 0, 0),
                    Duration = TimeSpan.FromMilliseconds(200),
                    EasingFunction = new QuarticEase()
                };
                this.PasswordTextBlock.BeginAnimation(MarginProperty, animation);
            }
        }
    }
}
