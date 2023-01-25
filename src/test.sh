#!/bin/bash

# shellcheck disable=SC2164
cd cmake-build-debug



dir=../../resources/labor4/examples2022
mw_dir=../../resources/labor4/maszyna_wirtualna
cmp_dir=../../../src/cmake-build-debug

for test in example1.imp example2.imp example3.imp example4.imp example5.imp mytest0.imp
do
cd $cmp_dir
./kompilator ${dir}/${test} ${mw_dir}/output.mr
cd $mw_dir
echo $PWD
./maszyna-wirtualna-cln ./output.mr > ./output/op_${test}
done
