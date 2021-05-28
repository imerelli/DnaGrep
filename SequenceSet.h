#ifndef __SequenceSet_h
#define __SequenceSet_h

#include "FastaParserSequential.h"
#include "SequenceInfo.h"

//  typedef std::vector<SequenceInfo> SequenceArray;

class SequenceSet: public std::vector<SequenceInfo>
{
  public:
    void Read(FileReader &reader, bool maskit = false, bool lower = false)
      {
        FastaParserSequential parser(reader, maskit, lower);

        while(!parser.eof())
        {
          std::string sequence = parser.sequence();
          std::string name = parser.name();
          std::string comment = parser.comment();

          SequenceInfo seq(sequence, name, comment);
          push_back(seq);
          parser.next();          
        }
      }

	void Read(std::string file, bool maskit = false, bool lower = false)
      {
        FileReader reader(file);
		Read (reader, maskit, lower);
      }

	SequenceSet(void)
		{
		}
	
	SequenceSet(std::string file, bool maskit = false, bool lower = false)
		{
			Read(file, maskit, lower);
		}

	SequenceSet(FileReader &reader, bool maskit = false, bool lower = false)
		{
			Read(reader, maskit, lower);
		}
	
    void CleanDNA(void)
      {
        size_t j;
        std::vector<SequenceInfo>::iterator p;
        for (p = begin(); p != end(); p++)
          for (j = 0; j < p->size(); j++)
            {
              char ch = tolower((*p)[j]);
              if (ch != 'a' && ch != 'c' && ch != 'g' && ch != 't')
                ch = '-';
              p->setSequencePos(j,ch);
            }
      }

    size_t MaxLength(void)
      {
        size_t j = 0;
        std::vector<SequenceInfo>::iterator p;
        for (p = begin(); p != end(); p++)
		  if(p->size() > j)
			j = p->size();
		return j;
      }
};
#endif  // !__SequenceSet_h
