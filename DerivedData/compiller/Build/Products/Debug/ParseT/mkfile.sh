for ((a=12; a <= 100 ; a++))
do
    cat $a.in ./    abc/$a.in >> $a.in

done