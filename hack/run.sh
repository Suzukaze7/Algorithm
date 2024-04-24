#chmod +x ...

src="$1.cpp"
g++ $src -std=c++17 -DLTNS -include debug.cpp
if [ $# == 1 ]
then
    ./a.out
else
    in="$2.in"
    cat $in | ./a.out
fi
echo