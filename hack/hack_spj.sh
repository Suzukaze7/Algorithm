g++ input.cpp -o input
g++ hacked.cpp -o hacked
g++ spj.cpp -o spj

for ((cnt=1;; cnt++))
do
    echo -ne "\r${cnt}th hack"

    ./input > data.in
    ./hacked < data.in > data.result
    ./spj < data.in < data.result > data.out

    out=$(cat data.out)
    if [ "${out}" != "ok" ]
    then
        echo -e "\nhack success: ${out}"
        break
    fi
done