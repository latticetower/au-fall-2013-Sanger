// ParsingOfABIFormat.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
// basic specification url:
// http://projects.nfstc.org/workshops/resources/articles/ABIF_File_Format.pdf
//
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

//TODO: there is still some error with struct reading, so must fix, I hope this article would help:
//http://en.wikipedia.org/wiki/Data_structure_alignment
// let's try to read manually again, hope that help


template <class T>
void endian_swap(T *object)
{
  unsigned char* memoryPointer = reinterpret_cast<unsigned char*>(object);
  std::reverse(memoryPointer, memoryPointer + sizeof(T));
}

#pragma pack(push)
#pragma pack(1)
struct DirectoryEntry
{
        std::string tagName;
        int32_t tagNumber;
        int16_t elementType;
        int16_t elementSize;
        int32_t numOfElements;//when in header section, we must read this - at byte 18
        int32_t dataSize;
        int32_t dataOffset;//when in header section, we must read this - at byte 26
        int32_t dataHandle;
};
#pragma pack(pop)

struct DNASequence
{
		std::string sequence;
		std::vector<short> PeakPositions;
		std::vector<short> CYs;
		std::vector<short> GYs;
		std::vector<short> AYs;
		std::vector<short> TYs;
		std::string machineName;
		std::string sampleName;
		std::string sampleComment;
		std::string phredQuality;
		DNASequence(std::map<std::string, std::vector<DirectoryEntry*>> dirMap, std::ifstream *file)
		{
			sequence = GetInformationFromEntry<std::string>("PBAS", 2, dirMap, file); 
			sampleComment = GetInformationFromEntry<std::string>("CMNT", 1, dirMap, file); 
			GYs = GetInformationFromEntry<std::vector<short>>("DATA", 1, dirMap, file);
			AYs = GetInformationFromEntry<std::vector<short>>("DATA", 2, dirMap, file);
			TYs = GetInformationFromEntry<std::vector<short>>("DATA", 3, dirMap, file);
			CYs = GetInformationFromEntry<std::vector<short>>("DATA", 4, dirMap, file);
			PeakPositions = GetInformationFromEntry<std::vector<short>>("PLOC", 1, dirMap, file);
			phredQuality = GetInformationFromEntry<std::string>("PCON", 1, dirMap, file); 
			machineName = GetInformationFromEntry<std::string>("MCHN", 1, dirMap, file);
			sampleName = GetInformationFromEntry<std::string>("MCHN", 1, dirMap, file);
		}

		template<class T>
		T GetInformationFromEntry(std::string tag, unsigned int number, std::map<std::string, std::vector<DirectoryEntry*>> dirMap, std::ifstream *file)
		{
			std::vector<DirectoryEntry*>::iterator it = begin(dirMap[tag]);
			for(it = begin(dirMap[tag]); it != end(dirMap[tag]); ++it)
			{
				if((*it)->tagNumber == number)
					break;
			}
			T result;
			if(it != end(dirMap[tag]))
			{
				file->seekg((*it)->dataOffset);
				char tag[2000];
				file->read(reinterpret_cast<char*>(tag), (*it)->elementSize*(*it)->numOfElements);
				result = T(tag, (*it)->elementSize*(*it)->numOfElements);
				return result;
			}
			std::cout << "Can't return " << tag << " with parametr " << number << std::endl;
			return result;
		}

		template<>
		std::vector<short> GetInformationFromEntry(std::string tag, unsigned int number, std::map<std::string, std::vector<DirectoryEntry*>> dirMap, std::ifstream *file)
		{
			std::vector<DirectoryEntry*>::iterator it = begin(dirMap[tag]);
			for(it = begin(dirMap[tag]); it != end(dirMap[tag]); ++it)
			{
				if((*it)->tagNumber == number)
					break;
			}
			std::vector<short> result;
			if(it != end(dirMap[tag]))
			{
				file->seekg((*it)->dataOffset);
				short tempShort;
				for(int j = 0; j < (*it)->numOfElements; ++j)
                {
					file->read(reinterpret_cast<char*>(&tempShort), 2);
					endian_swap(&tempShort);
					result.push_back(tempShort);
                }				
				return result;
			}
			std::cout << "Can't return " << tag << " with parametr " << number << std::endl;
			return result;
		}
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
void readData(std::istream & inputStream, DirectoryEntry* dirEntry)
{
  char tag[4];
  inputStream.read(reinterpret_cast<char*>(tag), 4);
  dirEntry->tagName = std::string(tag, 4);
  //std::reverse(&dirEntry->tagName[0], &dirEntry->tagName[4]);
  inputStream.read(reinterpret_cast<char*>(&dirEntry->tagNumber), 4);
  inputStream.read(reinterpret_cast<char*>(&dirEntry->elementType), 2);
  inputStream.read(reinterpret_cast<char*>(&dirEntry->elementSize), 2);
  inputStream.read(reinterpret_cast<char*>(&dirEntry->numOfElements), 4);
  inputStream.read(reinterpret_cast<char*>(&dirEntry->dataSize), 4);
  inputStream.read(reinterpret_cast<char*>(&dirEntry->dataOffset), 4);
  inputStream.read(reinterpret_cast<char*>(&dirEntry->dataHandle), 4);
  //endian_swap(&dirEntry->tagName);
  endian_swap(&dirEntry->tagNumber);
  endian_swap(&dirEntry->elementType);
  endian_swap(&dirEntry->elementSize);
  endian_swap(&dirEntry->numOfElements);
  endian_swap(&dirEntry->dataSize);
  endian_swap(&dirEntry->dataOffset);
  endian_swap(&dirEntry->dataHandle);
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
  file.read(reinterpret_cast<char*>(&file_version), 2);// we needn't read this, actually
  std::cout << file_version << std::endl;
  file.seekg(6, std::ios::beg);//DirectoryEntry in header section begins at this offset
  readData(file, &header);//FIX: this way of reading doesn't help either.
  //TODO: read specification, look at file_version?

  print(std::cout, header);

  std::cout << "Got data offset: " << header.dataOffset << std::endl;
  //DirectoryEntry is followed by 47 2-byte integers - according to specification, 
  //they are reserved, we should ignore them.
  //then go to the header.dataOffset position:
  
  DirectoryEntry* dirData = new DirectoryEntry[header.numOfElements];
  
  //Map, storing all directiry entries of file
  std::map<std::string, std::vector<DirectoryEntry*>> directoryMap;
  
  //TODO: should read dirData manually
  file.seekg(header.dataOffset);
  
  //read data
  for (int i = 0; i < header.numOfElements; i++)
  {
    readData(file, &dirData[i]);
	directoryMap[dirData[i].tagName].push_back(&dirData[i]);
  }

  //Creating struct, storing all nessescary fields
  DNASequence *DNA = new DNASequence(directoryMap, &file);

  if (file)
  {
    std::cout << "all DirectoryEntries read successfully" << std::endl;
  }
  else 
  {
    std::cout << "error: only " << file.gcount() << " could be read" << std::endl;
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