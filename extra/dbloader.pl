#! /usr/bin/perl

$MINMM12= -38.81;
$MINMM23= -58.45;
$MINHS12= -38.81;
$MINHS23= -58.45;

$RSSMM12= -38.81;
$RSSMM23= -58.45;
$RSSHS12= -38.81;
$RSSHS23= -58.45;

$dirtoget="results/"; 

opendir(IMD, $dirtoget) || die("Cannot open directory $dirtoget"); 
@thefiles= readdir(IMD);
closedir(IMD);

%done = ();

print("-- MySQL loader for RSS Database\n");
print("-- ------------------------------------------------------\n");
print("\n");

foreach $filename (@thefiles)
{
  chomp($filename);
  if ( $filename =~ /^DnaGrab_(chr[^_]+)_([^_]+)_([^_]+).tbl$/) 
  {
  	$chr = $1;
  	$species = $2;
  	$type = $3;
  	
  	$key = $chr."_".$species;
	if ($species=="Human") {
	  if($type == 12) {
	    $RSSSCORE=$RSSHS12;
	    $MINSCORE=$MINHS12;
	  } else {
	    $RSSSCORE=$RSSHS23;
	    $MINSCORE=$MINHS23;
	  }
	} else {
          if($type == 12) {
	    $RSSSCORE=$RSSMM12;
	    $MINSCORE=$MINMM12;
	  } else {
            $RSSSCORE=$RSSMM23;
            $MINSCORE=$MINMM23;
	  }
	}
  
	if (not(exists ($done{$key})))
	{
	  print("--\n");
	  print("-- Table structure for table DnaGrab_".$chr."_".$species."\n");
	  print("--\n");
	  print("\n");
	  print("DROP TABLE IF EXISTS DnaGrab_".$chr."_".$species.";\n");
	  print("CREATE TABLE DnaGrab_".$chr."_".$species." (\n");
	  print("  Seq_Key varchar(50) NOT NULL default '',\n");
	  print("  Method varchar(10) default NULL,\n");
	  print("  Cromosoma varchar(6) default NULL,\n");
	  print("  Locus varchar(6) default NULL,\n");
	  print("  Score bigint(20) default NULL,\n");
	  print("  Loc_Start bigint(20) default NULL,\n");
	  print("  Loc_End bigint(20) default NULL,\n");
	  print("  Sequenza varchar(50) default NULL,\n");
	  print("  Strand char(1) default NULL,\n");
	  print("  Ric varchar(10) default NULL,\n");
	  print("  Organism varchar(10) default NULL,\n");
	  print("  Ric_Value decimal(5,2) default NULL,\n");
	  print("  PRIMARY KEY  (Seq_Key)\n");
	  print(") ENGINE=MyISAM DEFAULT CHARSET=latin1;\n");
	  print("\n");
	}
	$done{$key} = 1;

	print("-- Dumping data for table DnaGrab_".$chr."_".$species." from ".$filename."\n");
	print("--\n");
	print("\n");
	print("ALTER TABLE DnaGrab_".$chr."_".$species." DISABLE KEYS;\n");
	print("LOCK TABLES DnaGrab_".$chr."_".$species." WRITE;\n");
	print("\n");
	#print("INSERT INTO DnaGrab_".$chr."_".$species." VALUES \n");

	open(TABLE, $dirtoget."/".$filename) or die ("Can not open $filename");
	$firstline = 1;
        $count=0;
	while ($line = <TABLE>)
	{
	  chomp($line);
	  
	  if ($line =~ /^([^\t]+)\t([^\t]+)\t([^\t]+)\t([^\t]+)\t([^\t]+)\t([^\t]+)$/)
	  {
		$begins = $2;
		$ends = $3;
		$seq = $4;
		$strand= $5;
		$score= $6;

		$rank = int( 0.5+(1000*($MINSCORE-$score)/$MINSCORE) );
		$pass = ($score>$RSSSCORE)?"P":"F";
		$score = (int($score*100+0.5)/100);

		#print ($firstline?"":",\n");
		print("INSERT INTO DnaGrab_".$chr."_".$species." VALUES");
		print("  ('all".$type."-".$species."-".$chr."-".$begins."-".$ends."-str".$strand."Ric:".$score."','DnaGrep++','".$chr."','all".$type."',".$rank.",".$begins.",".$ends.",'".$seq."','".$strand."','".$pass."','".$species."','".$score."');\n");
		$firstline = 0;
		$count++;
	  }
	  else{
		print("ERRORE!!\n");
	   }
	}
	print(";\n");
	
	print("\n");
	print("UNLOCK TABLES;\n");
	print("ALTER TABLE DnaGrab_".$chr."_".$species." ENABLE KEYS;\n");
	print("\n");
	#print($species."---".$chr."---".$type."---".$count."\n");
	close(TABLE);

  }
}
