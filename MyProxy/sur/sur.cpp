// sur.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
void log(char* str){
	FILE* f = fopen("C:\\MyProxy.log","a");
	fputs(str, f);
	fputs("\n",f);
	fclose(f);
}
BOOL APIENTRY ServiceRights( char* account )
{
	log(account);
	NTSTATUS nts;
	
	LSA_OBJECT_ATTRIBUTES ObjectAttributes;
	memset(&ObjectAttributes, 0, sizeof(ObjectAttributes));
	LSA_HANDLE PolicyHandle;

	nts = LsaOpenPolicy( NULL,
		&ObjectAttributes, POLICY_LOOKUP_NAMES|POLICY_CREATE_ACCOUNT, &PolicyHandle );

	ULONG CountOfRights = 0;


	DWORD cbSid = 1024;
	PSID AccountSid = malloc(cbSid);
	DWORD cbDomainName = 1024;
	LPTSTR DomainName = (LPTSTR) malloc(cbDomainName);
	SID_NAME_USE eUse;

	BOOL ret = LookupAccountName( NULL, account, //HOME\\installer
		AccountSid, &cbSid, DomainName, &cbDomainName, &eUse );
	if(ret){
		log("Account found");
	}else{
		log("Account not found");
	}
	if(ret){
		if(!strchr(account, '\\')){
			char s1[1024];
			sprintf(s1,"%s\\%s", DomainName, account);
			strcpy(account, s1);
			log(account);
		}
		WCHAR  Buffer1[64] = L"SeServiceLogonRight";
		LSA_UNICODE_STRING UserRights[1] = {wcslen(Buffer1)*2, 64, Buffer1};
		nts = LsaAddAccountRights( PolicyHandle, AccountSid, UserRights, 1 );
		if(nts != 0)
			ret = FALSE;
		if(ret){
			log("Rights added");
		}else{
			log("Rights not added");
		}
	}

	free(DomainName);
	free(AccountSid);
	LsaClose(PolicyHandle);
	return ret;
}