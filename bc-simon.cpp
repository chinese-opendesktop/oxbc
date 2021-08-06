 /*
 * bc-simon.cpp
 * 
 * Thu Oct 4 2012 Simon Sun <simon.sun@ossii.com.tw>
 * 
 * Version: 0.1-1
 * 
 * MIT License
 * 
 */
 
 
#include <iostream>
#include <string>
#include <complex>
#include <sstream>
using namespace std;

complex<double> sTOc(string exprString){
     complex<double> exprValue = 0 ;
     if(exprString.length() != 0){
         if (exprString[exprString.length()-1] == 'i'){
            if(exprString.length() == 1){
                exprValue = complex<double>(0,1);
            }else{
            exprValue = complex<double>(0,stod(exprString.substr(0,exprString.length()-1)));
            }
        }else{
            exprValue = complex<double>(stod(exprString),0);
        }
    }
    return  exprValue;
}

int findKeyOperator(string exprString)
{
    int posPlus = -1 , posMinus = -1 , posTimes = -1 , posDivide = -1 , posPower = -1 , posFound = -1 ;
    int exprLevel = 0 ;
    
    //~ for (int i = 0 ; i < exprString.length()-1 ; i++ ){
        //~ switch(exprString[i]) { 
            //~ case ')':
                //~ exprLevel += 1 ;
                //~ break;
            //~ case '(':
                //~ exprLevel -= 1 ;
                //~ break;
            //~ case '+': 
                //~ if (exprLevel == 0 ){
                    //~ posPlus = i ;
                //~ }
                //~ break; 
            //~ case '-': 
                //~ if ( exprLevel == 0  ){
                    //~ posMinus= i ;
                //~ }
                //~ break; 
            //~ case '*': 
                //~ if ( exprLevel == 0  ){
                    //~ posTimes= i ;
                //~ }
                //~ break; 
            //~ case '/': 
                //~ if ( exprLevel == 0  ){
                    //~ posDivide= i ;
                //~ }
                //~ break;
            //~ case '^':
                //~ if ( posPower == -1 && exprLevel == 0  ){
                    //~ posPower= i ;
                //~ }
                //~ break;
        //~ } 
    //~ }
    
    for (int i = exprString.length()-1 ; i  >= 0 ; i-- ){
        switch(exprString[i]) { 
            case ')':
                exprLevel += 1 ;
                break;
            case '(':
                exprLevel -= 1 ;
                break;
            case '+': 
                if ( posPlus == -1 && exprLevel == 0 ){
                    posPlus = i ;
                }
                break; 
            case '-': 
                if ( posMinus == -1 && exprLevel == 0  ){
                    posMinus= i ;
                }
                break; 
            case '*': 
                if ( posTimes == -1 && exprLevel == 0  ){
                    posTimes= i ;
                }
                break; 
            case '/': 
                if ( posDivide == -1 && exprLevel == 0  ){
                    posDivide= i ;
                }
                break;
            case '^':
                if ( posPower == -1 && exprLevel == 0  ){
                    posPower= i ;
                }
                break;
        } 
    }
    
    if( posPlus > 0 ){
        posFound = posPlus ;
    }
    else if ( posMinus > 0 ){
        posFound = posMinus ;
    }
    else if ( posTimes != -1 ){
        posFound = posTimes ;
    }
    else if ( posDivide != -1 ){
        posFound = posDivide ;
    }
    else if ( posPower != -1 ){
        posFound = posPower;
    }
    else if (posPlus==0 || posMinus ==0){
        posFound = 0;
    }
    
    return posFound;
}


complex<double> exprEval(string exprString)			
{	
    int pos = 0 , posFound;
    complex<double> exprValue = 0 ;
    string  exprLeft = "", exprRight = "" ;

    posFound = findKeyOperator(exprString);
    
    if (posFound != -1){
        exprLeft = exprString.substr(0,posFound);            
        exprRight = exprString.substr(posFound+1,exprString.length()-1);    
        switch(exprString[posFound]){
                case '+': 
                    exprValue = exprEval(exprLeft) + exprEval(exprRight);                
                    break; 
                case '-': 
                    exprValue = exprEval(exprLeft) - exprEval(exprRight);      
                    break; 
                case '*': 
                    exprValue = exprEval(exprLeft) * exprEval(exprRight);      
                    break; 
                case '/': 
                    exprValue = exprEval(exprLeft) / exprEval(exprRight);
                    break; 
                case '^': 
                    exprValue = pow(exprEval(exprLeft) , exprEval(exprRight));
                    break; 
        }
    }else{
        if(exprString[0] == '(' && exprString[exprString.length()-1] == ')'){
            exprString =  exprString.substr(1,exprString.length()-2) ;
            exprValue = exprEval(exprString);
        }else{
            exprValue =  sTOc(exprString) ;
        }
    }
    return exprValue; 		
}

string argvTOs(int argc , char* argv[]){
    string exprString , argvString;
    for(int i=1; i<argc;i++){
        argvString = string(argv[i]);
        for(int k=0 ; k<argvString.length() ; k++){
            if(argvString[k] != ' '){
                exprString +=argvString[k];
            }
        }
    }
    return exprString;
}

string cTOs(complex<double>  exprValue){
    stringstream exprStream;
    float exprReal = float(exprValue.real()+1)-1;
    float exprImag = float(exprValue.imag()+1)-1;
    if( exprImag == 0 || exprReal != 0){
        exprStream << exprReal;
    }
    if (exprImag != 0 ){
        if (exprImag > 0 && exprReal != 0){
            exprStream << '+';
        }
        if (exprImag == -1){
            exprStream << '-';
        } else if (exprImag != 1){
            exprStream << exprImag;
        }
        exprStream << 'i';
    }
    return exprStream.str();
}

int main(int argc, char* argv[]){
    complex<double> totalValue;
    string exprString;
    if( argc > 1){ 
        exprString = argvTOs(argc,argv);
        totalValue = exprEval(exprString);
        cout << cTOs(totalValue) <<endl;    
    }   
    else{
        cout << "(c) 2012 OSSII, MIT License\n";
        cout << "Usage: "<<argv[0] <<" ARITH_EXPR\n" ;
        return 1;
    }
}

