#!/bin/sh
#Author: Patrik Holop

POSIXLY_CORRECT=yes

#test terminal
if test -t 1; then
		NUMBEROFCOL=`expr $(tput cols) - 1`
	else
		NUMBEROFCOL=79
fi

#constants
maxh=$((NUMBEROFCOL-12))
IGNOREARG="-9151davs25da$fae51wa!"
maxsizes=0
NORMALIZED=0
NORMALIZEDFLAG=0
IGNORE=0

#print hash
printhash() {
if [ ! -z "$maxsizes" ]; then
	if [ ! "$1" -eq "0" ]; then
	for i in `seq 1 $1`
		do
		  if [ $i -eq 0 ]; then
    		:
		  elif [ $NORMALIZEDFLAG -eq 1 ]; then
		    LINE=$(($1 * 100 / maxsizes))
			MAX=$((LINE * maxh / 100))
			
    		if [ $i -gt $MAX ]; then
		      break;
    		else
		      printf "#"
		    fi
		  else
		    printf "#"
		  fi
		done 
fi
fi
printf "\n"
}

#arguments
while getopts ":ni:" opt; do
  case $opt in
    i)
	  if [ $IGNORE -eq 0 ]; then
	      IGNORE=1
    	  IGNOREARG=$OPTARG
		  IGNOREARG=`echo "$IGNOREARG" | sed 's/\^/\//'`
		  IGNOREARG=`echo "$IGNOREARG" | sed 's/[$]/\(\/\|\$\)/'`
		  #echo "$IGNOREARG"
	  else
		 >&2 echo "Error: More regexes on input"
     	 exit 1 
      
	  fi	
      ;;
    n)
	  if [ $NORMALIZED -eq 0 ]; then
     	 NORMALIZED=1
	  	 NORMALIZEDFLAG=1
	  else
		 >&2 echo "Error: More normalizes on input"
     	 exit 1       
	  fi
      ;;
    \?)
      >&2 echo "Invalid option"
      exit 1
      ;;
    :)
      >&2 echo "Option needs arguments"
      exit 1
      ;;
  esac
done

#path is the last argument
shift $(($OPTIND - 1))
if [ $# -eq 0 ]; then
  pathtofile="."
elif [ $# -eq 1 ]; then
  if [ -d "$1" ]; then
    pathtofile="$1"
  else
	>&2 echo "Wrong path"
    exit 1
  fi  
else
  >&2 echo "Too many arguments"	
  exit 1
fi

#test persission denies
te=$(find $pathtofile -type d 2>/dev/null)
if [ "$?" != "0" ]; then >&2 echo "Permission denied error"; exit 1; fi
ts=$(find $pathtofile -type f 2>/dev/null)
if [ "$?" != "0" ]; then >&2 echo "Permission denied error"; exit 1; fi

#find numbers of files
ND=$(find $pathtofile -type d | egrep -v "\/.*$IGNOREARG.*" | wc -l)
NF=$(find $pathtofile -type f | egrep -v "\/.*$IGNOREARG.*" | wc -l)

#number of files
hB=$(find $pathtofile -type f -size -100c | egrep -v "\/.*$IGNOREARG.*" | wc -l)
maxsizes=$hB
oK=$(find $pathtofile -type f -size +99c -a -size -1024c | egrep -v "\/.*$IGNOREARG.*" | wc -l)
if [ $oK -gt $maxsizes ]; then maxsizes=$oK; fi
tK=$(find $pathtofile -type f -size +1023c -a -size -10240c | egrep -v "\/.*$IGNOREARG.*" | wc -l)
if [ $tK -gt $maxsizes ]; then maxsizes=$tK; fi
hK=$(find $pathtofile -type f -size +10239c -a -size -102400c | egrep -v "\/.*$IGNOREARG.*" | wc -l)
if [ $hK -gt $maxsizes ]; then maxsizes=$hK; fi
oM=$(find $pathtofile -type f -size +102399c -a -size -1048576c | egrep -v "\/.*$IGNOREARG.*" | wc -l)
if [ $oM -gt $maxsizes ]; then maxsizes=$oM; fi
tM=$(find $pathtofile -type f -size +1048575c -a -size -10485760c | egrep -v "\/.*$IGNOREARG.*" | wc -l)
if [ $tM -gt $maxsizes ]; then maxsizes=$tM; fi
hM=$(find $pathtofile -type f -size +10485759c -a -size -104857600c | egrep -v "\/.*$IGNOREARG.*" | wc -l)
if [ $hM -gt $maxsizes ]; then maxsizes=$hM; fi
oG=$(find $pathtofile -type f -size +104857599c -a -size -1073741824c | egrep -v "\/.*$IGNOREARG.*" | wc -l)
if [ $oG -gt $maxsizes ]; then maxsizes=$oG; fi
mG=$(find $pathtofile -type f -size +1073741823c | egrep -v "\/.*$IGNOREARG.*" | wc -l)
if [ $mG -gt $maxsizes ]; then maxsizes=$mG; fi

#main program
printf "Root directory: %s\n" $pathtofile
printf "Directories: %d\n" $ND
printf "All files: %d\n" $NF
echo "File size histogram:"

if [ $maxh -gt $maxsizes ]; then NORMALIZEDFLAG=0; fi
printf "  <100 B  : "; printhash $hB;
printf "  <1 KiB  : "; printhash $oK; 
printf "  <10 KiB : "; printhash $tK; 
printf "  <100 KiB: "; printhash $hK; 
printf "  <1 MiB  : "; printhash $oM; 
printf "  <10 MiB : "; printhash $tM; 
printf "  <100 MiB: "; printhash $hM; 
printf "  <1 GiB  : "; printhash $oG; 
printf "  >=1 GiB : "; printhash $mG; 

#File type histogram
echo "File type histogram:"
PROM="`find $pathtofile -type f -print0 | egrep -z -v "\/.*$IGNOREARG.*" | xargs -0 file -b | awk '{if(length($0)>40) printf "@%.40s...\n", $0; else printf "@%s\n", $0}' |sort | uniq -c | awk -F " @" '{printf $1; printf ":"; $1=""; print $0}' |sort -r -n -k 1 -t :`"

if [ "$?" != "0" ]; then >&2 echo "Unexpected error"; exit 1; fi

colm=$((NUMBEROFCOL-47))
#echo "$PROM"
#item_count * 100 / max_count
NORMALIZEDFLAG=$NORMALIZED
if [ ! -z "$PROM" ]; then 
	maxcount="$(printf $PROM | awk -F ":" '{printf "%d",$1;}')"
	if [ "$?" != "0" ]; then >&2 echo "Unexpected error"; exit 1; fi
	if [ $colm -gt $maxcount ]; then NORMALIZEDFLAG=0; fi

	#if ('"$NORMALIZED"'==1) {if(43+i < '"$NUMBEROFCOL"') printf "#";} else printf "#";
	echo "`echo "$PROM" |awk -v maxc=$maxcount -v colmax=$colm -F ":" '{printf " %-*s", 44, $2; printf ": "; if ('"$NORMALIZEDFLAG"'==1) {itempercento=$1*100/maxc; hashs=int( itempercento*colmax/100 ); poc=hashs;} else poc=$1; for (i=0; i<poc; i++) {printf "#"} printf "\n"}'| head -10`"
	if [ "$?" != "0" ]; then >&2 echo "Unexpected error"; exit 1; fi
fi

# predposledne column -t -s"@" | 
exit 0
