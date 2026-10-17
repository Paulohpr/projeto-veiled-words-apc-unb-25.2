// Caso voce esteja jogando no windows, leia o README

#include <stdio.h>
#include <stdlib.h> // biblioteca necessaria para usar a funcao system
#include <string.h>

// diretivas

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

// cores para colocar no printf

#define VERMELHO   "\x1b[1;31m"
#define VERDE      "\x1b[1;32m"
#define AMARELO    "\x1b[1;33m"
#define AZUL       "\x1b[1;34m"
#define MAGENTA    "\x1b[1;35m"
#define CIANO      "\x1b[1;36m"
#define RESET      "\x1b[0m"
// referencias da lista no README.txt

// variaveis globais - colocar de acordo com a necessidade

char painel_letras_max_5[6][10]; // matriz para ser usado nas fases com no maximo 5 letras.
int qtd_palavras = 0;
char lista_palavras[100][10], tamanho_palavras[100][10], informe_palavras[100]; // 
int verifica_contador[100]; // verifica se o jogador nao esta tentando informar se a mesma palavra mais de uma vez
int opcao_switch_case;
char tecla_enter = '\n';
char sim_ou_nao;
int ja_joguei = 0;

typedef struct{
    char nickname[20]; // usar struct para nickname e pontuacao 
    int pontuacao;
} dadosdojogador;

dadosdojogador DadosDoJogador = {"",0}; // tenho que zerar tanto a string nickname qnt a pontuacao com zero, pra n ficar com lixo de memoria

// arquivos -- globais
FILE* fd; // lembrar de usar o fclose() "se abriu tem que fechar!" - fd -> entrada txt
char file[] = "entrada.txt";

FILE* fb; // fb -> ranking.bin
char filebin[] = "ranking.bin";

// declaracao de funcoes anteriormente para que o compilador saiba antes de chamar a funcao e evitar o erro implicit declaration
void limpar_buffer();
void loop_jogo();
int configuracoes_jogo();
void instrucoes_jogo();
void ranking_jogo ();
int chamar_menu();
int fases_do_jogo();
void chamar_jogo();
void bubblesort();
void leitura_ranking_jogo();
void zerar_ranking();

    void limpar_buffer(){ // limpar o buffer da memoria, principalmente com o uso de strings getchar pra linux e fflsuh windows
        #ifdef _WIN32
            fflush(stdin);
        #else
            getchar(); 
        #endif
    }

    int fases_do_jogo(){ // carregar fase
        char leitura_arq[100]; // salvar o que ler no fscanf, lembre-se de quando achar o '-' ignorar, ele separa as fases

        if(fscanf(fd, "%s", leitura_arq) == EOF)// se for o final do arquivo EOF, ent nao tem mais fases, o jogo acabou
            return 0;
        
        if(strcmp(leitura_arq, "-") == 0){  // agora, se for o hifen o fscanf vai pular, lembre-se q o fscanf ele funciona com se fosse um cursor, ele le linha por linha
            if(fscanf(fd, "%s", leitura_arq) == EOF)
                 return 0;
        }
            
        int quantas_letras = strlen(leitura_arq);

        for(int i = 0; i < 6; i++){ // preenche toda a matriz de letras com #, pra depois colocar os espacos
            for(int j = 0; j < 10; j++){
                painel_letras_max_5[i][j] = '#';
            }
            painel_letras_max_5[i][9] = '\0';
        }

        if(quantas_letras == 5)
            painel_letras_max_5[2][2] = leitura_arq[0], painel_letras_max_5[4][2] = leitura_arq[1], painel_letras_max_5[3][5] = leitura_arq[2], painel_letras_max_5[2][8] = leitura_arq[3], painel_letras_max_5[4][8] = leitura_arq[4];
        else if(quantas_letras == 6)
            painel_letras_max_5[2][2] = leitura_arq[0], painel_letras_max_5[4][2] = leitura_arq[1], painel_letras_max_5[3][5] = leitura_arq[2], painel_letras_max_5[2][8] = leitura_arq[3], painel_letras_max_5[4][8] = leitura_arq[4], painel_letras_max_5[1][5] = leitura_arq[5];
        
        fscanf(fd, "%d", &qtd_palavras);
        
        for(int i = 0; i < qtd_palavras; i++){
            fscanf(fd, "%s", lista_palavras[i]);
            verifica_contador[i] = 0; // saber se a palavra ja foi encontrada alguma vez
        
            int tam = strlen(lista_palavras[i]); // vai colocar '-' de acordo com o tamanho da string, para que o jogador saiba o tamanho da palavra
                for(int tt = 0; tt < tam; tt++)
                    tamanho_palavras[i][tt] = '-';
                
                tamanho_palavras[i][tam] = '\0';
            }

        return 1; 
    }
    
    void loop_jogo(){
        
        int cont = 0;
        int acertou = 0;
        
        while(acertou < qtd_palavras){ // condicao de parada pro while n ficar loopando infinitamente
            system(CLEAR);
            
            printf(AZUL "|=====" RESET VERMELHO " < JOGO VEILED WORDS > " RESET AZUL"=====|\n\n" RESET);

            for(int p_let = 0; p_let < 6; p_let++){ // a ideia eh preencher a matriz com '#' e aonde n tem letra coloca-se um espaco, esse laço de repeticao faz isso trocar # por espaco se necessario
                for(int p_let2 = 0; p_let2 < 10; p_let2++){
                    if(painel_letras_max_5[p_let][p_let2] == '#')
                        painel_letras_max_5[p_let][p_let2] = ' ';
               }
            }

            for(int idx = 0; idx < 6; idx++){
                printf(AMARELO "%s" RESET, painel_letras_max_5[idx]);
                    
                if(idx != 0)
                    printf("\n"); 
                if(idx == 0)
                    printf(MAGENTA "\tPONTUACAO: %d\n" RESET, DadosDoJogador.pontuacao); // usar struct para salvar pontuacao, como pede na referencia do trabalho
                }
            
            printf(MAGENTA "Encontre %d palavras:\n\n" RESET, qtd_palavras);
            
            for(int p = 0; p < qtd_palavras; p++)
                printf(AMARELO "%s\n" RESET, tamanho_palavras[p]);
            printf("\n");
            
            if(cont < qtd_palavras){
                printf(CIANO "Informe a Palavra: " RESET);
                scanf("%s", informe_palavras); 
                limpar_buffer(); 
            }
            
            int encontrei = 0;
       
            for(int idx = 0; idx < qtd_palavras; idx++){ 
                if(strcmp(informe_palavras, lista_palavras[idx]) == 0){
                    encontrei = 1;
                    
                    if(verifica_contador[idx] == 1){
                        DadosDoJogador.pontuacao -= 10;
                        printf(CIANO "Esta palavra ja foi encontrada! Tecle <enter> " RESET);
                        scanf("%c", &tecla_enter);
                    }
                    else{
                        DadosDoJogador.pontuacao += 100;
                        verifica_contador[idx] = 1;
                        strcpy(tamanho_palavras[idx], lista_palavras[idx]);
                        acertou++;
                        cont++;

                        printf(AMARELO "PARABENS !!! voce acertou uma palavra!!!! " RESET CIANO "tecla <ENTER> "RESET);
                        scanf("%c", &tecla_enter); // lembra-se q o enter seria um \n, por isso n posso usar limpar_buffer(); ele limparia o '\n'
                        printf("\n\n");
                    }
                    break;  
                }
            }

            if(encontrei == 0) {
                DadosDoJogador.pontuacao -= 10;
                printf(VERMELHO "Palavra inexistente! " RESET CIANO "Tecle <enter> " RESET);
                scanf("%c", &tecla_enter);
             }
        }
            
            DadosDoJogador.pontuacao += 25;
            system(CLEAR);
            printf(AMARELO "\n\nParabens! voce completou a fase, acertou 5 palavras!!!\n" RESET);
            printf(MAGENTA "PONTUACAO ATUAL: %d \n\n" RESET, DadosDoJogador.pontuacao);
            
            printf(CIANO "Tecle <enter> para prosseguir: \n" RESET);
            scanf("%c", &tecla_enter);
    }
            
    void chamar_jogo(){
        if(fd == NULL)
            fd = fopen("entrada.txt", "r");
        if(fd == NULL){
            printf(VERMELHO "Arquivo nao existe, ou nao foi encontrado! (LEIA O README.txt)\n" RESET);
            return;
        }

        while(1){
            int acabou = fases_do_jogo();

            if(acabou == 0){
                system(CLEAR);
                printf(AMARELO "PARABENS!!!! Voce zerou o jogo!\n" RESET);
                
                printf(CIANO "Voce deseja voltar ao menu? (S/N): " RESET);
                    scanf("%c", &sim_ou_nao);

                    if(sim_ou_nao == 'S' || sim_ou_nao == 's'){
                        ja_joguei++;
                        fclose(fd); 
                        fd = NULL; 
                        ranking_jogo();    
                        chamar_menu();
                    }
                    else
                break;
            }
        
            loop_jogo();
        }

        fclose(fd); 
        fd = NULL; 
        ranking_jogo();
    }

    int confguracoes_jogo(){
       
        printf(AZUL "+==+=+=+==" RESET VERMELHO "CONFIGURACOES" RESET AZUL"==+=+=+==+\n\n" RESET);
        printf(AMARELO "1 - Zerar Ranking\n" RESET);
        printf(AMARELO "2 - Modo Dificuldade\n" RESET);
        printf(AMARELO "3 - Edtior de fases\n" RESET);
        printf(AMARELO "4 - Voltar ao menu principal\n\n" RESET);

        printf(CIANO "Digite a opcao desejada: " RESET);
        scanf(" %d", &opcao_switch_case);

        switch(opcao_switch_case){ // switch case para o jogador escolher as opcoes do menu de configuracoes
                case 1: // ranking
                    system(CLEAR);
                    zerar_ranking();
                    break;  

                case 2: // dificuldade
                    system(CLEAR);
                    printf(CIANO "Modo dificuldade em desenvolvimento, digite <enter>...\n" RESET);
                    limpar_buffer();
                    scanf("%c", &tecla_enter);
                    chamar_menu();
                    break;

                case 3: // editor
                    system(CLEAR);
                    printf(CIANO "Editor de Fases em desenvolvimento, digite <enter>...\n" RESET);
                    limpar_buffer();
                    scanf("%c", &tecla_enter);
                    chamar_menu();
                    break;

                case 4: // menu
                    system(CLEAR);
                    chamar_menu(); // menu iniciar 
                    break;
        }
        return 0;
    }

    void instrucoes_jogo(){
        printf(AZUL "Instrucoes sobre o jogo |=====" RESET VERMELHO " < VEILED WORDS > " RESET AZUL"=====|\n\n" RESET);

        printf(AMARELO "1. OBJETIVO:\n" RESET);
        printf(VERDE "A partir das letras embaralhadas encontre todas\n" RESET);
        printf(VERDE "as palavras que estao ocultas para concluir as fases.\n\n" RESET);
        
        printf(AMARELO "2. PONTUACAO:\n" RESET);
        printf(VERDE "  -> Ganhe 100 pontos por acerto, caso erre ou coloque uma palavra repetida -10 pontos!\n" RESET);
        printf(VERDE"  -> Bonus de 25 pontos por terminar uma fase.\n\n" RESET);
        
        printf(AMARELO "3. REGRAS:\n" RESET);
        printf(VERDE "O jogador so pode usar as letras que aparecem na tela\n" RESET); 
        printf(VERDE "cada letra só pode ser usada uma vez para formar a palavra\n" RESET);
        printf(VERDE "ou seja, se houver apenas um 'a', voce nao pode escrever CASA\n" RESET);      
        printf(VERDE "pois possui dois 'a'.\n\n" RESET);
        
        printf(AMARELO "4. COMO JOGAR:\n" RESET);
        printf(VERDE "O jogo deixa explicito o tamanho de cada palavra com '-' (hifens), use isso como referencia!\n" RESET);
        printf(VERDE "O jogo possui algumas fases, para zerar eh necessario passar por todas e vencer!\n" RESET);
        printf(VERDE "Use o teclado para digitar as palavras que voce encontrou\n\n" RESET);
        
        printf(AMARELO "5. EVOLUCAO:\n" RESET);
        printf(VERDE "O sistema de ranking do jogo eh uma tabela de pontuacao com seus respectivos jogadores (nicknames)\n" RESET);
        printf(VERDE "no qual esta ordenado do jogador com maior pontuacao ao menor!\n\n" RESET);
        
        printf(MAGENTA "Aproveite o jogo!!!!\n\n" RESET);
        limpar_buffer();

        printf(CIANO "Tecle <enter> para prosseguir..." RESET);
        scanf("%c", &tecla_enter);
        chamar_menu();

    }

    void ranking_jogo(){
        fb = fopen("ranking.bin", "ab");
        
        if(fb != NULL){ // o arquivo tem que existir pra conseguir escrever!
            fwrite(&DadosDoJogador, sizeof(DadosDoJogador), 1, fb);
            fclose(fb);
        }
        else
           printf(VERMELHO "ERRO!!! Ranking ainda nao existe, ou arquivo nao encontrado.\n" RESET); 
    }

    void leitura_ranking_jogo(){
        fb = fopen("ranking.bin", "rb");

        if(fb == NULL){ // ou seja o jogador iniciou pela primeira vez o jogo, e ainda n existe um arquivo ranking.bin
            system(CLEAR);
            printf(AZUL "|=====" RESET VERMELHO " < JOGO VEILED WORDS > " RESET AZUL"=====|\n\n" RESET);
            printf(AMARELO "RANKING:\n\n" RESET); 
            printf(VERMELHO "Ainda nao existe jogadores no ranking, tecle <enter>..." RESET);
            limpar_buffer();
            scanf("%c", &tecla_enter);
            chamar_menu();
            return;
        }

        dadosdojogador ranking_info_jogadores[50]; // vetor da struct que contem a pontuacao e o nickname dos jogadores
        int jogadores = 0;

            if(fb != NULL){
                while(fread(&ranking_info_jogadores[jogadores], sizeof(dadosdojogador), 1, fb) == 1)
                    jogadores++;
            }

            for(int idx = 0; idx < jogadores - 1; idx++){ // bubblesort que foi ensinado em sala de aula, vai ordenar de forma decrescente a pontuacao dos jogadores conforme o projeto pede
                for(int jdx = 0; jdx < jogadores - idx - 1; jdx++){
                        
                    if(ranking_info_jogadores[jdx].pontuacao < ranking_info_jogadores[jdx + 1].pontuacao){
                        dadosdojogador aux = ranking_info_jogadores[jdx];
                        ranking_info_jogadores[jdx] = ranking_info_jogadores[jdx + 1];
                        ranking_info_jogadores[jdx + 1] = aux;
                    }
                }
            }   
            
        system(CLEAR);
        printf(AZUL "|=====" RESET VERMELHO " < JOGO VEILED WORDS > " RESET AZUL"=====|\n\n" RESET);
        printf(AMARELO "RANKING:\n\n" RESET);

        for(int i = 0; i < jogadores; i++)
            printf(MAGENTA"%d: "RESET VERDE"%s\n" RESET,ranking_info_jogadores[i].pontuacao, ranking_info_jogadores[i].nickname);
        
        fclose(fb); // nao preciso mais do arquivo binario aberto, ja ordenou e printou
        printf("\n");
        limpar_buffer();

        printf(CIANO "Tecle <enter> para continuar: " RESET);
        scanf("%c", &tecla_enter);
        chamar_menu();
    }

    void zerar_ranking(){
        while(1){
            printf(VERMELHO "Confirma reinicializar o ranking? (S/N)? " RESET);
            scanf(" %c", &sim_ou_nao);
            limpar_buffer();
        
            if(sim_ou_nao == 'S' || sim_ou_nao == 's'){
                fb = fopen("ranking.bin", "wb");    
                
            if(fb != NULL){ // o arquivo tem q existir pra ser zerado por isso o uso do if 
                fclose(fb);
                system(CLEAR);
                printf(CIANO "Ranking zerado! tecle <enter> para continuar\n" RESET);
            }
            else    
                printf(VERMELHO "ERRO!!! Ranking ainda nao existe, ou arquivo nao encontrado\n!" RESET); 
               
            scanf("%c", &tecla_enter);
            break;
            }
            else if(sim_ou_nao == 'N' || sim_ou_nao == 'n')
                break;

            if(sim_ou_nao != 'S' || sim_ou_nao != 's' || sim_ou_nao != 'N' || sim_ou_nao != 'n'){ // caso o jogador digite qlqr coisa diferente de S/s OU N/n
                printf(VERMELHO "ERRO!, digite novamente...\n\n" RESET);
            }
        }
        chamar_menu(); // o jogador volta pro menu iniciar
    }

    int chamar_menu(){
        system(CLEAR);

            printf(AZUL "|=====" RESET VERMELHO " < JOGO VEILED WORDS > " RESET AZUL"=====|\n\n" RESET);
            
            printf(AMARELO "1 - Jogar\n" RESET);
            printf(AMARELO "2 - Configuracoes\n" RESET);
            printf(AMARELO "3 - Instrucoes\n" RESET);
            printf(AMARELO "4 - Ranking\n" RESET);
            printf(AMARELO "5 - Sair\n\n" RESET);

            printf(CIANO "Digite a opcao desejada: " RESET);
            scanf("  %d", &opcao_switch_case);

            switch(opcao_switch_case){ // switch case para o jogador escolher as opcoes do menu
                case 1: // jogar
                    system(CLEAR);
                    if(ja_joguei == 1){
                        printf(VERMELHO"EII!!!! voce nao pode jogar mais de uma vez," RESET CIANO " digite <enter>... \n" RESET);
                        limpar_buffer();
                        scanf("%c", &tecla_enter);
                        chamar_menu();
                    }
                    chamar_jogo();
                    break;  

                case 2: // configuracoes
                    system(CLEAR);
                    confguracoes_jogo();
                    break;

                case 3: // instrucoes
                    system(CLEAR);
                    instrucoes_jogo();
                    break;

                case 4: // ranking
                    system(CLEAR);
                    leitura_ranking_jogo();
                    break;

                case 5: // encerrar o jogo - sair
                    return 0; 
            }
    }

    int main(){
        system(CLEAR); // funcao de limpar o terminal - tela, nesse caso eu vou limpar antes pra ficar mais bonito no terminal!
        printf("\n\n\n");
        
        // tela de boas vindas
        printf(AZUL "Bem vindo(a) ao Jogo" RESET VERMELHO " VEILED WORDS!!!!\n\n" RESET);
        
        printf(CIANO "Informe seu nickname: " RESET);
        
        scanf("%[^\n]", DadosDoJogador.nickname); // scanf aprimorado para o jogador colocar o nickname com espacos caso queira
        limpar_buffer(); // limpar o buffer no n windows e linux fflush e getchar

        system(CLEAR); // funcao de limpar o terminal - tela
        chamar_menu(); // menu iniciar 

       return 0;
    }
