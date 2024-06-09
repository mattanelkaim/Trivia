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
using static System.Net.Mime.MediaTypeNames;

namespace ClientGUI
{
    /// <summary>
    /// Interaction logic for CreateRoomPage.xaml
    /// </summary>
    public partial class CreateRoomPage : Page
    {
        public string RoomName { get; set; } = "";
        public string MaxPlayers { get; set; } = "";
        public string QuestionCount { get; set; } = "";
        public string QuestionTimeout { get; set; } = "";

        [System.Text.RegularExpressions.GeneratedRegex("[1-9]\\d*")] // Only allow numbers greater than 0
        private static partial System.Text.RegularExpressions.Regex MyRegex();

        public CreateRoomPage()
        {
            InitializeComponent();
            DataContext = this;
        }


        private void NumberOnlyTextBox_PreviewTextInput(object sender, System.Windows.Input.TextCompositionEventArgs e)
        {
            e.Handled = !MyRegex().IsMatch(((TextBox)sender).Text + e.Text); // Check input after concatenating current text with new input
        }

        private void HomeButton_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new MenuPage());
        }

        private void CreateSubmit_Click(object sender, RoutedEventArgs e)
        {
            // validating input
            if (this.RoomName == String.Empty || !(Int32.Parse(this.MaxPlayers) > 0 && Int32.Parse(this.MaxPlayers) <= 10) || !(Int32.Parse(this.QuestionCount) > 0 && Int32.Parse(this.QuestionCount) <= 10) || !(Int32.Parse(this.QuestionTimeout) > 0 && Int32.Parse(this.QuestionTimeout) <= 60))
            {
                MessageBox.Show("One or more of the fields are out of bounds!");
                return;
            }

            Helper.ResponseType responseCode = Helper.SendMessage(new {roomName = RoomName, maxUsers = MaxPlayers, questionCount = QuestionCount, answerTimeout = QuestionTimeout}, Helper.RequestType.CreateRoom).code;
        }

        private void Field_GotFocus(object sender, RoutedEventArgs e)
        {
            TextBox textBox = (TextBox)sender;
            TextBlock textBlock;

            // Determine which field is focused
            if (textBox.Name.Contains("RoomName"))
            {
                textBlock = this.RoomNameTextBlock;
            }
            else if (textBox.Name.Contains("MaxPlayers"))
            {
                textBlock = this.MaxPlayersTextBlock;
            }
            else if (textBox.Name.Contains("QuestionCount"))
            {
                textBlock = this.QuestionCountTextBlock;
            }
            else if (textBox.Name.Contains("QuestionTimeout"))
            {
                textBlock = this.QuestionTimeoutTextBlock;
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

        private void Field_LostFocus(object sender, RoutedEventArgs e)
        {
            TextBox textBox = (TextBox)sender;
            TextBlock textBlock;
            string fieldValue;

            // Determine which field is focused
            if (textBox.Name.Contains("RoomName"))
            {
                textBlock = this.RoomNameTextBlock;
                fieldValue = RoomName;
            }
            else if (textBox.Name.Contains("MaxPlayers"))
            {
                textBlock = this.MaxPlayersTextBlock;
                fieldValue = MaxPlayers;
            }
            else if (textBox.Name.Contains("QuestionCount"))
            {
                textBlock = this.QuestionCountTextBlock;
                fieldValue = QuestionCount;
            }
            else if (textBox.Name.Contains("QuestionTimeout"))
            {
                textBlock = this.QuestionTimeoutTextBlock;
                fieldValue = QuestionTimeout;
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
    }
}
