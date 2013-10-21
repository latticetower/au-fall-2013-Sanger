// ParsingOfABIFormat.cpp: определяет точку входа для консольного приложения.
//basic specification url:
//http://projects.nfstc.org/workshops/resources/articles/ABIF_File_Format.pdf
//
#include <iostream>
#include <fstream>

struct DirectoryEntry
{
	int tagName;
	int tagNumber;
	short elementType;
	short elementSize;
	int numOfElements;
	int dataSize;
	int dataOffset;
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

void readHeader(std::istream& inputStream, DirectoryEntry *mydir)
{
  inputStream.read((char*) &mydir->tagName, 4);
	inputStream.read((char*) &mydir->tagNumber, 4);
	inputStream.read((char*) &mydir->elementType, 2);
  inputStream.read((char*) &mydir->elementSize, 2);
	inputStream.read((char*) &mydir->numOfElements, 4);//must read this - at byte 18
	inputStream.read((char*) &mydir->dataSize, 4);
	inputStream.read((char*) &mydir->dataOffset, 4);//must read this - at byte 26
  inputStream.read((char*) &mydir->dataHandle, 4);
}


int main()
{
	DirectoryEntry header;
	std::ifstream file("input.ab1", std::ios_base::binary);	
	unsigned char temp = '\0';
	char file_format[5];
	char file_version[2];
  
  file.read(file_format, 4);
  file_format[4] = '\0';

	if (strcmp(file_format, "ABIF") == 0)
	{
		std::cout << "File has wrong format!";
		return 1;
	}
  file.read(file_version, 2);

  readHeader(file, &header);
  //DirectoryEntry is followed by 47 2-byte integers - according to specification, 
  //they are reserved, we should ignore them:
  file.seekg(47*2, std::ios_base::cur);
  file.seekg(header.dataOffset);
  
		
	system("pause");
	return 0;
}