/*
Sviluppo di un programma � Supermercato
Un supermercato vuole gestire gli articoli dei propri reparti.Per ciascun articolo devono essere
memorizzati :
- un codice alfanumerico
- il nome
- la casa produttrice
- il reparto di appartenenza
- il prezzo unitario
- la quantit� presente nel supermercato.
0) Progettare la struttura dati adatta a mantenere in memoria centrale le informazioni di ogni
prodotto. In particolare, utilizzare una lista realizzata tramite strutture e puntatori.
1) Scrivere un opportuno main in cui sia previsto un men� di scelta delle operazioni seguenti, che
invochi tali funzioni passando in modo opportuno i parametri ritenuti necessari
2) Realizzare una funzione che legga da standard input i dati di un articolo e li inserisca in modo
opportuno all�interno della lista degli articoli
3) Scrivere una funzione che calcoli e restituisca il prezzo massimo e minimo degli articoli di una
determinata casa produttrice passata come parametro
4) Scrivere una funzione che restituisca in un opportuno vettore gli articoli di un determinato
reparto passato come parametro
5) Scrivere una funzione che crei e restituisca in una lista l�elenco degli articoli aventi quantit�
minore di un determinato valore passato in ingresso alla funzione
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define REPARTO_NUMERO	3
#define REPARTO_FRUTTA	0
#define REPARTO_CARNE	1
#define REPARTO_SALUMI	2
#define REPARTO_NON_DEFINITO -1

#define STRINGA_DIMENSIONE	128

const char TABELLA_REPARTI[REPARTO_NUMERO][STRINGA_DIMENSIONE] = { "frutta", "carne", "salumi" };

int stringa2reparto(const char* stringa) {
	for (int i = 0; i < REPARTO_NUMERO; ++i)
		if (strcmp(TABELLA_REPARTI[i], stringa) == 0)
			return i;
	return -1;
}

char* reparto2stringa(int reparto) {
	if (reparto >= REPARTO_NUMERO || reparto < 0)
		return "non definito";
	return TABELLA_REPARTI[reparto];
}

typedef struct _s_prodotto {
	char codice[STRINGA_DIMENSIONE];
	char nome[STRINGA_DIMENSIONE];
	char casa_produttrice[STRINGA_DIMENSIONE];
	int reparto_appartenenza;
	int prezzo_unitario;
	int quantita_presente;
} t_prodotto;

void prodotto_stampa(FILE* file_uscita, t_prodotto prodotto) {
	printf("codice:\t\t\t%s\nnome:\t\t\t%s\ncasa produttrice:\t%s\nreparto:\t\t%s\nprezzo:\t\t\t%.2f\nquantita':\t\t%d\n\n",
		prodotto.codice,
		prodotto.nome,
		prodotto.casa_produttrice,
		reparto2stringa(prodotto.reparto_appartenenza),
		prodotto.prezzo_unitario / 100.00,
		prodotto.quantita_presente);
}

typedef struct _s_nodo_prodotto {
	t_prodotto dato;
	struct _s_nodo_prodotto* successivo;
} _t_nodo_prodotto;
typedef _t_nodo_prodotto* t_lista_prodotti;

// la funzione restituisce ilpuntatore al prodotto appena inserito o NULL
// nel caso di problema
t_prodotto* lista_aggiungi(t_lista_prodotti* pt_lista, t_prodotto prodotto) {
	_t_nodo_prodotto* aux = (_t_nodo_prodotto*)malloc(sizeof(_t_nodo_prodotto));
	if (aux == 0)
		return NULL;
	aux->dato = prodotto;
	aux->successivo = *pt_lista;
	*pt_lista = aux;
	return &(aux->dato);
}

t_prodotto* elenco_prodotti_reparto(t_lista_prodotti lista, int reparto, int* numero_elementi) { 
	//lavora su un array di prodotti, di dimensioni da calcolare (2 while: 1 per la dim e uno x assegnazione
	int i=0,contatore = 0;
	t_lista_prodotti aux = lista;
	
	while (lista != NULL) {
		if (lista->dato.reparto_appartenenza == reparto) {
			contatore++;
		}
		lista = lista->successivo;
	}

	t_prodotto* vettore_prodotti = (t_prodotto*)malloc(sizeof(t_prodotto) * contatore);

	while (aux != NULL) {
		if (aux->dato.reparto_appartenenza == reparto) {
			vettore_prodotti[i] = aux->dato;
			i++; //farà contatore cicli! //questo però dentro perchè se no fa cicli quant'è la dim della lista
		}
		aux = aux->successivo;
	}
	*numero_elementi = contatore; //importante!!! se non deferenzio (*(pt*) = radice ) assegno un valore ad un puntatore!!
	
	return vettore_prodotti;

}

t_prodotto* elenco_prodotti_reparto_bis(t_lista_prodotti lista, int reparto, int* numero_elementi) {
	int contatore = 0;
	t_lista_prodotti aux = lista;
	int dimensione_vettore = 2; // valore due scelto per fare scattare il meccanismo
	t_prodotto* vettore = (t_prodotto*)malloc(sizeof(t_prodotto) * dimensione_vettore);
	t_prodotto* result;
	while (lista != NULL) {
		if (lista->dato.reparto_appartenenza == reparto) {
			if (contatore < dimensione_vettore)
				vettore[contatore] = lista->dato;
			else {
				dimensione_vettore += 2;
				result = realloc(vettore, sizeof(t_prodotto) * dimensione_vettore);
				if (result == NULL) {
					printf("Errore di allocazione\n");
					free(vettore);
					return NULL;
				}
				vettore = result;
				vettore[contatore] = lista->dato;
			}
			++contatore;
		}
		lista = lista->successivo;
	}
	result = realloc(vettore, sizeof(t_prodotto) * contatore);
	if (result == NULL) {
		printf("Errore di allocazione\n");
		free(vettore);
		return NULL;
	}
	*numero_elementi = contatore;
	return result;
}

t_lista_prodotti crisi(t_lista_prodotti lista, int flag,int *numero_elementi) {
	//*numero_elementi lo passo perchè devo tornare quanti valori ho 
	*numero_elementi = 0; //perchè deferenzio, *(pt*) = radice

	int contatore = 0, i = 0;
	t_lista_prodotti aux = lista; //così posso fare 2 while
	while (lista != NULL) {
		if (lista->dato.quantita_presente <= flag) {
			++contatore;
		}
		lista = lista->successivo;
	}
	//t_lista_prodotti selezione = (t_lista_prodotti*)malloc(sizeof(t_lista_prodotti));
	//selezione = NULL;
	t_lista_prodotti selezione = NULL; //che differenza c'è?
	//t_lista_prodotti = t_nodo_prodotti* 

	
	
	while (aux != NULL) {
		if (aux->dato.quantita_presente <= flag) {
			lista_aggiungi(&selezione, aux->dato);
			//selezione = selezione->successivo; //mi cancella tutta la lista
		}
		aux = aux->successivo;
	}
	*numero_elementi = contatore; //col * perchè *(pt*) = radice 

	return selezione;
}

int menu(void) {
	int scelta = 0;
	char buffer[STRINGA_DIMENSIONE];
	do {
		printf("\n \
1) Carica da file \n \
2) funzione che legge da standard input \n \
3) calcolo del prezzo massimo e minimo degli articoli di una determinata casa produttrice \n \
4) articoli di un determinato reparto \n \
5) elenco degli articoli aventi quantit� minore di un determinato valore \n\n");
		printf(">>> ");
		fgets(buffer, STRINGA_DIMENSIONE, stdin);
		scelta = atoi(buffer);
	} while (scelta < 0 || scelta > 5);
	return scelta;
}

// f : lista * stringa -> int * int
void calcolo_estremi(t_lista_prodotti lista, const char* casa_produttrice,int* massimo, int* minimo) { 
	//siccome torna void, devo lavorare per riferimento quindi int* max min 
	//const char* invece è il discorso passo meno pesante (pt) ma non devo modificarlo : const

	*massimo=0; //deferenzio e assegno il valore
	*minimo = lista->dato.prezzo_unitario;
	//per massimo ok 0, tutti gli saranno maggiori, invece per minimo prendo il primo prezzo e vedo se sono minori
	while (lista != NULL) {
		if (strcmp(casa_produttrice, lista->dato.casa_produttrice) == 0) {
			if (lista->dato.prezzo_unitario > *massimo) { //*max perchè devo deferenziare (*(pt*))=> valore radice
				*massimo = lista->dato.prezzo_unitario; //idem se non metto *max assegno un valore ad un puntatore! devo deferenziare
			}
			if (lista->dato.prezzo_unitario < *minimo) {
				*minimo = lista->dato.prezzo_unitario;
			}
		}
		lista = lista->successivo;
	}
	/*
	while (lista != NULL) {
		if (strcmp(casa_produttrice, lista->dato.casa_produttrice) == 0) {
			if (lista->dato.prezzo_unitario < *minimo) {
				minimo = lista->dato.prezzo_unitario;
			}
		}
		lista = lista->successivo;
	}
	//SE METTO 2 CICLI WHILE IL SECONDO NON ESEGUE PERCHE' LA LISTA FINISCE COL PRIMO 
	*/

}

//
// questa funzione legge i dati dal file elenco.txt, opera le conversioni necessarie
// e li carica nella lista.
// la funzione restituisce il numero di recordo letti o -1 in caso di errore
// 
// La funzione mostra un esempio di uso di scanf per leggere nomi che comprendono
// caratteri spaziatura

int carica_file(const char* nome_file, t_lista_prodotti* pt_lista) {
	FILE* f;
	if ((f = fopen(nome_file, "r")) == NULL)
		return -1;

	t_prodotto p;
	char reparto[STRINGA_DIMENSIONE];
	float prezzo;
	int contatore = 0;
	while (fscanf(f, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%f\n%d\n", p.codice,
		p.nome, p.casa_produttrice, &reparto, &prezzo, &p.quantita_presente) == 6) {
		p.reparto_appartenenza = stringa2reparto(reparto);
		p.prezzo_unitario = prezzo * 100;
		lista_aggiungi(pt_lista, p);
		++contatore;
	}
	return contatore;
}

int main(void) {
	int scelta;
	t_lista_prodotti elenco_prodotti = NULL;
	char buffer[STRINGA_DIMENSIONE];
	char nome_file[STRINGA_DIMENSIONE];
	int caricati = 0;
	t_prodotto prodotto;
	t_prodotto* vettore_prodotti; //quesito 4
	int flag5 = 0; //quesito 5
	t_lista_prodotti selezione = NULL;

	do {
		switch (menu()) {
		case 0: // Uscita
			printf("Il programma sta per terminare\n");
			return 1;
		case 1: // carica da file, funzione aggiunta per caricare i dati per il test pi�
			// rapidamente
			// 1. chiamare la funzione carica con i parametri corretti (il file di ingreso si chiama elenco.txt
			// 2. stampare il numero di elemento caricati
			printf("\nNome del file : ");
			fgets(buffer, STRINGA_DIMENSIONE, stdin);
			buffer[strlen(buffer) - 1] = '\0';
			strcpy(nome_file, buffer);
			caricati = carica_file(nome_file, &elenco_prodotti); //se no al posto di nome file diretto "elenco.txt"
			printf("\nCaricati %d elementi.  ", caricati);
			break;
		case 2:
			// 2) Realizzare una funzione che legga da standard input i dati di un articolo e li inserisca in modo
			// opportuno all�interno della lista degli articoli			
			// 1. leggere un record da console
			printf("\nInserire il codice : ");
			fgets(buffer, STRINGA_DIMENSIONE, stdin);
			buffer[strlen(buffer) - 1] = '\0';
			strcpy(prodotto.codice, buffer);

			printf("\nInserire il nome : ");
			fgets(buffer, STRINGA_DIMENSIONE, stdin);
			buffer[strlen(buffer) - 1] = '\0';
			strcpy(prodotto.nome, buffer);

			printf("\nInserire la casa produttrice : ");
			fgets(buffer, STRINGA_DIMENSIONE, stdin);
			buffer[strlen(buffer) - 1] = '\0';
			strcpy(prodotto.casa_produttrice, buffer);

			printf("\nInserire il reparto : ");
			fgets(buffer, STRINGA_DIMENSIONE, stdin);
			buffer[strlen(buffer) - 1] = '\0';
			//prodotto.reparto_appartenenza = buffer;
			prodotto.reparto_appartenenza = stringa2reparto(buffer); //perchè in input ho stringa ma conservo int

			printf("\nInserire il  prezzo unitario : ");
			/*fgets(buffer, STRINGA_DIMENSIONE, stdin);
			buffer[strlen(buffer) - 1] = '\0';
			prodotto.prezzo_unitario = buffer;*/
			fgets(buffer, STRINGA_DIMENSIONE, stdin);
			prodotto.prezzo_unitario = atoi(buffer); //Convert string to integer

			printf("\nInserire la quantita' : ");
			/*fgets(buffer, STRINGA_DIMENSIONE, stdin);
			buffer[strlen(buffer) - 1] = '\0';
			prodotto.quantita_presente = buffer;*/
			fgets(buffer, STRINGA_DIMENSIONE, stdin);
			prodotto.quantita_presente = atoi(buffer);

			// 2. inserire nell'archivio
			//aggiungi alla lista = t_prodotto * lista_aggiungi(t_lista_prodotti * pt_lista, t_prodotto prodotto) 
			lista_aggiungi(&elenco_prodotti, prodotto); //non serve assegnazione perchè lavora per riferimento la lista
			break;
		case 3:
			// Scrivere una funzione che calcoli e restituisca il prezzo massimo e minimo degli articoli di una
			// determinata casa produttrice passata come parametro
			printf("\t casa produttrice: ");
			fgets(buffer, STRINGA_DIMENSIONE, stdin);
			buffer[strlen(buffer) - 1] = '\0';
			int valore_massimo, valore_minimo; //qui li dichiaro valori, ma la funzione li vuole per riferimento 
			//-> & nella call e * nella funzione
			calcolo_estremi(elenco_prodotti, buffer, &valore_massimo, &valore_minimo);
			printf("\n valore massimo : %d, valore minimo: %d\n", valore_massimo, valore_minimo);
			break;
		case 4:
			printf("\t Nome reparto: "); //scrivo nome reparto, faccio arrivare intero alla funzione con s2r
			fgets(buffer, STRINGA_DIMENSIONE, stdin);
			buffer[strlen(buffer) - 1] = '\0';

			int numero_elementi;
			// 1. chiamare la funzione con i parametri corretti

			//numero_elementi viene passato come puntatore perchè già ritorno il vettore_prod, non posso tornare anche quello
			//quindi &vettore nella call
			//t_prodotto* elenco_prodotti_reparto(t_lista_prodotti lista, int reparto, int* numero_elementi)
			vettore_prodotti = elenco_prodotti_reparto(elenco_prodotti, stringa2reparto(buffer), &numero_elementi);

			// 2. stampare il risultato
			for (int i = 0; i < numero_elementi; i++) {
				prodotto_stampa(stdout,vettore_prodotti[i]); //stdout perchè vuole l'output
			}
			break;
		case 5:
			// Esercizio aggiuntivo
			// implementare la funzione
			printf("\t Quantita' massima : ");
			fgets(buffer, STRINGA_DIMENSIONE, stdin);
			flag5=atoi(buffer);
			selezione = crisi(elenco_prodotti, flag5,&numero_elementi); 
			//se faccio il for dopo non ho come contare quanti cicli devo fare! passo numero_elementi
			for (int i = 0; i < numero_elementi; i++) {
				prodotto_stampa(stdout, selezione->dato);
				selezione = selezione->successivo;
			}


			break;
		}
	} while (1);
}