#!/usr/bin/python
# -*- coding: utf-8 -*- 

# 
#   bc-robert.py
#   modify from bc-simon.cpp by Simon Sun <simon.sun@ossii.com.tw>
#
#  Fri Mar 1 2013 Robert Wei <robert.wei@ossii.com.tw>
#  
#  Version: 0.1-1
#  
#  MIT License

import sys

def sTOc(exprString):
    if exprString:
        if exprString[-1] == 'i':
            if exprString == 'i':
                exprValue = 1j
            else:
                exprValue = float(exprString[0:-1]) * 1j
        else:
            exprValue = float(exprString)
    else:
        exprValue = 0

    return  exprValue


def findKeyOperator(exprString):
    posPlus = -1
    posMinus = -1
    posTimes = -1
    posDivide = -1
    posPower = -1
    posFound = -1
    exprLevel = 0

    for i in range(0, len(exprString)):
        if exprString[i] == ')':
            exprLevel += 1
        elif exprString[i] == '(':
            exprLevel -= 1
        elif exprString[i] == '+': 
            if exprLevel == 0:
                posPlus = i 
        elif exprString[i] == '-': 
            if exprLevel == 0:
                posMinus= i
        elif exprString[i] == '*': 
            if exprLevel == 0:
                posTimes= i
        elif exprString[i] == '/': 
            if exprLevel == 0:
                posDivide= i
        elif exprString[i] == '^':
            if posPower == -1 and exprLevel == 0:
                posPower= i

    if posPlus > 0:
        posFound = posPlus
    elif posMinus > 0:
        posFound = posMinus
    elif posTimes != -1:
        posFound = posTimes
    elif posDivide != -1:
        posFound = posDivide
    elif posPower != -1:
        posFound = posPower
    elif posPlus == 0 or posMinus == 0:
        posFound = 0
    
    return posFound


def exprEval(exprString):
    posFound = findKeyOperator(exprString);
    
    if posFound != -1:
        exprLeft = exprString[ :posFound]
        exprRight = exprString[posFound+1: ]
        if exprString[posFound] == '+':
            exprValue = exprEval(exprLeft) + exprEval(exprRight)                
        elif exprString[posFound] == '-': 
            exprValue = exprEval(exprLeft) - exprEval(exprRight)     
        elif exprString[posFound] == '*': 
            exprValue = exprEval(exprLeft) * exprEval(exprRight)
        elif exprString[posFound] == '/': 
            exprValue = exprEval(exprLeft) / exprEval(exprRight)
        elif exprString[posFound] == '^': 
            exprValue = complex(exprEval(exprLeft)) ** exprEval(exprRight)
    else:
        if exprString and exprString[0] == '(' and exprString[-1] == ')':
            exprString = exprString[1:-1]
            exprValue = exprEval(exprString)
        else:
            exprValue =  sTOc(exprString)

    return exprValue

def argvTOs(argv):
    exprString = ''
    for i in range(1, len(argv)):
        argvString = argv[i].split(' ')
        for k in argvString:
            exprString += k
    return exprString

def cTOs(exprValue):
    exprStream = ""
    exprReal = (exprValue.real+1)-1
    exprImag = (exprValue.imag+1)-1
    if exprReal or not exprImag:
        s = str(exprReal)
        if s[-2:] == '.0':
            s = s[:-2]
        exprStream += s
    
    if exprImag != 0:
        if exprReal and exprImag > 0:
            exprStream += '+'
        
        if exprImag == -1:
            exprStream += '-'
        elif exprImag != 1:
            s = str(exprImag)
            if s[-2:] == '.0':
                s = s[:-2]
            exprStream += s
        
        exprStream += 'i'
       
    return  exprStream


if len(sys.argv) > 1: 
    exprString = argvTOs(sys.argv)
    totalValue = exprEval(exprString)
    print cTOs(totalValue)
else:
    sys.exit("(c) 2013 OSSII, MIT License\n"
            + "Usage: " + sys.argv[0] + " ARITH_EXPR")
