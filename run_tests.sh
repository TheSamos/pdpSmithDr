#!/bin/bash

script_name=$0

usage()
{
    echo
    echo "Usage: ${script_name} [option=reset,run]"
    echo "       Options:"
    echo "           reset - remove previous build and start a new one"
    echo "           run - run tests from existing configuration"
    echo
}

usage

nb_param=$#

if [ ${nb_param} -ne 1 ] ; then
    exit 2
fi
option=$1

doReset=0
doRun=0

if [ ${option} == "run" ] ; then
    doRun=1
else
    if [ ${option} == "reset" ] ; then
	doReset=1
	doRun=1
    else
	echo "Unrecognized option: ${option}"
	exit 2
    fi
fi

#Number of compilation threads
#An optimal value is often the number of processors+1
nb_procs=1
unamestr=`uname`
case "$unamestr" in
    "Linux")
	nb_procs=`grep -c 'model name' /proc/cpuinfo 2>/dev/null`
	;;
    "Darwin")
	nb_procs=`sysctl -n hw.ncpu`
	;;
    *)
	echo "Unknown system"
	nb_procs=1
	;;
esac
nb_threads=`expr $nb_procs + 1`


# ----- compilation -----------
current_dir=`pwd`
script_dir="$(dirname "$(readlink -f ${BASH_SOURCE[0]})")"
cd ${script_dir}

build_dir="build"

if [ ! -d ${build_dir} ] ; then
    doReset=1
    doRun=1
fi

if [ ${doReset} -eq 1 ] ; then
    rm -rf ${build_dir}
    mkdir ${build_dir}
fi

cd ${build_dir}
build_command="make -j ${nb_threads}"

if [ ${doReset} -eq 1 ] ; then
    cmake -D CMAKE_BUILD_TYPE=Debug -D SMITHDR_TESTS=ON .. && ${build_command} # no GUI
fi


# ----- run the tests -----------
results_dir="tests_results"
rm -rf ${results_dir}
mkdir ${results_dir}
run()
{
    echo "Running $1"
    out=${results_dir}/`basename $1`
    valgrind -v --tool=memcheck --leak-check=full --show-reachable=yes --log-file=${out}_valgrind_output.txt $* > ${out}_exec_trace.txt 2>&1
}

if [ ${doRun} -eq 1 ] ; then
    run ./bin/test_checkerboard 100 100 10 ${results_dir}/checkerboard.pgm

    run ./bin/test_Fourier ${results_dir}/courbe.dat ../../../data/Lena.pgm ${results_dir}/Fourier_ForwBack2d.pgm

    run ./bin/test_frontend ../../../data/Lena.pgm vertical 0.4 ${results_dir}/Mirror.pgm ${results_dir}/Mix.pgm

    rm -rf ${results_dir}/Hartley && mkdir ${results_dir}/Hartley && ./bin/test_Hartley ../../../data/Lena.pgm 9 ${results_dir}/Hartley

    run ./bin/test_ImageFiltering ../../../data/Lena_noiseR20.pgm ${results_dir}/median.pgm median 5

    run ./bin/test_ImageRestoration ../../../data/Lena_uniform5.pgm 5 ${results_dir}

    run ./bin/test_io ../../../data/Lena.pgm ${results_dir}/out.pgm

    run ./bin/test_io_nifti1 ../../../data/t1_icbm_normal_3mm_pn3_rf20.hdr ${results_dir}/t1.nii

    run ./bin/test_iterators

    run ./bin/test_mesh

    run ./bin/test_parameter

    run ./bin/test_plugins ../../../data/addresses.txt

    run ./bin/test_RegionGrowing ../../../data/SheppLogan.pgm 2d 10 false ${results_dir}

    run ./bin/test_tomo ../../../data/sinogram.pgm radon ${results_dir}/radon.pgm
    run ./bin/test_tomo ../../../data/sinogram.pgm fbp ${results_dir}/fbp.pgm
    run ./bin/test_tomo ../../../data/sinogram.pgm sart ${results_dir}/sart.pgm
    run ./bin/test_tomo ../../../data/sinogram.pgm osem ${results_dir}/osem.pgm

fi


cd ${current_dir}