#include <stdio.h>
#include <stdlib.h>

//0->espai buit
//1->jugador -->tirada parell
//2->ordinador -->tirada senar

#define N 8

//TODO:
//  - crearArbre: fer mes nivells
//  - puntuacio: canviar els valors de si no es guanya per valors amb sentit
//  -funcio que et digui cada fill a quina col tira un cop sabem les cols_disp
//  -mostraTauler: canvi de 0/1/2 al que volem que es vegi
//  -eliminar nodes arbre


typedef struct node{
    char tauler [N][N];
    struct node* *fills; // fills es un camp, es un apuntador que te node
    int n_fills;
    int valor;
    int comp; //fila completa
} Node;

void mostraNode(Node *node, int nivell){
    for(int i=1;i<=nivell;i++)
        printf("\t");
    printf("- %d\n", node->valor);
}

void mostraNivell(Node *pare, int nivell){
    for (int i=0; i<pare->n_fills;i++)
        mostraNode(pare->fills[i],nivell);
}

void recorreArbre(Node *arrel){
    mostraNivell(arrel,1);
    for (int i=0; i<arrel->n_fills;i++)
        mostraNivell(arrel -> fills[i],7);
}

void recorreArbreRec(Node *arrel, int nivell){
    mostraNode(arrel,nivell);
    if (arrel->n_fills>0){
        for (int i=0; i<arrel->n_fills;i++)
            recorreArbreRec(arrel->fills[i], nivell+1);
    }

}


void max(Node *node){
    int aux=-100;
    for (int i=0; i<node->n_fills; i++){
        if (node->fills[i]->valor>aux){
            aux = node->fills[i]->valor;
        }
    }
    node->valor=aux;
    printf("node->valor (max): %d\n", node->valor);
}

void min(Node *node){
    int aux=100;
    for (int i=0; i<node->n_fills; i++){
        if (node->fills[i]->valor<aux){
            aux = node->fills[i]->valor;
        }
    }
    node->valor=aux;
    printf("node->valor (min): %d\n", node->valor);
}


void miniMax(Node *arrel, int nivell){ //primer entra nivell 0
    for (int i=0; i<arrel->n_fills; i++){
        miniMax(arrel->fills[i], nivell+1);
    }
    if (arrel->n_fills != 0){
        if (nivell % 2 ==0){
            //es el jugador
            min(arrel);
        }
        else{
            max(arrel);
        }
    }


}

void copiarTauler(Node *pare, Node *p){
    for (int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            p->tauler[i][j] = pare->tauler[i][j];
        }
    }
}

void puntuacio(Node *node, int nivell){
    int aux = 0;
    for (int i=1; i<3; i++){
        aux=guanyar(node, i);
        if (aux != 0 ){
            break;
        }
    }
        if (aux==1){
            //si guanya el jugador
                node->valor = -100;
        }
        else if (aux==2){ //si guanya ordinador
                node->valor = 100;
            }

        else{ //si no guanya ningu

            node->valor = rand()%100; //canviar-ho a valor random

        }


}


int col_fill(Node *pare, int numDeFill){
    //funcio on tirara el fill
}

Node *crearNode(Node *pare, int numDeFill, int nivell){
    //printf("crea Node\n");
    //creem un node
    Node *p=malloc(sizeof(Node));
    //copia el tauler del pare
    copiarTauler(pare, p);

    p->valor=0;
    //funcio que et digui cada fill a quina col tira

    PosarFitxa(p, numDeFill, nivell+1);
    //calcula el num de fills que pot tenir p

    //puntuar cada nivell --> si guanya--> escull aquella col directament
    //                    --> si perd-->escull aquella col per evitar-ho
    //                    --> sino -->minimaxnormal
    if(nivell<2){
            p->n_fills = cols_disp(p);
            p->fills=malloc(p->n_fills * sizeof(Node *));


    }else{
        p->n_fills=0;
        puntuacio(p, nivell);

    }

    return p;
}

void crearNivell(Node *pare, int nivell){
    printf("crea Nivell\n");
    printf("arrel n-fills= %d\n ",pare->n_fills);
    for (int i=0; i<pare->n_fills;i++)
        pare -> fills[i]=crearNode(pare,i,nivell);



}

void crearArbre(Node *arrel){
    //printf("crea Arbre\n");
    crearNivell(arrel,1);
    for (int i=0; i<arrel->n_fills;i++)
        crearNivell(arrel -> fills[i],2);
}

void inicialitzacio(Node *arrel){
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            arrel->tauler[i][j]=0;
        }
    }
}

//cal afegir el canvi de 0/1/2 al que volem que es vegi
void mostraTauler(Node *arrel) {
    for (int i=0; i<N; i++){
        printf("----------------------------------------\n");
        for (int j=0; j<N; j++){
            printf("| %d |", arrel->tauler[i][j]);
        }
        printf("\n");
        if (i==N-1){
            printf("----------------------------------------\n");
            for (int j=0; j<N; j++){
            printf("| %d |", j+1);
            }
            printf("\n");
        }

    }
}



void PosarFitxa(Node *arrel, int columna, int torn){
    if ((arrel->comp)<=N-1){ //cal comprovar fila completa
        if (arrel->tauler[N-1-(arrel->comp)][columna]==0){
            if (torn % 2== 0) {
                arrel->tauler[N-1- (arrel->comp)][columna]=1; //Jugador
            }
            else {
                arrel->tauler[N-1- (arrel->comp)][columna]=2; //Ordinador
            }
            (arrel->comp)=0;
        }
        else{
            arrel->comp = (arrel->comp)+1; //veiem si una col te la posicio ocupada (començant per abaix)
            PosarFitxa(arrel, columna, torn);
        }
    }
    else {
        return;
    }
}


int ult_fila_compl(Node *arrel){
    int compl = 0;
    for (int j=0; j<N; j++){
        if (arrel->tauler[0][j]!=0){
            compl=compl+1;
        }
    }
    if (compl == 8){
        return 1;
    }
    else{
        return 0;
    }
}



//comprovem horitzontal
int horitzontal(Node *arrel, int jugador){
        for(int i=N-1; i>-1; i--){ //files
            for(int j=0; j<4; j++){ //columnes
                if(arrel->tauler[i][j] == jugador){
                    if (arrel->tauler[i][j]==arrel->tauler[i][j+1] && arrel->tauler[i][j]==arrel->tauler[i][j+2] && arrel->tauler[i][j]==arrel->tauler[i][j+3]){
                    return 1; // quan s'ha fet 4 en ratlla, retorna un 1
                    }
                }

                }
            }
            return 0;
}

// comprovem vertical
int vertical(Node *arrel, int jugador){
    for (int j=0; j<N; j++){ //col
        for (int i=N-1; i>2; i--){
            if(arrel->tauler[i][j] == jugador){
                if (arrel->tauler[i][j]==arrel->tauler[i-1][j] && arrel->tauler[i][j]==arrel->tauler[i-2][j] && arrel->tauler[i][j]==arrel->tauler[i-3][j]) {
                       return 1;
            }
        }
    }
    }
    return 0;

}

// comprovem diagonal ascendent
int diagonalasc(Node *arrel, int jugador){
    for (int i=N-1; i>2; i--){
            for (int j=0; j<5; j++){
                if(arrel->tauler[i][j] == jugador){
                    if(arrel->tauler[i][j]==arrel->tauler[i-1][j+1] && arrel->tauler[i][j]==arrel->tauler[i-2][j+2] && arrel->tauler[i][j] == arrel->tauler[i-3][j+3]){
                        return 1;
                    }
                }
            }
        }
    return 0;
}

// comprovem diagonal desc
int diagonaldesc(Node *arrel, int jugador){
    for (int i=0; i<5; i++){
            for (int j=0; j<5; j++){
                if(arrel->tauler[i][j] == jugador){
                    if(arrel->tauler[i][j]==arrel->tauler[i+1][j+1] && arrel->tauler[i][j]==arrel->tauler[i+2][j+2] && arrel->tauler[i][j] == arrel->tauler[i+3][j+3]){
                        return 1;
                    }
                }
            }
        }
    return 0;
}

int guanyar(Node *arrel, int jugador){
    if (horitzontal(arrel, jugador)==1 || vertical(arrel, jugador)==1 || diagonalasc(arrel, jugador)==1 || diagonaldesc(arrel, jugador)==1){
        if (jugador==1){ //jugador
             return 1;
        }
        else{
            return 2; //ordinador
        }

    }
    else{
        return 0;
    }
}

int cols_disp(Node *arrel){
    int disp=0;
    for (int i=0; i<N; i++){
        if (arrel->tauler[0][i]==0){
            disp=disp+1;
        }
    }
    return disp;

}


int esborrar_arbre(Node *pare){
    for (int i=0; i<pare->n_fills; i++){
        esborrar_arbre(pare->fills[i]);
    }
    free(pare->fills);
}

int escollir_columna_ordinador(Node *node){
    for (int i=0; i< node->n_fills; i++){
        if (node->valor == node->fills[i]->valor){
            return i;
        }
    }
}

int tirada_ordinador(Node *arrel){
    printf("tirada ordinador\n");
    //columnes disponibles = n_fills
    arrel->n_fills = cols_disp(arrel);
    //crea arbre
    crearArbre(arrel);
    //recorre arbre rec

    miniMax(arrel,0);
    esborrar_arbre(arrel);
    //return choosecol
    recorreArbreRec(arrel,0);
    //printf("valor arrel minimax: %d\n", arrel->valor);
    return escollir_columna_ordinador(arrel);

}

int tiradaJugador(Node *arrel){
    int col;

        printf("A quina columna vols tirar? (1-8)\n");
        scanf("%d", &col);
        while(col>8 || col<0 || arrel->tauler[0][col-1]!=0){
            printf("torna a escollir columna (out of range)\n");
            printf("A quina columna vols tirar? (1-8)\n");
            scanf("%d", &col);
        }
        col=col-1;



    return col;
}



int main() {
    Node *arrel=malloc(sizeof(Node)); //arrel es un apuntador
    arrel->n_fills=N;
    arrel->fills=malloc(arrel->n_fills * sizeof(Node *));
    arrel->valor=0;
    //inicialitzacio
    inicialitzacio(arrel);
    //mostra tauler
    mostraTauler(arrel);
    int torn = 0;
    int col = 0;
    int jugador = 1;
    while(ult_fila_compl(arrel)==0 && guanyar(arrel, jugador)==0){
        if (torn % 2 == 0){
            jugador = 1;
            col = tiradaJugador(arrel);
        }
        else{
            jugador=2;
            col = tirada_ordinador(arrel);
        }
        PosarFitxa(arrel, col, torn);
        mostraTauler(arrel);
        torn = torn + 1;

    }
    printf("S'ha acabat el joc\n");
    //torn hauria de ser 0 pero hem sumat 1
    if (torn % 2 == 1 ){ //quan guanyar retorna 1
        printf("Ha guanyat el jugador!");
    }
    else {
        printf("Ha guanyat l'ordinador");
    }

    //recorreArbreRec(arrel,0);

    return 0;
}
