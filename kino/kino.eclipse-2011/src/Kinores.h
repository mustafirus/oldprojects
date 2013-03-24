#define ID_KINO_FIRST					2000

#define ID_FILE_FIRST					ID_KINO_FIRST
#define ID_FILE_EXPORT					ID_FILE_FIRST
#define ID_FILE_PRINT_SETUP 				(ID_FILE_FIRST + 1)
#define ID_FILE_PRINT					(ID_FILE_FIRST + 2)
#define ID_FILE_EXIT					(ID_FILE_FIRST + 3)

#define ID_EDIT_FIRST					(ID_KINO_FIRST + 20)
#define ID_EDIT_CUT 					(ID_EDIT_FIRST)
#define ID_EDIT_COPY					(ID_EDIT_FIRST + 1)
#define ID_EDIT_PASTE					(ID_EDIT_FIRST + 2)
#define ID_EDIT_CLEAR					(ID_EDIT_FIRST + 3)
#define ID_EDIT_UNDO					(ID_EDIT_FIRST + 4)
#define ID_EDIT_SELECT_ALL				(ID_EDIT_FIRST + 5)
#define ID_EDIT_LAST					(ID_EDIT_SELECT_ALL)

#define ID_RECORD_FIRST					(ID_KINO_FIRST + 40)
#define ID_RECORD_INSERT				(ID_RECORD_FIRST)
#define ID_RECORD_DELETE				(ID_RECORD_FIRST + 1)
#define ID_RECORD_SAVE					(ID_RECORD_FIRST + 2)
#define ID_RECORD_UNDO					(ID_RECORD_FIRST + 3)
#define ID_RECORD_REFRESH				(ID_RECORD_FIRST + 4)
#define ID_RECORD_SORT_ASC             	(ID_RECORD_FIRST + 5)
#define ID_RECORD_SORT_DSC              (ID_RECORD_FIRST + 6)
#define ID_RECORD_SORT_NONE             (ID_RECORD_FIRST + 7)
#define ID_RECORD_LAST					(ID_RECORD_SORT_NONE)

#define ID_CAT_FIRST					(ID_FILE_FIRST + 100)
#define ID_CAT_DOG						(ID_CAT_FIRST)
#define ID_CAT_BABY 					(ID_CAT_FIRST + 1)
#define ID_CAT_FUCKING					(ID_CAT_FIRST + 2)
#define ID_CAT_GOFUCK					(ID_CAT_FIRST + 3)
#define ID_CAT_SHOW 					(ID_CAT_FIRST + 4)
#define ID_CAT_OWNER					(ID_CAT_FIRST + 5)
#define ID_CAT_STREET					(ID_CAT_FIRST + 6)
#define ID_CAT_CITY 					(ID_CAT_FIRST + 7)
#define ID_CAT_COLOR					(ID_CAT_FIRST + 8)
#define ID_CAT_BREED					(ID_CAT_FIRST + 9)
#define ID_CAT_REFERY					(ID_CAT_FIRST + 10)
#define ID_CAT_CLUB 					(ID_CAT_FIRST + 11)
#define ID_CAT_FACTORY					(ID_CAT_FIRST + 12)
#define ID_CAT_FORMS                    		(ID_CAT_FIRST + 13)
#define ID_CAT_LAST 					(ID_CAT_FORMS)

#define	ACT_FLAG_THREAD					0x80000000
#define	ACT_FLAG_BLANK					0x40000000
#define	ID_FORM_FIRST					1000
#define	ID_FORM_DEFAULT					(ID_FORM_FIRST)
#define	ID_FORM_CARD					(ID_FORM_FIRST + 1)
#define	ID_FORM_CARD_THR				(ID_FORM_CARD|ACT_THREAD_FLAG)
#define	ID_FORM_LIST					(ID_FORM_FIRST + 2)
#define	ID_FORM_LAST					1099
#define	ID_PROC_FIRST					1100
#define	ID_PROC_LAST					1199

#define ID_DOG_TITUL                    		(ID_FORM_FIRST + 3)
#define ID_PEDIGREE                     		(ID_FORM_FIRST + 4)
#define ID_BABYADD                      		(ID_PROC_FIRST + 1)
#define ID_BABY_UPGRADE 				(ID_PROC_FIRST + 1)

#define ID_MISC							4000
#define ID_SHOWALL						(ID_MISC +3)
#define ID_SHOWSYS						(ID_MISC +4)
#define ID_WAIT_MESSAGE		 		        (ID_MISC +5)
#define ID_READY_MESSAGE 			        (ID_MISC +6)
