#include "AbifReader.h"


template <class T>
void endian_swap(T *object)
{
  unsigned char* memoryPointer = reinterpret_cast<unsigned char*>(object);
  std::reverse(memoryPointer, memoryPointer + sizeof(T));
}

//FIX: too much magic in this method
int ABIFReader::ReadAll()
{
  DirectoryEntry header;
  std::ifstream file(fileName, std::ios_base::binary);        
  if (!file.is_open())
  {
    std::cout << "File couldn't be opened, for the reasons unknown...";
    return 1;
  }

  if (checkFileFormat(file))
	  return 2;

  file.seekg(6, std::ios::beg);//DirectoryEntry in header section begins at this offset
  readData(file, &header);//FIX: this way of reading doesn't help either.
  //TODO: read specification, look at file_version?
  printDirectory(std::cout, header);
  //go to the header.dataOffset position:  
  DirectoryEntry* dirData = new DirectoryEntry[header.numOfElements];
  //Map, storing all directiry entries of file
  std::map<std::string, std::vector<DirectoryEntry*> > directoryMap;
  //TODO: should read dirData manually
  file.seekg(header.dataOffset);
 
   //read data
  for (int i = 0; i < header.numOfElements; i++)
  {
    readData(file, &dirData[i]);
	  directoryMap[dirData[i].tagName].push_back(&dirData[i]);
  }

  //Creating struct, storing all nessescary fields

  DNA = new DNASequence(directoryMap, &file);
  if(endWork(file, dirData))
	  return 3;
  isReady = true;
  return 0;
}

ABIFReader::DNASequence::DNASequence()
{
    
}
ABIFReader::DNASequence* ABIFReader::getDNA()
{
  return this->DNA;
}

void ABIFReader::setDNA(std::map<std::string, std::vector<DirectoryEntry*> > dirMap, std::ifstream *file)
{
  this->DNA = new DNASequence(dirMap, file);
}

ABIFReader::ABIFReader(std::string fileName)
{
  this->fileName = fileName;
}

std::string ABIFReader::getName()
{
  if (isReady)
    return this->getDNA()->getDNAName();
  return "";
}

std::string ABIFReader::getQuality()
{
  if (isReady)
    return this->getDNA()->getQuality();
  return "";
}

std::string ABIFReader::getSequence()
{
  if (isReady)
    return this->getDNA()->getSequence();
  return "";
}


std::string ABIFReader::DNASequence::getDNAName()
{
  return this->sampleName;
}

std::string ABIFReader::DNASequence::getQuality()
{
  return this->phredQuality;
}
//
std::string ABIFReader::DNASequence::getSequence()
{
  return this->sequence;
}

std::vector<std::vector<short> > ABIFReader::getXY()
{
  return this->getDNA()->getXY();
}

std::vector<std::vector<short> > ABIFReader::DNASequence::getXY()
{
  std::vector<std::vector<short> > XY;
  XY.push_back(this->PeakPositions);
  XY.push_back(this->GYs);
  XY.push_back(this->AYs);
  XY.push_back(this->TYs);
  XY.push_back(this->CYs);
  return XY;
}

void ABIFReader::printDirectory(std::ostream& outputStream, DirectoryEntry& dirEntry)
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

void ABIFReader::readData(std::istream & inputStream, DirectoryEntry* dirEntry)
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

int ABIFReader::checkFileFormat(std::ifstream& file)
{
  char file_format[5];  
  file.read(file_format, 4);
  file_format[4] = '\0';
  if (strcmp(file_format, "ABIF") != 0)
  {
          std::cout << "File has wrong format! " << file_format << std::endl;
          system("pause");
          return 2;
  }
  return 0;
}

int ABIFReader::endWork(std::ifstream &file, DirectoryEntry *dirData)
{
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
  return 0;
}

ABIFReader::DNASequence::DNASequence(std::map<std::string, std::vector<DirectoryEntry*> > dirMap, std::ifstream *file)
{
			sequence = GetStringInformationFromEntry("PBAS", 2, dirMap, file); 
			sampleComment = GetStringInformationFromEntry("CMNT", 1, dirMap, file); 
			GYs = GetInformationFromEntry("DATA", 1, dirMap, file);
			AYs = GetInformationFromEntry("DATA", 2, dirMap, file);
			TYs = GetInformationFromEntry("DATA", 3, dirMap, file);
			CYs = GetInformationFromEntry("DATA", 4, dirMap, file);
			PeakPositions = GetInformationFromEntry("PLOC", 1, dirMap, file);
			phredQuality = GetStringInformationFromEntry("PCON", 1, dirMap, file); 
			machineName = GetStringInformationFromEntry("MCHN", 1, dirMap, file);
			sampleName = GetStringInformationFromEntry("SMPL", 1, dirMap, file);
}

void ABIFReader::writeToFasta()
{
    std::ofstream file(fileName.substr(0, fileName.length() - 4) + ".fasta", std::ios_base::binary);
    file << ">" << fileName.substr(0, fileName.length() - 4)  << std::endl;
    file << getSequence();
}

void ABIFReader::writeToDat()
{
    std::ofstream file(fileName.substr(0, fileName.length() - 4) + ".dat", std::ios_base::binary);
    file << ">" << fileName.substr(0, fileName.length() - 4)  << std::endl;
    std::vector<std::vector<short> > buffer = getXY();
    for(int i = 0; i < buffer.size(); ++i)
    {
      file << buffer[i].size() << std::endl;
      for(int j = 0; j < buffer[i].size(); ++j)
      {
        file << buffer[i][j] << " ";
      }
      file << std::endl;
    }
}

std::vector<short> ABIFReader::DNASequence::GetInformationFromEntry(std::string tag, unsigned int number, 
  std::map<std::string, std::vector<DirectoryEntry*> > dirMap, std::ifstream *file)
{
	std::vector<DirectoryEntry*>::iterator it = std::begin(dirMap[tag]);
  for(it = dirMap[tag].begin(); it != dirMap[tag].end(); ++it)
	{
		if((*it)->tagNumber == number)
			break;
	}
	std::vector<short> result;
	if(it != dirMap[tag].end())
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

std::string ABIFReader::DNASequence::GetStringInformationFromEntry(std::string tag, unsigned int number, 
  std::map<std::string, std::vector<DirectoryEntry*> > dirMap, std::ifstream *file)
{
  std::vector<DirectoryEntry*>::iterator it = dirMap[tag].begin();
  for(it = dirMap[tag].begin(); it != dirMap[tag].end(); ++it)
  {
    if((*it)->tagNumber == number)
      break;
  }
  std::string result;
  if(it != dirMap[tag].end())
  {
    file->seekg((*it)->dataOffset);
    char tag[2000];
    file->read(reinterpret_cast<char*>(tag), (*it)->elementSize*(*it)->numOfElements);
    result = std::string(tag, (*it)->elementSize*(*it)->numOfElements);
    return result;
  }
  std::cout << "Can't return " << tag << " with parameter " << number << std::endl;
  return result;
}
