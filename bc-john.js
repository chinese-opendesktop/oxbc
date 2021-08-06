#!/usr/bin/node
var Complex = require('Complex'); //引用Complex

function ArithExpr(expr){
        this.expr=expr;
    }

ArithExpr.prototype={
    exprEval : function (exprString){
        var posFound = this.findKeyOperator(exprString);
        
        var pos = 0 , exprValue = 0 , exprLeft = "" , exprRight = "";
        if (posFound != -1){
            exprLeft = exprString.substr(0,posFound);            
            exprRight = exprString.substr(posFound+1,exprString.length-1);
            if(this.exprEval(exprLeft)==undefined){ exprLeft=0  }  //如果前面沒有值則轉成0  
            switch(exprString[posFound]){ 
                   case '+': 
                        exprValue = this.exprEval(exprLeft).add(this.exprEval(exprRight));                           
                        break;
                    case '-': 
                        exprValue = this.exprEval(exprLeft).subtract(this.exprEval(exprRight));       
                        break; 
                    case '*': 
                        exprValue = this.exprEval(exprLeft).multiply(this.exprEval(exprRight));     
                        break; 
                    case '/': 
                        exprValue = this.exprEval(exprLeft).divide(this.exprEval(exprRight));  
                        break; 
                    case '^': 
                        exprValue = this.exprEval(exprLeft).pow(this.exprEval(exprRight));  
                        break; 
            }
        }else{
            if(exprString[0] == '(' && exprString[exprString.length-1] == ')'){
                exprString =  exprString.substr(1,exprString.length-2) ;
                exprValue = this.exprEval(exprString);
            }else{
                exprValue =  this.sTOc(exprString);
            }
        }
    return exprValue; 		
    },

    findKeyOperator : function (exprString){
        var posAdd = -1 , posSub = -1 , posMul = -1 , posDiv = -1 , posPow = -1 , posFound = -1 , exprLevel = 0 ;
    
        for (var i = 0 ; i  <= exprString.length-1 ; i++ ){
            switch(exprString[i]) { 
                case ')':
                    exprLevel += 1 ;
                    break;
                case '(':
                    exprLevel -= 1 ;
                    break;
                case '+': 
                    if( exprLevel == 0 )
                        posAdd = i ;                
                    break; 
                case '-': 
                    if ( exprLevel == 0 )//必須重後面往前推
                        posSub= i ;                
                    break; 
                case '*': 
                    if ( exprLevel == 0 )
                        posMul= i ;                
                    break; 
                case '/': 
                    if ( exprLevel == 0 )//必須重後面往前推
                        posDiv= i ;                
                    break;
                case '^':
                    if( posPow==-1 && exprLevel == 0 )
                            posPow= i ;                
                    break;
            } 
        }
            
        if( posAdd > 0 ){
            posFound = posAdd ;
        }
        else if ( posSub > 0 ){
            posFound = posSub ;
        }
        else if ( posMul != -1 ){
            posFound = posMul ;
        }
        else if ( posDiv != -1 ){
            posFound = posDiv ;
        }
        else if ( posPow != -1 ){
            posFound = posPow;
        }
        else if (posAdd==0 || posSub ==0){
            posFound = 0;
        }
        return posFound;    
    },
    
    sTOc : function (exprString){
        var  exprReal , exprImg;
        var exprValue;
         if(exprString.length != 0){
             if (exprString[exprString.length-1] == 'i'){
                if(exprString.length == 1){
                    exprValue = new Complex(0,1);
                }else{
                    exprValue = new Complex(0,parseFloat(exprString.substr(0,exprString.length-1)));
                }
            }else{
                exprValue = new Complex(parseFloat(exprString),0);
            }
        }    
        return  exprValue;
    },
    
     cTOs : function (exprValue){
        if(exprValue.im==0){
            exprValue=(parseFloat(exprString.real.toString()).toFixed(10))*1  //把答案四捨五入再回傳回去
        }else{
            exprValue=exprValue.toString()
            }
        return exprValue;
    }
}

var AE=new ArithExpr();

if(process.argv[2]){
    var exprString=process.argv.slice(2).join("").replace(/ /g,"");
     exprString = AE.exprEval(exprString);//呼叫function exprEval
     console.log(AE.cTOs(exprString));
}else{      
    console.log("(c) 2012 OSSII, MIT License")  
    console.log("Usage: "+process.argv[1]+" ARITH_EXPR");
}
