#!/bin/bash

script_name=$0

usage()
{
    echo
    echo "Usage: ${script_name} [option=reset,configure,build]"
    echo "       Options:"
    echo "           reset - remove previous build and start a new one"
    echo "           configure - configure using GUI then build"
    echo "           build - build from existing configuration"
    echo
}

usage

nb_param=$#

if [ ${nb_param} -ne 1 ] ; then
    exit 2
fi
option=$1

doReset=0
doConfigure=0
doBuild=0

if [ ${option} == "build" ] ; then
    doBuild=1
else
    if [ ${option} == "configure" ] ; then
	doConfigure=1
	doBuild=1
    else
	if [ ${option} == "reset" ] ; then
	    doReset=1
	    doConfigure=1
	    doBuild=1
	else
	    echo "Unrecognized option: ${option}"
	    exit 2
	fi
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


# ----- do the job -----------
current_dir=`pwd`
script_dir="$(dirname "$(readlink -f ${BASH_SOURCE[0]})")"
cd ${script_dir}

build_dir="build"

if [ ! -d ${build_dir} ] ; then
    doReset=1
    doConfigure=1
    doBuild=1
fi

if [ ${doReset} -eq 1 ] ; then
    rm -rf ${build_dir}
    mkdir ${build_dir}
fi

cd ${build_dir}
build_command="make -j ${nb_threads}"
if [ ${doConfigure} -eq 1 ] ; then
    cmake .. && ${build_command}
else
    if [ ${doBuild} -eq 1 ] ; then
 	cmake .. && ${build_command} # no GUI
    fi
fi

cd ${current_dir}
