#include <stdio.h>
#include <stdlib.h>

#define N 8
//Jugador 1 --> X --> parell
//Jugador 2 --> O --> senar

typedef struct pas
{
    char tauler[N][N];
    int tirada; //num tirada/ronda
    int comp;  //files completes
}Pas;

void mostraTauler(Pas *pas) {
    for (int i=0; i<N; i++){
        printf("----------------------------------------\n");
        for (int j=0; j<N; j++){
            printf("| %c |", pas->tauler[i][j]);
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

void inicialitzacio(Pas *pas){
    pas->tirada = 0;
    pas->comp = 0;
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            pas->tauler[i][j]=' ';
        }
    }
}

void PosarFitxa(int columna,Pas *pas){
    if ((pas->comp)<=N-1){
        if (pas->tauler[N-1-(pas->comp)][columna]==' '){
            if (pas->tirada % 2== 0) {
                pas->tauler[N-1- (pas->comp)][columna]='X'; //Jugador A
            }
            else {
                pas->tauler[N-1- (pas->comp)][columna]='O'; //Jugador B
            }
            (pas->comp)=0;
            mostraTauler(pas);
        }
        else{
            pas->comp = (pas->comp)+1;
            PosarFitxa(columna,pas);
        }
    }
    else {
        return;
    }
}

//comprovem horitzontal
int horitzontal(Pas *pas){
        for(int i=N-1; i>-1; i--){ //files
            for(int j=0; j<4; j++){ //columnes
                if(pas->tauler[i][j] != ' '){
                    if (pas->tauler[i][j]==pas->tauler[i][j+1] && pas->tauler[i][j]==pas->tauler[i][j+2] && pas->tauler[i][j]==pas->tauler[i][j+3]){
                    return 1; // quan s'ha fet 4 en ratlla, retorna un 1
                    }
                }

                }
            }
            return 0;
}

// comprovem vertical
int vertical(Pas *pas, int col){
    for (int i=N-1; i>2; i--){
        if(pas->tauler[i][col] != ' '){
            if (pas->tauler[i][col]==pas->tauler[i-1][col] && pas->tauler[i][col]==pas->tauler[i-2][col] && pas->tauler[i][col]==pas->tauler[i-3][col]) {
                return 1;
            }
        }
    }
    return 0;

}

// comprovem diagonal ascendent
int diagonalasc(Pas *pas){
    for (int i=N-1; i>2; i--){
            for (int j=0; j<5; j++){
                if(pas->tauler[i][j] != ' '){
                    if(pas->tauler[i][j]==pas->tauler[i-1][j+1] && pas->tauler[i][j]==pas->tauler[i-2][j+2] && pas->tauler[i][j] == pas->tauler[i-3][j+3]){
                        return 1;
                    }
                }
            }
        }
    return 0;
}

// comprovem diagonal desc
int diagonaldesc(Pas *pas){
    for (int i=0; i<5; i++){
            for (int j=0; j<5; j++){
                if(pas->tauler[i][j] != ' '){
                    if(pas->tauler[i][j]==pas->tauler[i+1][j+1] && pas->tauler[i][j]==pas->tauler[i+2][j+2] && pas->tauler[i][j] == pas->tauler[i+3][j+3]){
                        return 1;
                    }
                }
            }
        }
    return 0;
}

int guanyar(Pas *pas, int col){
    if (horitzontal(pas)==1 || vertical(pas, col)==1 || diagonalasc(pas)==1 || diagonaldesc(pas)==1){
        if (pas->tirada % 2 == 0){
            printf("Ha guanyat el jugador 1!!!\n");
        }
        else {
            printf("Ha guanyat el jugador 2!!!\n");
        }
        return 1;
    }
    else{
        return 0;
    }
}

int ult_fila_compl(Pas *pas){
    int compl = 0;
    for (int j=0; j<N; j++){
        if (pas->tauler[0][j]!=' '){
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

int jugada(Pas *pas){
    int col;
    if (pas->tirada % 2== 0){
            printf("A quina columna vols tirar jugador 1? (1-8)\n");
            // el jugador escull una columna on tirar la seva fitxa
            scanf("%d", &col);
            while(col>8 || pas->tauler[0][col-1]!=' '){
                printf("torna a escollir columna (out of range)\n");
                printf("A quina columna vols tirar? (1-8)\n");
                scanf("%d", &col);
            }
            col=col-1;
    }
    else{
        printf("A quina columna vols tirar jugador 2? (1-8)\n");
            // el jugador escull una columna on tirar la seva fitxa
            scanf("%d", &col);
            while(col>8 || pas->tauler[0][col-1]!=' '){
                printf("torna a escollir columna (out of range)\n");
                printf("A quina columna vols tirar? (1-8)\n");
                scanf("%d", &col);
            }
            col=col-1;
    }
    //s'agafa la columna col (dit pel jugador)
    PosarFitxa(col, pas);
    while(ult_fila_compl(pas)==0 && guanyar(pas, col)==0){
        (pas->tirada)=(pas->tirada)+1;
        jugada(pas);
        return 0;
    }
    printf("S'ha acabat el joc.\n");
    return 1;
}

void main(){
    Pas *tauler1 = malloc(sizeof(Pas));
    inicialitzacio(tauler1);
    mostraTauler(tauler1);
    jugada(tauler1);





}
