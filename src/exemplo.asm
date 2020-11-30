TrIANGuLO: 
			EQU 1
SECTION TEXT	;linha dos dados
		INPUT		B
		INPUT		H
		LOAD		B
		MuL		H
		IF TRIANGULO
		DIV		DOIS
		STORE		R
		OUTPUT	R
		STOP
SECTION DATA
	B:		
		SPACE
	H:		
		SPACE
	R:		
	SPACE 5 
	DOIS:	CONST		2