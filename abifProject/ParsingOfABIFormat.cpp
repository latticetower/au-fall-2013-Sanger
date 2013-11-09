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
		std::string name;
		std::string qualitySequence;
		std::string DataString[4];
		std::vector<short> PeakPositions;
		std::vector<short> CYs;
		std::vector<short> GYs;
		std::vector<short> AYs;
		std::vector<short> TYs;
		std::string machineName;
		std::string sampleName;
		std::string sampleComment;
		std::string phredQuality;
		std::string averagePakEntries;
		int32_t capillarNumber;
	//	std::string peakSpacing;
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
  //Vector, storing pointers to entries, storing DNA Sequences
  std::vector<DirectoryEntry*> sequenceEntries;
  //Vector, storing pointers to entries, storing signal entries
  std::vector<DirectoryEntry*> signalEntries;
  
  std::vector<DirectoryEntry*> dataEntries;
  std::vector<DirectoryEntry*> peakEntries;
  std::vector<DirectoryEntry*> machineNameEntries;
  std::vector<DirectoryEntry*> sampleNameEntries;
  std::vector<DirectoryEntry*> sampleCommentNameEntries;
  std::vector<DirectoryEntry*> capillarEntries;
  std::vector<DirectoryEntry*> qualityEntries;
  std::vector<DirectoryEntry*> averagePeakEntries;
  ///std::vector<DirectoryEntry*> peakSpacingEntries;
  //TODO: should read dirData manually
  file.seekg(header.dataOffset);
  //read data
  //file.read(reinterpret_cast<char*>(dirData), header.numOfElements * sizeof(DirectoryEntry));
  for (int i = 0; i < header.numOfElements; i++)
  {
    readData(file, &dirData[i]);
	std::string a = dirData[i].tagName;

	//Taking DNA strings from ABIFFile
	if(dirData[i].tagName == "PBAS" && dirData[i].tagNumber == 2)
	{
		sequenceEntries.push_back(&dirData[i]);
		print(std::cout, dirData[i]);
	}

	if(dirData[i].tagName == "SPAC" && dirData[i].tagNumber == 1)
	{
		averagePeakEntries.push_back(&dirData[i]);
		print(std::cout, dirData[i]);
	}


	if(dirData[i].tagName == "S/N%")
	{
		signalEntries.push_back(&dirData[i]);
		print(std::cout, dirData[i]);
	}

	if(dirData[i].tagName == "DATA")
	{
		dataEntries.push_back(&dirData[i]);
		print(std::cout, dirData[i]);
	}
	if(dirData[i].tagName == "PLOC")
	{
		peakEntries.push_back(&dirData[i]);
		print(std::cout, dirData[i]);
	}
	if(dirData[i].tagName == "MCHN")
	{
		machineNameEntries.push_back(&dirData[i]);
		print(std::cout, dirData[i]);
	}
	
	if(dirData[i].tagName == "SMPL")
	{
		sampleNameEntries.push_back(&dirData[i]);
		print(std::cout, dirData[i]);
	}

	if(dirData[i].tagName == "CMNT")
	{
		sampleCommentNameEntries.push_back(&dirData[i]);
		print(std::cout, dirData[i]);
	}

	if(dirData[i].tagName == "LANE")
	{
		capillarEntries.push_back(&dirData[i]);
		print(std::cout, dirData[i]);
	}

	if(dirData[i].tagName == "PCON")
	{
		qualityEntries.push_back(&dirData[i]);
		print(std::cout, dirData[i]);
	}
	//if(dirData[i].tagName == "SPAC" && dirData[i].tagNumber == 1)
	//{
	//	peakSpacingEntries.push_back(&dirData[i]);
	//	print(std::cout, dirData[i]);
	//}
  }


  //Taking DNA sequence from directory
  DNASequence DNA;
  for(int i = 0; i < sequenceEntries.size(); ++i)
  {

	  file.seekg(sequenceEntries[i]->dataOffset);
	  char tag[2000];
	  file.read(reinterpret_cast<char*>(tag), sequenceEntries[i]->elementSize*sequenceEntries[i]->numOfElements);
	  std::string DNAstring = std::string(tag, sequenceEntries[i]->elementSize*sequenceEntries[i]->numOfElements);
	  DNA.name = DNAstring;
  }	

  
  for(int i = 0; i < dataEntries.size(); ++i)
  {

	  file.seekg(dataEntries[i]->dataOffset);
	  short Gshort = 0;
	  if(dataEntries[i]->tagNumber == 1)
	  {
		  for(int j = 0; j < dataEntries[i]->numOfElements; ++j)
		  {
				file.read(reinterpret_cast<char*>(&Gshort), 2);
				endian_swap(&Gshort);
				DNA.GYs.push_back(Gshort);
		  }
	  }
	  if(dataEntries[i]->tagNumber == 2)
	  {
		  		  for(int j = 0; j < dataEntries[i]->numOfElements; ++j)
		  {

		    file.read(reinterpret_cast<char*>(&Gshort), 2);
			endian_swap(&Gshort);
			DNA.AYs.push_back(Gshort);
		  }
	  }
	  if(dataEntries[i]->tagNumber == 3)
	  {
		  		  for(int j = 0; j < dataEntries[i]->numOfElements; ++j)
		  {

		    file.read(reinterpret_cast<char*>(&Gshort), 2);
			endian_swap(&Gshort);
			DNA.TYs.push_back(Gshort);
		  }
	  }
	  if(dataEntries[i]->tagNumber == 4)
	  {
		  		  for(int j = 0; j < dataEntries[i]->numOfElements; ++j)
		  {

				file.read(reinterpret_cast<char*>(&Gshort), 2);
				endian_swap(&Gshort);
				DNA.CYs.push_back(Gshort);
		  }
	  }
	  //std::cout << DATAstring << std::endl;
  }	
  for(int i = 0; i < peakEntries.size(); ++i)
  {
	  if(peakEntries[i]->tagNumber == 1)
	  {
		  short peakpos = 0;
		  file.seekg(peakEntries[i]->dataOffset);
		  for(int j = 0; j < peakEntries[i]->numOfElements; ++j)
		  {
			  file.read(reinterpret_cast<char*>(&peakpos), 2);
			  endian_swap(&peakpos);
			  DNA.PeakPositions.push_back(peakpos);			  
		  }
		  //for(int j = 1; j < DNA.PeakPositions.size(); j++)
		  //{
			 // std::cout << DNA.PeakPositions[j] - DNA.PeakPositions[j-1] << " ";
		  //}
		  //char tag[4000];
		 // file.read(reinterpret_cast<char*>(tag), peakEntries[i]->elementSize*peakEntries[i]->numOfElements);
		  //std::string DNAstring = std::string(tag, peakEntries[i]->elementSize*peakEntries[i]->numOfElements);
		  //DNA.PeakPositions = DNAstring;
	  }
  }	

    for(int i = 0; i < machineNameEntries.size(); ++i)
  {
		  file.seekg(machineNameEntries[i]->dataOffset);
		  char tag[30];
		  file.read(reinterpret_cast<char*>(tag), machineNameEntries[i]->elementSize*machineNameEntries[i]->numOfElements);
		  std::string DNAstring = std::string(tag, machineNameEntries[i]->elementSize*machineNameEntries[i]->numOfElements);
		  DNA.machineName = DNAstring;
  }

	    for(int i = 0; i < sampleNameEntries.size(); ++i)
  {
		  file.seekg(sampleNameEntries[i]->dataOffset);
		  char tag[30];
		  file.read(reinterpret_cast<char*>(tag), sampleNameEntries[i]->elementSize*sampleNameEntries[i]->numOfElements);
		  std::string DNAstring = std::string(tag, sampleNameEntries[i]->elementSize*sampleNameEntries[i]->numOfElements);
		  DNA.sampleName = DNAstring;
  }	

  for(int i = 0; i < averagePeakEntries.size(); ++i)
  {
		  //file.seekg(averagePeakEntries[i]->dataOffset);
		  char* tag;
		//  tag = reinterpret_cast<char*>(&(averagePeakEntries[i]->dataOffset));
		  //std::string DNAstring = std::string(tag, averagePeakEntries[i]->elementSize*averagePeakEntries[i]->numOfElements);
		  //DNA.averagePakEntries = DNAstring;
  }	
	   for(int i = 0; i < sampleCommentNameEntries.size(); ++i)
  {
		  file.seekg(sampleCommentNameEntries[i]->dataOffset);
		  char tag[100];
		  file.read(reinterpret_cast<char*>(tag), sampleCommentNameEntries[i]->elementSize*sampleCommentNameEntries[i]->numOfElements);
		  std::string DNAstring = std::string(tag, sampleCommentNameEntries[i]->elementSize*sampleCommentNameEntries[i]->numOfElements);
		  DNA.sampleComment = DNAstring;
  }	
	   for(int i = 0; i < qualityEntries.size(); ++i)
  {
		  file.seekg(qualityEntries[i]->dataOffset);
		  char tag[2000];
		  file.read(reinterpret_cast<char*>(tag), qualityEntries[i]->elementSize*qualityEntries[i]->numOfElements);
		  std::string DNAstring = std::string(tag, qualityEntries[i]->elementSize*qualityEntries[i]->numOfElements);
		  DNA.phredQuality = DNAstring;
  }	

	 //  for(int i = 0; i < capillarEntries.size(); ++i)
  //{
		//  file.seekg(capillarEntries[i]->dataOffset);
		//  char tag[100];
		//  file.read(reinterpret_cast<char*>(tag), capillarEntries[i]->elementSize*capillarEntries[i]->numOfElements);
		//  std::string DNAstring = std::string(tag, capillarEntries[i]->elementSize*capillarEntries[i]->numOfElements);
		//  DNA.capillarNumber = 1;
  //}	
  // for(int i = 0; i < peakSpacingEntries.size(); ++i)
  //{
		//  file.seekg(peakSpacingEntries[i]->dataOffset);
		//  char tag[4];
		//  file.read(reinterpret_cast<char*>(tag), peakSpacingEntries[i]->elementSize*peakSpacingEntries[i]->numOfElements);
		//  //endian_swap(&tag);
		//  //std::string DNAstring = std::string(tag, peakSpacingEntries[i]->elementSize*peakSpacingEntries[i]->numOfElements);
		//  DNA.peakSpacing = tag;
	 // 
  //}	
  if (file)
  {
    std::cout << "all DirectoryEntries read successfully";
  }
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