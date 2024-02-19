#include <stdio.h>
#include <stdlib.h>

//0->espai buit
//1->jugador -->tirada parell
//2->ordinador -->tirada senar

#define N 8

//int comptador=0; //per comprovar

typedef struct node{
    char tauler [N][N];
    struct node* *fills; // fills es un camp, es un apuntador que te node
    int n_fills;
    int valor;
    int tirada; //TODO eliminar-ho i canviar-ho per torn?
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
void copiarTauler(Node *pare, Node *p){
    for (int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            p->tauler[i][j] = pare->tauler[i][j];
        }
    }
}

void puntuacio(Node *node, int col){
    if (guanyar(node, col)==1){
                if(node->tirada % 2 == 0 ){ //si guanya el jugador
                    node->valor = -100;
                }
                else{ //si guanya ordinador
                    node->valor = 100;
                }
        }
        else{ //si no guanya ningu

            node->valor = 3; //canviar-ho a valor random
            printf(" valor %d\n",node->valor);

        }
                    printf(" funcio puntuacio\n");

}

Node *crearNode(Node *pare, int numDeFill, int nivell){
    //printf("crea Node\n");
    //creem un node
    Node *p=malloc(sizeof(Node));
    //copia el tauler del pare
    copiarTauler(pare, p);
    (p->tirada) = (nivell + 1) % 2; //cal comprovar-ho (nivell parell=tirada d'ordinador=tirada senar)
    p->valor=0;
    //posa fitxa a la columna numDeFill // i si una col ja esta plena?
    PosarFitxa(p, numDeFill);
    //calcula el num de fills que pot tenir p
    if(nivell<2){
            p->n_fills = cols_disp(p);
                p->fills=malloc(p->n_fills * sizeof(Node *));


    }else{
        p->n_fills=0;
        puntuacio(p, numDeFill);

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
    arrel->tirada=0;
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

//TODO funcio que comprovi files completes
void PosarFitxa(Node *arrel, int columna){
    if ((arrel->comp)<=N-1){ //cal comprovar fila completa
        if (arrel->tauler[N-1-(arrel->comp)][columna]==0){
            if (arrel->tirada % 2== 0) {
                arrel->tauler[N-1- (arrel->comp)][columna]=1; //Jugador
            }
            else {
                arrel->tauler[N-1- (arrel->comp)][columna]=2; //Ordinador
            }
            (arrel->comp)=0;
        }
        else{
            arrel->comp = (arrel->comp)+1;
            PosarFitxa(arrel, columna);
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
int horitzontal(Node *arrel){
        for(int i=N-1; i>-1; i--){ //files
            for(int j=0; j<4; j++){ //columnes
                if(arrel->tauler[i][j] != 0){
                    if (arrel->tauler[i][j]==arrel->tauler[i][j+1] && arrel->tauler[i][j]==arrel->tauler[i][j+2] && arrel->tauler[i][j]==arrel->tauler[i][j+3]){
                    return 1; // quan s'ha fet 4 en ratlla, retorna un 1
                    }
                }

                }
            }
            return 0;
}

// comprovem vertical
int vertical(Node *arrel, int col){
    for (int i=N-1; i>2; i--){
        if(arrel->tauler[i][col] != 0){
            if (arrel->tauler[i][col]==arrel->tauler[i-1][col] && arrel->tauler[i][col]==arrel->tauler[i-2][col] && arrel->tauler[i][col]==arrel->tauler[i-3][col]) {
                return 1;
            }
        }
    }
    return 0;

}

// comprovem diagonal ascendent
int diagonalasc(Node *arrel){
    for (int i=N-1; i>2; i--){
            for (int j=0; j<5; j++){
                if(arrel->tauler[i][j] != 0){
                    if(arrel->tauler[i][j]==arrel->tauler[i-1][j+1] && arrel->tauler[i][j]==arrel->tauler[i-2][j+2] && arrel->tauler[i][j] == arrel->tauler[i-3][j+3]){
                        return 1;
                    }
                }
            }
        }
    return 0;
}

// comprovem diagonal desc
int diagonaldesc(Node *arrel){
    for (int i=0; i<5; i++){
            for (int j=0; j<5; j++){
                if(arrel->tauler[i][j] != 0){
                    if(arrel->tauler[i][j]==arrel->tauler[i+1][j+1] && arrel->tauler[i][j]==arrel->tauler[i+2][j+2] && arrel->tauler[i][j] == arrel->tauler[i+3][j+3]){
                        return 1;
                    }
                }
            }
        }
    return 0;
}

//TODO repassar les tirades
int guanyar(Node *arrel, int col){
    if (horitzontal(arrel)==1 || vertical(arrel, col)==1 || diagonalasc(arrel)==1 || diagonaldesc(arrel)==1){
        if (arrel->tirada % 2 == 0){
            printf("Ha guanyat l'ordinador!!!\n");
        }
        else {
            printf("Ha guanyat el jugador!!!\n");
        }
        return 1;
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

int tirada_ordinador(Node *arrel){
    printf("tirada ordinador\n");
    //columnes disponibles = n_fills
    arrel->n_fills = cols_disp(arrel);
    //crea arbre
    crearArbre(arrel);
    //recorre arbre rec
    recorreArbreRec(arrel,0);
    //TODO fer el minimax que retirni la columna a escollir
    return 1;

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
    while(ult_fila_compl(arrel)==0 && guanyar(arrel, col)==0){
        if (torn % 2 == 0){
            col = tiradaJugador(arrel);
        }
        else{
            col = tirada_ordinador(arrel);
        }
        PosarFitxa(arrel, col);
        mostraTauler(arrel);
        (arrel->tirada)=(arrel->tirada)+1;
        torn = torn + 1;

    }

    //recorreArbreRec(arrel,0);
    //crear el node arrel
    //posar-li el tauler (inicialment buit)
    //crear l'array de fills ja que crearArbre presuposa tot aixo
    printf("Hola!\n");
    return 0;
}
