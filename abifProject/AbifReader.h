#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <cstring>
#include <stdint.h>
#include <iterator>

template <class T>
void endian_swap(T *object);

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

/*
���������. ��� ������ SWIG.
1. ���� ��������� > ������ � ��������, ������ �� ��������� ��������
*/
class ABIFReader
{
  public:
    ABIFReader(const std::string& fileName);
    int ReadAll();
	  void writeToFasta();
	  void writeToDat();
	  const std::string& getName() const;
	  const std::string& getQuality() const;
    const std::string& getSequence() const;
	  const std::vector<std::vector<short> >& getXY() const;

    struct DNASequence
    {
    private:
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

	    std::string GetStringInformationFromEntry(std::string, unsigned int, 
        std::map<std::string, std::vector<DirectoryEntry*> >, std::ifstream&);

		  std::vector<short> GetInformationFromEntry(std::string, unsigned int, 
        std::map<std::string, std::vector<DirectoryEntry*> >, std::ifstream&);

    public:
      DNASequence();
      DNASequence(std::map<std::string, std::vector<DirectoryEntry*> >, std::ifstream&);
      const std::string& getSequence() const;
	    const std::string& getQuality() const;
	    const std::vector<std::vector<short> >& getXY() const;
      const std::string& getDNAName() const;
	  };

    void setDNA(std::map<std::string, std::vector<DirectoryEntry*> >, std::ifstream&);
    const DNASequence& getDNA() const;

  private:
    DNASequence* DNA;
    std::string fileName;
    bool isReady;
	  int checkFileFormat(std::ifstream&);
    int endWork(std::ifstream&, std::vector<DirectoryEntry>);
    void printDirectory(std::ostream&, DirectoryEntry&);
	  void readData(std::istream&, DirectoryEntry*);
};