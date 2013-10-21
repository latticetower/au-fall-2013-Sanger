// ParsingOfABIFormat.cpp: определяет точку входа для консольного приложения.
// basic specification url:
// http://projects.nfstc.org/workshops/resources/articles/ABIF_File_Format.pdf
//
#include <iostream>
#include <fstream>

struct DirectoryEntry
{
	int tagName;
	int tagNumber;
	short elementType;
	short elementSize;
	int numOfElements;//when in header section, we must read this - at byte 18
	int dataSize;
	int dataOffset;//when in header section, we must read this - at byte 26
	int dataHandle;
};

void print(std::ostream& outputStream, DirectoryEntry& dirEntry)
{
  outputStream << "DirectoryEntry data"   << std::endl 
    << "tagName:       " << dirEntry.tagName       << std::endl 
    << "tagNumber:     " << dirEntry.tagNumber     << std::endl 
    << "elementType:   " << dirEntry.elementType   << std::endl 
    << "elementSize:   " << dirEntry.elementSize   << std::endl 
    << "numOfElements: " << dirEntry.numOfElements << std::endl 
    << "dataSize:      " << dirEntry.dataSize      << std::endl 
    << "dataOffset:    " << dirEntry.dataOffset    << std::endl 
    << "dataHandle:    " << dirEntry.dataHandle    << std::endl;
}

int main()
{
	DirectoryEntry header;
	std::ifstream file("input.ab1", std::ios_base::binary);	
  // TODO: should change file and location to support reading file as a cmd parameter
	char file_format[5];
	short file_version;
  
  file.read(file_format, 4);
  file_format[4] = '\0';

	if (strcmp(file_format, "ABIF") == 0)
	{
		std::cout << "File has wrong format!";
		return 1;
	}
  file.read((char*) &file_version, 2);// we needn't read this, actually
  file.seekg(6, std::ios::beg);//DirectoryEntry in header section begins at this offset
  file.read((char*) &header, sizeof(header));

  std::cout << "Got data offset: " << header.dataOffset << std::endl;
  //DirectoryEntry is followed by 47 2-byte integers - according to specification, 
  //they are reserved, we should ignore them.
  //then go to the header.dataOffset position:
  DirectoryEntry* dirData = new DirectoryEntry[header.numOfElements];
  file.seekg(header.dataOffset);
  //read data
  file.read((char*) dirData, header.numOfElements * sizeof(DirectoryEntry));

  delete[] dirData;//clear memory before exit	
	system("pause");
	return 0;
}