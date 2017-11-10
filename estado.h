#ifndef ___ESTADO_H___
#define ___ESTADO_H___

/**
@file estado.h
Definição do estado e das funções que convertem estados em strings e vice-versa
*/

/** \brief O nº máximo de inimigos */
#define MAX_INIMIGOS		100

/** \brief O nº máximo de obstáculos */
#define MAX_OBSTACULOS		100

/**
\brief Estrutura que armazena uma posição
*/
typedef struct posicao {
	/** A coordenada x do jogador */
	char x;
	/** A coordenada y do jogador */
	char y;
} POSICAO;

/**
\brief Estrutura que armazena o estado do jogo
*/
typedef struct estado {
	/** A posição do jogador */
	POSICAO jog;
	/** A posição da saida */
	POSICAO goal;
	/** O nº de inimigos */
	char num_inimigos;	
	/** O nº de obstáculos */
	char num_obstaculos;
	/** O nivel onde o jogador se encontra */
	char nivel;
	/** O array que contem os HP's dos inimigos */
	char inimigoHP[MAX_INIMIGOS];
	/** O HP do jogador */
	char jogHP;
	/** A pontuação do jogador */
	int score;
	/** O dinheiro do jogador */
	int gold;
	/**Se 1, o jogador ja escolheu a classe, se 0 o jogador ainda nao escolheu a classe*/
	char class_value;
	/** Se 1, classe == Knight, se 2, classe == Mage, se 3, classe == Archer, se 4, classe == SunBro*/
	char class_sprite;
	/** 0 corresponde a ñ mostrar range dos inimigos, 1 corresponde a mostrar a range */
	char range_value;
	/** 1 significa pickaxe esta ativado, 0 significa pickaxe ñ esta ativado */
	char pick_value;
	/** numero maximo de usos do pickaxe */
	char pick_maxUse;
	/** 1 significa ataque com flechas esta ativo, 0 significa o contrario */
	char atk_OP;
	/** 1 significa que pathfinding esta ativo, 0 significa o contrario */
	char path_value;
	/** A vida dos obstaculos */
	char obstaculoHP[MAX_OBSTACULOS];
	/** Array com a posição dos inimigos */
	POSICAO inimigo[MAX_INIMIGOS];
	/** Array com a posição dos obstáculos */
	POSICAO obstaculo[MAX_OBSTACULOS];
} ESTADO;

#endif
