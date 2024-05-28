using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace ClientGUI
{
    /// <summary>
    /// Interaction logic for SignupPage.xaml
    /// </summary>
    public partial class SignupPage : Page
    {
        public string Username { get; set; } = "";
        public string Password { private get; set; } = "";
        public string Email { get; set; } = "";

        public SignupPage()
        {
            InitializeComponent();
            this.DataContext = this;
        }

        private void LoginSubmit_Click(object sender, RoutedEventArgs e)
        {
            if (this.Username == string.Empty || this.Password == string.Empty)
                MessageBox.Show("One or more of the fields is empty!");

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

        private void Email_GotFocus(object sender, RoutedEventArgs e)
        {
            ThicknessAnimation marginAnimation = new()
            {
                To = new Thickness(2, 0, 0, 0),
                Duration = TimeSpan.FromMilliseconds(200),
                EasingFunction = new QuarticEase()
            };
            this.EmailTextBlock.BeginAnimation(MarginProperty, marginAnimation);

            DoubleAnimation fontSizeAnimation = new()
            {
                To = 12,
                Duration = TimeSpan.FromMilliseconds(200),
                EasingFunction = new QuarticEase()
            };
            this.EmailTextBlock.BeginAnimation(FontSizeProperty, fontSizeAnimation);
        }

        private void Email_LostFocus(object sender, RoutedEventArgs e)
        {
            if (Username.Length == 0)
            {
                ThicknessAnimation marginAnimation = new()
                {
                    To = new Thickness(28, 33, 0, -38), // -38 to overlap with field, 33 to save space above (save 5 for fontSize change)
                    Duration = TimeSpan.FromMilliseconds(200),
                    EasingFunction = new QuarticEase()
                };
                this.EmailTextBlock.BeginAnimation(MarginProperty, marginAnimation);

                DoubleAnimation fontSizeAnimation = new()
                {
                    To = 16,
                    Duration = TimeSpan.FromMilliseconds(200),
                    EasingFunction = new QuarticEase()
                };
                this.EmailTextBlock.BeginAnimation(FontSizeProperty, fontSizeAnimation);
            }
        }

        private void LoginLink_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new LoginPage());
        }
    }
}
