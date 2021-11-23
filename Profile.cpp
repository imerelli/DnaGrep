#include <fstream>
#include <iostream>
#include "Profile.h"
#include "FastaParserSequential.h"
#include "FileReader.h"
#include <limits.h>

static size_t const kEntries = 256;

struct Block
  {
    int mArray[kEntries];
  };

int Profile(bool Verbose, bool Quiet,
  std::string Output, std::vector<std::string>List)
  {
   Block Zeros;

   for(size_t i = 0; i < kEntries; i++)
     Zeros.mArray[i] = 0;

    if (Output == "-")
      {
        if (!Quiet)
          std::cerr << "Using standard output." << std::endl;
      }
    else
      {
        if (!Quiet)
          std::cerr << "Using " << Output << " for output." << std::endl;
      }

    size_t totlen = 0;
    std::string Composed;
    for(size_t i = 0; i < List.size(); i++)
      {
        int nseq = 0;
        size_t minlen = INT_MAX;
        FileReader reader(List[i]);
        FastaParserSequential parser(reader);
        std::vector<Block> Counts;

        if(!Quiet)
          std::cerr << "Processing " << List[i] << " ";
 
        while(!parser.eof())
          {
            std::string Sequence = parser.sequence();

            if(Sequence.length() < minlen)
              minlen = Sequence.length();

            while (Counts.size() < Sequence.length())
              Counts.push_back(Zeros);
            for(size_t c = 0; c < Sequence.length(); c++)
              (Counts[c].mArray[(int) Sequence[c]]) ++;
            parser.next();
            ++nseq;

            if (Verbose && !(nseq % 1000))
              std::cerr << nseq;
            else if (Verbose && !(nseq % 100))
              std::cerr << ".";
          }

        if(!Quiet)
          std::cerr << "done (" << nseq << " sequences, len = " << minlen <<")." << std::endl;

        totlen += minlen;
        std::string Profile;
        for (size_t i = 0; i < minlen; i++)
          {
            bool flag = false;

            Profile.append("[");
            for (size_t c = 0; c < kEntries; c++)
              {
                if (Counts[i].mArray[c])
                  {
                    char buf[36];

                    snprintf(buf, 31, "%c:%d", (int) c, Counts[i].mArray[c]);
                    buf[35] = 0;

                    if (flag)
                      Profile.append(",");

                    Profile.append(buf);

                    flag = true;
                  }
              }
            Profile.append("]");
          }

        if(!Quiet)
          std::cerr << "Profile = " << Profile <<std::endl;
        Composed.append(Profile);
      }

    if (!Quiet)
      std::cerr << "Composed profile built." << std::endl;
    std::cerr.flush();

    if (Output == "-")
      {
        std::cout << Composed << std::endl;
      }
    else
      {
        std::ofstream outfile (Output.c_str(), std::ios::binary);
        outfile << Composed << std::endl;
        outfile.close();
      }

    if (!Quiet)
      std::cerr << "Composed profile len = " << totlen<< std::endl;

      return 0;
    }
