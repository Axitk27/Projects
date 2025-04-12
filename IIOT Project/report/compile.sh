#!/bin/bash

REPORTFILE=IIOT-Report.tex
LOGOUTPUT="$REPORTFILE-compile.log"

if [ ! -f $REPORTFILE ]; then
    echo "$REPORTFILE not found, exiting.";
    exit -1;
fi


echo "Generating report..."

xelatex -synctex=1 -interaction=nonstopmode $REPORTFILE 2>&1 > $LOGOUTPUT
biber ${REPORTFILE%.tex} 2>&1 >> $LOGOUTPUT
xelatex -synctex=1 -interaction=nonstopmode $REPORTFILE 2>&1 >> $LOGOUTPUT
xelatex -synctex=1 -interaction=nonstopmode $REPORTFILE 2>&1 >> $LOGOUTPUT
echo "Finished generation..."
