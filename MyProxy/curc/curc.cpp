// curc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main(int argc, char* argv[])
{
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

	BOOL ret = LookupAccountName( NULL, "HOME\\installer", //HOME\\installer
		AccountSid, &cbSid, DomainName, &cbDomainName, &eUse );
	if(ret){
		WCHAR  Buffer1[64] = L"SeServiceLogonRight";
		LSA_UNICODE_STRING UserRights[1] = {wcslen(Buffer1)*2, 64, Buffer1};
		nts = LsaAddAccountRights( PolicyHandle, AccountSid, UserRights, 1 );
	}

	free(DomainName);
	free(AccountSid);
	LsaClose(PolicyHandle);

	return 0;
}
