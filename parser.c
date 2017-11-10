#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estado.h"
#include "cgi.h"
#include "parser.h"

/**
@file parser.c
Ficheiro que contêm a maioria do codigo necessario para a execução do jogo em si e de todos os diferentes comandos que este têm
*/

/**
\brief Função que testa se as coordenadas para onde o jogador ou inimigo se vão tentar mexer estão dentro do limite do mapa
@param x A coordenada x do jogador/inimigo
@param y A coordenada y do jogador/inimigo
@returns Um int, 0 ou 1, que corresponde a 0 caso as coordenadas estejam fora do limite do mapa, e 1 caso as coordenadas estejam dentro do limite do mapa
*/ 
int pos_valida(int x, int y) {
	return x >= 0 && y >= 0 && x < SIZE && y < SIZE;
}

/**
\brief Função que vai imprimir imagens no ecra, usada para imprimir imagens que ñ tem a ver com o tabuleiro de jogo, pois estas precisam de mais liberdade de coordenadas na sua posição
@param x A coordenada x onde a imagem vai ser colocada
@param y A coordenada y onde a imagem vai ser colocada
@param tamX O comprimento que a imagem vai ter
@param tamY A largura que a imagem vai ter
@param name O nome correspondente a imagem que se quer imprimir
*/
void print_image (int x, int y, int tamX, int tamY, char * name){
	printf("<image x=%d y=%d height=%d width=%d xlink:href=\"%s\"/>\n",x,y,tamX,tamY,name);
}

/**
\brief Função que vai gerar um numero aleatorio entre 0 e 9, sendo este numero depois usado para representar uma das coordenadas x ou y, dos inimigos/obstaculos
@returns Um int entre 0 e 9, para ser usado como coordenada
*/
int generate_Pos () {
	 int r;
	 r=rand() %SIZE;
	 return r;
}

/**
\brief Função que vai verificar se as posições dos inimigos geradas atraves do uso da generate_pos ja estão ocupadas pelo jogador, pela saída ou por outros inimigos
@param e Corresponde ao estado de jogo atual
@param ix Corresponde á coordenada x do inimigo
@param iy Corresponde á coordenada y do inimigo
@param i Corresponde á posição deste mesmo inimigo no array, com fim a impedir que a função leia para lá do que foi definido, pois esta e a ultima posição no array 
@param r Corresponde a variavel que esta a indicar a posiçao do jogador e da saida, ela pode estar entre o valor 0 a 3
@returns Um int, 0 ou 1, 0 se a posiçao estiver ocupada, gerando assim uma nova posiçao para o inimigo, e 1 se a posiçao estiver livre
*/
int validate_PosI (ESTADO e, int ix, int iy, int i, int r){
	int it;	
	if((e.jog.x==ix)&&(e.jog.y==iy)){return 0;}
	if((e.goal.x==ix)&&(e.goal.y==iy)){return 0;}
	if(r==0){
		if((ix==8&&iy==9)||(ix==8&&iy==8)||(ix==9&&iy==8)){return 0;}}
	if(r==1){
		if((ix==1&&iy==1)||(ix==0&&iy==1)||(ix==1&&iy==0)){return 0;}}
	if(r==2){
		if((ix==1&&iy==8)||(ix==0&&iy==8)||(ix==1&&iy==9)){return 0;}}
	if(r==3){	
		if((ix==8&&iy==0)||(ix==8&&iy==1)||(ix==9&&iy==1)){return 0;}}
	for(it=0;it<i;it++){
		if((e.inimigo[it].x==ix)&&(e.inimigo[it].y==iy)){return 0;}
	}
	return 1;}

/**
\brief Função que vai verificar se as posições dos obstaculos geradas atraves do uso da generate_pos ja estão ocupadas pelo jogador, pela saída, pelos inimigos ou por outros obstaculos
@param e Corresponde ao estado de jogo atual
@param ox Corresponde á coordenada x do obstaculo
@param oy Corresponde á coordenada y do obstaculo
@param o Corresponde á posição deste mesmo obstaculo no array, com fim a impedir que a função leia para lá do que foi definido, pois esta e a ultima posição no array
@param i Corresponde ao numero total de inimigos que existem, para que a funçao saiba quantas variaveis testar no array dos inimigos
@param r Corresponde a variavel que esta a indicar a posiçao do jogador e da saida, ela pode estar entre o valor 0 a 3
@returns Um int, 0 ou 1, 0 se a posiçao estiver ocupada, gerando assim uma nova posiçao para o obstaculo, e 1 se a posiçao estiver livre
*/
int validate_PosO (ESTADO e, int ox, int oy, int o, int i, int r){
	int ot;
	if((e.jog.x==ox)&&(e.jog.y==oy)){return 0;}
	if((e.goal.x==ox)&&(e.goal.y==oy)){return 0;}
	if(r==0){
		if((ox==8&&oy==9)||(ox==8&&oy==8)||(ox==9&&oy==8)||(ox==7&&oy==7)){return 0;}}
	if(r==1){
		if((ox==1&&oy==1)||(ox==0&&oy==1)||(ox==1&&oy==0)||(ox==2&&oy==2)){return 0;}}
	if(r==2){
		if((ox==1&&oy==8)||(ox==0&&oy==8)||(ox==1&&oy==9)||(ox==2&&oy==7)){return 0;}}
	if(r==3){	
		if((ox==8&&oy==0)||(ox==8&&oy==1)||(ox==9&&oy==1)||(ox==7&&oy==2)){return 0;}}
	for(ot=0;ot<o;ot++){
		if((e.obstaculo[ot].x==ox)&&(e.obstaculo[ot].y==oy)){return 0;}}
	for(ot=0;ot<i;ot++){
		if((e.inimigo[ot].x==ox)&&(e.inimigo[ot].y==oy)){return 0;}
	}
	return 1;}
/**
\brief Funçao que vai verificar se a posiçao para onde o jogador esta a tentar mexer-se esta ocupada por um obstaculo
@param e Corresponde ao estado de jogo atual
@param x Corresponde a coordenada x do jogador
@param y Corresponde a coordenada y do jogador
@param o Corresponde ao numero de obstaculos existentes, com fim a impedir que a funçao leia para la do que foi definido no array
@returns Um int, 0 ou 1, 0 se a posiçao estiver ocupada, e 1 se a posiçao estiver livre
*/
int validate_PosPl (ESTADO e, int x, int y, int o){
	int t;
	for(t=0;t<o;t++){
		if((e.obstaculo[t].x==x)&&(e.obstaculo[t].y==y)){return 0;}
	}
	return 1;}

/**
\brief Esta e uma das funçoes que vao inicializar o estado de jogo, sendo esta a primeira a ser executada, pois as variaveis nela definidas nao podem levar reset ao passar de um nivel para o proximo, por isso sao definidas aqui, e a funçao e executada apenas uma vez por jogo
@returns o estado de jogo com algumas das variaveis definidas
*/
ESTADO inicializar0(){
	ESTADO e;
	memset(&e, 0, sizeof(ESTADO));
	e.nivel=0;
	e.score=0;
	e.gold=0;
	e.class_value = 0;
	e.class_sprite = 0;
	return e;}

/**
\brief Esta e a segunda funçao que inicializa o estado do jogo, esta vai definir as variaveis que se alteram sempre que o jogador entra num novo nivel
@param e Corresponde ao estado de jogo incompleto, fornecido pela inicializar0
@returns o estado de jogo com todas as variaveis ja definidas
*/
ESTADO inicializar(ESTADO e) {
	int r,i,o,h,j;
	e.nivel++;
	r=rand() %4; /**Esta variavel (r) vai tomar um valor entre 0 e 3 (r=rand() %4;), este valor vai depois corresponder a uma das 4 posiçoes predefinidas do jogador e da saida no começo do nivel*/
	if(r==0){
		e.jog.x=9;
		e.jog.y=9;
		e.goal.x=4;
		e.goal.y=4;}
	if(r==1){
		e.jog.x=0;
		e.jog.y=0;
		e.goal.x=5;
		e.goal.y=5;}
	if(r==2){
		e.jog.x=0;
		e.jog.y=9;
		e.goal.x=5;
		e.goal.y=4;}
	if(r==3){
		e.jog.x=9;
		e.jog.y=0;
		e.goal.x=4;
		e.goal.y=5;}
	e.num_inimigos = 9+(e.nivel);
	e.num_obstaculos = 19+(e.nivel);
	e.range_value = 0;
	e.pick_value = 0;
	if(e.class_sprite==3){e.pick_maxUse = 6;}
	else{e.pick_maxUse = 3;}
	e.atk_OP = 0;
	e.path_value = 0;
	if(e.class_sprite==0){e.jogHP=2;}
	if(e.class_sprite==1){e.jogHP=3;}
	if(e.class_sprite==2){e.jogHP=1;}
	if(e.class_sprite==3){e.jogHP=1;}
	if(e.class_sprite==4){e.jogHP=2;}
	for(h=0;h<e.num_inimigos;h++){ 
		e.inimigoHP[h] = 1;}
	for(j=0;j<e.num_obstaculos;j++){ 
		e.obstaculoHP[j] = 1;}
	for(i=0;i<e.num_inimigos;i++){
		e.inimigo[i].x = generate_Pos();
		e.inimigo[i].y = generate_Pos();
		while(validate_PosI(e, e.inimigo[i].x, e.inimigo[i].y, i, r)!=1)
			{e.inimigo[i].x = generate_Pos();e.inimigo[i].y = generate_Pos();}}
	for(o=0;o<e.num_obstaculos;o++){
		e.obstaculo[o].x = generate_Pos();
		e.obstaculo[o].y = generate_Pos();
		while(validate_PosO(e, e.obstaculo[o].x, e.obstaculo[o].y, o, e.num_inimigos, r)!=1)
			{e.obstaculo[o].x = generate_Pos();e.obstaculo[o].y = generate_Pos();}}
		return e;}

/**
\brief Função que vai ler o conteudo do ficheiro que esta guardado, e converte-lo para o estado de jogo. Mas caso não haja nenhum ficheiro ou ele esteja vazio, ela vai começar um estado de jogo novo
@param name Corresponde ao nome da savefile que vai ser lida
@returns O Estado de jogo lido a partir da savefile, ou caso esta não exista, um estado de jogo completamente novo
*/
ESTADO read_file (char name[]){
	ESTADO e;
	FILE *fptr;
	char path[1240] ="/var/www/html/saves/";
	strcat(path,name);
	fptr=fopen(path,"rb");
	if(fptr){
		if((fread(&e, sizeof(ESTADO), 1, fptr)) == 0){
			e=inicializar0();
			e=inicializar(e);}
		fclose(fptr);}
	else{
		e=inicializar0();
		e=inicializar(e);}
	return e;
}

/**
\brief Função que vai guardar o estado de jogo num certo ficheiro
@param e Corresponde ao estado de jogo que vai ser guardado
@param name Corresponde ao nome da savefile onde o estado de jogo vai ser guardado
*/
void save_file (ESTADO e,char name[]){
	FILE *fptr;
	char path[1240] ="/var/www/html/saves/";
	strcat(path,name);
	fptr = fopen(path,"wb");
	fwrite(&e,sizeof(ESTADO),1,fptr);
	fclose(fptr);
}

/**
\brief Funçao que vai guardar a lista de scores realizados pelos jogadores no ficheiro
@param score_list Corresponde ao array que contem os scores para ser guardado no ficheiro
*/
void save_scorefile (int score_list[]){
	FILE *fptr;
	int i;
	fptr = fopen("/var/www/html/saves/scorefile","wb");
	for(i=0;i<5;i++){
	fwrite(&score_list[i],sizeof(int),1,fptr);}
	fclose(fptr);
}

/**
\brief Esta funçao vai guardar os nomes dos 5 melhores jogadores ate ao momento, vai fazer isto ao colocar 5 nomes diferentes em 5 ficheiros diferentes, cada um com a posiçao correspondente do jogador no nome do ficheiro
@param name Corresponde ao nome da savefile onde o estado de jogo vai ser guardado
@param i Este valor serve para dizer em que ficheiro vai guardar o nome do jogador que acabou de receber
*/

void save_namefile (char name[],int i){
	FILE *fptr;
	if(i==1){
		fptr = fopen("/var/www/html/saves/name1","w");
		for(i=0;name[i]!='\0';i++){
		fwrite(&name[i],sizeof(char),1,fptr);}
		fclose(fptr);
	}
	if(i==2){
		fptr = fopen("/var/www/html/saves/name2","w");
		for(i=0;name[i]!='\0';i++){
		fwrite(&name[i],sizeof(char),1,fptr);}
		fclose(fptr);
	}
	if(i==3){
		fptr = fopen("/var/www/html/saves/name3","w");
		for(i=0;name[i]!='\0';i++){
		fwrite(&name[i],sizeof(char),1,fptr);}
		fclose(fptr);
	}
	if(i==4){
		fptr = fopen("/var/www/html/saves/name4","w");
		for(i=0;name[i]!='\0';i++){
		fwrite(&name[i],sizeof(char),1,fptr);}
		fclose(fptr);
	}
	if(i==5){
		fptr = fopen("/var/www/html/saves/name5","w");
		for(i=0;name[i]!='\0';i++){
		fwrite(&name[i],sizeof(char),1,fptr);}
		fclose(fptr);
	}
}

/**
\brief Esta funçao vai reordenar os nomes dos jogadores, dependendo do lugar que o jogador que jogou agora teve na lista das pontuaçoes
@param name Corresponde ao nome da savefile onde o estado de jogo vai ser guardado
@param r Esta funçao e invocada pela funçao save_score, que vai colocar a pontuaçao do jogador num lugar nos top 5, a posiçao do jogador nos array do score vai ser este valor, isto para meter o nome desse jogador no seu ficheiro apropriado
*/
void save_scorename (char name[],int r){
	FILE *name1; FILE *name2; FILE *name3; FILE *name4; FILE *name5;
	char n1[9]; char n2[9]; char n3[9]; char n4[9]; char n5[9]; 
	char nr1[9]; char nr2[9]; char nr3[9]; char nr4[9]; char nr5[9];

	name1=fopen("/var/www/html/saves/name1","r");
	if(name1){
		if(fgets(n1,9,name1))
			fclose(name1);
		else{printf("error\n");}}
	else{strcpy(n1,"aaaaaaaa\0");}
	
	name2=fopen("/var/www/html/saves/name2","r");
	if(name2){
		if(fgets(n2,9,name2))
			fclose(name2);
		else{printf("error\n");}}
	else{strcpy(n2,"aaaaaaaa\0");}
	
	name3=fopen("/var/www/html/saves/name3","r");
	if(name3){
		if(fgets(n3,9,name3))
			fclose(name3);
		else{printf("error\n");}}
	else{strcpy(n3,"aaaaaaaa\0");}

	name4=fopen("/var/www/html/saves/name4","r");
	if(name4){
		if(fgets(n4,9,name4))
			fclose(name4);
		else{printf("error\n");}}
	else{strcpy(n4,"aaaaaaaa\0");}
	
	name5=fopen("/var/www/html/saves/name5","r");
	if(name5){
		if(fgets(n5,9,name5))
			fclose(name5);
		else{printf("error\n");}}
	else{strcpy(n5,"aaaaaaaa\0");}
	
	if(r==0){
		strcpy(nr1,name);
		strcpy(nr2,n1);
		strcpy(nr3,n2);
		strcpy(nr4,n3);
		strcpy(nr5,n4);
		save_namefile(nr1,1);
		save_namefile(nr2,2);
		save_namefile(nr3,3);
		save_namefile(nr4,4);
		save_namefile(nr5,5);
		return;
	}
	if(r==1){
		strcpy(nr2,name);
		strcpy(nr3,n2);
		strcpy(nr4,n3);
		strcpy(nr5,n4);
		save_namefile(nr2,2);
		save_namefile(nr3,3);
		save_namefile(nr4,4);
		save_namefile(nr5,5);
		return;
	}
	if(r==2){
		strcpy(nr3,name);
		strcpy(nr4,n3);
		strcpy(nr5,n4);
		save_namefile(nr3,3);
		save_namefile(nr4,4);
		save_namefile(nr5,5);
		return;
	}
	if(r==3){
		strcpy(nr4,name);
		strcpy(nr5,n4);
		save_namefile(nr4,4);
		save_namefile(nr5,5);
		return;
	}
	if(r==4){
		strcpy(nr5,name);
		save_namefile(nr5,5);
		return;
	}
}

/**
\brief Funçao que vai multiplicar o score do jogador por 1.2, isto so e possivel quando a classe a ser usada e o SunBro
@param e Corresponde ao estado atual de jogo
@returns O estado de jogo com a nova pontuaçao do jogador
*/
ESTADO score_mult (ESTADO e){
	int c;
	c = e.score * 1.2;
	e.score = c;
	return e;
}

/**
\brief Funçao que vai vai pegar no score do jogo atual, vai ler o ficheiro que tem os scores e vai comparar o score atual com os que tao no array que lee. Se o score atual for maior que algum do array, este vai ocupar o seu lugar e "puxar" os outros para baixo. Existem no total 5 scores no array
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void save_score (ESTADO e,char name[]){
	FILE *fptr;
	int score_list[5];
	int i,tx,ty,r;
	tx=0; ty=0; r=0;
	if(e.class_sprite==4){e = score_mult(e);}
	fptr=fopen("/var/www/html/saves/scorefile","rb");
	if(fptr){
		if(fread(&score_list, sizeof(int),5, fptr))
			fclose(fptr);
		else{printf("error\n");}}
	else{
		for(i=0;i<5;i++){
			score_list[i]=0;
		}
	}
	for(i=0;(e.score < score_list[i])&&(i<5);i++);
	if((i<5) && ((e.score > score_list[4]))){
		r=i;
		tx = score_list [i];
		score_list [i] = e.score;
		i++;
		while(i<5){
			ty = score_list[i];
			score_list[i] = tx;
			tx = ty;
			i++;}
		save_scorefile(score_list);
		save_scorename(name,r);
	}
} 

/**
\brief Esta funçao vai imprimir no ecra a informaçao sobre os 5 melhores scores realizados ate ao momento
*/
void print_score (){
	int score_list[5];
	char n1[9]; char n2[9]; char n3[9]; char n4[9]; char n5[9];
	FILE *fptr;  FILE *name1; FILE *name2; FILE *name3; FILE *name4; FILE *name5;
	fptr=fopen("/var/www/html/saves/scorefile","rb");
	if(fptr){
		if (fread(&score_list, sizeof(int),5, fptr))
			fclose(fptr);
		else{printf("error\n");}}
	else{return;}
	
	name1=fopen("/var/www/html/saves/name1","r");
	if(name1){
		if(fgets(n1,9,name1))
			fclose(name1);
		else{printf("error\n");}}
	else{return;}
	
	name2=fopen("/var/www/html/saves/name2","r");
	if(name2){
		if(fgets(n2,9,name2))
			fclose(name2);
		else{printf("error\n");}}
	else{return;}
	
	name3=fopen("/var/www/html/saves/name3","r");
	if(name3){
		if(fgets(n3,9,name3))
			fclose(name3);
		else{printf("error\n");}}
	else{return;}

	name4=fopen("/var/www/html/saves/name4","r");
	if(name4){
		if(fgets(n4,9,name4))
			fclose(name4);
		else{printf("error\n");}}
	else{return;}
	
	name5=fopen("/var/www/html/saves/name5","r");
	if(name5){
		if(fgets(n5,9,name5))
			fclose(name5);
		else{printf("error\n");}}
	else{return;}

	printf("<TEXT X=900 Y=115> %s </TEXT></A>\n", n1);
	printf("<TEXT X=1050 Y=115> %d </TEXT></A>\n",score_list[0]);
	printf("<TEXT X=900 Y=155> %s </TEXT></A>\n", n2);
	printf("<TEXT X=1050 Y=155> %d </TEXT></A>\n",score_list[1]);
	printf("<TEXT X=900 Y=195> %s </TEXT></A>\n", n3);
	printf("<TEXT X=1050 Y=195> %d </TEXT></A>\n",score_list[2]);
	printf("<TEXT X=900 Y=235> %s </TEXT></A>\n", n4);
	printf("<TEXT X=1050 Y=235> %d </TEXT></A>\n",score_list[3]);
	printf("<TEXT X=900 Y=275> %s </TEXT></A>\n", n5);
	printf("<TEXT X=1050 Y=275> %d </TEXT></A>\n",score_list[4]);
}

/**
\brief Funçao que vai imprimir a imagem de background do jogo
*/
void print_background(){
	print_image(0,-315,1400,1500,"http://localhost/images/back.jpg");
}

/**
\brief Esta funçao vai imprimir o tabuleiro onde se joga 
*/
void print_board () {
	int X, Y;

	for (Y=0;Y<5;Y++)
		for (X=0; X<5; X++)
			IMAGEM(X,Y,TAM*2, "floor.jpg");
}

/**
\brief Esta funçao vai imprimir a saida para o proximo nivel
@param e Corresponde ao estado de jogo atual, para saber em que coordenada imprime a saida
*/
void print_goal (ESTADO e) {
   IMAGEM(e.goal.x,e.goal.y,TAM,"saida_2.png");
}

/**
\brief Esta funçao vai calcular a diferença entre dois numeros
@param z Corresponde a um numero 1 
@param x Corresponde a um numero 2
@returns o valor da diferença entre o numero 1 e o numero 2
*/
int diferenca_xy(int z, int x){
	int i;
	i=z-x;
	return i;
}

/**
\brief Esta funçao vai somar dois numeros
@param z Corresponde a um numero 1 
@param x Corresponde a um numero 2
@returns o valor da soma do numero 1 e o numero 2
*/
int soma_xy(int z, int x){
	int i;
	i=z+x;
	return i;
}

/**
\brief Esta funçao vai verificar se existe algum inimigo na posiçao fornecida
@param e Corresponde ao estado de jogo atual
@param x Corresponde a coordenada x fornecida
@param y Corresponde a coordenada y fornecida
@param i Corresponde ao numero total de inimigos existente no array do estado de jogo
@returns Um int, caso a posiçao esteja ocupada por um inimigo, vai dar return da posiçao da posiçao desse mesmo inimigo no array. Se a posiçao nao estivar ocupada, vai dar return do valor 100
*/
int chk_for_enemy (ESTADO e, int x, int y, int i){
	int t;
	for(t=0;t<i;t++){
		if((e.inimigo[t].x==x)&&(e.inimigo[t].y==y)){return t;}
	}
	return 100;
}

/**
\brief Esta funçao vai verificar se existe algum inimigo na posiçao fornecida
@param e Corresponde ao estado de jogo atual
@param x Corresponde a coordenada x fornecida
@param y Corresponde a coordenada y fornecida
@param i Corresponde ao numero total de inimigos existente no array do estado de jogo
@param l Corresponde a posiçao do inimigo no array, cujas coordenadas foram fornecidas, para que nao se teste a si proprio na funçao
@returns Um int, 0 ou 1, 0 se as coordenadas fornecidas estiverem ocupadas por um inimigo, ou 1 caso elas estejam livres 
*/
int chk_for_enemy_2 (ESTADO e, int x, int y, int i, int l){
	int t;
	for(t=0;t<i;t++){
		if(t!=l){
		if((e.inimigo[t].x==x)&&(e.inimigo[t].y==y)){return 0;}}
	}
	return 1;
}

/**
\brief Esta funçao vai verificar se o jogador esta na posiçao fornecida
@param e Corresponde ao estado de jogo atual
@param x Corresponde a coordenada x fornecida
@param y Corresponde a coordenada y fornecida
@returns Um int, 0 ou 1, 1 se as coordenadas fornecidas estiverem ocupadas pelo jogador, ou 0 caso elas estejam livres
*/
int isPlayer(ESTADO e, int x, int y){
	if(e.jog.x==x && e.jog.y==y){return 1;}
	else {return 0;}
}
/**
\brief Esta funçao vai verificar se existe algum obstaculo na posiçao fornecida
@param e Corresponde ao estado de jogo atual
@param x Corresponde a coordenada x fornecida
@param y Corresponde a coordenada y fornecida
@param i Corresponde ao numero total de obstaculos existente no array do estado de jogo
@returns Um int, caso a posiçao esteja ocupada por um obstaculo, vai dar return da posiçao desse mesmo obstaculo no array. Se a posiçao nao estivar ocupada, vai dar return do valor 100
*/
int chk_for_obstacle (ESTADO e, int x, int y, int i){
	int t;
	for(t=0;t<i;t++){
		if((e.obstaculo[t].x==x)&&(e.obstaculo[t].y==y)){return t;}
	}
	return 100;
}

/* near player */

/**
\brief Função que testa se as coordenadas para onde o jogador ou inimigo se vão tentar mexer estão dentro do limite do mapa
@param x A coordenada x do jogador/inimigo
@param y A coordenada y do jogador/inimigo
@returns Um int, 0 ou 1, que corresponde a 0 caso as coordenadas estejam fora do limite do mapa, e 1 caso as coordenadas estejam dentro do limite do mapa
*/ 
int pos_valida2(int x, int y){
	if(x >= 0 && y >= 0 && x < SIZE && y < SIZE){return 1;}
	return 0;
}

/**
\brief Função que testa se dois pares de coordenadas sao iguais
@param x A coordenada x1 fornecida
@param y A coordenada y1 fornecida
@param a A coordenada x2 fornecida
@param b A coordenada y2 fornecida
@returns Um int, 0 ou 1, que corresponde a 0 caso os pares de coordenadas sejam diferentes, e 1 caso os pares de coordenadas sejam iguais
*/ 
int iguais (int x, int y, int a, int b){
	if(x==a && y==b){return 1;}
	return 0;
}
/**
\brief Funçao que vai efetuar o movimento dos inimigos, atraves do uso de um sistema de campo de visao, assim, caso o jogador esteja a 4 ou menos casas de distancia de um inimigo este vai movver-se na sua direçao
@param e Corresponde ao estado de jogo atual
@param x Corresponde a coordenada x do jogador
@param y Corresponde a coordenada y do jogador
@returns A funçao da return de um novo estado ja com as posiçoes alteradas dos inimigos que tinham o jogador no seu campo de visao 
*/
ESTADO enemy_move (ESTADO e, int x, int y){
	int i,dx,dy,x_reserva,y_reserva;
	x_reserva = 0;
	y_reserva = 0;
	for(i=0;i<e.num_inimigos;i++){
		dx=diferenca_xy(e.inimigo[i].x, x);
		dy=diferenca_xy(e.inimigo[i].y, y);
		if(dx<=4 && dx>=-4 && dy<=4 && dy>=-4){
			if(e.jogHP==0){return e;}
			if(dx>0 && dy>0){
				x_reserva=e.inimigo[i].x;
				y_reserva=e.inimigo[i].y;
				e.inimigo[i].x+=-1;
				e.inimigo[i].y+=-1;
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=1;e.inimigo[i].y+=0;/*1*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=-1;e.inimigo[i].y+=1;/*2*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x=x_reserva;e.inimigo[i].y=y_reserva;/*0*/}}
			if(dx<0 && dy<0){
				x_reserva=e.inimigo[i].x;
				y_reserva=e.inimigo[i].y;
				e.inimigo[i].x+=1;
				e.inimigo[i].y+=1;
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=0;e.inimigo[i].y+=-1;/*1*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=-1;e.inimigo[i].y+=1;/*2*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x=x_reserva;e.inimigo[i].y=y_reserva;/*0*/}}
			if(dx>0 && dy<0){
				x_reserva=e.inimigo[i].x;
				y_reserva=e.inimigo[i].y;
				e.inimigo[i].x+=-1;
				e.inimigo[i].y+=1;
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=1;e.inimigo[i].y+=0;/*1*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=-1;e.inimigo[i].y+=-1;/*2*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x=x_reserva;e.inimigo[i].y=y_reserva;/*0*/}}
			if(dx<0 && dy>0){
				x_reserva=e.inimigo[i].x;
				y_reserva=e.inimigo[i].y;
				e.inimigo[i].x+=1;
				e.inimigo[i].y+=-1;
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=0;e.inimigo[i].y+=1;/*1*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=-1;e.inimigo[i].y+=-1;/*2*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x=x_reserva;e.inimigo[i].y=y_reserva;/*0*/}}
			if(dx==0 && dy<0){
				x_reserva=e.inimigo[i].x;
				y_reserva=e.inimigo[i].y;
				e.inimigo[i].x+=0;
				e.inimigo[i].y+=1;
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=-1;e.inimigo[i].y+=0;/*1*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=2;e.inimigo[i].y+=0;/*2*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x=x_reserva;e.inimigo[i].y=y_reserva;/*0*/}}
			if(dx==0 && dy>0){
				x_reserva=e.inimigo[i].x;
				y_reserva=e.inimigo[i].y;
				e.inimigo[i].x+=0;
				e.inimigo[i].y+=-1;
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=1;e.inimigo[i].y+=0;/*1*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=-2;e.inimigo[i].y+=0;/*2*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x=x_reserva;e.inimigo[i].y=y_reserva;/*0*/}}
			if(dx<0 && dy==0){
				x_reserva=e.inimigo[i].x;
				y_reserva=e.inimigo[i].y;
				e.inimigo[i].x+=1;
				e.inimigo[i].y+=0;
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=0;e.inimigo[i].y+=-1;/*1*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=0;e.inimigo[i].y+=2;/*2*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x=x_reserva;e.inimigo[i].y=y_reserva;/*0*/}}
			if(dx>0 && dy==0){
				x_reserva=e.inimigo[i].x;
				y_reserva=e.inimigo[i].y;
				e.inimigo[i].x+=-1;
				e.inimigo[i].y+=0;
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=0;e.inimigo[i].y+=-1;/*1*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x+=0;e.inimigo[i].y+=2;/*2*/}
				if(validate_PosPl(e,e.inimigo[i].x,e.inimigo[i].y,e.num_obstaculos)!=1 || pos_valida2(e.inimigo[i].x,e.inimigo[i].y)!=1 || iguais(e.inimigo[i].x,e.inimigo[i].y,e.goal.x,e.goal.y)==1 || chk_for_enemy_2(e, e.inimigo[i].x, e.inimigo[i].y, e.num_inimigos, i)!=1){
					e.inimigo[i].x=x_reserva;e.inimigo[i].y=y_reserva;/*0*/}}
		}		
		if(isPlayer(e, e.inimigo[i].x, e.inimigo[i].y)==1){
			e.jogHP+=-1;
			if(e.score>=1){e.score-=1;};
			e.inimigo[i].x=x_reserva;
			e.inimigo[i].y=y_reserva;}
		}
	return e;
}

/**
\brief Esta funçao vai executar os diversos movimentos que o jogador pode fazer, mover-se para a goal, matar inimigos, destruir obstaculos e mover-se para casas diferentes
@param e Corresponde ao estado de jogo atual
@param dx Corresponde a deslocaçao na coordenada x que o jogador vai fazer
@param dy Corresponde a deslocaçao na coordenada y que o jogador vai fazer
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void print_movement(ESTADO e, int dx, int dy,char name[]) {
	ESTADO new = e;
	int x = e.jog.x + dx;
	int y = e.jog.y + dy;
	int h;
	
	if(new.atk_OP==1){new.atk_OP = 0;}

	if (x==e.goal.x && y==e.goal.y){
			new.jog.x = x;
			new.jog.y = y;
			new.score += 5;
			new = inicializar(new);
			save_file(new,name);
			return;}
	
		if(chk_for_enemy(e,x,y,e.num_inimigos)!=100){
			h = chk_for_enemy(e,x,y,e.num_inimigos);
			new.inimigoHP[h] += -1;
			new.inimigo[h].x=30;
			new.inimigo[h].y=30;
			new.score += 1;
			new.gold += 1;
			new.jog.x = e.jog.x;
			new.jog.y = e.jog.y;
			new= enemy_move(new, new.jog.x, new.jog.y);
			save_file(new,name);
			return;}

		if ((validate_PosPl(e,x,y,e.num_obstaculos)==0) && (e.pick_value!=0) && (e.pick_maxUse!=0)){
			h = chk_for_obstacle(e,x,y,e.num_obstaculos);
			new.obstaculoHP[h] += -1;
			new.obstaculo[h].x=31;
			new.obstaculo[h].y=31;
			new.jog.x = e.jog.x;
			new.jog.y = e.jog.y;
			new.pick_maxUse += -1;
			new.pick_value = 0;
			new.gold += 1;
			new= enemy_move(new, new.jog.x, new.jog.y);
			save_file(new,name);
			return;} 

	new.jog.x = x;
 	new.jog.y = y;
	new = enemy_move(new, new.jog.x, new.jog.y);

	save_file(new,name);
}

/**
\brief Esta funçao e uma versao diferente da funçao anterior, esta serve para as classes especiais que podem atacar em casas para os quais nao se podem mexer, ou seja os unicos movimentos efetuados nesta sao apenas os de matar inimigos e destruir obstaculos
@param e Corresponde ao estado de jogo atual
@param dx Corresponde a deslocaçao na coordenada x que o jogador vai fazer
@param dy Corresponde a deslocaçao na coordenada y que o jogador vai fazer
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void print_movement_2 (ESTADO e, int dx, int dy,char name[]) {
	ESTADO new = e;
	int x = e.jog.x + dx;
	int y = e.jog.y + dy;
	int h;

	if(chk_for_enemy(e,x,y,e.num_inimigos)!=100){
		h = chk_for_enemy(e,x,y,e.num_inimigos);
		new.inimigoHP[h] += -1;
		new.inimigo[h].x=30;
		new.inimigo[h].y=30;
		new.score += 1;
		new.gold += 1;
		new.jog.x = e.jog.x;
		new.jog.y = e.jog.y;
		new= enemy_move(new, new.jog.x, new.jog.y);
		save_file(new,name);
		return;}

	if ((validate_PosPl(e,x,y,e.num_obstaculos)==0) && (e.pick_value!=0) && (e.pick_maxUse!=0)){
			h = chk_for_obstacle(e,x,y,e.num_obstaculos);
			new.obstaculoHP[h] += -1;
			new.obstaculo[h].x=31;
			new.obstaculo[h].y=31;
			new.jog.x = e.jog.x;
			new.jog.y = e.jog.y;
			new.pick_maxUse += -1;
			new.pick_value = 0;
			new.gold += 1;
			new= enemy_move(new, new.jog.x, new.jog.y);
			save_file(new,name);
		return;}}

/**
\brief Funçao que imprime a imagem usada para comprar poçoes, caso a vida do jogador seja 0 ou o ouro do jogador ser inferior a 12, esta imagem nao vai ser clicavel, logo nao existe efeito no estado de jogo
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
@param action Corresponde ao valor da açao que foi efetuada pelo jogador, diferentes açoes possuem valores diferentes que levam a movimentos diferentes
*/
void print_pv (ESTADO e,char name[],int action){
	char link[MAX_BUFFER];
	if(e.jogHP==0 || e.gold<12){
		print_image(500,225,100,150,"http://localhost/images/potion.png");
		return;}
	if(e.gold>=12){
		sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
		ABRIR_LINK(link);
		print_image(500,225,100,150,"http://localhost/images/potion.png");
		FECHAR_LINK;}
}

/**
\brief Funçao que imprime a imagem usada para ativar o display da zona de ataque dos inimigos, caso a vida do jogador seja 0 esta imagem nao vai ser clicavel, logo nao existe efeito no estado de jogo
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
@param action Corresponde ao valor da açao que foi efetuada pelo jogador, diferentes açoes possuem valores diferentes que levam a movimentos diferentes
*/
void print_RButton (ESTADO e,char name[],int action){
	char link[MAX_BUFFER];
	if(e.jogHP>0){	
	sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
	ABRIR_LINK(link);
	print_image(650,175,100,150,"http://localhost/images/atkN.png");
	FECHAR_LINK;}
	else{print_image(650,175,100,150,"http://localhost/images/atkN.png");}
}

/**
\brief Funçao que imprime a imagem usada para ativar a destruiçao de obstaculos, caso a vida do jogador seja 0 esta imagem nao vai ser clicavel, logo nao existe efeito no estado de jogo
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
@param action Corresponde ao valor da açao que foi efetuada pelo jogador, diferentes açoes possuem valores diferentes que levam a movimentos diferentes
*/
void print_PButton (ESTADO e,char name[],int action){
	char link[MAX_BUFFER];
	if(e.jogHP>0){
	sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
	ABRIR_LINK(link);
	print_image(650,225,100,150,"http://localhost/images/PICKAXE.png");
	FECHAR_LINK;}
	else{print_image(650,225,100,150,"http://localhost/images/PICKAXE.png");}
}

/**
\brief Funçao que imprime a imagem usada para ativar o uso do arco, caso a vida do jogador seja 0, o ouro inferior a 10, ou o valor da sua classe ser 2/3, esta imagem nao vai ser clicavel, logo nao existe efeito no estado de jogo
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
@param action Corresponde ao valor da açao que foi efetuada pelo jogador, diferentes açoes possuem valores diferentes que levam a movimentos diferentes
*/
void print_BButton (ESTADO e,char name[],int action){
	char link[MAX_BUFFER];
	if(e.class_sprite==2||e.class_sprite==3){return;}
	if(e.gold>=10 && e.jogHP>0 ){
	sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
	ABRIR_LINK(link);
	print_image(500,175,100,150,"http://localhost/images/Bow.png");
	FECHAR_LINK;}
	else {print_image(500,175,100,150,"http://localhost/images/Bow.png");}
}

/**
\brief Funçao que imprime a imagem usada para ativar o uso da bomba, caso a vida do jogador seja 0 ou o ouro inferior a 20, esta imagem nao vai ser clicavel, logo nao existe efeito no estado de jogo
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
@param action Corresponde ao valor da açao que foi efetuada pelo jogador, diferentes açoes possuem valores diferentes que levam a movimentos diferentes
*/
void print_bomb (ESTADO e,char name[],int action){
	char link[MAX_BUFFER];
	if(e.jogHP>0 && e.gold>=20){
	sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
	ABRIR_LINK(link);
	print_image(500,275,100,150,"http://localhost/images/bomb.png");
	FECHAR_LINK;}
	else {print_image(500,275,100,150,"http://localhost/images/bomb.png");}
}

/**
\brief Funçao que imprime a imagem usada para ativar o display de um dos caminhos possiveis para poder-se chegar a saida, caso a vida do jogador seja 0, esta imagem nao vai ser clicavel, logo nao existe efeito no estado de jogo
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
@param action Corresponde ao valor da açao que foi efetuada pelo jogador, diferentes açoes possuem valores diferentes que levam a movimentos diferentes
*/
void print_pf(ESTADO e,char name[],int action){
	char link[MAX_BUFFER];
	if(e.jogHP>0){
	sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
	ABRIR_LINK(link);
	print_image(650,275,100,150,"http://localhost/images/Pathfinder.png");
	FECHAR_LINK;}
	else{print_image(650,275,100,150,"http://localhost/images/Pathfinder.png");}
}

/**
\brief Funçao que vai ser executada caso a imagem imprimida em print_pf seja clicada, ela vai alterar o valor da variavel do estado path_value, de 0 para 1 ou de 1 para 0
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void pathfind (ESTADO e,char name[]){
	if (e.path_value==0){e.path_value=1;}
	else {e.path_value=0;}
	save_file(e,name);
}

/**
\brief Funçao que vai ser executada caso a imagem imprimida em print_BButton seja clicada, ela vai alterar o valor da variavel do estado atk_OP, de 0 para 1 ou de 1 para 0, e vai retirar 10 de ouro ao jogador. Mas o jogador pode sempre receber o ouro de volta caso cancele o uso do arco
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void bow (ESTADO e,char name[]){
	if(e.atk_OP==0){e.atk_OP=1;e.gold+=-10;}
	else{e.atk_OP=0;e.gold+=10;}
	save_file(e,name);	
}

/**
\brief Funçao que vai ser executada caso a imagem imprimida em print_bomb seja clicada, ela vai matar os inimigos que estao em casas adjacentes ao jogador e tambem vai retirar 20 de ouro ao jogador 
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void bomb (ESTADO e,char name[]){
	int i,dx,dy;
	for (i=0;i<e.num_inimigos;i++){
		dx=diferenca_xy(e.inimigo[i].x,e.jog.x);
		dy=diferenca_xy(e.inimigo[i].y,e.jog.y);
			if(dx<=1 && dx>=-1 && dy<=1 && dy>=-1){
				e.inimigoHP[i]+=-1;
				e.inimigo[i].x+=30;
				e.inimigo[i].y+=30;
				e.score+=1;
				e.gold+=1;}}
	e.gold+=-20;
	save_file(e,name);
}

/**
\brief Funçao que vai ser executada caso a imagem imprimida em print_PButton seja clicada, ela vai alterar o valor da variavel do estado pick_value, de 0 para 1 ou de 1 para 0
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void pickaxe (ESTADO e,char name[]){
	if(e.pick_value==0){e.pick_value=1;}
	else{e.pick_value=0;}
	save_file(e,name);
}

/**
\brief Funçao que vai ser executada caso a imagem imprimida em print_pv seja clicada, ela vai adiconar um ponto de vida ao jogador, e retirar 12 de ouro ao jogador
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void potHP (ESTADO e,char name[]){
	e.jogHP+=1;
	e.gold+=-12;
	save_file(e,name);
}

/**
\brief Funçao que vai ser executada caso a imagem imprimida em print_RButton seja clicada, ela vai alterar o valor da variavel do estado range_value, de 0 para 1 ou de 1 para 0
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void range (ESTADO e,char name[]){
	if(e.range_value==0){e.range_value=1;}
	else{e.range_value=0;}
	save_file(e,name);	
}

/**
\brief Funçao que vai ser executada caso a imagem imprimida em print_Restart seja clicada, ela vai recomeçar o jogo a partir do inicio
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void weakling (ESTADO e,char name[]){
	e = inicializar0();
	e = inicializar(e);
	save_file(e,name);
}

/**
\brief Esta funçao vai pegar no valor da açao que foi obtida atraves do link, e vai realizar as funçoes correspondentes a essa açao 
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
@param action Corresponde ao valor da açao que foi efetuada pelo jogador, diferentes açoes possuem valores diferentes que levam a movimentos diferentes
*/
void process_action(ESTADO e,char name[], int action) {
	if(action==0){return;}
	if(action==96){e.class_sprite=1; e.jogHP=3; save_file(e,name); return;}
	if(action==97){e.class_sprite=2; e.jogHP=1; save_file(e,name); return;}
	if(action==98){e.class_sprite=3; e.jogHP=1; e.pick_maxUse = 6; save_file(e,name); return;}
	if(action==99){e.class_sprite=4; e.jogHP=2; save_file(e,name); return;}
	if(action==13){range(e,name);}
	if(action==28){pathfind(e,name);}
	if(e.path_value==1||e.range_value==1){return;}
	
	if(e.class_sprite==3){
		if(action==2){print_movement(e,  0, -1, name);}
		if(action==3){print_movement(e,  0, +1, name);}
		if(action==4){print_movement(e, -1,  0, name);}
		if(action==5){print_movement(e, +1,  0, name);}
		if(action==16){print_movement(e, 0, +2, name);}
		if(action==17){print_movement(e, 0, -2, name);}
		if(action==18){print_movement(e, +2, 0, name);}
		if(action==19){print_movement(e, -2, 0, name);}
		if(action==16){print_movement_2(e, 0, +2, name);return;}
		if(action==17){print_movement_2(e, 0, -2, name);return;}
		if(action==18){print_movement_2(e, +2, 0, name);return;}
		if(action==19){print_movement_2(e, -2, 0, name);return;}
		if(action==20){print_movement_2(e, +1, +2, name);return;}
		if(action==21){print_movement_2(e, +1, -2, name);return;}
		if(action==22){print_movement_2(e, +2, +1, name);return;}
		if(action==23){print_movement_2(e, +2, -1, name);return;}
		if(action==24){print_movement_2(e, -1, -2, name);return;}
		if(action==25){print_movement_2(e, -1, +2, name);return;}
		if(action==26){print_movement_2(e, -2, +1, name);return;}
		if(action==27){print_movement_2(e, -2, -1, name);return;}
		if(action==29){print_movement_2(e, -2, -2, name);return;}
		if(action==30){print_movement_2(e, +2, +2, name);return;}
		if(action==31){print_movement_2(e, +2, -2, name);return;}
		if(action==32){print_movement_2(e, -2, +2, name);return;}
		}
	if(e.class_sprite==2){
		if(action==16){print_movement_2(e, 0, +2, name);return;}
		if(action==17){print_movement_2(e, 0, -2, name);return;}
		if(action==18){print_movement_2(e, +2, 0, name);return;}
		if(action==19){print_movement_2(e, -2, 0, name);return;}
		if(action==20){print_movement_2(e, +1, +2, name);return;}
		if(action==21){print_movement_2(e, +1, -2, name);return;}
		if(action==22){print_movement_2(e, +2, +1, name);return;}
		if(action==23){print_movement_2(e, +2, -1, name);return;}
		if(action==24){print_movement_2(e, -1, -2, name);return;}
		if(action==25){print_movement_2(e, -1, +2, name);return;}
		if(action==26){print_movement_2(e, -2, +1, name);return;}
		if(action==27){print_movement_2(e, -2, -1, name);return;}
		if(action==29){print_movement_2(e, -2, -2, name);return;}
		if(action==30){print_movement_2(e, +2, +2, name);return;}
		if(action==31){print_movement_2(e, +2, -2, name);return;}
		if(action==32){print_movement_2(e, -2, +2, name);return;}}
	
	if(action==2){print_movement(e,  0, -1, name);return;}
	if(action==3){print_movement(e,  0, +1, name);return;}
	if(action==4){print_movement(e, -1,  0, name);return;}
	if(action==5){print_movement(e, +1,  0, name);return;}
	if(action==6){print_movement(e, +1, +1, name);return;}
	if(action==7){print_movement(e, +1, -1, name);return;}
	if(action==8){print_movement(e, -1, -1, name);return;}
	if(action==9){print_movement(e, -1, +1, name);return;}
	if(action==10){weakling(e,name);return;}
	if(action==11){potHP(e,name);return;}
	if(action==12){pickaxe(e,name);return;}
	if(action==14){bomb(e,name);return;}
	if(action==15){bow(e,name);return;}
	if(action==16){print_movement(e, 0, +2, name);return;}
	if(action==17){print_movement(e, 0, -2, name);return;}
	if(action==18){print_movement(e, +2, 0, name);return;}
	if(action==19){print_movement(e, -2, 0, name);return;}
	if(action==20){print_movement(e, +1, +2, name);return;}
	if(action==21){print_movement(e, +1, -2, name);return;}
	if(action==22){print_movement(e, +2, +1, name);return;}
	if(action==23){print_movement(e, +2, -1, name);return;}
	if(action==24){print_movement(e, -1, -2, name);return;}
	if(action==25){print_movement(e, -1, +2, name);return;}
	if(action==26){print_movement(e, -2, +1, name);return;}
	if(action==27){print_movement(e, -2, -1, name);return;}
	if(action==29){print_movement(e, -2, -2, name);return;}
	if(action==30){print_movement(e, +2, +2, name);return;}
	if(action==31){print_movement(e, +2, -2, name);return;}
	if(action==32){print_movement(e, -2, +2, name);return;}
}

/**
\brief Funçao que vai imprimir a imagem do jogador, existe um total de 4 imagens diferentes cada uma com um valor diferente representado na variavel do estado class_sprite 
@param e Corresponde ao estado de jogo atual
*/
void print_player (ESTADO e) {
	if(e.class_sprite==0){return;}
	else{
		if(e.class_sprite==1){IMAGEM(e.jog.x, e.jog.y, TAM, "knight_sprite.png");}

		if(e.class_sprite==2){IMAGEM(e.jog.x, e.jog.y, TAM, "mage_sprite2.png");}

		if(e.class_sprite==3){IMAGEM(e.jog.x, e.jog.y, TAM, "archer_sprite.png");}

		if(e.class_sprite==4){IMAGEM(e.jog.x, e.jog.y, TAM, "solaire_sprite.png");}
	}}
	
/**
\brief Funçao que vai imprimir as imagens dos inimigos nas suas posiçoes correspondentes
@param e Corresponde ao estado de jogo atual
*/
void print_enemies (ESTADO e) {
	int i;
	for(i = 0; i < e.num_inimigos; i++){
		if(e.inimigoHP[i] != 0){
			IMAGEM(e.inimigo[i].x, e.inimigo[i].y, TAM, "irongiant.png");
		}
	}
}

/**
\brief Funçao que vai imprimir a vida que o jogador, esta representada no canto inferior esquerdo do ecra, cada coraçao representa 1 vida
@param e Corresponde ao estado de jogo atual
*/
void print_vida (ESTADO e){
	int i,x,y;
	x=0;
	y=500;
	for(i=0;i<e.jogHP;i++){
		print_image(x,y,75,75,"http://localhost/images/heart.png");
		x+=75;
	}
}

/**
\brief Funçao que vai imprimir as imagens dos obstaculos nas suas posiçoes correspondentes
@param e Corresponde ao estado de jogo atual
*/
void print_walls (ESTADO e) {
	int i;
	for(i = 0; i < e.num_obstaculos; i++)
		IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, TAM, "creepwall.png");
}

/**
\brief Funçao que é ativada quando o jogador tiver vida==0, e vai imprimir a imagem a dizer "YOU DIED"
@param e Corresponde ao estado de jogo atual
*/
void print_gitgud (ESTADO e){
	if(e.jogHP==0)
		IMAGEM(0,0,TAM*SIZE,"death.png");
}

/**
\brief Funçao que é ativada quando o jogador tiver vida==0, e vai imprimir uma imagem clicavel que vai recomeçar o jogo 
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
@param action Corresponde ao valor da açao que foi efetuada pelo jogador, diferentes açoes possuem valores diferentes que levam a movimentos diferentes
*/
void print_restart (ESTADO e,char name[],int action){
	char link[MAX_BUFFER];
	if(e.jogHP==0){
		sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
		ABRIR_LINK(link);
		print_image(500,426,100,150,"http://localhost/images/Restart.png");
		FECHAR_LINK;}}

/**
\brief Funçao que vai imprimir as imagens que nao possuem um link e vai tambem imprimir as diferentes variaveis que correspondem aos recursos do jogador, tal como o nivel em que esta, o ouro que, os usos que tem restantes na picareta e tambem a sua pontuaçao 
@param e Corresponde ao estado de jogo atual
*/
void print_vsr (ESTADO e){
	print_image(760,60,400,450,"http://localhost/images/scoreBoard.jpg");
	print_image(500,125,100,150,"http://localhost/images/shop.png");
	print_image(500,0,100,150,"http://localhost/images/SCORE.png");
	print_image(650,0,100,150,"http://localhost/images/Levels.png");
	print_image(910,0,100,150,"http://localhost/images/topscores.png");
	print_image(550,75,50,50,"http://localhost/images/caixa.png");
	print_image(700,75,50,50,"http://localhost/images/caixa.png");
	print_image(795,254,40,40,"http://localhost/images/caixa.png");
	print_image(650,125,100,150,"http://localhost/images/Tools.png");	
	if(e.nivel<=9){printf("<TEXT X=720 Y=105> %d </TEXT></A>\n", e.nivel);}
	else{printf("<TEXT X=715 Y=105> %d </TEXT></A>\n", e.nivel);}
	if(e.score<=9){printf("<TEXT X=570 Y=105> %d </TEXT></A>\n", e.score);}
	if(e.score>9&&e.score<=99){printf("<TEXT X=565 Y=105> %d </TEXT></A>\n", e.score);}
	if(e.score>99){printf("<TEXT X=560 Y=105> %d </TEXT></A>\n", e.score);}
	printf("<TEXT X=809 Y=280> %d </TEXT></A>\n", e.pick_maxUse);

	print_image(0,540,100,150,"http://localhost/images/GOLD.png");
	printf("<TEXT X=150 Y=596> %d </TEXT></A>\n", e.gold);
	print_image(183,578,22,22,"http://localhost/images/coin.png");
}

/**
\brief Funçao que vai imprimir as imagens nas casas que os inimigos conseguem atacar, só é ativada quando o jogador clicar na imagem gerada pela funçao print_RButton
@param e Corresponde ao estado de jogo atual
@param ix Corresponde a coordenada x do inimigo
@param iy Corresponde a coordenada y do inimigo
@param dx Corresponde ao desvio no eixo x entre o inimigo e onde a imagem vai ser imprimida
@param dy Corresponde ao desvio no eixo y entre o inimigo e onde a imagem vai ser imprimida
*/
void print_Range(ESTADO e,int ix,int iy,int dx,int dy){
	int x = ix + dx;
	int y = iy + dy;
	if(!pos_valida(x, y)){return;}	
	if(x==e.goal.x && y==e.goal.y){return;}
	if(validate_PosPl(e,x,y,e.num_obstaculos)==0){return;}
	if(chk_for_enemy(e,x,y,e.num_inimigos)!=100){return;}
	IMAGEM(x,y,TAM,"markerRed.png");
}

/**
\brief Funçao precedente á funçao print_Range, esta vai chamar a funçao print_Range para todos os desvios dx,dy possiveis a partir da posição do inimigo
@param e Corresponde ao estado de jogo atual
*/		
void enemy_Range(ESTADO e) {
	int i,x,y;
	for(i=0;i<e.num_inimigos;i++){	
		x=e.inimigo[i].x;		
		y=e.inimigo[i].y;
		print_Range(e,x,y,0,-1);
		print_Range(e,x,y,0,+1);
		print_Range(e,x,y,-1,0);
		print_Range(e,x,y,+1,0);
		print_Range(e,x,y,+1,+1);
		print_Range(e,x,y,+1,-1);
		print_Range(e,x,y,-1,-1);
		print_Range(e,x,y,-1,+1);
	}
}

/**
\brief Funçao precedente á funçao enemy_Range, que a vai chamar so quando o valor da variavel do estado range_value==1
@param e Corresponde ao estado de jogo atual
*/
void print_range_aux(ESTADO e){
	if(e.range_value==1){enemy_Range (e);}
}

/**
\brief Esta funçao vai criar um estado de jogo novo caso o parametro action==0, ou caso contrario vai ler do ficheiro o estado de jogo
@param e Estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
@param action Corresponde ao valor da açao que foi efetuada pelo jogador, diferentes açoes possuem valores diferentes que levam a movimentos diferentes
@returns Vai devolver um estado de jogo novo, ou o que foi lido do ficheiro
*/
ESTADO get_new_state (ESTADO e,char name[],int action){
	if(action==0){
		e = inicializar0();
		e = inicializar(e);
		save_file(e,name);
	}
	else {
		e = read_file (name);
	}
	return e;
}

/**
\brief Esta função vai tentar indicar o caminho mais rapido para se poder chegar ao goal a partir da posiçao onde o jogador se encontra
@param e Corresponde ao estado de jogo atual
*/
void pathfinding(ESTADO e){
	int x,y,i,z,dx,dy,x_old,y_old,x_old2,y_old2;
	i=0;
	z=0;
	x=e.jog.x;
	y=e.jog.y;
	x_old=x;
	y_old=y;
	x_old2=-1;
	y_old2=-1;
	while(i<25){
	dx=diferenca_xy(x,e.goal.x);
	dy=diferenca_xy(y,e.goal.y);
	if(dx>0 && dy>0){
		x+=-1;
		y+=-1;
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=1;y+=0;}/*1*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-1;y+=1;}/*2*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=2;y+=-1;}/*3*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-2;y+=2;}/*4*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=2;y+=-1;}/*5*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-1;y+=1;}/*6*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=1;y+=0;}/*7*/
	}
	if(dx<0 && dy<0){
		x+=1;
		y+=1;
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-1;y+=0;}/*1*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1 ){x+=1;y+=-1;}/*2*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-2;y+=1;}/*3*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=2;y+=-2;}/*4*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-2;y+=1;}/*5*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=1;y+=-1;}/*6*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-1;y+=0;}/*7*/
	}	
	if(dx>0 && dy<0){
		x+=-1;
		y+=1;
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=1;y+=0;}/*1*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-1;y+=-1;}/*2*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=2;y+=1;}/*3*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-2;y+=-2;}/*4*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=2;y+=1;}/*5*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-1;y+=-1;}/*6*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=1;y+=0;}/*7*/
	}
	if(dx<0 && dy>0){
		x+=1;
		y+=-1;
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-1;y+=0;}/*1*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=1;y+=1;}/*2*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-2;y+=-1;}/*3*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=2;y+=2;}/*4*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-2;y+=-1;}/*5*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=1;y+=1;}/*6*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-1;y+=0;}/*7*/
	}
	if(dx==0 && dy<0){
		x+=0;
		y+=1;
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=1;y+=0;}/*1*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-2;y+=0;}/*2*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=2;y+=-1;}/*3*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-2;y+=0;}/*4*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=2;y+=-1;}/*5*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-2;y+=0;}/*6*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=1;y+=0;}/*7*/
	}
	if(dx==0 && dy>0){
		x+=0;
		y+=-1;
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=1;y+=0;}/*1*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-2;y+=0;}/*2*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=2;y+=1;}/*3*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-2;y+=0;}/*4*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=2;y+=1;}/*5*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-2;y+=0;}/*6*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=1;y+=0;}/*7*/
	}
	if(dx<0 && dy==0){
		x+=1;
		y+=0;
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=0;y+=-1;}/*1*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=0;y+=2;}/*2*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-1;y+=-2;}/*3*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=0;y+=2;}/*4*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=-1;y+=-2;}/*5*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=0;y+=2;}/*6*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=0;y+=-1;}/*7*/
	}
	if(dx>0 && dy==0){
		x+=-1;
		y+=0;
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=0;y+=-1;}/*1*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=0;y+=2;}/*2*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=1;y+=-2;}/*3*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=0;y+=2;}/*4*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=1;y+=-2;}/*5*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=0;y+=2;}/*6*/
		if(validate_PosPl(e,x,y,e.num_obstaculos)!=1 || pos_valida2(x,y)!=1 || iguais(x,y,x_old,y_old)==1 || iguais(x,y,x_old2,y_old2)==1){x+=0;y+=-1;}/*7*/
	}
	IMAGEM(x,y,TAM,"markerBlue.png");
	i++;
	z++;
	if(z>2){x_old2=x_old;y_old2=y_old;}
	if(z>1){x_old=x;y_old=y;}
	if(x==e.goal.x && y==e.goal.x){break;}
	}
}

/**
\brief Funçao precedente á funçao pathfinding, que a vai invocar quando o valor da variavel do estado path_value==1
@param e Corresponde ao estado de jogo atual
*/
void print_pathAux(ESTADO e){
	if(e.path_value==1) pathfinding(e);
}

/**
\brief Funçao que vai testar se existe algum obstaculo nas coordenadas (x,y)==(jog.x + dx,jog.y + dy)
@param e Corresponde ao estado de jogo atual
@param dx Corresponde ao desvio no eixo x entre o jogador e a posiçao que vai ser testada
@param dy Corresponde ao desvio no eixo y entre o jogador e a posiçao que vai ser testada
@returns Um int,0 ou 1, 0 se a posiçao estiver ocupada por um obstaculo, 1 se estiver livre
*/
int test_wall_mid_aux(ESTADO e,int dx,int dy){
	int x = e.jog.x + dx;
	int y = e.jog.y + dy;
	if(validate_PosPl(e,x,y,e.num_obstaculos)==0){return 0;}
	else{return 1;}
}

/**
\brief Funçao precedente a funçao test_wall_mid_aux, que a vai invocar com parametros diferentes, dependentes de qual o desvio (dx,dy) do jogador é pretendido testar. A funçao em si vai testar se existe um obstaculo entre a posiçao do jogador e a posiçao que ele pretende atacar, dai os desvios dx e dy
@param e Corresponde ao estado de jogo atual
@param dx Corresponde ao desvio no eixo x entre o jogador e a posiçao que vai ser testada
@param dy Corresponde ao desvio no eixo y entre o jogador e a posiçao que vai ser testada
@returns Um int, 0 ou 1, este valor vai ser o valor que é devolvido pela funçao test_wall_mid_aux, mas caso o desvio dx e dy que foi fornecido a esta funçao nao esteja corresponda a nenhum dos casos para teste, a funçao vai devolver 1 
*/
int test_wall_mid(ESTADO e,int dx,int dy){
	if(dx==-2 && dy==-2){return(test_wall_mid_aux(e,-1,-1));}
	if((dx==-2 && dy==-1)||(dx==-2 && dy==0)||(dx==-2 && dy==1)){return(test_wall_mid_aux(e,-1,0));}
	if(dx==-2&& dy==2){return(test_wall_mid_aux(e,-1,+1));}
	if((dx==-1 && dy==2)||(dx==0 && dy==2)||(dx==1 && dy==2)){return(test_wall_mid_aux(e,0,+1));}
	if(dx==2 && dy==2){return(test_wall_mid_aux(e,+1,+1));}
	if((dx==2 && dy==1)||(dx==2 && dy==0)||(dx==2 && dy==-1)){return(test_wall_mid_aux(e,+1,0));}
	if(dx==2 && dy==-2){return(test_wall_mid_aux(e,+1,-1));}
	if((dx==1 && dy==-2)||(dx==0 && dy==-2)||(dx==-1 && dy==-2)){return(test_wall_mid_aux(e,0,-1));}
	else{return 1;}
}

/**
\brief Funçao que vai imprimir as imagens que indicam as casas para as quais o jogador pode mover-se ou realizar açoes tal como matar inimigos ou destruir objetos 
@param e Corresponde ao estado de jogo atual
@param dx Corresponde ao desvio no eixo x entre o jogador e a posiçao que vai ser testada
@param dy Corresponde ao desvio no eixo y entre o jogador e a posiçao que vai ser testada
@param name String que contem o nome do ficheiro onde o estado esta guardado
@param action Corresponde ao valor da açao que foi efetuada pelo jogador, diferentes açoes possuem valores diferentes que levam a movimentos diferentes
*/
void get_next_action_aux (ESTADO e,int dx,int dy,int action,char name[]){
	int x = e.jog.x + dx;
	int y = e.jog.y + dy;
	char link[MAX_BUFFER];
	if (e.range_value==1||e.path_value==1){return;}
	if(!pos_valida(x, y)){return;}
	if(e.jogHP==0){return;}
	if(test_wall_mid(e,dx,dy)==0){return;}
	if((e.class_sprite==3) && (chk_for_enemy(e,x,y,e.num_inimigos)!=100)){return;}
	if(chk_for_enemy(e,x,y,e.num_inimigos)!=100){
		sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
		ABRIR_LINK(link);
	  	IMAGEM(x,y,TAM, "markerRed.png");
	  	IMAGEM(x, y, TAM, "irongiant.png");
		FECHAR_LINK;
		return;}
	if((validate_PosPl(e,x,y,e.num_obstaculos)==0) && (e.pick_value!=0) && (e.pick_maxUse!=0)){
		sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
		ABRIR_LINK(link);
	  	IMAGEM(x,y,TAM, "markerGrey.png");
	  	IMAGEM(x, y, TAM, "creepwall.png");
		FECHAR_LINK;
		return;}
	if(x==e.goal.x && y==e.goal.y){
		sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
		ABRIR_LINK(link);
	  	print_goal(e);
		FECHAR_LINK;
		return;}
	if(validate_PosPl(e,x,y,e.num_obstaculos)==0){return;}
	if(e.atk_OP==0){
	sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
	ABRIR_LINK(link);
	IMAGEM(x,y,TAM, "markerGrey.png");
	FECHAR_LINK;}
}

/**
\brief Esta funçao e uma versao diferente da funçao anterior, esta serve para as classes especiais que podem atacar em casas para os quais nao se podem mexer, ou seja as unicas imagens que sao imprimidas nesta funçao, sao as que indicam quando o inimigo esta em range para se atacar ou o obstaculo em range para se poder destruir
@param e Corresponde ao estado de jogo atual
@param dx Corresponde ao desvio no eixo x entre o jogador e a posiçao que vai ser testada
@param dy Corresponde ao desvio no eixo y entre o jogador e a posiçao que vai ser testada
@param name String que contem o nome do ficheiro onde o estado esta guardado
@param action Corresponde ao valor da açao que foi efetuada pelo jogador, diferentes açoes possuem valores diferentes que levam a movimentos diferentes
*/
void get_next_action_aux_2 (ESTADO e,int dx,int dy,int action,char name[]){
	int x = e.jog.x + dx;
	int y = e.jog.y + dy;
	char link[MAX_BUFFER];
	if (e.range_value==1||e.path_value==1){return;}
	if(!pos_valida(x, y)){return;}
	if(e.jogHP==0){return;}
	if(test_wall_mid(e,dx,dy)==0){return;}
	if(chk_for_enemy(e,x,y,e.num_inimigos)!=100){
		sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
		ABRIR_LINK(link);
	  	IMAGEM(x,y,TAM, "markerRed.png");
	  	IMAGEM(x, y, TAM, "irongiant.png");
		FECHAR_LINK;
		return;}
	if ((validate_PosPl(e,x,y,e.num_obstaculos)==0) && (e.pick_value!=0) && (e.pick_maxUse!=0)){
		sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
		ABRIR_LINK(link);
	  	IMAGEM(x,y,TAM, "markerGrey.png");
	  	IMAGEM(x, y, TAM, "creepwall.png");
		FECHAR_LINK;
		return;}
}

/**
\brief Funçao que vai ser usada para imprimir as casas para onde o jogador pode mover ou atacar, quando a classe que esta a ser jogada pelo jogador é um Knight ou um SunBro
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void knight_sunbro_move (ESTADO e,char name[]){
	get_next_action_aux(e,  0, -1, 2, name);
	get_next_action_aux(e,  0, +1, 3, name);
	get_next_action_aux(e, -1,  0, 4, name);
	get_next_action_aux(e, +1,  0, 5, name);
	get_next_action_aux(e, +1, +1, 6, name);
	get_next_action_aux(e, +1, -1, 7, name);
	get_next_action_aux(e, -1, -1, 8, name);
	get_next_action_aux(e, -1, +1, 9, name);
}

/**
\brief Funçao que vai ser usada para imprimir as casas para onde o jogador pode mover ou atacar, quando a classe que esta a ser jogada pelo jogador é um Mage
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void mage_move (ESTADO e,char name[]){
	get_next_action_aux(e,  0, -1, 2, name);
	get_next_action_aux(e,  0, +1, 3, name);
	get_next_action_aux(e, -1,  0, 4, name);
	get_next_action_aux(e, +1,  0, 5, name);
	get_next_action_aux(e, +1, +1, 6, name);
	get_next_action_aux(e, +1, -1, 7, name);
	get_next_action_aux(e, -1, -1, 8, name);
	get_next_action_aux(e, -1, +1, 9, name);

	get_next_action_aux_2(e, 0, +2, 16, name);
	get_next_action_aux_2(e, 0, -2, 17, name);
	get_next_action_aux_2(e, +2, 0, 18, name);
	get_next_action_aux_2(e, -2, 0, 19, name);
	get_next_action_aux_2(e, +1, +2, 20, name);
	get_next_action_aux_2(e, +1, -2, 21, name);
	get_next_action_aux_2(e, +2, +1, 22, name);
	get_next_action_aux_2(e, +2, -1, 23, name);
	get_next_action_aux_2(e, -1, -2, 24, name);
	get_next_action_aux_2(e, -1, +2, 25, name);
	get_next_action_aux_2(e, -2, +1, 26, name);
	get_next_action_aux_2(e, -2, -1, 27, name);
	get_next_action_aux_2(e, -2, -2, 29, name);
	get_next_action_aux_2(e, +2, +2, 30, name);
	get_next_action_aux_2(e, +2, -2, 31, name);
	get_next_action_aux_2(e, -2, +2, 32, name);
}

/**
\brief Funçao que vai ser usada para imprimir as casas para onde o jogador pode mover ou atacar, quando a classe que esta a ser jogada pelo jogador é um Archer
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void archer_move (ESTADO e,char name[]){
	get_next_action_aux(e,  0, -1, 2, name);
	get_next_action_aux(e,  0, +1, 3, name);
	get_next_action_aux(e, -1,  0, 4, name);
	get_next_action_aux(e, +1,  0, 5, name);
	get_next_action_aux(e, 0, +2, 16, name);
	get_next_action_aux(e, 0, -2, 17, name);
	get_next_action_aux(e, +2, 0, 18, name);
	get_next_action_aux(e, -2, 0, 19, name);

	get_next_action_aux_2(e, 0, +2, 16, name);
	get_next_action_aux_2(e, 0, -2, 17, name);
	get_next_action_aux_2(e, +2, 0, 18, name);
	get_next_action_aux_2(e, -2, 0, 19, name);
	get_next_action_aux_2(e, +1, +2, 20, name);
	get_next_action_aux_2(e, +1, -2, 21, name);
	get_next_action_aux_2(e, +2, +1, 22, name);
	get_next_action_aux_2(e, +2, -1, 23, name);
	get_next_action_aux_2(e, -1, -2, 24, name);
	get_next_action_aux_2(e, -1, +2, 25, name);
	get_next_action_aux_2(e, -2, +1, 26, name);
	get_next_action_aux_2(e, -2, -1, 27, name);
	get_next_action_aux_2(e, -2, -2, 29, name);
	get_next_action_aux_2(e, +2, +2, 30, name);
	get_next_action_aux_2(e, +2, -2, 31, name);
	get_next_action_aux_2(e, -2, +2, 32, name);
}

/**
\brief Funçao que vai invocar todas as funçoes que originam imagens com link, e que involvem açoes que mudam o estado de jogo
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void get_next_action (ESTADO e,char name[]){
	print_restart(e,name,10);
	print_pv(e,name,11);
	print_PButton(e,name,12);
	print_RButton(e,name,13);
	print_bomb(e,name,14);
	print_BButton(e,name,15);
	print_pf(e,name,28);
	
	if(e.class_sprite==1||e.class_sprite==4){knight_sunbro_move(e,name);}
	
	if(e.class_sprite==2){mage_move(e,name);}
	
	if(e.class_sprite==3){archer_move(e,name);}

	if(e.atk_OP==1){
		get_next_action_aux(e, 0, +2, 16, name);
		get_next_action_aux(e, 0, -2, 17, name);
		get_next_action_aux(e, +2, 0, 18, name);
		get_next_action_aux(e, -2, 0, 19, name);
		get_next_action_aux(e, +1, +2, 20, name);
		get_next_action_aux(e, +1, -2, 21, name);
		get_next_action_aux(e, +2, +1, 22, name);
		get_next_action_aux(e, +2, -1, 23, name);
		get_next_action_aux(e, -1, -2, 24, name);
		get_next_action_aux(e, -1, +2, 25, name);
		get_next_action_aux(e, -2, +1, 26, name);
		get_next_action_aux(e, -2, -1, 27, name);}
}

/**
\brief Funçao que vai imprimir o menu onde podemos escolher qual a classe que queremos usar durante o jogo que vamos fazer
@param action Corresponde ao valor da açao que foi efetuada pelo jogador, diferentes açoes possuem valores diferentes que levam a movimentos diferentes
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void get_class_sprite (int action,char name[]){
	char link[MAX_BUFFER];
	if(action==96){
		sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
		ABRIR_LINK(link);
		print_image(200,120,400,300,"http://localhost/images/class_border.png");
		print_image(274,85,100,150,"http://localhost/images/kplate.png");
		print_image(324,157,100,50,"http://localhost/images/knight_sprite.png");
		printf("<TEXT X=265 Y=275> %s </TEXT></A>\n", "Vida : 3");
		printf("<TEXT X=265 Y=305> %s </TEXT></A>\n", "Alcance do ataque :");
		printf("<TEXT X=292 Y=325> %s </TEXT></A>\n", "1 Casa");
		printf("<TEXT X=265 Y=355> %s </TEXT></A>\n", "Movimento :");
		printf("<TEXT X=292 Y=375> %s </TEXT></A>\n", "Alcance de 1 Casa");
		printf("<TEXT X=292 Y=395> %s </TEXT></A>\n", "Em 8 direcoes");
		FECHAR_LINK;
	}
	if(action==97){
		sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
		ABRIR_LINK(link);
		print_image(420,120,400,300,"http://localhost/images/class_border.png");
		print_image(494,85,100,150,"http://localhost/images/mplate.png");
		print_image(543,157,100,50,"http://localhost/images/mage_sprite2.png");
		printf("<TEXT X=483 Y=275> %s </TEXT></A>\n", "Vida : 1");
		printf("<TEXT X=483 Y=305> %s </TEXT></A>\n", "Alcance do ataque :");
		printf("<TEXT X=510 Y=325> %s </TEXT></A>\n", "2 Casas");
		printf("<TEXT X=483 Y=355> %s </TEXT></A>\n", "Movimento :");
		printf("<TEXT X=510 Y=375> %s </TEXT></A>\n", "Alcance de 1 Casa");
		printf("<TEXT X=510 Y=395> %s </TEXT></A>\n", "Em 8 direcoes");
		FECHAR_LINK;
	}
	if(action==98){
		sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
		ABRIR_LINK(link);
		print_image(640,120,400,300,"http://localhost/images/class_border.png");
		print_image(714,85,100,150,"http://localhost/images/aplate.png");
		print_image(762,157,100,50,"http://localhost/images/archer_sprite.png");
		printf("<TEXT X=701 Y=275> %s </TEXT></A>\n", "Vida : 1");
		printf("<TEXT X=701 Y=305> %s </TEXT></A>\n", "Alcance do ataque :");
		printf("<TEXT X=728 Y=325> %s </TEXT></A>\n", "2 Casas");
		printf("<TEXT X=701 Y=355> %s </TEXT></A>\n", "Movimento :");
		printf("<TEXT X=728 Y=375> %s </TEXT></A>\n", "Alcance de 2 Casas");
		printf("<TEXT X=728 Y=395> %s </TEXT></A>\n", "Em 4 direcoes");
		FECHAR_LINK;
	}
	if(action==99){
		sprintf(link, "http://localhost/cgi-bin/dc?%s,%d", name, action);
		ABRIR_LINK(link);
		print_image(860,120,400,300,"http://localhost/images/class_border.png");
		print_image(934,85,100,150,"http://localhost/images/splate.png");
		print_image(981,157,100,50,"http://localhost/images/solaire_sprite.png");
		printf("<TEXT X=919 Y=275> %s </TEXT></A>\n", "Vida : 2");
		printf("<TEXT X=919 Y=305> %s </TEXT></A>\n", "Alcance do ataque :");
		printf("<TEXT X=946 Y=325> %s </TEXT></A>\n", "1 Casa");
		printf("<TEXT X=919 Y=355> %s </TEXT></A>\n", "Movimento :");
		printf("<TEXT X=946 Y=375> %s </TEXT></A>\n", "Alcance de 1 Casa");
		printf("<TEXT X=946 Y=395> %s </TEXT></A>\n", "Em 8 direcoes");
		printf("<TEXT X=919 Y=425> %s </TEXT></A>\n", "Blessing of the Sun");
		FECHAR_LINK;
	}
}

/**
\brief Funçao precedente a funçao anterior que a vai invocar 4 vezes, com os 4 valores da variavel action diferentes, sendo estes valores depois incorparados no link gerado na funçao anterior
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void print_class_menu (char name[]){
	get_class_sprite(96, name);
	get_class_sprite(97, name);
	get_class_sprite(98, name);
	get_class_sprite(99, name);
}

/**
\brief Funçao que vai imprimir a imagem de background do menu onde se escolhe as classes, e tambem vai alterar o valor da variavel do estado class_value para 1, com fim a que este menu nao seja imprimido mais nenhuma vez até o jogo ser começado de novo outra vez
@param e Corresponde ao estado de jogo atual
@param name String que contem o nome do ficheiro onde o estado esta guardado
*/
void get_class (ESTADO e,char name[]){
	if(e.class_value==0){
		e.class_value = 1;
		save_file(e,name);
		print_image(0,-315,1400,1500,"http://localhost/images/back2.jpg");
		print_class_menu(name);
	}
	else{
		save_file(e,name);
		return;}
}

/**
\brief Funçao parse, que vai invocar todas as outras funçoes anteriormente definidas, com fim a construir o tabuleiro do jogo com todos os elementos que lhe pertence 
*/
void parse (){
	int k,action;
	char *args = getenv ("QUERY_STRING");
	ESTADO 	e;
	char name[100];
	srand(time(NULL));
	if(strlen(args) == 0){
		strcpy(name, "savefile");
		action=0;}
	else{
		if(strlen(args) == 1){
			k=sscanf(args,"%[^,]",name);
			action=0;}
		else{
			k=sscanf(args,"%[^,],%d",name,&action);}}
	
	printf("%d\n",k);
	
	print_background();

	print_board();

	e = get_new_state(e,name,action);
	
	process_action(e,name,action); 
	
	e = read_file(name);

	print_enemies(e);
	print_walls(e);
	print_goal(e);
	
	get_next_action(e,name);

	e = read_file(name);
	print_player(e);	
	print_vida(e);
	print_vsr(e);
	print_range_aux(e);
	print_pathAux(e);
	print_gitgud(e);
	if(e.jogHP==0){save_score(e,name);}
	print_score();
	get_class(e,name);
}

