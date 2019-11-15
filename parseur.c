#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globaleVar.h"

#define MAX_LIGNE 100


#define E_JSP					0 
#define E_ACTION				1
#define E_TACHE					2
#define E_ASSOCIATION_OPTION	3
#define E_ASSOCIATION_TACHE		4
#define E_ASSOCIATION_ACTION	5
#define E_LISTE_OPTIONS_ACTION	6
#define E_LISTE_OPTIONS_TACHE	7
#define E_COMMENTAIRE			1000


// Structures de donnees


struct Option {
	
	char *cle ;
	char *valeur ;
} ;

struct Action {
	
	char *nom ;
	char *url ;
	
	int nbOptions ;
	struct Option *options ;
} ;

struct Tache {
	
	char *nom ;
	int secondes ;
	int minutes ;
	int heures ;
	
	int nbActions ;
	struct Action **actions ; 
} ;



// Découpe une ligne pour en extraire une cle et une valeur

void getAssociation( char *l , char *c , char *v ){
	
	int i = 1 ;
	int j = 0 ;
	
	
	// Enlever espaces début
	while( l[ i ] == ' ' ){
		i++ ;
	}
	
	
	// Recuperer la cle
	while( l[ i ] != ' ' ){
		c[ j ] = l[ i ] ;
		i++ ;
		j++ ;
	}
	c[ j ] = '\0' ;
	
	// On passe la ->
	while( l[ i ] == ' ' || l[ i ] == '-' || l[ i ] == '>' ){
		i++ ;
	}
	
	j = 0 ;
	
	
	// Recuperer la valeur
	while( l[ i ] != ' ' && l[ i ] != '}' ){
		v[ j ] = l[ i ] ;
		i++ ;
		j++ ;
	}
	v[ j ] = '\0' ;
	
	printf( "---> [%s][%s]\n" , c , v ) ;
	
	
}

// Retourner un pointeur sur l'action dont on connait le nom
struct Action * getAction( struct Action * actions , int nbActions , char *nom ){
	
	int i = 0 ;
	for( i = 0 ; i < nbActions ; i++ ){
		
		if( strcmp( actions[ i ].nom , nom ) == 0 ){
			return &( actions[ i ] ) ;					// Retourner l'adresse de l'action
														// Adresse de l'action d'indice i dont le nom correspond au nom recherche
		}
	}
	
	return NULL ;
	
}

// Obtenir la liste des noms d'une action
char** getNomActions( char *s , int *n ){
	
	char **t = NULL  ;
	
	int j = 0 ;
	char buffer[ 100 ] ;
	
	
	int i = 0 ;
	
	while( i < strlen( s ) ){
		printf( "%d %c\n" , i , s[ i ] ) ;
		
		if( s[ i ] == '{'  ){
			if( s[ i + 1 ] != '}' ){
				*n = 1 ;
				t = ( char ** ) calloc( 1 , sizeof( char * ) ) ;
				j = 0 ;
			}
		}
		else if( s[ i ] == ',' ){
			buffer[ j ] = '\0' ;
			t[ *n - 1 ] = ( char * ) malloc( j ) ;
			strcpy( t[ *n - 1 ] , buffer) ;

			*n += 1 ;
			t = ( char ** ) realloc( t , *n * sizeof( char * ) ) ;
			j = 0 ;
		}
		else if( s[ i ] == '}' ){
			buffer[ j ] = '\0' ;
			t[ *n - 1 ] = ( char * ) malloc( j ) ;
			strcpy( t[ *n - 1 ] , buffer ) ;
		}
		
		else {
			buffer[ j ] = s[ i ] ;
			j++ ;
		}
		
		i++ ;
	}
	
	return t ;
	
}

//Recuperation des variables globales
extern int nbActions;
extern int nbTaches;
extern struct Action *actions;
extern struct Tache *taches;

void Parse(){
	nbActions = 0;
	nbTaches = 0;
	taches = NULL;
	actions = NULL;
	puts( "**************" ) ;
	
	
	int etat = E_JSP ;

	char ligne[ MAX_LIGNE ] ;	// Buffer (tampon)

	FILE *source = fopen( "test.sconf" , "r" ) ;
	
	
	while( fgets( ligne , MAX_LIGNE , source ) ){
		
		int lng = strlen( ligne ) ;
		if( ligne[ lng - 1 ] == '\n' ){			// On nettoie la ligne
			ligne[ lng - 1 ] = '\0' ;
		}
		
		puts( ligne ) ;
		
		
		if( ligne[ 0 ] == '=' && ligne[ 1 ] != '=' ){	// On commence une action
			
			etat = E_ACTION ;
			nbActions += 1 ;		// Une action de plus
			
			if( actions == NULL ){
				actions = ( struct Action * ) calloc( 1 , sizeof( struct Action ) ) ;		// Insertion premier element
			}
			else {
				actions = ( struct Action * ) realloc( actions , nbActions * sizeof( struct Action ) ) ;	// Insertion autres éléments
			}
			
		}
		else if( ligne[ 0 ] == '=' && ligne[ 1 ] == '=' ){	// On commence une tache
			
			etat = E_TACHE ;
			nbTaches += 1 ;
			
			if( taches == NULL ){
				taches = ( struct Tache * ) calloc( 1 , sizeof( struct Tache ) ) ;
			}
			else {
				taches = ( struct Tache * ) realloc( taches , nbTaches * sizeof( struct Tache ) ) ;
			}
			taches[ nbTaches - 1 ].secondes = 0 ;
			taches[ nbTaches - 1 ].minutes = 0 ;
			taches[ nbTaches - 1 ].heures = 0 ;
			
		}
		if( ligne[ 0 ] == '{' ){
			
			if( etat == E_ACTION ){
				
				char cle[ 100 ] ;
				char valeur[ 200 ] ;
				
				getAssociation( ligne , cle , valeur ) ;
				puts( cle ) ;
				puts( valeur ) ;
				
				if( strcmp( cle , "name" ) == 0 ){
					actions[ nbActions - 1 ].nom = ( char *) malloc( strlen( valeur ) + 1 ) ;
					strcpy( actions[ nbActions - 1 ].nom , valeur ) ;
				}
				else if( strcmp( cle , "url" ) == 0 ){
					actions[ nbActions - 1 ].url = ( char *) malloc( strlen( valeur ) + 1 ) ;
					strcpy( actions[ nbActions - 1 ].url , valeur ) ;
				}
				
			}
			
			else if( etat == E_TACHE ){
				
				
				char cle[ 100 ] ;
				char valeur[ 200 ] ;
				
				getAssociation( ligne , cle , valeur ) ;
				puts( cle ) ;
				puts( valeur ) ;
				
				if( strcmp( cle , "name" ) == 0 ){
					taches[ nbTaches - 1 ].nom = ( char *) malloc( strlen( valeur ) + 1 ) ;
					strcpy( taches[ nbTaches - 1 ].nom , valeur ) ;
				}
				else if( strcmp( cle , "second" ) == 0 ){
					taches[ nbTaches - 1 ].secondes = atoi( valeur ) ;
				}
				else if( strcmp( cle , "minute" ) == 0 ){
					taches[ nbTaches - 1 ].minutes = atoi( valeur ) ;
				}
				else if( strcmp( cle , "hour" ) == 0 ){
					taches[ nbTaches - 1 ].heures = atoi( valeur ) ;
				}
				
			}
			
			else if( etat == E_LISTE_OPTIONS_TACHE ){
				
				puts( "Mes actions" ) ;
				
				
				// Obtenir la liste des noms des actions associees a la tache en cours de creation
				int nb ;
				char **nomActions = getNomActions( ligne , &nb ) ;
				
				int i ;
				
				// Reserver la mémoire pour stocker les pointeurs sur les actions
				taches[ nbTaches - 1 ].nbActions = nb ;
				taches[ nbTaches - 1 ].actions = ( struct Action ** ) calloc( nb , sizeof( struct Action * ) ) ;
				
				
				// Initialiser chaque pointeur de la liste des pointeurs sur les actions
				for( i = 0 ; i < nb ; i++ ){
					
					printf( "---> Nom Action : %s\n" ,  nomActions[ i ] ) ;
					
					// Recuperer le pointeur d'une action en fonction de son nom
					struct Action *action = getAction( actions , nbActions , nomActions[ i ] ) ;
					
					printf( "\nAction :\n\tNom : %s\n\tURL : %s\n\n" , action->nom , action->url ) ;
					
					// Stocker le pointeur sur l'action dans la liste des pointeurs 
					taches[ nbTaches - 1 ].actions[ i ] = action ;
					
				} 
				
				
				
				
			}
			
		}
		
		else if( ligne[ 0 ] == '+' && etat == E_TACHE ) {
			etat = E_LISTE_OPTIONS_TACHE ;
		}
		else if( ligne[ 0 ] == '+' ) {
			etat = E_JSP ;
		}
		else {
			//etat = E_JSP ;
		}
			
	}
	
	fclose( source ) ;
	
	
	int i ;
	
	
	puts( "\n\n##################\n\nListe des actions\n" ) ;
	for( i = 0 ; i < nbActions ; i++ ){
		
		puts( "-------------------------" ) ;
		puts( actions[ i ].nom ) ;
		puts( actions[ i ].url ) ;
		puts( "") ;
		
	}
	
	
	puts( "\n\n##################\n\nListe des taches\n" ) ;
	for( i = 0 ; i < nbTaches ; i++ ){
		
		puts( "-------------------------" ) ;
		puts( taches[ i ].nom ) ;
		printf( "%d secondes\n" , taches[ i ].secondes ) ;
		printf( "%d minutes\n" , taches[ i ].minutes ) ;
		printf( "%d heures\n" , taches[ i ].heures ) ;
		puts( "") ;
		
		int j ;
		
		for( j = 0 ; j < taches[ i ].nbActions ; j++ ){
			
			puts( taches[ i ].actions[ j ]->nom ) ;
			puts( taches[ i ].actions[ j ]->url ) ;
			puts( "") ;
			
		}
		
		
	}
	
	printf("nbActions in parse : %d\n", nbActions);

}
