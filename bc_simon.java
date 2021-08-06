import java.*;
//~ import java.text.NumberFormat;

public class bc_simon {

    private static String argsTOs(String[] args){
        String exprString ="";  
         if( args.length >= 1 ){
            for (int i=0;i<args.length;i++){
                exprString += args[i].replaceAll(" ", "");
            }
        }else{
            exprString = "no";
        } 
        return exprString;
    }

    public static void main (String[] args){
        String exprString = argsTOs(args);
        ArithExpr arithExpr = new ArithExpr();
        arithExpr.expr=exprString;
        double exprVal = arithExpr.exprEval(exprString);
        System.out.println(exprVal);
    }
}

class ArithExpr {
    String expr;
    
    //~ public ArithExpr(String expr){
        //~ super();
    //~ }
    
    //~ void alert() {
        //~ System.out.println("void alert() : "+expr);
    //~ }
    
    private int findKeyOperator(String exprString){
        int posPlus = -1 , posMinus = -1 , posTimes = -1 , posDivide = -1 , posPower = -1 , posFound = -1 ;
        int exprLevel = 0 ;

        for (int i = exprString.length()-1 ; i  >= 0 ; i-- ){
            switch(exprString.charAt(i)) { 
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
    
    private double sTOc(String exprString){
        double exprValue;
        exprValue = Double.parseDouble(exprString);

        return  exprValue;
    }
    
    
    public double exprEval(String exprString){

        int pos = 0 , posFound;
        double exprValue = 0 ;
        String  exprLeft = "", exprRight = "" ;
    
        posFound = findKeyOperator(exprString);

        if (posFound != -1){
            exprLeft = exprString.substring(0,posFound);            
            exprRight = exprString.substring(posFound+1,exprString.length());
            switch(exprString.charAt(posFound)){
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
            }
        }
        else{
            if(exprString.charAt(0) == '(' && exprString.charAt(exprString.length()-1) == ')'){
                exprString =  exprString.substring(1,exprString.length()-1) ;
                exprValue = exprEval(exprString);
            }else{
                exprValue =  sTOc(exprString) ;
            }
        }
        return exprValue; 		
    }
}

