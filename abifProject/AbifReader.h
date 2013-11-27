#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <stdint.h>

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


class ABIFReader
{
  public:
    ABIFReader(std::string);
    int ReadAll();
	  void writeToFasta();
	  void writeToDat();
	  std::string getName();
	  std::string getQuality();
    std::string getSequence();
	  std::vector<std::vector<short>> getXY();
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
	    std::string GetStringInformationFromEntry(std::string, unsigned int, std::map<std::string, std::vector<DirectoryEntry*>>, std::ifstream*);
		  std::vector<short> GetInformationFromEntry(std::string, unsigned int, std::map<std::string, std::vector<DirectoryEntry*>>, std::ifstream*);
    public:
      DNASequence();
      DNASequence(std::map<std::string, std::vector<DirectoryEntry*>>, std::ifstream*);
      std::string getSequence();
	    std::string getQuality();
	    std::vector<std::vector<short>> getXY();
      std::string getDNAName();
	  };
    void setDNA(std::map<std::string, std::vector<DirectoryEntry*>>, std::ifstream*);
    DNASequence* getDNA();
  private:
    DNASequence* DNA;
    std::string fileName;
    bool isReady;
	  int checkFileFormat(std::ifstream&);
    int endWork(std::ifstream&, DirectoryEntry*);
    void printDirectory(std::ostream&, DirectoryEntry&);
	  void readData(std::istream&, DirectoryEntry*);
};