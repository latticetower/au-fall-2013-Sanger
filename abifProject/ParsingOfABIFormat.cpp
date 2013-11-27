// ParsingOfABIFormat.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
// basic specification url:
// http://projects.nfstc.org/workshops/resources/articles/ABIF_File_Format.pdf
//
#include <iostream>
#include "AbifReader.h"

//TODO: there is still some error with struct reading, so must fix, I hope this article would help:
//http://en.wikipedia.org/wiki/Data_structure_alignment
// let's try to read manually again, hope that help

int main(int argc, char *argv[])
{
  for(int i = 1; i <= 3; ++i)
  {
    ABIFReader reader(argv[i]);
    if(reader.ReadAll())
      return 1;
    reader.writeToDat();
    reader.writeToFasta();
  }
  system("pause");
  return 0;
}