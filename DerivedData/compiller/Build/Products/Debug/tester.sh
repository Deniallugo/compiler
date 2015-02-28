#
#echo  >Declaration/outfile.out
#for ((a=1; a <= 52 ; a++))
#do
#
#echo $a " Test" >> Declaration/outfile.out
#cat Declaration/$a.in >> Declaration/outfile.out
#echo  "\n" out File>> Declaration/outfile.out
#./compiller -pd Declaration/$a.in >> Declaration/outfile.out
#echo  "\n" >> Declaration/outfile.out
#
##if diff Declaration/$a.out Declaration/$a.exm
##
##then    echo OK >> Declaration/outfile.out
##else    echo bad >> Declaration/outfile.out
##fi
#done
#open Declaration/outfile.out;

#
#echo  >Expression/outfile.out
#
#for ((a=1; a <= 100 ; a++))
#do
#echo $a " Test" >> Expression/outfile.out
#cat Expression/$a.in >> Expression/outfile.out
#echo  "\n" out File>> Expression/outfile.out
#
#./compiller -pe Expression/$a.in >> Expression/outfile.out
#echo  "\n" >> Expression/outfile.out
#
##if diff Expression/$a.out Expression/$a.exm
##
##then    echo OK
##else    echo bad
##fi
##
#done
#open Expression/outfile.out;

echo  >TypeChecking/outfile.out

for ((a=1; a <= 180 ; a++))
do
echo Test $a
echo $a " Test" >> TypeChecking/outfile.out
cat TypeChecking/$a.in >> TypeChecking/outfile.out
echo  "\n\n"out File>> TypeChecking/outfile.out
#./compiller -pd TypeChecking/$a.in >> TypeChecking/outfile.out
./compiller -pd TypeChecking/$a.in > TypeChecking/$a.out
echo  "\n" >> TypeChecking/outfile.out
if diff TypeChecking/$a.out TypeChecking/$a.exm
echo "\n"
then    echo OK
cat TypeChecking/$a.out >> TypeChecking/outfile.out
else    echo bad
open TypeChecking/$a.exm
open TypeChecking/$a.in


fi
done
open TypeChecking/outfile.out



#
#fi
#
#done
#open TypeChecking/*.in
#open TypeChecking/*.out


#
#echo  >Statements/outfile.out
#
#for ((a=1; a <= 26 ; a++))
#do
#echo Test $a
#echo $a " Test" >> Statements/outfile.out
#cat Statements/$a.in >> Statements/outfile.out
#echo  "\n\n"out File>> Statements/outfile.out
##./compiller -pd TypeChecking/$a.in >> TypeChecking/outfile.out
#./compiller -pd Statements/$a.in > Statements/$a.out
#echo  "\n" >> Statements/outfile.out
#if diff Statements/$a.out Statements/$a.exm
#then    echo OK
#cat Statements/$a.out >> Statements/outfile.out
echo "\n"
#else    echo bad
#echo BAD TEST >> Statements/outfile.out
#open Statements/$a.out
#open Statements/$a.in
#
#
#fi
#done
#open Statements/outfile.out