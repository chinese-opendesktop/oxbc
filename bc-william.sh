#!/bin/bash
# bc-william.sh
# (c) 2012 William Chao <william.chao@ossii.com.tw>
# MIT License

shopt -s extglob
declare -l zeroString='0000'
declare -i zeroLength=${#zeroString}

function findKeyOperator {
  declare -l exprString=$1
  declare -i exprLevel=0
  declare -i posPlus=-1
  declare -i posMinus=-1
  declare -i posTimes=-1
  declare -i posDivide=-1
  declare -i posPower=-1
  declare -i posFound=-1
  declare -i i=0
  while [ "$i" -lt ${#exprString} ] ; do
    exprChar=${exprString:$i:1}
    if [ \"$exprChar\" = '"("' -o \"$exprChar\" = '")"' ] ; then
      [ \"$exprChar\" = '"("' ] && let exprLevel+=1 || let exprLevel-=1
    elif [ "$exprLevel" -eq 0 ] ; then
      if [ \"$exprChar\" = '"+"' ] ; then
        posPlus=$i
      elif [ \"$exprChar\" = '"-"' ] ; then
        posMinus=$i
      elif [ \"$exprChar\" = '"*"' ] ; then
        posTimes=$i
      elif [ \"$exprChar\" = '"/"' ] ; then
        posDivide=$i
      elif [ \"$exprChar\" = '"^"' -a "$posPower" -eq -1 ] ; then
        posPower=$i
      fi
    fi
  i+=1
  done
  if [ $posPlus -gt 0 ] ; then
    posFound=$posPlus
  elif [ $posMinus -gt 0 ] ; then
    posFound=$posMinus
  elif [ $posTimes -gt -1 ] ; then
    posFound=$posTimes
  elif [ $posDivide -gt -1 ] ; then
    posFound=$posDivide
  elif [ $posPower -gt -1 ] ; then
    posFound=$posPower
  elif [ $posPlus -eq 0 -o $posMinus -eq 0 ] ; then
    posFound=0
  fi
  echo $posFound
}

function zeroAdd {
  declare -l valFloat=$1
  declare -i valLength=${#valFloat}
  declare -i posPoint=-1
  declare -i i=0
  while [ "$i" -lt "$valLength" ] ; do
    if [ "${valFloat:$i:1}" = '.' ] ; then
      posPoint=$i
    fi
    i+=1
  done
  declare -l valString=$valFloat$zeroString
  if [ "$posPoint" -ne -1 ] ; then
    valString=${valString/./}
    valString=${valString:0:$posPoint+$zeroLength}
  fi
  if [ ${valString:0:1} = '-' ] ; then
    declare -i valInt=-10#${valString:1}
  else
    declare -i valInt=10#$valString
  fi
  echo $valInt
}

function zeroSub {
  if [ "$1" -lt 0 ] ; then
    declare -l valInt=${1:1}
    declare -i negative=1
  else
    declare -l valInt=$1
    declare -i negative=0
  fi
  declare -i valLength=${#valInt}
  if [ "$valLength" -gt "$zeroLength" ] ; then
    declare -l valString=$valInt
  else
    declare -l valString=${zeroString:0:$zeroLength+1-$valLength}$valInt
  fi
  valLength=${#valString}
  declare -l valFloat=${valString:0:$valLength-$zeroLength}.${valString:$valLength-$zeroLength:$zeroLength}
  if [ "$negative" -eq 1 ] ; then
    valFloat=-$valFloat
  fi
  valFloat=${valFloat%%?(.)*(0)}
  echo $valFloat
}

function complex.add {
  declare -i valReal=`zeroAdd $1`+`zeroAdd $3`
  declare -i valImag=`zeroAdd $2`+`zeroAdd $4`
  declare -l exprReal=`zeroSub $valReal`
  declare -l exprImag=`zeroSub $valImag`
  echo $exprReal $exprImag
}
function complex.sub {
  declare -i valReal=`zeroAdd $1`-`zeroAdd $3`
  declare -i valImag=`zeroAdd $2`-`zeroAdd $4`
  declare -l exprReal=`zeroSub $valReal`
  declare -l exprImag=`zeroSub $valImag`
  echo $exprReal $exprImag
}
function complex.mul {
  declare -i valReal=`zeroAdd $1`*`zeroAdd $3`-`zeroAdd $2`*`zeroAdd $4`
  declare -i valImag=`zeroAdd $1`*`zeroAdd $4`+`zeroAdd $2`*`zeroAdd $3`
  declare -l exprReal=`zeroSub $valReal`
  declare -l exprImag=`zeroSub $valImag`
  exprReal=`zeroSub $exprReal`
  exprImag=`zeroSub $exprImag`
  echo $exprReal $exprImag
}
function complex.div {
  declare -i valDeno=`zeroAdd $3`*`zeroAdd $3`+`zeroAdd $4`*`zeroAdd $4`
  declare -i valReal=`zeroAdd $1`*`zeroAdd $3`+`zeroAdd $2`*`zeroAdd $4`
  declare -i valImag=`zeroAdd $2`*`zeroAdd $3`-`zeroAdd $1`*`zeroAdd $4`
  valReal=`zeroAdd $valReal`/$valDeno
  valImag=`zeroAdd $valImag`/$valDeno
  declare -l exprReal=`zeroSub $valReal`
  declare -l exprImag=`zeroSub $valImag`
  echo $exprReal $exprImag
}
function complex.pow {
  if [ "$3" != "${3/./}" -o "$4" -ne 0 ] ; then
    echo -n "Unsupported" >&2
    exit 1
  fi
  if [ "$3" -lt 0 ] ; then
    declare -i valDeno=`zeroAdd $1`*`zeroAdd $1`+`zeroAdd $2`*`zeroAdd $2`
    declare -i valReal=`zeroAdd $1`*1$zeroString*1$zeroString/$valDeno
    declare -i valImag=-1*`zeroAdd $2`*1$zeroString*1$zeroString/$valDeno
    declare -l exprReal=`zeroSub $valReal`
    declare -l exprImag=`zeroSub $valImag`
    declare -i valPower=-1*$3
  else
    declare -l exprReal=$1
    declare -l exprImag=$2
    declare -i valPower=$3
  fi
  declare -l exprValue="1 0"
  for i in `seq $valPower` ; do
    exprValue=`complex.mul $exprValue $exprReal $exprImag`
  done
  echo $exprValue
}

function sTOc {
  declare -l exprString=$1
  declare -i exprLength=${#exprString}
  declare -l exprValue="0 0"
  if [ "$exprLength" -ne 0 ] ; then
    if [ "${exprString:$exprLength-1}" = 'i' ] ; then
      if [ "$exprLength" -eq 1 ] ; then
        exprValue="0 1"
      else
        exprValue="0 ${exprString%i}"
      fi
    else
      exprValue="$exprString 0"
    fi
  fi
  echo $exprValue
}

function cTOs {
  declare -l exprReal=$1
  declare -l exprImag=$2
  declare -l exprString
  if [ `zeroAdd $exprImag` -eq 0 -o `zeroAdd $exprReal` -ne 0 ] ; then
    exprString=$exprReal
  fi
  if [ `zeroAdd $exprImag` -ne 0 ] ; then
    if [ `zeroAdd $exprImag` -gt 0 -a `zeroAdd $exprReal` -ne 0 ] ; then
      exprString+='+'
    fi
    if [ `zeroAdd $exprImag` -eq -1$zeroString ] ; then
      exprString+='-'
    elif [ `zeroAdd $exprImag` -ne 1$zeroString ] ; then
      exprString+=$exprImag
    fi
    exprString+='i'
  fi
  echo $exprString
}

function exprEval {
  declare -l exprString=$1
  declare -i exprLength=${#exprString}
  declare -i posFound=`findKeyOperator $exprString`
  declare -l exprValue="0 0"
  if [ "$posFound" -ne -1 ] ; then
    declare -l exprLeft=${exprString:0:$posFound}
    declare -l valLeft=`exprEval $exprLeft`
    declare -l exprRight=${exprString:$posFound+1}
    declare -l valRight=`exprEval $exprRight`
    case "${exprString:$posFound:1}" in
      '+') exprValue=`complex.add $valLeft $valRight` ;;
      '-') exprValue=`complex.sub $valLeft $valRight` ;;
      '*') exprValue=`complex.mul $valLeft $valRight` ;;
      '/') exprValue=`complex.div $valLeft $valRight` ;;
      '^') exprValue=`complex.pow $valLeft $valRight` ;;
    esac
  elif [ \"${exprString:0:1}\" = '"("' -a \"${exprString:$exprLength-1}\" = '")"' ] ; then
    exprString=${exprString:1:$exprLength-2}
    exprValue=`exprEval $exprString`
  else
    exprValue=`sTOc $exprString`
  fi
  echo "$exprValue"
}

if [ $# -eq 0 ] ; then
  echo "(c) 2012 OSSII, MIT License"
  echo "Usage: $0 ARITH_EXPR"
  exit 1
else
  declare -l exprString=`printf '%s' $@`
  declare -l exprValue=`exprEval $exprString`
  echo `cTOs $exprValue`
fi

