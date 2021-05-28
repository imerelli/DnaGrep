#!/usr/bin/perl
$spec="Mouse";
$max=24;

use DBI;
use DBD::mysql;
my $dbh = DBI->connect ("DBI:mysql:rssnew:localhost:3306","rss","html2003") or die "non mi posso connettere al database:$DBI::errstr\n";

$sql="select Cytoband FROM Chr_band_Mouse";
$sth = $dbh-> prepare ($sql) or die "Non posso preparare lo statement: $DBI::errstr\n";
$sth->execute () or die "Accidenti, non posso eseguire il comando SQL: $DBI::errstr\n";

$count=0;

while ($name = $sth->fetchrow_array() ){
	$name=~m/chr(\w+)([p|q]\w+\.*\w*)/;
	if ($count == 0){
		$past=$1;
print "
if (select_chromosoma == \"chr$1\" && select_species == \"Mouse\" ) \{
ClearOptionsFast('pippo');
";

	}
	elsif ($past != $1){
		print "
return
\}
if (select_chromosoma == \"chr$1\" && select_species == \"Mouse\" ) \{
ClearOptionsFast('pippo');
";
		$past=$1;
		$count=0;
	}
	

	print "option$count = new Option(\"chr$1$2\",\"chr$1$2\")
document.rss_search21.band.options[$count] = option$count
";
$count++;
}
print "return
}";
