
void SetByte(DWORD offset,BYTE value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,1,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(BYTE*)(offset) = value;

	VirtualProtect((void*)offset,1,OldProtect,&OldProtect);
}

void SetByteUnsafe(DWORD offset,BYTE value) // OK
{
	*(BYTE*)(offset) = value;
}

void SetWord(DWORD offset,WORD value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,2,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(WORD*)(offset) = value;

	VirtualProtect((void*)offset,2,OldProtect,&OldProtect);
}

void SetDword(DWORD offset,DWORD value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,4,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(DWORD*)(offset) = value;

	VirtualProtect((void*)offset,4,OldProtect,&OldProtect);
}

void SetFloat(DWORD offset,float value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,4,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(float*)(offset) = value;

	VirtualProtect((void*)offset,4,OldProtect,&OldProtect);
}

void SetDouble(DWORD offset,double value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,8,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(double*)(offset) = value;

	VirtualProtect((void*)offset,8,OldProtect,&OldProtect);
}

void SetCompleteHook(BYTE head,DWORD offset,...) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,5,PAGE_EXECUTE_READWRITE,&OldProtect);

	if(head != 0xFF)
	{
		*(BYTE*)(offset) = head;
	}

	DWORD* function = &offset+1;

	*(DWORD*)(offset+1) = (*function)-(offset+5);

	VirtualProtect((void*)offset,5,OldProtect,&OldProtect);
}

void SetCompleteHookUnsafe(BYTE head,DWORD offset,...) // OK
{
	if(head != 0xFF)
	{
		*(BYTE*)(offset) = head;
	}

	DWORD* function = &offset+1;

	*(DWORD*)(offset+1) = (*function)-(offset+5);
}

void MemoryCpy(DWORD offset,void* value,DWORD size) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,size,PAGE_EXECUTE_READWRITE,&OldProtect);

	memcpy((void*)offset,value,size);

	VirtualProtect((void*)offset,size,OldProtect,&OldProtect);
}

void MemorySet(DWORD offset,DWORD value,DWORD size) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,size,PAGE_EXECUTE_READWRITE,&OldProtect);

	memset((void*)offset,value,size);

	VirtualProtect((void*)offset,size,OldProtect,&OldProtect);
}

LPVOID HookFunction(const LPVOID offset, LPVOID function, DWORD size)
{
	LPVOID newf = malloc(size + 6);

	DWORD OldProtect;

	VirtualProtect(offset, size, PAGE_EXECUTE_READWRITE, &OldProtect);

	memcpy(newf, offset, size);

	PBYTE j = (PBYTE)((DWORD)newf + size);

	j[0] = 0x68;

	*(DWORD*)&j[1] = (DWORD)((DWORD)offset + size);

	j[5] = 0xC3;

	j = (PBYTE)offset;

	j[0] = 0x68;

	*(DWORD*)&j[1] = (DWORD)(DWORD)function;

	j[5] = 0xC3;

	VirtualProtect(offset, size, OldProtect, &OldProtect);

	return newf;
}

void PacketArgumentEncrypt(char* out_buff,char* in_buff,int size) // OK
{
	BYTE XorTable[3] = {0xFC,0xCF,0xAB};

	for(int n=0;n < size;n++)
	{
		out_buff[n] = in_buff[n]^XorTable[n%3];
	}
}

void ReplaceString(std::string &src, std::string const& search, std::string const& replace)
{
	size_t pos = -1;

	while ( true )
	{
		pos = FindString(src, search);

		if ( pos == size_t(-1) )
			break;

		src.replace(pos, search.length(), replace);
	}
}

void CreateParentDir(fs::path pPath)
{
	fs::path pParent = fs::_Parse_parent_path(pPath.wstring());
	fs::create_directories(pParent);
}
