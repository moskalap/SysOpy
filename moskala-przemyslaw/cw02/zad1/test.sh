#!/bin/bash
by="bytes"
txt=".txt"
cp="cp"


for records in 1000 10000
do
	for bytes in 4 512 4096 8192
	do
		./fio -file $bytes$by$records$txt -fsize $records -rsize $bytes -fun generate
		for function in shuffle sort
		do
		
				for v in l s
				do
					rm $bytes$by$records$cp$txt
					cp $bytes$by$records$txt  $bytes$by$records$cp$txt
					echo "$function $bytes$by$records version $v" >> wyniki.txt;
					{ time ./fio -file $bytes$by$records$txt -fsize $records -rsize $bytes -fun $function -version $v 2> wyniki.stderr ; } 2>> wyniki.txt
						
				done
	
		
			
		done
	done
done

