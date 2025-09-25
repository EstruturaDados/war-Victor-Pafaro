#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TERRITORIOS 5
#define TAM_STRING 50


struct Territorio{
    char nome[30];
    char cor[10];
    int tropas;
};

void limparBufferEntrada(){
    int c;
    while((c=getchar()) != '\n' && c != EOF);
}

int main(){

    struct Territorio territorios[MAX_TERRITORIOS];

    int totalTerritorio = 0; // CORREÇÃO: arrays em C começam com o índice 0.


    printf("----------------- COMEÇANDO O JOGO ---------------------");

    do{

        printf("\n--- Cadastrando território %d", totalTerritorio + 1);

        printf("\nNome do territorio: ");
        fgets(territorios[totalTerritorio].nome,TAM_STRING,stdin);

        printf("\nCor do Exercito: ");
        fgets(territorios[totalTerritorio].cor,TAM_STRING,stdin);


        printf("\nNumero de tropas: ");
        scanf("%d", &territorios[totalTerritorio].tropas);
        limparBufferEntrada();

        totalTerritorio++;
        

    }while(totalTerritorio < MAX_TERRITORIOS);

    printf("\n======================================= ");
    printf("\n     MAPA DO MUNDO - ESTADO ATUAL       ");
    printf("\n======================================= ");   

    for(int i = 0; i<totalTerritorio; i++){
        printf("\n\nTERRITORIO %d: ", i + 1);
        printf("\n   - Nome: %s", territorios[i].nome);
        printf("\n   - Dominado por: Exercito %s", territorios[i].cor);
        printf("\n   - Tropas: %d \n\n", territorios[i].tropas);
    }

    return 0;
}