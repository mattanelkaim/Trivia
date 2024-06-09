using Microsoft.VisualBasic;
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
using System.Xml.Linq;

namespace ClientGUI
{
    /// <summary>
    /// Interaction logic for JoinRoomPage.xaml
    /// </summary>
    public partial class JoinRoomPage : Page
    {
        public struct Room
        {
            public int maxPlayers { get; set; }
            public string name { get; set; }
            public int questionCount { get; set; }
            public int questionTimeout { get; set; }
            public int status { get; set; }
        }

        private Dictionary<string, Room>? rooms;
        StackPanel roomsStackPanel { get; set; }

        public JoinRoomPage()
        {
            InitializeComponent();
            this.DataContext = this;
            rooms = FetchRoomsFromDB();
            showRooms();
        }

        public static Dictionary<string, Room>? FetchRoomsFromDB()
        {
            return Helper.SendGetRoomsRequest();
        }

        private void HomeButton_Click(object sender, RoutedEventArgs? e)
        {
            this.NavigationService.Navigate(new MenuPage());
        }

        private void HighlightRoom_Hover(object sender, MouseEventArgs e)
        {
            if (sender is Grid roomRow) // Every row is a grid by itself
            {
                ColorAnimation animation = new()
                {
                    To = Color.FromArgb(128, 173, 216, 255),
                    Duration = TimeSpan.FromMilliseconds(200)
                };
                roomRow.Background.BeginAnimation(SolidColorBrush.ColorProperty, animation);
            }
        }
        
        private void showRooms()
        {
            foreach ((string id, Room room) in rooms)
            {
                // Adding all properties to the grid to match the xaml reference
                Grid roomGrid = new()
                {
                    Name = "room" + id,
                    Background = Brushes.Transparent
                };
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(50) });
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(1, GridUnitType.Star) });
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(100) });
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(100) });
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(100) });

                Polygon polygon1 = new()
                {
                    Style = (Style)FindResource("parallelogram"),
                    Points = [new Point(0, 0), new Point(42, 0), new Point(50, 32), new Point(0, 32)],
                    Margin = new Thickness(0) // Override style
                };
                roomGrid.Children.Add(polygon1);
                Grid.SetColumn(polygon1, 0);


                Image image = new()
                {
                    Width = 25,                    
                    VerticalAlignment = VerticalAlignment.Center,
                    Source = new BitmapImage(new Uri(room.status == 0 ? "Images/White/enter.png" : "Images/White/closed.png", UriKind.Relative))
                };

                if (room.status == 0)
                {
                    Button button = new()
                    {
                        BorderThickness = new Thickness(0),
                        BorderBrush = new SolidColorBrush(Colors.Transparent),
                        Background = new SolidColorBrush(Colors.Transparent),
                        ToolTip = "Enter this room!",
                        Cursor = Cursors.Hand,
                        Foreground = new SolidColorBrush(Colors.Transparent),
                    };
                    Helper.RemoveButtonHighlighting(button);
                    button.Content = image;
                    roomGrid.Children.Add(button);
                }
                else
                {
                    image.ToolTip = "Too late to enter...";
                    image.Cursor = Cursors.Help;
                    roomGrid.Children.Add(image);
                }

                Polygon polygon2 = new()
                {
                    Style = (Style)FindResource("parallelogram"),
                    Points = [new Point(0, 0), new Point(138, 0), new Point(146, 32), new Point(8, 32)]
                };
                roomGrid.Children.Add(polygon2);
                Grid.SetColumn(polygon2, 1);

                TextBlock textBlock1 = new()
                {
                    Text = room.name,
                    Style = (Style)FindResource("cell")
                };
                roomGrid.Children.Add(textBlock1);
                Grid.SetColumn(textBlock1, 1);

                Polygon polygon3 = new()
                {
                    Style = (Style)FindResource("parallelogram"),
                    Points = [new Point(0, 0), new Point(100, 0), new Point(108, 32), new Point(8, 32)],
                    Margin = new Thickness(-18, 0, 0, 0),
                    Fill = (Brush)FindResource("gradient")
                };
                roomGrid.Children.Add(polygon3);
                Grid.SetColumn(polygon3, 2);

                TextBlock textBlock2 = new()
                {
                    Style = (Style)FindResource("cell"),
                    Margin = new Thickness(-10, 0, 0, 0),
                    Text = FetchPlayersInRoom() + " / " + room.maxPlayers
                };
                roomGrid.Children.Add(textBlock2);
                Grid.SetColumn(textBlock2, 2);

                Polygon polygon4 = new()
                {
                    Points = [new Point(0, 0), new Point(110, 0), new Point(118, 32), new Point(8, 32)],
                    Style = (Style)FindResource("parallelogram"),
                    Margin = new Thickness(-18, 0, 0, 0)
                };
                roomGrid.Children.Add(polygon4);
                Grid.SetColumn(polygon4, 3);

                TextBlock textBlock3 = new()
                {
                    Style = (Style)FindResource("cell"),
                    Text = room.questionCount.ToString(),
                    Margin = new Thickness(-10, 0, 0, 0)
                };
                roomGrid.Children.Add(textBlock3);
                Grid.SetColumn(textBlock3, 3);

                Polygon polygon5 = new()
                {
                    Points = [new Point(0, 0), new Point(108, 0), new Point(108, 32), new Point(8, 32)],
                    Margin = new Thickness(-8, 0, 0, 0),
                    Style = (Style)FindResource("parallelogram")
                };
                roomGrid.Children.Add(polygon5);
                Grid.SetColumn(polygon5, 4);

                TextBlock textBlock4 = new()
                {
                    Style = (Style)FindResource("cell"),
                    Text = room.questionTimeout.ToString()
                };
                roomGrid.Children.Add(textBlock4);
                Grid.SetColumn(textBlock4, 4);



                // Create EventTrigger objects for MouseEnter and MouseLeave
                EventTrigger enterTrigger = new()
                {
                    RoutedEvent = Button.MouseEnterEvent  // Set the event to trigger on
                };

                EventTrigger leaveTrigger = new()
                {
                    RoutedEvent = Button.MouseLeaveEvent
                };

                // Add BeginStoryboard actions to the timelines
                BeginStoryboard enterAction = new()
                {
                    Storyboard = (Storyboard)FindResource("fadeIn")  // Reference the fadeIn Storyboard
                };
                enterTrigger.Actions.Add(enterAction);

                BeginStoryboard leaveAction = new()
                {
                    Storyboard = (Storyboard)FindResource("fadeOut") // Reference the fadeOut Storyboard
                };
                leaveTrigger.Actions.Add(leaveAction);

                // Add the triggers to the element
                roomGrid.Triggers.Add(enterTrigger);
                roomGrid.Triggers.Add(leaveTrigger);

                RoomsStackPanel.Children.Add(roomGrid);
            }
        }

        private int FetchPlayersInRoom()
        {
            return 6;
        }

    }
}
