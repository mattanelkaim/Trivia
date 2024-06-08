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
        StackPanel roomsStackPanel {  get; set; }

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
                ColorAnimation animation = new ()
                {
                    To = Color.FromArgb(128, 173, 216, 255),
                    Duration = TimeSpan.FromMilliseconds(400)
                };
                roomRow.Background.BeginAnimation(SolidColorBrush.ColorProperty, animation);
            }
        }
        
        private void showRooms()
        {
            foreach ((string id, Room room) in rooms)
            {
                Grid roomGrid = new Grid();                

                // adding all properties to the grid to match the xaml
                roomGrid.Name = "room" + id;
                roomGrid.Background = Brushes.Transparent;
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(50) });
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(1, GridUnitType.Star) });
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(100) });
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(100) });
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(100) });

                Polygon polygon1 = new Polygon();
                polygon1.Points = new PointCollection() { new Point(0, 0), new Point(42, 0), new Point(50, 32), new Point(0, 32) };
                polygon1.Margin = new Thickness(0);                
                polygon1.Style = (Style)FindResource("parallelogram");                
                roomGrid.Children.Add(polygon1);
                Grid.SetColumn(polygon1, 0);

                Button button = new Button();
                button.BorderBrush = new SolidColorBrush(Colors.Transparent);
                button.BorderThickness = new Thickness(0);
                button.ToolTip = "Enter this room!";
                button.Cursor = Cursors.Help;
                button.Background = new SolidColorBrush(Colors.Transparent);

                Image image = new Image();
                image.Source = new BitmapImage(new Uri(room.status == 0 ? "Images/White/enter.png" : "Images/White/closed.png", UriKind.Relative));
                image.HorizontalAlignment = HorizontalAlignment.Left;
                image.VerticalAlignment = VerticalAlignment.Center;
                image.Width = 25;

                button.Content = image;
                roomGrid.Children.Add(button);

                Polygon polygon2 = new Polygon();                
                polygon2.Points = new PointCollection() { new Point(0, 0), new Point(138, 0), new Point(146, 32), new Point(8, 32) };
                polygon2.Style = (Style)FindResource("parallelogram");                                               
                roomGrid.Children.Add(polygon2);
                Grid.SetColumn(polygon2, 1);

                TextBlock textBlock1 = new TextBlock();
                textBlock1.Text = room.name;
                textBlock1.Style = (Style)FindResource("cell");
                roomGrid.Children.Add(textBlock1);
                Grid.SetColumn(textBlock1, 1);

                Polygon polygon3 = new Polygon();                                
                polygon3.Style = (Style)FindResource("parallelogram");                                                
                polygon3.Points = new PointCollection() { new Point(0, 0), new Point(100, 0), new Point(108, 32), new Point(8, 32) };
                polygon3.Margin = new Thickness(-18, 0, 0, 0);
                polygon3.Fill = (Brush)FindResource("gradient");
                roomGrid.Children.Add(polygon3);
                Grid.SetColumn(polygon3, 2);

                TextBlock textBlock2 = new TextBlock();                
                textBlock2.Style = (Style)FindResource("cell");
                textBlock2.Margin = new Thickness(-10, 0, 0, 0);
                textBlock2.Text = FetchPlayersInRoom().ToString() + " / " + room.maxPlayers.ToString();
                roomGrid.Children.Add(textBlock2);
                Grid.SetColumn(textBlock2, 2);

                Polygon polygon4 = new Polygon();                
                polygon4.Points = new PointCollection() { new Point(0, 0), new Point(110, 0), new Point(118, 32), new Point(8, 32) };
                polygon4.Style = (Style)FindResource("parallelogram");
                polygon4.Margin = new Thickness(-18, 0, 0, 0);
                roomGrid.Children.Add(polygon4);
                Grid.SetColumn(polygon4, 3);

                TextBlock textBlock3 = new TextBlock();                
                textBlock3.Style = (Style)FindResource("cell");                
                textBlock3.Text = room.questionCount.ToString();
                textBlock3.Margin = new Thickness(-10, 0, 0, 0);
                roomGrid.Children.Add(textBlock3);
                Grid.SetColumn(textBlock3, 3);

                Polygon polygon5 = new Polygon();                
                polygon5.Points = new PointCollection() { new Point(0, 0), new Point(108, 0), new Point(108, 32), new Point(8, 32) };
                polygon5.Margin = new Thickness(-8, 0, 0, 0);
                polygon5.Style = (Style)FindResource("parallelogram");
                roomGrid.Children.Add(polygon5);
                Grid.SetColumn(polygon5, 4);

                TextBlock textBlock4 = new TextBlock();                
                textBlock4.Style = (Style)FindResource("cell");                
                textBlock4.Text = room.questionTimeout.ToString();
                roomGrid.Children.Add(textBlock4);
                Grid.SetColumn(textBlock4, 4);



                //// Create EventTrigger objects for MouseEnter and MouseLeave
                //EventTrigger enterTrigger = new EventTrigger();
                //enterTrigger.RoutedEvent = Button.MouseEnterEvent;  // Set the event to trigger on                

                //EventTrigger leaveTrigger = new EventTrigger();
                //leaveTrigger.RoutedEvent = Button.MouseLeaveEvent;                

                //// Add BeginStoryboard actions to the timelines
                //BeginStoryboard enterAction = new BeginStoryboard();
                //enterAction.Storyboard = (Storyboard)FindResource("fadeIn");  // Reference the fadeIn Storyboard
                //enterTrigger.Actions.Add(enterAction);

                //BeginStoryboard leaveAction = new BeginStoryboard();
                //leaveAction.Storyboard = (Storyboard)FindResource("fadeOut"); // Reference the fadeOut Storyboard
                //leaveTrigger.Actions.Add(leaveAction);

                //// Create a TriggerCollection and add the triggers
                //TriggerCollection triggers = new TriggerCollection();
                //triggers.Add(enterTrigger);
                //triggers.Add(leaveTrigger);

                //roomGrid.AddHandler(MouseEnterEvent, (Delegate)HighlightRoom_Hover);

                RoomsStackPanel.Children.Add(roomGrid);
            }

        }

        private int FetchPlayersInRoom()
        {
            return 6;
        }

    }
}
