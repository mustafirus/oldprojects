//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_SYSTEM                  0xFF
#define FACILITY_APPLICATION             0xFFF
#define FACILITY_FRAME                   0x1F1
#define FACILITY_DBENGINE                0x1F0


//
// Define the severity codes
//


//
// MessageId: MSG_PK_NOT_FOUND
//
// MessageText:
//
// Primary Key '%1' not found in dictionary.
//
#define MSG_PK_NOT_FOUND                 ((DWORD)0xC1F00001L)

//
// MessageId: MSG_ERROR_CREATE_THREAD
//
// MessageText:
//
// Create thread for '%1' fail.
//
#define MSG_ERROR_CREATE_THREAD          ((DWORD)0xC1F00002L)

//
// MessageId: MSG_ERROR_LOAD_DICT
//
// MessageText:
//
// Create thread for '%1' fail.
//
#define MSG_ERROR_LOAD_DICT              ((DWORD)0xC1F00003L)

//
// MessageId: MSG_ERROR_GET_TABLE
//
// MessageText:
//
// Table '%1' not found in dict.
//
#define MSG_ERROR_GET_TABLE              ((DWORD)0xC1F00004L)

//
// MessageId: MSG_FK_NOT_FOUND
//
// MessageText:
//
// Foreign Key '%1' not found in table %2.
//
#define MSG_FK_NOT_FOUND                 ((DWORD)0xC1F00005L)

//
// MessageId: MSG_ERROR_FORM_NOTFOUND
//
// MessageText:
//
// Form not found for table '%1' key %2.
//
#define MSG_ERROR_FORM_NOTFOUND          ((DWORD)0xC1F00006L)

//
// MessageId: MSG_ERROR_FRAMETYPE_NOTFOUND
//
// MessageText:
//
// Frame of type '%1' not found.
//
#define MSG_ERROR_FRAMETYPE_NOTFOUND     ((DWORD)0xC1F10007L)

//
// MessageId: MSG_ERROR_FRAME_SYNTAX_ERROR
//
// MessageText:
//
// Syntax error in form.
//
#define MSG_ERROR_FRAME_SYNTAX_ERROR     ((DWORD)0xC1F10008L)

//
// MessageId: MSG_ERROR_FILE_NOTFOUND
//
// MessageText:
//
// File '%1' not found.
//
#define MSG_ERROR_FILE_NOTFOUND          ((DWORD)0xCFFF0009L)

