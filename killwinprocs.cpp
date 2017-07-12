/*
MIT License

Copyright (c) [2017] [Scott Velasquez]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "stdafx.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

using namespace std;

void KillProcByName( wstring ProcessName )
{
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Snapshot == INVALID_HANDLE_VALUE )
	{
		cout << "\t CreateToolhelp32Snapshot failed, bailing!\n\n";
	}

	PROCESSENTRY32 ProcessEntry;
	ZeroMemory(&ProcessEntry, sizeof(PROCESSENTRY32));
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);

	if ( !Process32First(Snapshot, &ProcessEntry) )
	{
		CloseHandle(Snapshot);
		cout << "\t Process32First failed, bailing!\n\n";
		return;
	}

	do 
	{
		if ( !wcscmp(ProcessEntry.szExeFile, ProcessName.data()) )
		{    
			cout << "\t Process found!  Killing process ID: " << ProcessEntry.th32ProcessID << "\n";
			HANDLE Process = OpenProcess(PROCESS_TERMINATE, 0, ProcessEntry.th32ProcessID);
			TerminateProcess(Process, 0);   
			CloseHandle(Process);  
		}
	} while ( Process32Next(Snapshot, &ProcessEntry) );

	CloseHandle(Snapshot);  
}

int main( int argc, char* argv[] )
{
	if ( argc < 2 )
	{
		cout << "Usage: killwinprocs processName1 processName2 etc.. \n";
		return 0;
	}
	
	for (int i=1; i<argc; ++i)
	{
		size_t ProcessLength = strlen(argv[i]) + 1;
		wstring ProcessNameString(ProcessLength, L'#');

		size_t LengthWritten;
		mbstowcs_s(&LengthWritten, &ProcessNameString[0], ProcessLength, argv[i], ProcessLength-1);
		cout << "Attempting to kill " << argv[i] << "\n";
		KillProcByName(ProcessNameString);
	}

	return 0;
}

