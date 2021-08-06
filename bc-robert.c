/*
 * bc-robert.c
 * 
 * Thu Oct 17 2012 Robert D. Wei <robert.wei@ossii.com.tw>
 * 
 * Version: 0.1.1
 * 
 * License: GPL 2.0
 * 
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define deEXPRESS AddSubtract
#define deUNIT PeelParenthes
#define deNUMBER GetNumber

#define ADD_SUB_L deEXPRESS
#define ADD_SUB_R MultiplieDivise
#define MUL_DIV_L MultiplieDivise
#define MUL_DIV_R ToThePowerOf
#define POW_BASE deUNIT
#define POW_EXP ToThePowerOf
#define IN_PRNTH deEXPRESS
#define NO_PRNTH	deNUMBER

void LinkAgrements (char* ExpressString, int argc, char* argv[] ) ;
char* RemoveSpaces (char* ExpressString) ;
float AddSubtract (const char* SubString, const char* StringTail) ;
float MultiplieDivise (const char* SubString, const char* StringTail) ;
float ToThePowerOf (const char* SubString, const char* StringTail) ;
float PeelParenthes (const char* SubString, const char* StringTail) ;
float GetNumber (const char* SubString, const char* StringTail) ;

int main (int argc, char* argv[]) {
	int Length = 0, Index ;
	for ( Index = 1 ; Index < argc ; Index++ )
		Length += strlen ( argv [Index] ) ;
	char ExpressString [ Length + 1 ] ;
	char* StringTail ;

	LinkAgrements ( ExpressString, argc, argv ) ;

	StringTail = RemoveSpaces ( ExpressString ) ;

	if ( StringTail >= ExpressString )
		printf ( "%f\n" , deEXPRESS ( ExpressString, StringTail )) ;
	else {
		printf ( "Usage: %s ARITH_EXPR\n" , argv [0] ) ;
	}
}

void LinkAgrements (char* ExpressString, int argc, char* argv[] ) {
	char *Origin, *Target = ExpressString ;
	int Index ;

	for ( Index = 1 ; Index < argc ; Index++ ) {
		Origin = argv [Index] ;
		while ( *Origin != '\0' ) {
			*Target = *Origin ;
		Origin++ ;	Target++ ;	}
	}
	*Target = '\0';
}

char* RemoveSpaces (char* ExpressString) {
	char *Origin = ExpressString , *Target = Origin - 1 ;

	while ( *Origin != '\0' ) {
		if ( *Origin != ' ' && *Origin!='\n'
				&& *Origin!='\'' && *Origin!='\"' ) {
			Target++ ;
			*Target = *Origin ;
		}
	Origin++ ; }
	
	return Target ; /* Return the new tail of the string. */
}

float AddSubtract (const char* SubString, const char* StringTail) {
	const char *Point ;
	int Chack = 0 ;

	for ( Point = StringTail ; Point > SubString ; Point-- ) {
		/* If there are, skip characters between parenthes pair.  */
		if ( *Point == ')' ) {
			Chack++ ;
			while ( Point > SubString && Chack != 0 ) {
				Point-- ;
				switch ( *Point ) {
					case ')' : Chack++ ; break ;
					case '(' : Chack-- ; break ;
				}
			}}
		/* Find '+'/'-' that is not a  number sign but an operator. */	
		else if ( *Point == '+' || *Point == '-' ) {
			if ( *(Point - 1) != '*' && *(Point - 1) != '/'
			 && *(Point - 1) != '^' && *(Point - 1) != '(' )
				break ; /* Found! Leave the for loop.*/
		}
	}
	
	if ( Point <= SubString )
		return ADD_SUB_R ( SubString , StringTail ) ;
	else if ( *Point == '+' )
		return ADD_SUB_L (SubString, Point-1) + ADD_SUB_R (Point+1, StringTail) ;
	else /* *Point == '-' */
		return ADD_SUB_L (SubString, Point-1) - ADD_SUB_R (Point+1, StringTail) ;
}

float MultiplieDivise (const char* SubString, const char* StringTail) {
	const char *Point ;
	int Chack = 0 ;

	for ( Point = StringTail ; Point > SubString ; Point-- ) {
		/* If there are, skip characters between parenthes pair.  */
		if ( *Point == ')' ) {
			Chack++ ;
			while ( Point > SubString && Chack != 0 ) {
				Point-- ;
				switch ( *Point ) {
					case ')' : Chack++ ; break ;
					case '(' : Chack-- ; break ;
				}
			}}
		else if ( *Point == '*' || *Point == '/' )
			break ; /* Found operator, leave the for loop.*/
	}
	
	if ( Point <= SubString )
		return MUL_DIV_R ( SubString , StringTail ) ;
	else if ( *Point == '*' )
		return MUL_DIV_L (SubString, Point-1) * MUL_DIV_R (Point+1, StringTail) ;
	else /* *Point == '/' */
		return MUL_DIV_L (SubString, Point-1) / MUL_DIV_R (Point+1, StringTail) ;
}

float ToThePowerOf (const char* SubString, const char* StringTail) {
	const char *Point ;
	int Chack = 0 ;

	for ( Point = SubString ; Point < StringTail ; Point++ ) {
		/* If there are, skip characters between parenthes pair.  */
		if ( *Point == '(' ) {
			Chack++ ;
			while ( Point < StringTail && Chack != 0 ) {
				Point++ ;
				switch ( *Point ) {
					case '(' : Chack++ ; break ;
					case ')' : Chack-- ; break ;
				}
			}}
		else if ( *Point == '^' )
			break ; /* Found operator, leave the for loop.*/
	}
	
	if ( Point >= StringTail )
		return POW_BASE ( SubString , StringTail ) ;
	else /* *Point == '^' */
		return pow ( POW_BASE ( SubString , Point - 1 ) ,
						POW_EXP ( Point + 1 , StringTail )   ) ;
}

float PeelParenthes (const char* SubString, const char* StringTail) {

	if ( *StringTail == ')' ) {
		if ( *SubString == '-' )
			return -1 * IN_PRNTH ( SubString + 2 , StringTail - 1) ;
		else
			return IN_PRNTH ( SubString + 1 , StringTail - 1) ; }
	else return NO_PRNTH ( SubString , StringTail ) ;
}

float GetNumber (const char* SubString, const char* StringTail) {
	char Tmp ;
	float Number ;

	Tmp = *( (char*) StringTail + 1) ;
	*( (char*) StringTail + 1) = '\0';
	
		Number = atof ( SubString ) ;
		
	*( (char*) StringTail + 1) = Tmp;

	return Number ;
}
