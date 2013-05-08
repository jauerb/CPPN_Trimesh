#!/bin/bash
randSeed="1"
thread=""
options=""
baseDir="../HyperNEAT/out/tmp"
subDir=""


while getopts "r:t:s:w:h:a:g:b:" opt; do
  case $opt in
    r)
      randSeed=$OPTARG
      options="$options-r $OPTARG "
      ;;
    t)
      thread=$OPTARG
      options="$options-t $OPTARG "      
      ;;      
    s)
      options="$options-boxSpacing $OPTARG "            
      ;;            
    w)
      options="$options-boxWidth $OPTARG "                  
      ;;            
    h)
      options="$options-boxHeight $OPTARG "                  
      ;;               
    a)
      options="$options-boxAngle $OPTARG "                  
      ;;                
    g)
      options="$options-gravityAngle $OPTARG "                  
      ;;   
    b)
      baseDir=$OPTARG                 
      options="$options-baseDir $baseDir "
      ;;                           
    \?)
      echo "Invalid option: -$OPTARG "
      ;;
  esac
done

options="$options-baseDir $baseDir "

if [ "$thread" == "" ]
then
	subDir=`printf '%s/Files%d' $baseDir $randSeed`
else
	subDir=`printf '%s/Files%d_%d' $baseDir $randSeed $thread`	
fi

indicesFileName="$subDir/Indices.dat"
verticesFileName="$subDir/Vertices.dat"
tempFileName="$subDir/temp.dat"
resultsFileName="$subDir/Results.dat"
paramsFileName="$subDir/Params.dat"
readyFileName="$subDir/Ready.dat"


while true ;
do
	while [ ! -f $indicesFileName ] || [ ! -f $verticesFileName ] || [ ! -f $paramsFileName ] || [ ! -f $readyFileName ] ;
	do
		sleep 0.005 #sleep 5 ms
		if [ $? != 0 ]
		then
			echo 'WON"T SLEEP!'
			echo $PATH
			exit -1
		fi
	done
	./EvolveMorph $options-null -once
	if [ $? != 0 ]
	then
		echo 'ERROR: Evaluation Crashed, will return 0'
		echo '0' > $tempFileName
		rm $indicesFileName
		rm $verticesFileName

		mv $tempFileName $resultsFileName	
		touch $readyFileName	
	else
		echo ' ********************** EVALUATION FINISHED!! ***********************'
	fi
done
