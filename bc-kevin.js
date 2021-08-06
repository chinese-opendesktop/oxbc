#!/usr/bin/node
var Complex = require('Complex');

var argv=process.argv;
var a,b,exprString="",topic="";

if (argv.length > 2) {
   for (a = 2 ; a < argv.length ; a++ ) {
                           
        for (b = 0 ; b < argv[a].length; b++ ) {        
         if ( argv[a][b] != ' ') {
            exprString += argv[a][b] ; 
            }
           }
      }
topic=exprString;      
console.log(cTOs(cal(exprString)));   

   }
else 
  console.log( "Usage:" + argv[0] + " ARITH_EXPR" ) ;           


function exprEval(exprString){
var iapos=-1,iaposadd=-1, iaposminus=-1, iaposmul=-1,iaposdiv=-1,iaposth_pow=-1 ;
var iawithin=-1;
var ia;
var pos=0,pos_pow=0;

for (ia=0;ia<=exprString.length;ia++) {
	switch(exprString[ia]){
         case '(':
         pos += 1;
         break;

         case ')':
         pos -= 1;
         break;
//~ ( iaposth_pow == -1  || iaposth_pow >= ia ) && 
         case '^':         
         if (iaposth_pow == -1 && pos == 0 ) {
             iaposth_pow=ia ;
            }
         break; 
             
         case '+':
          if (iaposadd==-1 && pos ==0 )
             {
              iaposadd=ia;
              }
         break;
 //~ iaposminus==-1 && 
         case '-':
          if (pos ==0 )
             {
              iaposminus=ia;
             }
          break;

         case '*':
          if (iaposmul==-1 && pos ==0 )
             {
              iaposmul=ia;  
             }
         break;
//~ iaposdiv==-1 && 
         case '/':
          if (pos ==0 )
              {
               iaposdiv=ia;
              }
         break;          
          }
}
     if (iaposadd!=-1)
         {
          iapos= iaposadd;
          }
     else if (iaposminus!=-1)
             {
              iapos= iaposminus ;
              }
     else if (iaposmul!=-1)
             {
            iapos= iaposmul ;
              }
     else if (iaposdiv!=-1)
             {
            iapos= iaposdiv ;
              } 
 	 else if (iaposth_pow!=-1)
       {
        iapos= iaposth_pow;
        }             

//↑用if判斷先乘除後加減並將值傳入iapos↑
   return iapos;
}

function cal(exprString){

var exprLeft, exprRight;
var result;

var iapos=exprEval(exprString);

exprLeft=exprString.substr(0,iapos);
exprRight=exprString.substr(iapos+1,exprString.length );
switch(exprString[iapos])
{
case '^':
result=cal(exprLeft).pow(cal(exprRight));
break;

case '+':
result=cal(exprLeft).add(cal(exprRight));
break;

case '*':
result=cal(exprLeft).multiply(cal(exprRight));
break;

case '-':
result=cal(exprLeft).subtract(cal(exprRight));
break;

case '/':
result=cal(exprLeft).divide(cal(exprRight));
break;
//↑判斷計算方式。
default:
if (exprString[0]=='(' && exprString[exprString.length-1]==')' )
   {
    result=cal(exprString.substr(1,exprString.length-2));
    }
else
     {
      result= sTOc(exprString);
      
      }
//↑如exprString[iapos]的值不符合前述任何計算方式則if判斷是否為括號，是 則去除括號;否（預設為無運算符號的常數） 則將其轉換格式後回傳。
}
return result; 
}

function sTOc(exprString ){

    var exprValue = new Complex("","");
//    console.log("exprValue : " +exprValue)

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
//    console.log("exprValue out="+exprValue)
    return  exprValue;
}
  
function cTOs(exprValue){
//    console.log("exprValue==" +exprValue);
    return exprValue.toString();
}
