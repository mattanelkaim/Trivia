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
        public string Username { get; set; } = "";
        public string Password { private get; set; } = "";


        public MainWindow()
        {
            InitializeComponent();
            this.DataContext = this;
            this.LoginSubmit.Click += new RoutedEventHandler(LoginSubmit_Click);


            try
            {
                Communicator.Connect();
            }
            catch (Exception e)
            {
                MessageBox.Show("Connection failed: " + e.Message + "\nPlease try again later.");
                System.Windows.Application.Current.Shutdown();
            }
        }

        private void LoginSubmit_Click(object sender, RoutedEventArgs e)
        {
            string json = JsonSerializer.Serialize(new { username = Username, password = Password });

            string msg = Helper.Serialize(json, Helper.MessageType.Login);
            MessageBox.Show("[Sending]: " + msg);
            Communicator.Send(msg);

            string response = Communicator.Receive();
            MessageBox.Show("[Received]: " + response);
        }

        private void Username_GotFocus(object sender, RoutedEventArgs e)
        {
            ThicknessAnimation marginAnimation = new()
            {
                To = new Thickness(2, 0, 0, 0),
                Duration = TimeSpan.FromMilliseconds(200),
                EasingFunction = new QuarticEase()
            };
            this.UsernameTextBlock.BeginAnimation(MarginProperty, marginAnimation);

            DoubleAnimation fontSizeAnimation = new()
            {
                To = 12,
                Duration = TimeSpan.FromMilliseconds(200),
                EasingFunction = new QuarticEase()
            };
            this.UsernameTextBlock.BeginAnimation(FontSizeProperty, fontSizeAnimation);
        }

        private void Username_LostFocus(object sender, RoutedEventArgs e)
        {
            if (Username.Length == 0)
            {
                ThicknessAnimation marginAnimation = new()
                {
                    To = new Thickness(28, 23, 0, -28), // -28 to overlap with field, 23 to save space above (save 5 for fontSize change)
                    Duration = TimeSpan.FromMilliseconds(200),
                    EasingFunction = new QuarticEase()
                };
                this.UsernameTextBlock.BeginAnimation(MarginProperty, marginAnimation);

                DoubleAnimation fontSizeAnimation = new()
                {
                    To = 16,
                    Duration = TimeSpan.FromMilliseconds(200),
                    EasingFunction = new QuarticEase()
                };
                this.UsernameTextBlock.BeginAnimation(FontSizeProperty, fontSizeAnimation);
            }
        }

        private void Password_GotFocus(object sender, RoutedEventArgs e)
        {
            ThicknessAnimation marginAnimation = new()
            {
                To = new Thickness(2, 0, 0, 0),
                Duration = TimeSpan.FromMilliseconds(200),
                EasingFunction = new QuarticEase()
            };
            this.PasswordTextBlock.BeginAnimation(MarginProperty, marginAnimation);

            DoubleAnimation fontSizeAnimation = new()
            {
                To = 12,
                Duration = TimeSpan.FromMilliseconds(200),
                EasingFunction = new QuarticEase()
            };
            this.PasswordTextBlock.BeginAnimation(FontSizeProperty, fontSizeAnimation);
        }

        private void Password_LostFocus(object sender, RoutedEventArgs e)
        {
            if (Password.Length == 0)
            {
                ThicknessAnimation marginAnimation = new()
                {
                    To = new Thickness(28, 23, 0, -28), // -28 to overlap with field, 23 to save space above (save 5 for fontSize change)
                    Duration = TimeSpan.FromMilliseconds(200),
                    EasingFunction = new QuarticEase()
                };
                this.PasswordTextBlock.BeginAnimation(MarginProperty, marginAnimation);

                DoubleAnimation fontSizeAnimation = new()
                {
                    To = 16,
                    Duration = TimeSpan.FromMilliseconds(200),
                    EasingFunction = new QuarticEase()
                };
                this.PasswordTextBlock.BeginAnimation(FontSizeProperty, fontSizeAnimation);
            }
        }

        private void Password_PasswordChanged(object sender, RoutedEventArgs e)
        {
            Password = PasswordField.Password;
            // TODO(mattan): Add password validation
        }

        private void ShowPasswordCheckBox_Checked(object sender, RoutedEventArgs e)
        {
            PasswordTextBox.Text = Password;
            PasswordTextBox.Visibility = Visibility.Visible;
            PasswordField.Visibility = Visibility.Collapsed;
        }

        private void ShowPasswordCheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            PasswordField.Password = Password;
            PasswordTextBox.Visibility = Visibility.Collapsed;
            PasswordField.Visibility = Visibility.Visible;
        }
    }
}
