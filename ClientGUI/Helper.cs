﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ClientGUI
{
    internal class Helper
    {
        #region protocolHelper

        public static readonly ushort BYTES_RESERVED_FOR_CODE = 1;
        public static readonly ushort BYTES_RESERVED_FOR_MSG_LEN = 4;

        // Also in Login window xaml
        public static readonly ushort MAX_PASSWORD_LENGTH = 8;
        public static readonly ushort MAX_USERNAME_LENGTH = 16;

        public enum MessageType
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
        public static string Serialize(string content, MessageType code)
        {
            // Example for a login request (without spaces of course): 0 0038 {"username":"Mattan","password":"Gil"}
            string serializedCode = ((int)code).ToString().PadLeft(BYTES_RESERVED_FOR_CODE, '0');
            string serializedLen = content.Length.ToString().PadLeft(BYTES_RESERVED_FOR_MSG_LEN, '0');
            return serializedCode + serializedLen + content; 
        }

        #endregion protocolHelper


        #region XAMLMethodsHelper

            

        #endregion XAMLMethodsHelper
    }
}
