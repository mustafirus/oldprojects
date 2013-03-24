MessageIdTypedef = DWORD
FacilityNames=(
    System=0x0FF:FACILITY_SYSTEM
    DbEngine=0x1F0:FACILITY_DBENGINE
	Frame=0x1F1:FACILITY_FRAME
    MyApplication=0xFFF:FACILITY_APPLICATION
    )
LanguageNames=(Russian=0x19:ErrorCodes)

MessageId=0x1
Severity=Error
Facility=DbEngine
SymbolicName=MSG_PK_NOT_FOUND
Language=Russian
Primary Key '%1' not found in dictionary.
.

MessageId=0x2
Severity=Error
Facility=DbEngine
SymbolicName=MSG_ERROR_CREATE_THREAD
Language=Russian
Create thread for '%1' fail.
.

MessageId=0x3
Severity=Error
Facility=DbEngine
SymbolicName=MSG_ERROR_LOAD_DICT
Language=Russian
Create thread for '%1' fail.
.

MessageId=0x4
Severity=Error
Facility=DbEngine
SymbolicName=MSG_ERROR_GET_TABLE
Language=Russian
Table '%1' not found in dict.
.

MessageId=0x5
Severity=Error
Facility=DbEngine
SymbolicName=MSG_FK_NOT_FOUND
Language=Russian
Foreign Key '%1' not found in table %2.
.

MessageId=0x6
Severity=Error
Facility=DbEngine
SymbolicName=MSG_ERROR_FORM_NOTFOUND
Language=Russian
Form not found for table '%1' key %2.
.

MessageId=0x7
Severity=Error
Facility=Frame
SymbolicName=MSG_ERROR_FRAMETYPE_NOTFOUND
Language=Russian
Frame of type '%1' not found.
.

MessageId=0x8
Severity=Error
Facility=Frame
SymbolicName=MSG_ERROR_FRAME_SYNTAX_ERROR
Language=Russian
Syntax error in form.
.

MessageId=0x9
Severity=Error
Facility=MyApplication
SymbolicName=MSG_ERROR_FILE_NOTFOUND
Language=Russian
File '%1' not found.
.

