/*
 * SPLPv1.c
 * The file is part of practical task for System programming course.
 * This file contains validation of SPLPv1 protocol.
 */


/*
  Golubovich Margot
  group 13
*/


/*
---------------------------------------------------------------------------------------------------------------------------
# |      STATE      |         DESCRIPTION       |           ALLOWED MESSAGES            | NEW STATE | EXAMPLE
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
1 | INIT            | initial state             | A->B     CONNECT                      |     2     |
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
2 | CONNECTING      | client is waiting for con-| A<-B     CONNECT_OK                   |     3     |
  |                 | nection approval from srv |                                       |           |
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
3 | CONNECTED       | Connection is established | A->B     GET_VER                      |     4     |
  |                 |                           |        -------------------------------+-----------+----------------------
  |                 |                           |          One of the following:        |     5     |
  |                 |                           |          - GET_DATA                   |           |
  |                 |                           |          - GET_FILE                   |           |
  |                 |                           |          - GET_COMMAND                |           |
  |                 |                           |        -------------------------------+-----------+----------------------
  |                 |                           |          GET_B64                      |     6     |
  |                 |                           |        ------------------------------------------------------------------
  |                 |                           |          DISCONNECT                   |     7     |
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
4 | WAITING_VER     | Client is waiting for     | A<-B     VERSION ver                  |     3     | VERSION 2
  |                 | server to provide version |          Where ver is an integer (>0) |           |
  |                 | information               |          value. Only a single space   |           |
  |                 |                           |          is allowed in the message    |           |
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
5 | WAITING_DATA    | Client is waiting for a   | A<-B     CMD data CMD                 |     3     | GET_DATA a GET_DATA
  |                 | response from server      |                                       |           |
  |                 |                           |          CMD - command sent by the    |           |
  |                 |                           |           client in previous message  |           |
  |                 |                           |          data - string which contains |           |
  |                 |                           |           the following allowed cha-  |           |
  |                 |                           |           racters: small latin letter,|           |
  |                 |                           |           digits and '.'              |           |
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
6 | WAITING_B64_DATA| Client is waiting for a   | A<-B     B64: data                    |     3     | B64: SGVsbG8=
  |                 | response from server.     |          where data is a base64 string|           |
  |                 |                           |          only 1 space is allowed      |           |
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
7 | DISCONNECTING   | Client is waiting for     | A<-B     DISCONNECT_OK                |     1     |
  |                 | server to close the       |                                       |           |
  |                 | connection                |                                       |           |
---------------------------------------------------------------------------------------------------------------------------

IN CASE OF INVALID MESSAGE THE STATE SHOULD BE RESET TO 1 (INIT)

*/


#include <string.h>
#include "splpv1.h"

/* FUNCTION:  validate_message
 *
 * PURPOSE:
 *    This function is called for each SPLPv1 message between client
 *    and server
 *
 * PARAMETERS:
 *    msg - pointer to a structure which stores information about
 *    message
 *
 * RETURN VALUE:
 *    MESSAGE_VALID if the message is correct
 *    MESSAGE_INVALID if the message is incorrect or out of protocol
 *    state
 */
const char base64[128] =
    {0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 0, 0, 0, 1,
     1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 0, 0, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 0, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 0, 0, 0, 0, 0};

const char data[128] =
    {0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 0,
     1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 0, 0, 0, 0, 0};

int current_state = 1;
const char *commands[3] = {"GET_DATA", "GET_FILE", "GET_COMMAND"};
int previous_command;

enum test_status validate_message(struct Message *msg) {
  char *message = msg->text_message;
  int length = strlen(message); // not to calculate too many times
  int length_prev_command = strlen(commands[previous_command]);

  if (msg->direction == A_TO_B) { // simple checks on input and thats enough
    if (current_state == 1 && !strcmp(message, "CONNECT")) {
      current_state = 2;
      return MESSAGE_VALID;
    }

    if (current_state == 3) {
      if (!strcmp(message, "GET_VER")) {
        current_state = 4;
        return MESSAGE_VALID;
      } else if (!strcmp(message, "GET_DATA")) {
        previous_command = 0;
        current_state = 5;
        return MESSAGE_VALID;
      } else if (!strcmp(message, "GET_FILE")) {
        previous_command = 1;
        current_state = 5;
        return MESSAGE_VALID;
      } else if (!strcmp(message, "GET_COMMAND")) {
        previous_command = 2;
        current_state = 5;
        return MESSAGE_VALID;
      } else if (!strcmp(message, "GET_B64")) {
        current_state = 6;
        return MESSAGE_VALID;
      } else if (!strcmp(message, "DISCONNECT")) {
        current_state = 7;
        return MESSAGE_VALID;
      }
    }
  } else if (msg->direction == B_TO_A) {
    if (current_state == 2 && !strcmp(message, "CONNECT_OK")) {  // simple checks on input and thats enough
      current_state = 3;
      return MESSAGE_VALID;
    } else if (current_state == 4 && !strncmp(message, "VERSION ", 8)) { // simple checks on input + integer after VERSION
      for (int i = 8; i < length; ++i) {
        if (message[i] < '0' || message[i] > '9') {
          current_state = 1;
          return MESSAGE_INVALID;
        }
      }
      current_state = 3;
      return MESSAGE_VALID;
    } else if (current_state == 5 && !strncmp(message, commands[previous_command], length_prev_command) && message[length_prev_command] == ' ') { //some commands from state 3 (using their length) + 1 space
/*      if (message[length_prev_command + 1] == ' ' && strncmp(message + length_prev_command + 2, commands[previous_command], length_prev_command) != 0) { // have 2+ spaces and not the situation GET *2 spaces* GET
        current_state = 1;
        return MESSAGE_INVALID;
      }*/

      for (message += length_prev_command + 1; *message != ' '; ++message) { // check data before the last space (before last GET) + avoid situation GET      data GET (space not in data)
        if (!data[*message]) {
          current_state = 1;
          return MESSAGE_INVALID;
        }
      }

      if (strncmp(message + 1, commands[previous_command], length_prev_command) != 0) { //starts after data to check GET at the end
        current_state = 1;
        return MESSAGE_INVALID;
      }

      current_state = 3;
      return MESSAGE_VALID;
    } else if (current_state == 6 && !strncmp(message, "B64: ", 5)) { //////////////////
      message += 5;
      char *begin = message;
      for (; base64[*message]; ++message);
      int check = 0;
      for (; (check < 2) && (message[check] == '='); ++check);
      if ((message - begin + check) % 4 == 0 && !message[check]) {
        current_state = 3;
        return MESSAGE_VALID;
      }

/*      for (; (message - begin) < (length - 2); ++message) {
        if (!base64[*message]) {
          current_state = 1;
          return MESSAGE_INVALID;
        }
      }

      if ((message[length - 2] == '=' && message[length - 1] == '=') ||
          (message[length - 1] == '=' && base64[message[length - 2]]) ||
          (base64[message[length - 1]] && base64[message[length - 2]]) ||
          (message[length - 2] == '+' && message[length - 1] == '+') ||
          (message[length - 1] == '+' && base64[message[length - 2]])) {
        current_state = 3;
        return MESSAGE_VALID;
      }*/

    } else if (current_state == 7 && !strcmp(message, "DISCONNECT_OK")) { // simple checks on input and thats enough
      current_state = 1;
      return MESSAGE_VALID;
    }
  }
  current_state = 1;
  return MESSAGE_INVALID;
}