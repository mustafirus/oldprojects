#pragma once

namespace guidframe {

	const char* CF = "{97A14A9B-DF0F-4eed-928B-0C3E9911D8CC}";
	const char* VF = "{873C898C-5A0B-4114-9F19-4F8163002FDE}";
	const char* HF = "{64724214-F3DF-4639-89C8-5247B0B7C01E}";
	const char* FF = "{92F6DB28-CE31-4c82-82D2-2AFE8BD598FD}";
	const char* LF = "{8E3604F6-0344-494b-9759-160E02A75089}";
	const char* EF = "{DC3E3A0B-BAC8-4b09-B0DA-763A4E2D6721}";
#if 0
	const char* vec7 = "{AC8F173A-06E4-4141-A82F-09B0715CAE7E}";
	const char* vec8 = "{AC2FB9D9-E61A-460f-98AD-CFB335C71C14}";
	const char* vec9 = "{6B9F92EA-8426-44c9-B2D9-E1890816EB08}";
	const char* vec0 = "{0D819B5E-664B-4855-A9B5-E7FCD4D7B29D}";
#endif // 0

	struct TypeMap{
		const char* guid;
		char		cid;
	};
	TypeMap type_map[] = {
		{ CF, 0 },
		{ VF, 'V' },
		{ HF, 'H' },
		{ FF, 'F' },
		{ LF, 'L' },
		{ NULL, 0 }
	};
};

