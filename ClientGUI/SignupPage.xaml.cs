﻿using System;
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
        public string Username { get; set; }
        public string Password { private get; set; } = "";
        public string Email { get; set; } = "";

        public SignupPage(string Username = "")
        {
            InitializeComponent();
            this.DataContext = this;

            this.Username = Username;
            if (Username != string.Empty)
                Field_GotFocus(UsernameField, null);
        }

        private void SignupSubmit_Click(object sender, RoutedEventArgs? e)
        {
            // validating input
            if (this.Username == string.Empty || this.Password == string.Empty || this.Email == string.Empty)
            {
                MessageBox.Show("One or more of the fields is empty!");
                return;
            }

            string responseBuffer = Helper.SendMessage(new { username = Username, password = Password, email = Email }, Helper.RequestType.Register);

            if (responseBuffer[0] == Helper.ToChar(Helper.ResponseType.OK))
            {
                if (responseBuffer[15] == Helper.ToChar(Helper.ResponseType.OK))
                    this.NavigationService.Navigate(new MenuPage());
                else // Helper.ResponseType.USERNAME_ALREADY_EXISTS
                    MessageBox.Show("Username or Email are already taken");
            }
            else
            {
                throw new Exception(); // If this happens then there is a problem with the client
                                       // I didnt know which exception to throw
            }
        }

        private void Field_GotFocus(object sender, RoutedEventArgs? e)
        {
            TextBlock textBlock;

            // Determine which field is focused
            if (sender is TextBox textBox) // Insantiates variable textBox
            {
                if (textBox.Name.Contains("Username"))
                {
                    textBlock = this.UsernameTextBlock;
                }
                else if (textBox.Text.Contains("Password"))
                {
                    textBlock = this.PasswordTextBlock;
                }
                else if (textBox.Name.Contains("Email"))
                {
                    textBlock = this.EmailTextBlock;
                }
                else return; // Not a field we care about
            }
            else if (sender is PasswordBox)
            {
                textBlock = this.PasswordTextBlock;
            }
            else return; // Not a field we care about

            // Animate position (margin)
            ThicknessAnimation marginAnimation = new()
            {
                To = new Thickness(2, 0, 0, 0),
                Duration = TimeSpan.FromMilliseconds(200),
                EasingFunction = new QuarticEase()
            };
            textBlock.BeginAnimation(MarginProperty, marginAnimation);

            // Animate font size
            DoubleAnimation fontSizeAnimation = new()
            {
                To = 12,
                Duration = TimeSpan.FromMilliseconds(200),
                EasingFunction = new QuarticEase()
            };
            textBlock.BeginAnimation(FontSizeProperty, fontSizeAnimation);
        }

        private void Field_LostFocus(object sender, RoutedEventArgs? e)
        {
            TextBlock textBlock;
            string fieldValue;

            // Determine which field is focused
            if (sender is TextBox textBox) // Insantiates variable textBox
            {
                if (textBox.Name.Contains("Username"))
                {
                    textBlock = this.UsernameTextBlock;
                    fieldValue = Username;
                }
                else if (textBox.Text.Contains("Password"))
                {
                    textBlock = this.PasswordTextBlock;
                    fieldValue = Password;
                }
                else if (textBox.Name.Contains("Email"))
                {
                    textBlock = this.EmailTextBlock;
                    fieldValue = Email;
                }
                else return; // Not a field we care about
            }
            else if (sender is PasswordBox)
            {
                textBlock = this.PasswordTextBlock;
                fieldValue = Password;
            }
            else return; // Not a field we care about

            // Animate only if field is empty
            if (fieldValue.Length == 0)
            {
                // Animate position (margin)
                ThicknessAnimation marginAnimation = new()
                {
                    To = new Thickness(28, 23, 0, -28), // -28 to overlap with field, 23 to save space above (save 5 for fontSize change)
                    Duration = TimeSpan.FromMilliseconds(200),
                    EasingFunction = new QuarticEase()
                };
                textBlock.BeginAnimation(MarginProperty, marginAnimation);

                // Animate font size
                DoubleAnimation fontSizeAnimation = new()
                {
                    To = 16,
                    Duration = TimeSpan.FromMilliseconds(200),
                    EasingFunction = new QuarticEase()
                };
                textBlock.BeginAnimation(FontSizeProperty, fontSizeAnimation);
            }
        }

        private void Password_PasswordChanged(object sender, RoutedEventArgs? e)
        {
            Password = PasswordField.Password;
            // TODO(mattan): Add password validation
        }

        private void ShowPasswordCheckBox_Checked(object sender, RoutedEventArgs? e)
        {
            PasswordTextBox.Text = Password;
            PasswordTextBox.Visibility = Visibility.Visible;
            PasswordField.Visibility = Visibility.Collapsed;
        }

        private void ShowPasswordCheckBox_Unchecked(object sender, RoutedEventArgs? e)
        {
            PasswordField.Password = Password;
            PasswordTextBox.Visibility = Visibility.Collapsed;
            PasswordField.Visibility = Visibility.Visible;
        }

        private void LoginLink_Click(object sender, RoutedEventArgs? e)
        {
            this.NavigationService.Navigate(new LoginPage(Username));
        }
    }
}
