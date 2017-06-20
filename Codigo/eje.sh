rm sal
rm *.txt
g++ -O3 --std=c++11 *.cpp -o sal 2>&1 >/dev/null | grep -v -e '^/var/folders/*' -e '^[[:space:]]*\.section' -e '^[[:space:]]*\^[[:space:]]*~*'