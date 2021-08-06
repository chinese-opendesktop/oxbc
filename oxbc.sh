#!/bin/bash

cd /usr/libexec/oxbc

mybc=$1

if [ "$1" = "" ]; then
    echo "(c) 2012 OSSII, MIT License"
    echo "Usage: $0 [-simon-cpp|-robert-c|-john-js|-kevin-js|-william-sh|-simon-java|-robert-python] [ARITH_EXPR]"
else
    if [ -f bc$mybc ]; then
            shift
            if [ "$1" = "" ]; then
                while read line
                do
                    echo -n $line "=> " 
                    ./bc$mybc $line
                done < '/usr/libexec/oxbc/TESTS'
            else
                ./bc$mybc $@
            fi
    elif test -f bc-simon-java && java -version 2>/dev/null ; then
            ./bc-simon-java $@
    elif test -f bc-john-js && node -e 'require("Complex");' 2>/dev/null ; then
            ./bc-john-js $@
    elif test -f bc-kevin-js && node -e 'require("Complex");' 2>/dev/null; then
            ./bc-kevin-js $@
    elif [ -f bc-simon-cpp ]; then
            ./bc-simon-cpp $@
    elif [ -f bc-robert-python ]; then
            ./bc-robert-python $@
    elif [ -f bc-robert-c ]; then
            ./bc-robert-c $@
    elif [ -f bc-william-sh ]; then
            ./bc-william-sh $@
    fi
fi
