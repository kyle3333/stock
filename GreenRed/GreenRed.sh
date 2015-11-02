#!/bin/bash

####################Section 1######################
     #Pulls and edits the csv file from yahoo
###################################################

if ! [ -d $1 ]        #Checks if folder exsists
then                #if it does then
    mkdir $1        #Makes a new folder     
fi
numofdays=100       #Number of days to use in calculation
file=$1/$1.csv      #Filename of edited file
filefull=$1/$1"Full.csv"    #Filename of full file
wget -O $filefull "http://real-chart.finance.yahoo.com/table.csv?s="$1"&a=00&b=1&c=2010&d=00&e=1&f=2020&g=d&ignore=.csv"       #Pulls full file from yahoo using ticker symbol $1
sed "1d" < $filefull | cut -d ',' -f 1,5 > $file        #Cuts first line then (|) cuts all but the first and fith feild seperated by ',' 
date=$(tail -1 $file | cut -d ',' -f 1)         #Gets the lats life of $file and cuts all but the first feild seperated by ','
newfile=$(echo $file | cut -d '.' -f 1)GR$numofdays.csv     #Filename of GreenRed csv
linetotal=$(wc -l $file | cut -d ' ' -f 1)      #Gets the line total of the edited file

######################Section 2#########################
             #Generates GreenRed file by
             #making the calculations
########################################################

./mathfull $1 $linetotal $numofdays

#########################Section 3#############################
            #Plots the two graphs using gnuplot
            #by making a .pg script
###############################################################

echo "
#!/usr/bin/gnuplot

reset
set term jpeg size 1500,300
set datafile separator \",\"
set xdata time
set timefmt \"%Y-%m-%d\"
set format x \"%Y-%m\"
set xrange [\"$date\":\"2015-07-22\"]
set output \"$1/$1Graph.jpeg\"
plot \"$1/$1.csv\" using 1:2 with lines lc rgb \"blue\"
" > PlotB.pg && gnuplot PlotB.pg
echo "Built Blue Graph"

echo "
#!/usr/bin/gnuplot

reset
set term jpeg size 1500,300
set datafile separator \",\"
set xdata time
set timefmt \"%Y-%m-%d\"
set format x \"%Y-%m\"
set xrange [\"$date\":\"2015-07-22\"]
set output \"$1/$1GR"$numofdays"Graph.jpeg\"
plot \"$1/$1GR"$numofdays".csv\" using 1:3 with lines, \"\" using 1:2 with lines
" > PlotGR.pg && gnuplot PlotGR.pg
echo "Built Green/Red Graph"
echo
echo "Done!"
