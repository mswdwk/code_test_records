#!/bin/bash
gi=0
ALOOPTIME=10000
LOOP=100
function loop10k()
{
    declare -i i;
    i=0
    while [ $i -lt $ALOOPTIME ];
    do
    let i=$i+1;
    done
    gi=$i
}

#loop10k

for((i=0;i<$LOOP;i++));do
    loop10k
done
counter=`expr $LOOP \* $ALOOPTIME`
echo "loop $LOOP * $ALOOPTIME times"
