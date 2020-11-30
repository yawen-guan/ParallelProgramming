# maybe you should run it as root

echo "N = 300"

g++ dataMaker.cpp -o datamake -g
./datamake

g++ mainNormal.cpp -o normal -g
perf stat ./normal
perf stat -e cache-misses,cycles,instructions,mem-loads  ./normal

g++ mainDAG.cpp -o dag -g
perf stat ./dag
perf stat -e cache-misses,cycles,instructions,mem-loads  ./dag

# g++ mainStrassen_normal_ptr.cpp -o str -g
# perf stat ./str
# perf stat -e cache-misses,cycles,instructions,mem-loads  ./str

