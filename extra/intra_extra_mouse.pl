#!/usr/bin/perl
$spec="Mouse";
$max=21;

use DBI;
use DBD::mysql;
my $dbh = DBI->connect ("DBI:mysql:rssnew:localhost:3306","rss","html2003") or die "non mi posso connettere al database:$DBI::errstr\n";

$sql="select name,chrom,txStart,txEnd FROM Known_genes_$spec";
$sth = $dbh-> prepare ($sql) or die "Non posso preparare lo statement: $DBI::errstr\n";
$sth->execute () or die "Accidenti, non posso eseguire il comando SQL: $DBI::errstr\n";

$count=0;

while (($name,$chr,$start,$end) = $sth->fetchrow_array() ){

	if ($chr=~m/M/){next;}
	if ($chr=~m/_/){next;}

	$RSS12_space=0;
	$RSS23_space=0;
	$gene_space=$end-$start;
	$count++;

	#Per selezionare l'RSS basta che il suo start o il suo end siano dentro lo spazio delimitato dal gene
	$sql="SELECT Loc_Start,Loc_End FROM DnaGrab_".$chr."_$spec WHERE Organism='$spec' and Locus = 'all12' and ( (Loc_Start >= ($start) AND Loc_Start <= ($end)) OR (Loc_End >= ($start) AND Loc_End <= ($end)) )";
	$sth2 = $dbh-> prepare ($sql) or die "Non posso preparare lo statement: $DBI::errstr\n";
	$sth2->execute () or die "Accidenti, non posso eseguire il comando SQL: $DBI::errstr\n";
	$RSS12_number = $sth2->rows();
	$RSS12_space=0;
	while (($RSS_start,$RSS_end)=$sth2->fetchrow_array()){
	if ($RSS_start>=$start && $RSS_end<=$end){$RSS12_space=$RSS12_space+$RSS_end-$RSS_start;}
	if ($RSS_start>=$start && $RSS_end>=$end){$RSS12_space=$RSS12_space+$end-$RSS_start;}
	if ($RSS_start<=$start && $RSS_end<=$end){$RSS12_space=$RSS12_space+$RSS_end-$start;}
	}
	
	
	#Per selezionare l'RSS basta che il suo start o il suo end siano dentro lo spazio delimitato dal gene
	$sql="SELECT Loc_Start,Loc_End FROM DnaGrab_".$chr."_$spec WHERE Organism='$spec' and Locus = 'all23' and ( (Loc_Start >= ($start) AND Loc_Start <= ($end)) OR (Loc_End >= ($start) AND Loc_End <= ($end)) )";
        $sth3 = $dbh-> prepare ($sql) or die "Non posso preparare lo statement: $DBI::errstr\n";
        $sth3->execute () or die "Accidenti, non posso eseguire il comando SQL: $DBI::errstr\n";
        $RSS23_number = $sth3->rows();
	while (($RSS_start,$RSS_end)=$sth3->fetchrow_array()){
        if ($RSS_start>=$start && $RSS_end<=$end){$RSS23_space=$RSS23_space+$RSS_end-$RSS_start;}
        if ($RSS_start>=$start && $RSS_end>=$end){$RSS23_space=$RSS23_space+$end-$RSS_start;}
        if ($RSS_start<=$start && $RSS_end<=$end){$RSS23_space=$RSS23_space+$RSS_end-$start;}
        }
	
	#La n tiene conto del cromosoma
	$chr=~m/chr(.+)/;$n=$1;
	if ($n eq 'X'){$n=20;}
	if ($n eq 'Y'){$n=21;}

	#Numer of RSS for each gene
	$RSS23_number[$n]=$RSS23_number[$n]+$RSS23_number;
	$RSS12_number[$n]=$RSS12_number[$n]+$RSS12_number;	
	
	#Space
	$gene_space[$n]=$gene_space[$n]+$gene_space;
	$RSS12_space[$n]=$RSS12_space[$n]+$RSS12_space;
	$RSS23_space[$n]=$RSS23_space[$n]+$RSS23_space;
	
	#Tiene conto di quanti geni ci sono in ogni cromosoma
	$gene_number[$n]++;
	if ($RSS12_number==0){$RSS12_density=0}
	else{$RSS12_density=$gene_space/$RSS12_number;}
	if ($RSS23_number==0){$RSS23_density=0}
	else{$RSS23_density=$gene_space/$RSS23_number;}

	print "CHR:$chr\tGENE:$name\tDIM:$gene_space\tRSS12:$RSS12_number\tRSS23:$RSS23_number\tRSS12_space:$RSS12_space\tRSS23_space:$RSS23_space\tRSS12_density:$RSS12_density\tRSS23_density:$RSS23_density\n";
	#if ($count==20){last;}

}#foreach

print "\n";

print "RSS\tChr\tN_gene\tN_RSS12\tfrac\tgene_space\tRSS12_space\tdens_space\tRSS12_density\n";
for ($i=1;$i<($max+1);$i++){
$RSS12_gden=$RSS12_number[$i]/$gene_number[$i];
$RSS12_density=$gene_space[$i]/$RSS12_number[$i];
$RSS12_spaden=$gene_space[$i]/$RSS12_space[$i];
print "RSS12\tChr$i\t$gene_number[$i]\t$RSS12_number[$i]\t$RSS12_gden\t$gene_space[$i]\t$RSS12_space[$i]\t$RSS12_spaden\t$RSS12_density\n";
}

print "\n";

print "RSS\tChr\tN_gene\tN_RSS23\tfrac\tgene_space\tRSS23_space\tdens_space\tRSS23_density\n";
for ($i=1;$i<($max+1);$i++){
$RSS23_gden=$RSS23_number[$i]/$gene_number[$i];
$RSS23_density=$gene_space[$i]/$RSS23_number[$i];
$RSS23_spaden=$gene_space[$i]/$RSS23_space[$i];
print "RSS23\tChr$i\t$gene_number[$i]\t$RSS23_number[$i]\t$RSS23_gden\t$gene_space[$i]\t$RSS23_space[$i]\t$RSS23_spaden\t$RSS23_density\n";

}

print "\n";


