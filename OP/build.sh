make clean

make

rm ./temp.txt

cat ./itemList.txt | \
while read CMD; do
    echo $CMD >> ./temp.txt
        echo $CMD >> ./temp.txt
done

cp temp.txt temp.txt.tmp

sed '$ d' temp.txt.tmp > ./temp.txt
tail -n +2 ./temp.txt > list.txt

rm -f temp.txt.tmp
rm -f temp.txt