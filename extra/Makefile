PATTERN:= ca
MINMM12:= -38.81
MINMM23:= -58.45
MINHS12:= -38.81
MINHS23:= -58.45


MIN12:= 100
MIN23:= 128


HS:= $(patsubst genomes/Human/%,%,$(wildcard genomes/Human/*.fa))
MM:= $(patsubst genomes/Mouse/%,%,$(wildcard genomes/Mouse/*.fa))

CHRHS= $(HS:.fa=)
CHRMM= $(MM:.fa=)

TARGETS:= 	        $(foreach x,$(CHRHS),results/DnaGrab_$(x)_Human_12.tbl) \
			$(foreach x,$(CHRHS),results/DnaGrab_$(x)_Human_23.tbl) \
			$(foreach x,$(CHRMM),results/DnaGrab_$(x)_Mouse_12.tbl) \
			$(foreach x,$(CHRMM),results/DnaGrab_$(x)_Mouse_23.tbl)

STATSHS12:=	$(foreach x,$(CHRHS),tmp/$(x)_Human_12.cnt)
STATSHS23:=	$(foreach x,$(CHRHS),tmp/$(x)_Human_23.cnt)
STATSMM12:=	$(foreach x,$(CHRMM),tmp/$(x)_Mouse_12.cnt)
STATSMM23:=	$(foreach x,$(CHRMM),tmp/$(x)_Mouse_23.cnt)

DNAGREP= ./DnaGrep/DnaGrep
SCAN:= $(DNAGREP) model -q
STAT:= $(DNAGREP) modstats -q

CUT= /bin/cut
PASTE= /usr/bin/paste
TR= /usr/bin/tr
ECHO= /bin/echo
RM= /bin/rm
CAT= /bin/cat


results/DnaGrab_%_Human_12.tbl: genomes/Human/%.fa datasets/HS12.model datasets/HS12RSS.fasta
	$(SCAN) -m $(MINHS12) $< datasets/HS12RSS.fasta `$(CAT) datasets/HS12.model` $(PATTERN) > $@

tmp/%_Human_12.txt: genomes/Human/%.fa datasets/HS12.model datasets/HS12RSS.fasta
	$(STAT) -m -$(MIN12) -n $(MIN12) $< datasets/HS12RSS.fasta `$(CAT) datasets/HS12.model` $(PATTERN) > $@

tmp/HS12.head: 
	$(ECHO) HS12 $(CHRHS) > $@

tmp/HS12.data: tmp/HS12.scl $(STATSHS12)
	$(PASTE) tmp/HS12.scl $(STATSHS12) > tmp/HS12.data



results/DnaGrab_%_Mouse_12.tbl: genomes/Mouse/%.fa datasets/MM12.model datasets/MM12RSS.fasta
	$(SCAN) -m $(MINMM12) $< datasets/MM12RSS.fasta `$(CAT) datasets/MM12.model` $(PATTERN) > $@

tmp/%_Mouse_12.txt: genomes/Mouse/%.fa datasets/MM12.model datasets/MM12RSS.fasta
	$(STAT) -m -$(MIN12) -n $(MIN12) $< datasets/MM12RSS.fasta `$(CAT) datasets/MM12.model` $(PATTERN) > $@

tmp/MM12.head: 
	$(ECHO) MM12 $(CHRMM) > $@

tmp/MM12.data: tmp/MM12.scl $(STATSMM12)
	$(PASTE) tmp/MM12.scl $(STATSMM12) > tmp/MM12.data



results/DnaGrab_%_Human_23.tbl: genomes/Human/%.fa datasets/HS23.model datasets/HS23RSS.fasta
	$(SCAN) -m $(MINHS23) $< datasets/HS23RSS.fasta `$(CAT) datasets/HS23.model` $(PATTERN) > $@

tmp/%_Human_23.txt: genomes/Human/%.fa datasets/HS23.model datasets/HS23RSS.fasta
	$(STAT) -m -$(MIN23) -n $(MIN23) $< datasets/HS23RSS.fasta `$(CAT) datasets/HS23.model` $(PATTERN) > $@

tmp/HS23.head: 
	$(ECHO) HS23 $(CHRHS) > $@

tmp/HS23.data: tmp/HS23.scl $(STATSHS23)
	$(PASTE) tmp/HS23.scl $(STATSHS23) > tmp/HS23.data



results/DnaGrab_%_Mouse_23.tbl: genomes/Mouse/%.fa datasets/MM23.model datasets/MM23RSS.fasta
	$(SCAN) -m $(MINMM23) $< datasets/MM23RSS.fasta `$(CAT) datasets/MM23.model` $(PATTERN) > $@

tmp/%_Mouse_23.txt: genomes/Mouse/%.fa datasets/MM23.model datasets/MM23RSS.fasta
	$(STAT) -m -$(MIN23) -n $(MIN23) $< datasets/MM23RSS.fasta `$(CAT) datasets/MM23.model` $(PATTERN) > $@

tmp/MM23.head: 
	$(ECHO) MM23 $(CHRMM) > $@

tmp/MM23.data: tmp/MM23.scl $(STATSMM23)
	$(PASTE) tmp/MM23.scl $(STATSMM23) > tmp/MM23.data



statistics/%.txt: tmp/%.head tmp/%.data 
	$(CAT) $^ | $(TR) ' ' '\t' > $@

tmp/%.cnt: tmp/%.txt 
	$(CUT) -f3 $< > $@

tmp/%12.scl: tmp/chr1_Human_12.txt 
	$(CUT) -f1 tmp/chr1_Human_12.txt > $@

tmp/%23.scl: tmp/chr1_Human_23.txt
	$(CUT) -f1 tmp/chr1_Human_23.txt > $@


stats: statistics/HS12.txt statistics/HS23.txt statistics/MM12.txt statistics/MM23.txt

scan: $(TARGETS)

clean:
	$(RM) -f results/* statistics/* tmp/* 



