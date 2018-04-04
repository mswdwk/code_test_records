function add(a,b)
    return a+b
end
alooptime=10000
function loop10k()
    for i=0,alooptime,1 do
    end
end

i=0
LOOP1=10000
while(i<LOOP1)
do
i=i+1
loop10k()
end
counter=LOOP1*alooptime
print("loop ",LOOP1,"*",alooptime," times")
--print( add(3,4))
