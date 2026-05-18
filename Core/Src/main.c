/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ST7789\st7789.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define botEsqLer HAL_GPIO_ReadPin(botEsquerda_GPIO_Port, botEsquerda_Pin)
#define botDirLer HAL_GPIO_ReadPin(botDireita_GPIO_Port, botDireita_Pin)
#define botCmaLer HAL_GPIO_ReadPin(botCima_GPIO_Port, botCima_Pin)
#define botBxoLer HAL_GPIO_ReadPin(botBaixo_GPIO_Port, botBaixo_Pin)

#define tentMaxSenhas 3
#define tempMaxAlunosFora 3

#define roxoDS		0xa81f
#define verdeFODA	0x0463
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
int qntAlunosSala;
int qntAlunosReg;
int qntAlunosFora;
int qntTotalSaidas;
int indiceSaidaPareamento = 0;
int qntTotalEntradas;
int capArraySaida = 4;
int capArrayEntrada = 4;
int maxAlunosFora;
uint32_t somaTempos = 0;
uint32_t *arrayTempoSaida = NULL;
uint32_t *arrayTempoEntrada = NULL;
uint32_t *arrayMatriculas = NULL;
uint32_t tempoInicioAula;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void iniciarPrograma(void);
static void entrarSenha(void);
static void definirAlunos(void);
static void interfacePrincipal(void);
static void controlePresenca(int numEnviado);
static void controleSaida(int numEnviado);
static void encerrarPrograma(void);
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */
	arrayTempoSaida = malloc(capArraySaida * sizeof(uint32_t));
	arrayTempoEntrada = malloc(capArrayEntrada * sizeof(uint32_t));
	// malloc
	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_SPI1_Init();
	/* USER CODE BEGIN 2 */
	ST7789_Init();
	ST7789_Fill_Color(BLACK);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */
		//pegamos todas as funções e botamos em ordem
		iniciarPrograma();
		entrarSenha();
		ST7789_Fill_Color(BLACK);
		ST7789_WriteString(16, 26, "Bem-vindo professor.", Font_16x26, WHITE,
		BLACK);
		HAL_Delay(250);
		ST7789_Fill_Color(BLACK);
		definirAlunos();
		interfacePrincipal();
		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(ST7789_CS_GPIO_Port, ST7789_CS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB,
			ST7789_DC_Pin | ST7789_RST_Pin | GPIO_PIN_3 | GPIO_PIN_4
					| GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_RESET);

	/*Configure GPIO pin : ST7789_CS_Pin */
	GPIO_InitStruct.Pin = ST7789_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ST7789_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : ST7789_DC_Pin ST7789_RST_Pin PB3 PB4
	 PB5 PB6 */
	GPIO_InitStruct.Pin = ST7789_DC_Pin | ST7789_RST_Pin | GPIO_PIN_3
			| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : botEsquerda_Pin botBaixo_Pin botDireita_Pin botCima_Pin */
	GPIO_InitStruct.Pin = botEsquerda_Pin | botBaixo_Pin | botDireita_Pin
			| botCima_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
static void esperandoP10(void) {
	bool esperandoApertar = true;

	while (esperandoApertar) {
		GPIO_PinState botLer;
		botLer = botBxoLer;

		if (botLer == 0) {
			esperandoApertar = false;
		}
	}
	ST7789_Fill_Color(BLACK);
}

static void iniciarPrograma(void) {
	ST7789_WriteString(16, 50, "Class Manager", Font_16x26, verdeFODA, BLACK);
	ST7789_WriteString(94, 86, "34-DS", Font_11x18, roxoDS, BLACK);
	ST7789_WriteString(16, 200, "- PRESS PA10 -", Font_16x26, WHITE, BLACK);
	esperandoP10();
	//"printa" a tela inicial e espera o pa10
}

static void gerarSenha(int *senhaRand) {
	srand(HAL_GetTick());
	for (int bananas = 0; bananas < 4; bananas++) {
		int valGerado = (rand() % 4) + 1;
		senhaRand[bananas] = valGerado;
		//gera um numero por ves da senha
	}
}

static void desenharTelaSenha(void) {
	ST7789_WriteString(24, 10, "Entrar Senha", Font_16x26, verdeFODA, BLACK);
	ST7789_DrawFilledRectangle(63, 110, 20, 2, WHITE);
	ST7789_DrawFilledRectangle(93, 110, 20, 2, WHITE);
	ST7789_DrawFilledRectangle(123, 110, 20, 2, WHITE);
	ST7789_DrawFilledRectangle(153, 110, 20, 2, WHITE);
	ST7789_WriteString(110, 150, "1", Font_16x26, WHITE, BLACK);
	ST7789_WriteString(145, 180, "2", Font_16x26, WHITE, BLACK);
	ST7789_WriteString(110, 210, "3", Font_16x26, WHITE, BLACK);
	ST7789_WriteString(75, 180, "4", Font_16x26, WHITE, BLACK);
	//desenha a tela da senha
}
static void escreverSenha(int numEnviado, int qntNumeros, int *senhaEscrita) {
	//MUITO DIFICIL DE ENTENDER mas é facil de usar
	char numStr[3];
	//o char numStr tem três casas
	itoa(numEnviado, numStr, 10);
	//o numero enviado vai ser o numStr uhhhhh puts aqui eu realmente embananei faz tempo que agnt usou eu esqueci como funciona mds
	switch (qntNumeros) {
	//muda para cada "caso"
	case 1:
		ST7789_WriteString(63, 84, numStr, Font_16x26, WHITE, BLACK);
		break;
	case 2:
		ST7789_WriteString(93, 84, numStr, Font_16x26, WHITE, BLACK);
		break;
	case 3:
		ST7789_WriteString(123, 84, numStr, Font_16x26, WHITE, BLACK);
		break;
	case 4:
		ST7789_WriteString(153, 84, numStr, Font_16x26, WHITE, BLACK);
		break;
	}
	senhaEscrita[qntNumeros - 1] = numEnviado;
	//para cada caso de botão (1,2,3 ou 4) ele manda o valor a ser printado no lcd
}

static void enviarNumeroPraSenha(int numEnviado, int qntNumeros,
		int *senhaEscrita) {
	escreverSenha(numEnviado, qntNumeros, senhaEscrita);
	//bizarro // bizarro, cara.
}

static void entrarSenha(void) {
	//meu DEUS que absurdo imagina isso em assembly
	bool senhaCorreta = false;
	int qntNumeros = 0;
	int senhaRand[4];
	//a senha tem 4 "casas"
	int senhaEscrita[4];
	//a senha escrita que é o valor que você manda tambem tem 4 casas
	int numEnviado = 0;
	gerarSenha(senhaRand);
	//gera a senha de uma vês e senhaRand
	desenharTelaSenha();
	//MUITO auto explicativo
	for (int laranjas = 0; laranjas < 3; laranjas++) {
		//adoro o sistema de for que eu criei meu deus
		//aqui é o sistema de escrever a senha
		//qntNumeros é quantos numeros você mandou pra senha e o cap é 4
		//numEnviado é o VALOR do numero da senha (a senha foi a parte mais dificil pq tem o numero da senha real, os valores que agnt manda pra ela checar se asenha tá certa, a senha no display e agnt tendo que fazer malabarismo com esse tanto de string que tambem é uma int ao mesmo tempo foi horrivel)
		while (qntNumeros < 4) {
			if (botEsqLer == 0) {
				HAL_Delay(250);
				qntNumeros++;
				numEnviado = 4;
				enviarNumeroPraSenha(numEnviado, qntNumeros, senhaEscrita);
			} else if (botDirLer == 0) {
				HAL_Delay(250);
				qntNumeros++;
				numEnviado = 2;
				enviarNumeroPraSenha(numEnviado, qntNumeros, senhaEscrita);
			} else if (botCmaLer == 0) {
				HAL_Delay(250);
				qntNumeros++;
				numEnviado = 1;
				enviarNumeroPraSenha(numEnviado, qntNumeros, senhaEscrita);
			} else if (botBxoLer == 0) {
				HAL_Delay(250);
				qntNumeros++;
				numEnviado = 3;
				enviarNumeroPraSenha(numEnviado, qntNumeros, senhaEscrita);
			}
		}
		bool iguais = true;
		for (int roma = 0; roma < 4; roma++) {
			//a fruta romã é uma delicia nhamnham
			if (senhaEscrita[roma] != senhaRand[roma]) {
				iguais = false;
				break;
				//se a senha no array escrito for diferente da senha do array que foi gerado lá atrás ele seta iguais pra false oq pra mim é bem engraçado MAS FUNCIONA TÁ PARA DE JULGAR O CÓDIGO NOSSO ANA LETICIA <3
			}
		}
		if (iguais) {
			//E AI ELE SETA A VARIAVEL BOOL senhaCorreta PRA TRUE KKLOL
			senhaCorreta = true;
			break;
		} else {
			//e se não ele apaga a senha do lcd e volta a qntNumeros pra 0
			ST7789_DrawFilledRectangle(63, 84, 16, 26, BLACK);
			ST7789_DrawFilledRectangle(93, 84, 16, 26, BLACK);
			ST7789_DrawFilledRectangle(123, 84, 16, 26, BLACK);
			ST7789_DrawFilledRectangle(153, 84, 16, 26, BLACK);
			qntNumeros = 0;
			for (int maracujas = 0; maracujas < 4; maracujas++) {
				senhaEscrita[maracujas] = 0;
				//e reseta o array
			}
		}
	}
	if (!senhaCorreta) {
		ST7789_Fill_Color(BLACK);
		ST7789_WriteString(16, 100, "TENTATIVAS MÁXIMAS", Font_16x26, RED,
		BLACK);
		ST7789_WriteString(100, 135, "EXCEDIDAS!", Font_16x26, RED, BLACK);
		while (true) {
			// trava se errar 3 vezes
		} // else continua normal o codigo saindo da função
	}
}

static void desenharTelaAlunos() {
	//tela dos alunos
	ST7789_WriteString(24, 10, "Qnt. Alunos", Font_16x26, verdeFODA, BLACK);
	ST7789_DrawFilledRectangle(100, 110, 50, 2, WHITE);
	ST7789_WriteString(110, 150, "+1", Font_16x26, WHITE, BLACK);
	ST7789_WriteString(145, 180, "-5", Font_16x26, WHITE, BLACK);
	ST7789_WriteString(110, 210, "-1", Font_16x26, WHITE, BLACK);
	ST7789_WriteString(75, 180, "+5", Font_16x26, WHITE, BLACK);
}

static void enviarNumeroAlunos(int numEnviado) {
	//olha a magica
	qntAlunosReg += numEnviado;
	qntAlunosReg = (qntAlunosReg > 999) ? 999 : qntAlunosReg;
	qntAlunosReg = (qntAlunosReg < 0) ? 0 : qntAlunosReg;
	//itoa dnv meu deus
	//numStr agora tem 5 casas pq o numero de alunos maximo é 100 por que certo alguem viu que a maior sala de aula do mundo cabe 150 alunos oque em si é um absurdo, nossa sala nem 50 alunos tem e mal dá pra ter aula
	//além disso se vc se perguntar "mas 100 são 3 casas"
	//algumas coisas tem queterminar com /0 // por ser uma string.
	//portanto alguns numStr vão ter mais 2 caracteres
	char numStr[5];
	itoa(qntAlunosReg, numStr, 10);
	ST7789_DrawFilledRectangle(100, 84, 50, 26, BLACK);
	if (qntAlunosReg > 99) {
		ST7789_WriteString(100, 84, numStr, Font_16x26, WHITE, BLACK);
	} else if (qntAlunosReg > 9) {
		ST7789_WriteString(108, 84, numStr, Font_16x26, WHITE, BLACK);
	} else {
		ST7789_WriteString(116, 84, numStr, Font_16x26, WHITE, BLACK);
	}

}
static void definirAlunos(void) {
	//muito bonitinho que agnt separa as funções "internas" das que desenham no lcd
	desenharTelaAlunos();
	bool definindoAlunos = true;
	int numEnviado = 0;
	while (definindoAlunos) {
		int botoesPressionados = (botEsqLer == 0) + (botDirLer == 0)
				+ (botCmaLer == 0) + (botBxoLer == 0);

		HAL_Delay(150);

		if (botEsqLer == 0 && botoesPressionados == 1) {
			HAL_Delay(250);
			numEnviado = 5;
			enviarNumeroAlunos(numEnviado);

		} else if (botDirLer == 0 && botoesPressionados == 1) {
			HAL_Delay(250);
			numEnviado = -5;
			enviarNumeroAlunos(numEnviado);

		} else if (botCmaLer == 0 && botoesPressionados == 1) {
			HAL_Delay(250);
			numEnviado = 1;
			enviarNumeroAlunos(numEnviado);

		} else if (botBxoLer == 0 && botoesPressionados == 1) {
			HAL_Delay(250);
			numEnviado = -1;
			enviarNumeroAlunos(numEnviado);

		} else if (botoesPressionados >= 2) {
			definindoAlunos = false;
			//assim que agnt terminar de setar os alunos é só clicar 2 botoes ao mesmo tempo que ele seta pra falso
		}
	}
	//me desculpa mas OQUE É UM MALLOC
	//l: memory allocation. stupid.
	arrayMatriculas = realloc(arrayMatriculas, qntAlunosReg * sizeof(int));

	if (arrayMatriculas != NULL) {
		memset(arrayMatriculas, 0, qntAlunosReg * sizeof(int));
	}
}

static int confirmarMatricula(int numEnviado) {
	//essa parte agente se esforçou MUITO mentalmente pra ter a ideia
	//vc muda no proprio debug o valor oque é muito dahora, engraçado e complicado ao mesmo tempo
	if (numEnviado == -1)
		return 1;
	bool matriculaEnviada = false;
	int valMatricula = -1;
	while (!matriculaEnviada) {
		if (valMatricula != -1) {
			if (valMatricula == 0)
				return 0;
			for (int jacas = 0; jacas < qntAlunosReg; jacas++) {
				if (arrayMatriculas[jacas] == 0) {
					arrayMatriculas[jacas] = valMatricula;
					matriculaEnviada = true;
					return 1;
				} else if (valMatricula == arrayMatriculas[jacas])
					return 0;
			}
		}
	}
	return 0;
}

static void removerMatricula(void) {
	for (int cagaitas = (qntAlunosReg - 1); cagaitas >= 0; cagaitas--) {
		if (arrayMatriculas[cagaitas] != 0) {
			arrayMatriculas[cagaitas] = 0;
			break;
		}
	}
}

static void controlePresenca(int numEnviado) {
	int resultadoMatricula = confirmarMatricula(numEnviado);

	if (numEnviado == -1)
		removerMatricula();

	if (resultadoMatricula == 1 || numEnviado == -1) {
		qntAlunosSala += numEnviado;
		qntAlunosSala =
				(qntAlunosSala > qntAlunosReg) ? qntAlunosReg : qntAlunosSala;
		qntAlunosSala = (qntAlunosSala < 0) ? 0 : qntAlunosSala;
		char numStr[5];
		itoa(qntAlunosSala, numStr, 10);
		if (qntAlunosSala > 9) {
			ST7789_DrawFilledRectangle(90, 26, 32, 26, BLACK);
			ST7789_WriteString(90, 26, numStr, Font_16x26, WHITE, BLACK);
		} else if (numEnviado == -1 && qntAlunosSala == 9) {
			ST7789_DrawFilledRectangle(90, 26, 32, 26, BLACK);
			ST7789_WriteString(90, 26, "0", Font_16x26, WHITE, BLACK);
			ST7789_WriteString(106, 26, numStr, Font_16x26, WHITE, BLACK);
		} else {
			ST7789_DrawFilledRectangle(106, 26, 16, 26, BLACK);
			ST7789_WriteString(106, 26, numStr, Font_16x26, WHITE, BLACK);
		}
	} else if (resultadoMatricula == 0) {
		ST7789_WriteString(20, 200, "MATRICULA INVALIDA!", Font_11x18, WHITE,
		BLACK);
		HAL_Delay(2000);
		ST7789_WriteString(20, 200, "MATRICULA INVALIDA!", Font_11x18, BLACK,
		BLACK);
		//como a matricula é sempre 0 no estado não mutado dela
		//se for 0 ela não adiciona ao array
	}
}

static void controleSaida(int numEnviado) {
	//BAGUNÇA AAAAAAAAAAAAAAAAAAAAA
	//o controle de saida tá muito bem feito pelo menos
	//tava fazendo projete no dia ent tudo culpa da livia // fuck you !
	//MUITO BOM TAMBEM A PARTE DOS TICKS MEU DEUS eu demoraria 4 anos pra fazer algo pior que isso
	uint32_t tempTick = HAL_GetTick();

	if (numEnviado == -1 && qntAlunosFora <= 0) {
		return;
	}

	qntAlunosFora += numEnviado;
	qntAlunosFora =
			(qntAlunosFora > maxAlunosFora) ? maxAlunosFora : qntAlunosFora;

	if (numEnviado == 1) {
		qntTotalSaidas++;
		if (qntTotalSaidas >= capArraySaida) {
			int novoCap = capArraySaida * 2;
			uint32_t *temp = realloc(arrayTempoSaida,
					novoCap * sizeof(uint32_t));
			if (temp != NULL) {
				arrayTempoSaida = temp;
				capArraySaida = novoCap;
			}
		}
		arrayTempoSaida[qntTotalSaidas - 1] = tempTick;
	} else if (numEnviado == -1) {
		qntTotalEntradas++;
		if (qntTotalEntradas >= capArrayEntrada) {
			int novoCap = capArrayEntrada * 2;
			uint32_t *temp = realloc(arrayTempoEntrada,
					novoCap * sizeof(uint32_t));
			if (temp != NULL) {
				arrayTempoEntrada = temp;
				capArrayEntrada = novoCap;
			}
		}
		arrayTempoEntrada[qntTotalEntradas - 1] = tempTick;
		if (indiceSaidaPareamento < qntTotalSaidas) {
			uint32_t saida = arrayTempoSaida[indiceSaidaPareamento];

			if (tempTick >= saida) {
				somaTempos += (tempTick - saida);
				indiceSaidaPareamento++;
			}
		}
	}

	int qntDisponivel = maxAlunosFora - qntAlunosFora;

	char numStr[3];
	itoa(qntAlunosFora, numStr, 10);
	ST7789_DrawFilledRectangle(30, 100, 16, 26, BLACK);
	ST7789_WriteString(30, 100, numStr, Font_16x26, RED, BLACK);

	itoa(qntDisponivel, numStr, 10);
	ST7789_DrawFilledRectangle(190, 100, 16, 26, BLACK);
	ST7789_WriteString(190, 100, numStr, Font_16x26, GREEN, BLACK);
}

static void encerrarPrograma(void) {
	int menorQuantidade = indiceSaidaPareamento;

	uint32_t tempoMedioSaidas = 0;
	if (menorQuantidade > 0)
		tempoMedioSaidas = (somaTempos / menorQuantidade) / 1000;
	//matematica 2 B]
	//eletric boogaloo

	uint32_t tempoFimAula = HAL_GetTick();
	uint32_t tempoTotalAula = (tempoFimAula - tempoInicioAula) / 1000;
	//matematica 3 B]
	//o inimigo agora é outro
	ST7789_Fill_Color(BLACK);
	char numStr[11];
	ST7789_WriteString(8, 26, "AULA CONCLUIDA", Font_16x26, WHITE, BLACK);

	ST7789_WriteString(0, 100, "Pres.:", Font_16x26, WHITE, BLACK);
	itoa(qntAlunosSala, numStr, 10);
	ST7789_WriteString(112, 100, numStr, Font_16x26, WHITE, BLACK);

	ST7789_WriteString(0, 130, "Saidas:", Font_16x26, WHITE, BLACK);
	itoa(qntTotalSaidas, numStr, 10);
	ST7789_WriteString(128, 130, numStr, Font_16x26, WHITE, BLACK);

	ST7789_WriteString(0, 160, "Dur.:", Font_16x26, WHITE, BLACK);
	itoa(tempoTotalAula, numStr, 10);
	ST7789_WriteString(96, 160, numStr, Font_16x26, WHITE, BLACK);

	ST7789_WriteString(0, 190, "T.M.S.:", Font_16x26, WHITE, BLACK);
	itoa(tempoMedioSaidas, numStr, 10);
	ST7789_WriteString(144, 190, numStr, Font_16x26, WHITE, BLACK);
	while (1) {
		// FIM DO PROGRAMA
	}
}

static void interfacePrincipal(void) {
	tempoInicioAula = HAL_GetTick();
	maxAlunosFora = (qntAlunosReg > tempMaxAlunosFora) ?
	tempMaxAlunosFora :
															qntAlunosReg;
	ST7789_Fill_Color(BLACK);
	ST7789_WriteString(110, 80, "+", Font_16x26, WHITE, BLACK);
	ST7789_WriteString(145, 110, "E", Font_16x26, GREEN, BLACK);
	ST7789_WriteString(110, 140, "-", Font_16x26, WHITE, BLACK);
	ST7789_WriteString(75, 110, "S", Font_16x26, RED, BLACK);
	ST7789_DrawRectangle(70, 75, 165, 165, WHITE);
	ST7789_WriteString(30, 100, "0", Font_16x26, RED, BLACK);
	char numStr[3];
	itoa(maxAlunosFora, numStr, 10);
	ST7789_WriteString(190, 100, numStr, Font_16x26, GREEN, BLACK);
	ST7789_WriteString(90, 26, "00/", Font_16x26, WHITE, BLACK);
	itoa(qntAlunosReg, numStr, 10);
	ST7789_WriteString(138, 26, numStr, Font_16x26, WHITE, BLACK);
	while (1) {
		int botoesPressionados = (botEsqLer == 0) + (botDirLer == 0)
				+ (botCmaLer == 0) + (botBxoLer == 0);
		HAL_Delay(150);

		if (botEsqLer == 0 && botoesPressionados == 1) {
			HAL_Delay(250);
			int numEnviado = 1;
			controleSaida(numEnviado);
		} else if (botDirLer == 0 && botoesPressionados == 1) {
			HAL_Delay(250);
			int numEnviado = -1;
			controleSaida(numEnviado);
		} else if (botCmaLer == 0 && botoesPressionados == 1) {
			HAL_Delay(250);
			int numEnviado = 1;
			controlePresenca(numEnviado);
		} else if (botBxoLer == 0 && botoesPressionados == 1) {
			HAL_Delay(250);
			int numEnviado = -1;
			controlePresenca(numEnviado);
		} else if (botoesPressionados >= 2) {
			encerrarPrograma();
		}
	}
}
//ai pronto :D
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
