// ParsingOfABIFormat.cpp: ���������� ����� ����� ��� ����������� ����������.
// basic specification url:
// http://projects.nfstc.org/workshops/resources/articles/ABIF_File_Format.pdf
//
#include <iostream>
#include <fstream>
#include <stdint.h>

//TODO: there is still some error with struct reading, so must fix, I hope this article would help:
//http://en.wikipedia.org/wiki/Data_structure_alignment
// let's try to read manually again, hope that help
#pragma pack(push)
#pragma pack(1)
struct DirectoryEntry
{
	uint32_t tagName;
	uint32_t tagNumber;
	uint16_t elementType;
	uint16_t elementSize;
	uint32_t numOfElements;//when in header section, we must read this - at byte 18
	uint32_t dataSize;
	uint32_t dataOffset;//when in header section, we must read this - at byte 26
	uint32_t dataHandle;
};
#pragma pack(pop)

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
void readData(std::istream & inputStream, DirectoryEntry& dirEntry)
{
  inputStream.read((char*) &dirEntry.tagName, sizeof(uint32_t));
  inputStream.read((char*) &dirEntry.tagNumber, sizeof(uint32_t));
  inputStream.read((char*) &dirEntry.elementType, sizeof(uint16_t));
  inputStream.read((char*) &dirEntry.elementSize, sizeof(uint16_t));
  inputStream.seekg(18, std::ios::beg);
  inputStream.read((char*) &dirEntry.numOfElements, sizeof(uint32_t));
  inputStream.read((char*) &dirEntry.dataSize, sizeof(uint32_t));
  inputStream.seekg(26, std::ios::beg);
  inputStream.read((char*) &dirEntry.dataOffset, sizeof(uint32_t));
  inputStream.read((char*) &dirEntry.dataHandle, sizeof(uint32_t));
}

int main()
{
	DirectoryEntry header;
	std::ifstream file("input.ab1", std::ios_base::binary);	
  if (!file.is_open())
  {
    std::cout << "File couldn't be opened, for the reasons unknown...";
    return 1;
  }

  // TODO: should change file and location to support reading file as a cmd parameter
	char file_format[5];
	short file_version;
  
  file.read(file_format, 4);
  file_format[4] = '\0';

	if (strcmp(file_format, "ABIF") != 0)
	{
		std::cout << "File has wrong format! " << file_format << std::endl;
    system("pause");
		return 2;
	}
  file.read((char*) &file_version, 2);// we needn't read this, actually
  std::cout << file_version << std::endl;
  file.seekg(6, std::ios::beg);//DirectoryEntry in header section begins at this offset
  readData(file, header);//FIX: this way of reading doesn't help either.
  //TODO: read specification, look at file_version?

  print(std::cout, header);

  std::cout << "Got data offset: " << header.dataOffset << std::endl;
  //DirectoryEntry is followed by 47 2-byte integers - according to specification, 
  //they are reserved, we should ignore them.
  //then go to the header.dataOffset position:
  DirectoryEntry* dirData = new DirectoryEntry[header.numOfElements];
  file.seekg(header.dataOffset);
  //read data
  file.read((char*) dirData, header.numOfElements * sizeof(DirectoryEntry));
  if (file)
    std::cout << "all DirectoryEntries read successfully";
  else 
  {
    std::cout << "error: only " << file.gcount() << " could be read";
    delete[] dirData;//clear memory anyway
    file.close();//close file handle
    return 3;//and exit
  }
  //TODO: here we can print something
  file.close();
  delete[] dirData;//clear memory before exit	
	system("pause");
	return 0;
}