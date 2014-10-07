#!/bin/sh

#  Script.sh
#  compiller
#
#  Created by Данил on 30.09.14.
#  Copyright (c) 2014 Данил. All rights reserved.


for i in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36
do
echo "\n"test № $i
./DerivedData/compiller/Build/Products/Debug/compiller -f $i.in
if diff ./Test/Lexer/$i.exper ./Test/Lexer/$i.out >/dev/null ; then
echo OK
else
echo BAD
fi
done
exit 0;