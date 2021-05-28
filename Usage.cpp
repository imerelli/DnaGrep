#include "Usage.h"

int Usage(std::string Program)
  {
    std::cerr << "Usage: " << Program <<  " profile <options> <file> [...]" << std::endl;
    std::cerr << "  options for profile (use this to build the profiles):" << std::endl;
    std::cerr << "    -o <file> save patterns in file (default '-')" << std::endl;
    std::cerr << "    -v display verbose progress information" << std::endl;
    std::cerr << "    -q display only errors" << std::endl;
    std::cerr << std::endl;

    std::cerr << "Usage: " << Program <<  " scan <options> <file> <patterns> [...]" << std::endl;
    std::cerr << "  options for scan (the this to scan a fasta file):" << std::endl;
    std::cerr << "    -m <min> sets minimum score for matches (default .0001)" << std::endl;
    std::cerr << "    -f match forward strand only" << std::endl;
    std::cerr << "    -r match reverse strand only" << std::endl;
    std::cerr << "    -b match both forward and reverse strands (default)" << std::endl;
    std::cerr << "    -k masK out lowercase stuff in input" << std::endl;
    std::cerr << "    -i <file> read (more) patterns from <file>" << std::endl;
    std::cerr << "    -o <file> save output to file (default '-')" << std::endl;
    std::cerr << "    -a output hits as genome browser annotations (default)" << std::endl;
    std::cerr << "    -t output hits as table" << std::endl; 
    std::cerr << "    -u <bases> dump <bases> upstream match for table output (default 20)" << std::endl;
    std::cerr << "    -d <bases> dump <bases> downstream match for table output (default 20)" << std::endl;
    std::cerr << "    -v display verbose progress information" << std::endl;
    std::cerr << "    -q display only errors" << std::endl;
    std::cerr << std::endl;

    std::cerr << "Usage: " << Program <<  " model <options> <file> <examples> <model> <pattern>" << std::endl;
    std::cerr << "  options for scan (the this to scan a fasta file):" << std::endl;
    std::cerr << "    -m <min> sets minimum score for matches (default -50)" << std::endl;
    std::cerr << "    -f match forward strand only" << std::endl;
    std::cerr << "    -r match reverse strand only" << std::endl;
    std::cerr << "    -b match both forward and reverse strands (default)" << std::endl;
    std::cerr << "    -k masK out lowercase stuff in input" << std::endl;
    std::cerr << "    -o <file> save output to file (default '-')" << std::endl;
    std::cerr << "    -a output hits as genome browser annotations (default)" << std::endl;
    std::cerr << "    -t output hits as table" << std::endl; 
    std::cerr << "    -u <bases> dump <bases> upstream match for table output (default 20)" << std::endl;
    std::cerr << "    -d <bases> dump <bases> downstream match for table output (default 20)" << std::endl;
    std::cerr << "    -v display verbose progress information" << std::endl;
    std::cerr << "    -q display only errors" << std::endl;
    std::cerr << std::endl;

    std::cerr << "Usage: " << Program <<  " stats <options> <file> <patterns> [...]" << std::endl;
    std::cerr << "  options for stats (use this for statistics on matches):" << std::endl;
    std::cerr << "    -m <min> sets minimum score for matches (default the minimum for the model)" << std::endl;
    std::cerr << "    -f match forward strand only" << std::endl;
    std::cerr << "    -r match reverse strand only" << std::endl;
    std::cerr << "    -b match both forward and reverse strands (default)" << std::endl;
    std::cerr << "    -k masK out lowercase stuff in input" << std::endl;
    std::cerr << "    -i <file> read (more) patterns from <file>" << std::endl;
    std::cerr << "    -o <file> save output to file (default '-')" << std::endl;
    std::cerr << "    -n <buckets> collect distribution on <buckets> ranges (default 100)" << std::endl;
    std::cerr << "    -l use log scale" << std::endl;
    std::cerr << "    -v display verbose progress information" << std::endl;
    std::cerr << "    -q display only errors" << std::endl;
    std::cerr << std::endl;

    std::cerr << "Usage: " << Program <<  " modstats <options> <file> <examples> <model> <pattern>" << std::endl;
    std::cerr << "  options for scan (the this to scan a fasta file):" << std::endl;
    std::cerr << "    -m <min> sets minimum score for matches (default the minimum for the model)" << std::endl;
    std::cerr << "    -f match forward strand only" << std::endl;
    std::cerr << "    -r match reverse strand only" << std::endl;
    std::cerr << "    -b match both forward and reverse strands (default)" << std::endl;
    std::cerr << "    -k masK out lowercase stuff in input" << std::endl;
    std::cerr << "    -o <file> save output to file (default '-')" << std::endl;
    std::cerr << "    -v display verbose progress information" << std::endl;
    std::cerr << "    -q display only errors" << std::endl;
    std::cerr << "    -n <buckets> collect distribution on <buckets> ranges (default 100)" << std::endl;
    std::cerr << std::endl;

    return 1;
  }

