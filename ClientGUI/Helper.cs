﻿using System;
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

namespace ClientGUI
{
    internal class Helper
    {
        #region protocolHelper

#if false // to debug
    public static void _DEBUG_SHOW(string message) { _ = MessageBox.Show(message); }
#else
    public static void _DEBUG_SHOW(string message) {}
#endif

        public static readonly ushort BYTES_RESERVED_FOR_CODE = 1;
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
            Logout
        }

        public enum ResponseType
        {
            ERR, // ERROR won't compile
            OK,
            // Login
            LOGIN_FAILED,
            // Signup
            USERNAME_ALREADY_EXISTS
        }

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

        public static Response SendMessage(object structTosend, RequestType code)
        {
            string json = JsonSerializer.Serialize(structTosend);

            string msg = Helper.Serialize(json, code);
            Helper._DEBUG_SHOW("[Sending]: " + msg);
            Communicator.Send(msg);

            string responseBuffer = Communicator.Receive();
            Helper._DEBUG_SHOW("[Received]: " + responseBuffer);

            Response response = ExtractResponse(responseBuffer);

            if (response.code != ResponseType.OK)
            {
                throw new Exception(); // TODO: Throw a more specific exception
            }

            return response;
        }

        #endregion protocolHelper


        #region specificRequests

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

        public class HighScoresResponse
        {
            public Dictionary<string, ScoreboardPage.Highscore>? highScores { get; set; }
        }

        public class GetRoomsResponse
        {
            public Dictionary<string, JoinRoomPage.Room>? rooms { get; set; }
        }

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

            // Ideally expects {"status":1}
            return (ResponseType)JsonSerializer.Deserialize<ResponseWithStatus>(response.content).status;
        }

        public static ResponseType SendSignupRequest(string Username, string Password, string Email)
        {
            Response response = SendMessage(new { username = Username, password = Password, email = Email }, RequestType.Register);

            // Ideally expects {"status":1}
            return (ResponseType)JsonSerializer.Deserialize<ResponseWithStatus>(response.content).status;
        }

        public static PersonalStatsResponse SendPersonalStatsRequest()
        {
            Response response = SendMessage(new {}, RequestType.GetStatistics);

            // Expects {"userStatistics":{"correctAnswers":"7","games":"3","score":"3.416667","totalAnswers":"11"}}
            return JsonSerializer.Deserialize<PersonalStatsResponse>(response.content);
        }

        public static Dictionary<string, ScoreboardPage.Highscore>? SendScoreboardRequest()
        {
            Response response = SendMessage(new { }, RequestType.GetHighscore);

            return JsonSerializer.Deserialize<HighScoresResponse>(response.content)?.highScores;
        }

        public static Dictionary<string, JoinRoomPage.Room>? SendGetRoomsRequest()
        {
            Response response = SendMessage(new { }, RequestType.GetRooms);

            return JsonSerializer.Deserialize<GetRoomsResponse>(response.content)?.rooms;
        }

        #endregion specificRequests

        #region XAMLMethodsHelper

        // Also in Login window xaml
        public static readonly ushort MAX_PASSWORD_LENGTH = 8;
        public static readonly ushort MAX_USERNAME_LENGTH = 16;

        public static void HomeButton_Click(Page page, object sender, RoutedEventArgs? e)
        {
            page.NavigationService.Navigate(new MenuPage());
        }

        public class DeepCopy<T>
        {
            public static T Copy<T>(T obj)
            {
                if (obj == null)
                {
                    return default;
                }

                // Check for value types and strings (consider using IsValueType instead for .NET Core)
                if (obj is ValueType || obj is string)
                {
                    return obj;
                }

                // Reflection for complex objects
                var type = typeof(T);                

                if (type.IsClass)
                {
                    var newObj = Activator.CreateInstance<T>();
                    foreach (var property in type.GetProperties())
                    {
                        if (property.CanRead)
                        {
                            var value = property.GetValue(obj);
                            property.SetValue(newObj, Copy(value));
                        }
                    }
                    return newObj;
                }

                throw new ArgumentException($"Type '{type.Name}' is not supported for deep copy.");
            }
        }

        #endregion XAMLMethodsHelper
    }
}
