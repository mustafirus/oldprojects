#ifndef TABLEINFO
typedef struct COLINFO_tag
{
	int			len;
	int			offset;
	int			type;
	int			intr;
	char		name[32];
}COLINFO, *PCOLINFO;
typedef struct TABLEINFO_tag
{
	int			cols;
	int			size;
	COLINFO*	pColInfo;	
}TABLEINFO, *PTABLEINFO;
#endif //TABLEINFO
