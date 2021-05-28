Program= DnaGrep

SRCSPP:= $(wildcard *.cpp)
OBJS:= $(patsubst %.cpp,%.o,$(SRCSPP))

CXX= g++
CXXFLAGS= -g -Wall -O9

%.o: %.cpp Makefile
	${CXX} $(CXXFLAGS) -o $@ -c $<

${Program}: $(OBJS) Makefile
	${CXX} -o ${Program} $(OBJS) $(CXXFLAGS) $(LIBS)

clean:
	rm -f ${Program} $(OBJS)

depend:
	./makedep.sh ${CXX} ${SRCSPP} ${SRCS}

# Don't touch anything below this line
# DEPENDENCIES_BEGIN
BayesModel.o: BayesModel.cpp config.h BayesModel.h SequenceSet.h \
  FastaParserSequential.h FastaParserInline.h FileReader.h SequenceInfo.h
FuzzyPattern.o: FuzzyPattern.cpp config.h FuzzyPattern.h
Main.o: Main.cpp config.h Usage.h Profile.h Scan.h ModelScan.h \
  ModelStats.h Stats.h
ModelScan.o: ModelScan.cpp ModelScan.h config.h FileReader.h \
  FastaParserWindowed.h ShiftRegister.h FastaParserInline.h \
  FuzzyPattern.h BayesModel.h SequenceSet.h FastaParserSequential.h \
  SequenceInfo.h
ModelStats.o: ModelStats.cpp ModelStats.h config.h FileReader.h \
  FastaParserWindowed.h ShiftRegister.h FastaParserInline.h \
  FuzzyPattern.h BayesModel.h SequenceSet.h FastaParserSequential.h \
  SequenceInfo.h
Profile.o: Profile.cpp Profile.h config.h FastaParserSequential.h \
  FastaParserInline.h FileReader.h
Scan.o: Scan.cpp Scan.h config.h FileReader.h FastaParserWindowed.h \
  ShiftRegister.h FastaParserInline.h FuzzyPattern.h
Stats.o: Stats.cpp Stats.h config.h FileReader.h FastaParserWindowed.h \
  ShiftRegister.h FastaParserInline.h FuzzyPattern.h
Usage.o: Usage.cpp Usage.h config.h
