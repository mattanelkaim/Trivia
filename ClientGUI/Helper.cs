using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Security;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Animation;

namespace ClientGUI
{
    internal class Helper
    {
#region protocolHelper

#if false // to debug
    public static void DEBUG_SHOW(string message) { _ = MessageBox.Show(message); }
#else
    public static void DEBUG_SHOW(string message) {}
#endif

        public static readonly ushort BYTES_RESERVED_FOR_CODE = 2;
        public static readonly ushort BYTES_RESERVED_FOR_MSG_LEN = 4;

        public enum RequestType
        {
            Login,
            Register,
            GetPlayersInRoom,
            JoinRoom,
            CreateRoom,
            GetRooms,
            GetStatistics,
            GetHighscore,
            Logout,
            StartRoom,
            LeaveRoom,
            CloseRoom,
            GetRoomState,
            SubmitAnswer,
            LeaveGame,
            GetQuestion,
            GetGameResult,
        }

        public enum ResponseType
        {
            OK,
            // Login
            LOGIN_FAILED,
            // Signup
            INVALID_USERNAME,
            INVALID_PASSWORD,
            INVALID_EMAIL,
            USERNAME_ALREADY_EXISTS,
            // Join Room
            ROOM_IS_FULL,
            ROOM_IS_NOT_OPEN, // Either closed or in-game
                              // In-game
            NO_MORE_QUESTIONS,
            WAIT_FOR_OTHERS,
            // General Errors
            ERR, // ERROR won't compile
            ERR_NOT_FOUND,
            ERR_ALREADY_EXISTS,
        }

        public enum RoomStatus
        {
            OPEN,
            IN_GAME,
            CLOSED,
        };

        /// <summary>
        /// Serializes a JSON message with its code and length.
        /// </summary>
        /// <param name="content">The content (in JSON format) to be serialized.</param>
        /// <param name="code">The code (message type) to be serialized.</param>
        /// <returns>The serialized message.</returns>
        public static string Serialize(string content, RequestType code)
        {
            // Example for a login request (without spaces of course): 0 0038 {"username":"Mattan","password":"Gil"}
            string serializedCode = ((int)code).ToString().PadLeft(BYTES_RESERVED_FOR_CODE, '0');
            string serializedLen = content.Length.ToString().PadLeft(BYTES_RESERVED_FOR_MSG_LEN, '0');
            return serializedCode + serializedLen + content;
        }

        public static Response SendMessage(object structToSend, RequestType code)
        {
            string json = JsonSerializer.Serialize(structToSend);

            string msg = Helper.Serialize(json, code);
            Helper.DEBUG_SHOW("[Sending]: " + msg);
            Communicator.Send(msg);

            string responseBuffer = Communicator.Receive();
            Helper.DEBUG_SHOW("[Received]: " + responseBuffer);

            Response response = ExtractResponse(responseBuffer);

            if (response.code != ResponseType.OK)
            {
                throw new Exception(); // TODO: Throw a more specific exception
            }

            return response;
        }

#endregion protocolHelper


#region specificRequests

        // STRUCTS FOR RESPONSES

        public struct Response
        {
            public ResponseType code;
            public string content;
        }

        // Just a helper struct to deserialize the response
        public struct ResponseWithStatus
        {
            public int status { get; set; } // MUST HAVE A GETTER AND SETTER ELSE THIS SHITTY DESIRIALIZER WON'T WORK
        }

        public struct PersonalStatsResponse
        {
            public PersonalStatsPage.PersonalStats userStatistics { get; set; }
        }

        public struct HighScoresResponse
        {
            public Dictionary<string, ScoreboardPage.Highscore> highScores { get; set; }
        }

        public struct GetRoomsResponse
        {
            public Dictionary<string, JoinRoomPage.Room> rooms { get; set; }
        }

        public struct GetPlayersInRoomResponse
        {
            public List<string> playersInRoom { get; set; }
        }

        public struct GetRoomStateResponse
        {
            public WaitingRoomPage.RoomData roomState { get; set; }
        }

        public struct GetNextQuestionResponse
        {
            public int status { get; set; }
            public string question { get; set; }
            public Dictionary<string, string> answers { get; set; }
        }

        public struct SubmitAnswerResponse
        {
            public int status { get; set; }
            public int correctAnsID { get; set; }
        }

        // ACTUAL FUNCTIONS THAT SEND REQUESTS

        public static Response ExtractResponse(string response)
        {
            int responseCode = int.Parse(response[..BYTES_RESERVED_FOR_CODE]); // Idk what fucking syntax that is but it works
            int msgLen = int.Parse(response.Substring(BYTES_RESERVED_FOR_CODE, BYTES_RESERVED_FOR_MSG_LEN));
            string jsonResponse = response.Substring(BYTES_RESERVED_FOR_CODE + BYTES_RESERVED_FOR_MSG_LEN, msgLen);

            return new Response { code = (ResponseType)responseCode, content = jsonResponse };
        }

        public static ResponseType SendLoginRequest(string Username, string Password)
        {
            Response response = SendMessage(new { username = Username, password = Password }, RequestType.Login);

            // Response example: {"status":1}
            return (ResponseType)JsonSerializer.Deserialize<ResponseWithStatus>(response.content).status;
        }

        public static ResponseType SendSignupRequest(string Username, string Password, string Email)
        {
            Response response = SendMessage(new { username = Username, password = Password, email = Email }, RequestType.Register);

            // Response example: {"status":1}
            return (ResponseType)JsonSerializer.Deserialize<ResponseWithStatus>(response.content).status;
        }

        public static PersonalStatsResponse SendPersonalStatsRequest()
        {
            Response response = SendMessage(new { }, RequestType.GetStatistics);

            // Response example: {"userStatistics":{"correctAnswers":"7","games":"3","score":"3.416667","totalAnswers":"11"}}
            return JsonSerializer.Deserialize<PersonalStatsResponse>(response.content);
        }

        public static Dictionary<string, ScoreboardPage.Highscore> SendScoreboardRequest()
        {
            Response response = SendMessage(new { }, RequestType.GetHighscore);

            return JsonSerializer.Deserialize<HighScoresResponse>(response.content).highScores;
        }

        public static Dictionary<string, JoinRoomPage.Room> SendGetRoomsRequest()
        {
            Response response = SendMessage(new { }, RequestType.GetRooms);

            return JsonSerializer.Deserialize<GetRoomsResponse>(response.content).rooms;
        }

        public static List<string> SendGetPlayersInRoomRequest(string id)
        {
            Response response = SendMessage(new { roomId = id }, RequestType.GetPlayersInRoom);

            return JsonSerializer.Deserialize<GetPlayersInRoomResponse>(response.content).playersInRoom;
        }

        public static ResponseType SendJoinRoomRequest(string id)
        {
            Response response = SendMessage(new { roomId = id }, RequestType.JoinRoom);

            // Response example: {"status":1}
            return (ResponseType)JsonSerializer.Deserialize<ResponseWithStatus>(response.content).status;
        }

        public static ResponseType SendLeaveRoomRequest()
        {
            Response response = SendMessage(new { }, RequestType.LeaveRoom);

            return (ResponseType)JsonSerializer.Deserialize<ResponseWithStatus>(response.content).status;
        }

        public static ResponseType SendCloseRoomRequest()
        {
            Response response = SendMessage(new { }, RequestType.CloseRoom);

            return (ResponseType)JsonSerializer.Deserialize<ResponseWithStatus>(response.content).status;
        }

        public static ResponseType SendStartGameRequest()
        {
            Response response = SendMessage(new { }, RequestType.StartRoom);

            return (ResponseType)JsonSerializer.Deserialize<ResponseWithStatus>(response.content).status;
        }

        public static WaitingRoomPage.RoomData SendGetRoomStateRequest()
        {
            Response response = SendMessage(new { }, RequestType.GetRoomState);

            // Response example: 000118{"roomState":{"hasGameBegun":false,"playersInRoom":["admin","gil"],"questionCount":12,"questionTimeout":10,"state":0}}
            return JsonSerializer.Deserialize<GetRoomStateResponse>(response.content).roomState;
        }

        public static GetNextQuestionResponse SendGetNextQuestionRequest()
        {
            Response response = SendMessage(new { }, RequestType.GetQuestion);

            return JsonSerializer.Deserialize<GetNextQuestionResponse>(response.content);
        }

        public static SubmitAnswerResponse SendSubmitAnswerRequest(int AnswerId)
        {
            Response response = SendMessage(new {answerId = AnswerId }, RequestType.SubmitAnswer);

            return JsonSerializer.Deserialize<SubmitAnswerResponse>(response.content);
        }

        #endregion specificRequests


        #region XAMLMethodsHelper

        // Also in Login window xaml
        public static readonly ushort MAX_PASSWORD_LENGTH = 8;
        public static readonly ushort MAX_USERNAME_LENGTH = 16;

        public static void HomeButton_Click(Page page, RoutedEventArgs? e)
        {
            page.NavigationService.Navigate(new MenuPage());
        }

        public static void RemoveButtonHighlighting(Button button)
        {
            ControlTemplate controlTemplate = new(typeof(Button));

            FrameworkElementFactory border = new(typeof(Border))
            {
                Name = "border"
            };
            border.SetValue(Border.BackgroundProperty, new TemplateBindingExtension(Button.BackgroundProperty));
            border.SetValue(Border.BorderBrushProperty, new TemplateBindingExtension(Button.BorderBrushProperty));
            border.SetValue(Border.BorderThicknessProperty, new TemplateBindingExtension(Button.BorderThicknessProperty));

            FrameworkElementFactory contentPresenter = new(typeof(ContentPresenter));
            contentPresenter.SetValue(ContentPresenter.HorizontalAlignmentProperty, HorizontalAlignment.Center);
            contentPresenter.SetValue(ContentPresenter.VerticalAlignmentProperty, VerticalAlignment.Center);

            border.AppendChild(contentPresenter);
            controlTemplate.VisualTree = border;

            Trigger isPressedTrigger = new()
            {
                Property = Button.IsPressedProperty,
                Value = true
            };
            isPressedTrigger.Setters.Add(new Setter(Border.BackgroundProperty, Brushes.Transparent, "border"));
            controlTemplate.Triggers.Add(isPressedTrigger);

            Trigger isEnabledTrigger = new()
            {
                Property = UIElement.IsEnabledProperty,
                Value = false
            };

            isEnabledTrigger.Setters.Add(new Setter(Border.BackgroundProperty, Brushes.Gray, "border"));
            controlTemplate.Triggers.Add(isEnabledTrigger);

            button.Template = controlTemplate;
        }


        public static void FieldGotFocusEffect(TextBlock textBlock)
        {
            // Animate position (margin)
            ThicknessAnimation marginAnimation = new()
            {
                To = new Thickness(2, 0, 0, 0),
                Duration = TimeSpan.FromMilliseconds(200),
                EasingFunction = new QuarticEase()
            };
            textBlock.BeginAnimation(TextBlock.MarginProperty, marginAnimation);

            // Animate font size
            DoubleAnimation fontSizeAnimation = new()
            {
                To = 12,
                Duration = TimeSpan.FromMilliseconds(200),
                EasingFunction = new QuarticEase()
            };
            textBlock.BeginAnimation(TextBlock.FontSizeProperty, fontSizeAnimation);
        }

        public static void FieldLostFocusEffect(TextBlock textBlock)
        {
            // Animate position (margin)
            ThicknessAnimation marginAnimation = new()
            {
                To = new Thickness(28, 23, 0, -28), // -28 to overlap with field, 23 to save space above (save 5 for fontSize change)
                Duration = TimeSpan.FromMilliseconds(200),
                EasingFunction = new QuarticEase()
            };
            textBlock.BeginAnimation(TextBlock.MarginProperty, marginAnimation);

            // Animate font size
            DoubleAnimation fontSizeAnimation = new()
            {
                To = 16,
                Duration = TimeSpan.FromMilliseconds(200),
                EasingFunction = new QuarticEase()
            };
            textBlock.BeginAnimation(TextBlock.FontSizeProperty, fontSizeAnimation);
        }


        public static void ButtonGotHoverEffect(Button button)
        {
            ColorAnimation foregroundAnimation = new()
            {
                To = Colors.DeepPink,
                Duration = TimeSpan.FromMilliseconds(400),
                EasingFunction = new QuarticEase()
            };

            SolidColorBrush brush = new()
            {
                Color = Colors.DarkOrange
            };
            button.Foreground = brush;
            brush.BeginAnimation(SolidColorBrush.ColorProperty, foregroundAnimation);
        }

        public static void ButtonLostHoverEffect(Button button)
        {
            ColorAnimation foregroundAnimation = new()
            {
                To = Colors.DarkOrange,
                Duration = TimeSpan.FromMilliseconds(800),
                EasingFunction = new QuarticEase()
            };

            SolidColorBrush brush = new()
            {
                Color = Colors.HotPink
            };
            button.Foreground = brush;
            brush.BeginAnimation(SolidColorBrush.ColorProperty, foregroundAnimation);
        }

#endregion XAMLMethodsHelper
    }
}
