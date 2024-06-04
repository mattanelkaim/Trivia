using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows;

namespace ClientGUI
{
    internal class Helper
    {
        #region protocolHelper

#if true // to debug
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
            Error,
            OK,            
            LOGIN_FAILED,            
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

        public static string SendMessage(object structTosend, RequestType code)
        {
            string json = JsonSerializer.Serialize(structTosend);

            string msg = Helper.Serialize(json, code);
            Helper._DEBUG_SHOW("[Sending]: " + msg);
            Communicator.Send(msg);

            string responseBuffer = Communicator.Receive();
            Helper._DEBUG_SHOW("[Received]: " + responseBuffer);

            return responseBuffer;
        }

        #endregion protocolHelper


        #region specificRequests

        public struct Response
        {
            public ResponseType code;
            public string content;
        }

        // Just a helper method to deserialize the response
        public class ResponseWithStatus
        {
            public int status { get; set; } // MUST HAVE A GETTER AND SETTER ELSE THIS SHITTY DESIRIALIZER WON'T WORK
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
            string rawResponse = SendMessage(new { username = Username, password = Password }, RequestType.Login);
            
            Response response = ExtractResponse(rawResponse);

            if (response.code != ResponseType.OK)
            {
                throw new Exception(); // TODO: Throw a more specific exception
            }

            // Ideally expects {"status":1}
            return (ResponseType)JsonSerializer.Deserialize<ResponseWithStatus>(response.content).status;
        }

        public static ResponseType SendSignupRequest(string Username, string Password, string Email)
        {
            string rawResponse = SendMessage(new { username = Username, password = Password, email = Email }, RequestType.Register);

            Response response = ExtractResponse(rawResponse);

            if (response.code != ResponseType.OK)
            {
                throw new Exception(); // TODO: Throw a more specific exception
            }

            // Ideally expects {"status":1}
            return (ResponseType)JsonSerializer.Deserialize<ResponseWithStatus>(response.content).status;
        }

        #endregion specificRequests

        #region XAMLMethodsHelper

        // Also in Login window xaml
        public static readonly ushort MAX_PASSWORD_LENGTH = 8;
        public static readonly ushort MAX_USERNAME_LENGTH = 16;


        #endregion XAMLMethodsHelper
    }
}
