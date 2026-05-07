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
#define maxAlunosFora 3

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
int qntTotalEntradas;
int capArraySaida = 4;
int capArrayEntrada = 4;
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
		/*
		iniciarPrograma();
		entrarSenha();
		ST7789_Fill_Color(BLACK);
		ST7789_WriteString(16, 26, "Bem-vindo professor.", Font_16x26, WHITE,
		BLACK);
		HAL_Delay(250);
		ST7789_Fill_Color(BLACK);
		*/
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
	ST7789_WriteString(16, 200, "- PRESS P10 -", Font_16x26, WHITE, BLACK);
	esperandoP10();
}

static void gerarSenha(int *senhaRand) {
	srand(HAL_GetTick());
	for (int bananas = 0; bananas < 4; bananas++) {
		int valGerado = (rand() % 3) + 1;
		senhaRand[bananas] = valGerado;
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
}
static void escreverSenha(int numEnviado, int qntNumeros, int *senhaEscrita) {
	char numStr[2];
	itoa(numEnviado, numStr, 10);
	switch (qntNumeros) {
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
}

static void enviarNumeroPraSenha(int numEnviado, int qntNumeros,
		int *senhaEscrita) {
	escreverSenha(numEnviado, qntNumeros, senhaEscrita);
}

static void entrarSenha(void) {
	bool senhaCorreta = false;
	int qntNumeros = 0;
	int senhaRand[4];
	int senhaEscrita[4];
	int numEnviado = 0;
	gerarSenha(senhaRand);
	desenharTelaSenha();
	for (int laranjas = 0; laranjas < 3; laranjas++) {
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
		for (int i = 0; i < 4; i++) {
			if (senhaEscrita[i] != senhaRand[i]) {
				iguais = false;
				break;
			}
		}
		if (iguais) {
			senhaCorreta = true;
			break;
		} else {
			ST7789_DrawFilledRectangle(63, 84, 16, 26, BLACK);
			ST7789_DrawFilledRectangle(93, 84, 16, 26, BLACK);
			ST7789_DrawFilledRectangle(123, 84, 16, 26, BLACK);
			ST7789_DrawFilledRectangle(153, 84, 16, 26, BLACK);
			qntNumeros = 0;
			for (int maracujas = 0; maracujas < 4; maracujas++) {
				senhaEscrita[maracujas] = 0;
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
	ST7789_WriteString(24, 10, "Qnt. Alunos", Font_16x26, verdeFODA, BLACK);
	ST7789_DrawFilledRectangle(100, 110, 50, 2, WHITE);
	ST7789_WriteString(110, 150, "+1", Font_16x26, WHITE, BLACK);
	ST7789_WriteString(145, 180, "-5", Font_16x26, WHITE, BLACK);
	ST7789_WriteString(110, 210, "-1", Font_16x26, WHITE, BLACK);
	ST7789_WriteString(75, 180, "+5", Font_16x26, WHITE, BLACK);
}

static void enviarNumeroAlunos(int numEnviado) {
	qntAlunosReg += numEnviado;
	qntAlunosReg = (qntAlunosReg > 999) ? 999 : qntAlunosReg;
	qntAlunosReg = (qntAlunosReg < 0) ? 0 : qntAlunosReg;
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
	desenharTelaAlunos();
	bool definindoAlunos = true;
	int numEnviado = 0;
	while (definindoAlunos) {
		int botoesPressionados = (botEsqLer == 0) + (botDirLer == 0)
				+ (botCmaLer == 0) + (botBxoLer == 0);

		HAL_Delay(50);

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
		}
	}
	arrayMatriculas = malloc(qntAlunosReg * sizeof(int));
}

static int confirmarMatricula(void) {
	bool matriculaEnviada = false;
	int valMatricula = -1;
	while (!matriculaEnviada) {
		if (valMatricula != -1) {
			if (valMatricula == 0)
				return 0;
			for (int jacas = 0; jacas < sizeof(arrayMatriculas); jacas++) {
				if (valMatricula == arrayMatriculas[jacas])
					return 0;
				else if (arrayMatriculas[jacas] == 0) {
					arrayMatriculas[jacas] = valMatricula;
					matriculaEnviada = true;
					break;
				}
			}
		}
	}
	return 1;
}

static void controlePresenca(int numEnviado) {
	if (confirmarMatricula() == 1) {
		qntAlunosSala += numEnviado;
		qntAlunosSala =
				(qntAlunosSala > qntAlunosReg) ? qntAlunosReg : qntAlunosSala;
		qntAlunosSala = (qntAlunosSala < 0) ? 0 : qntAlunosSala;
		char numStr[3];
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
	} else if (confirmarMatricula() == 0) {
		ST7789_WriteString(20, 200, "MATRICULA INVALIDA!", Font_11x18, WHITE,
		BLACK);
	}
}

static void controleSaida(int numEnviado) {
	qntAlunosFora += numEnviado;
	qntAlunosFora =
			(qntAlunosFora > maxAlunosFora) ? maxAlunosFora : qntAlunosFora;
	qntAlunosFora = (qntAlunosFora > 0) ? qntAlunosFora : 0;

	if (numEnviado == 1 && qntAlunosFora <= maxAlunosFora) {
		uint32_t tempTick = HAL_GetTick();
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
		uint32_t tempTick = HAL_GetTick();
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
	}

	int qntDisponivel = 3 - qntAlunosFora;

	char numStr[3];
	itoa(qntAlunosFora, numStr, 10);
	ST7789_DrawFilledRectangle(30, 100, 16, 26, BLACK);
	ST7789_WriteString(30, 100, numStr, Font_16x26, RED, BLACK);

	itoa(qntDisponivel, numStr, 10);
	ST7789_DrawFilledRectangle(190, 100, 16, 26, BLACK);
	ST7789_WriteString(190, 100, numStr, Font_16x26, GREEN, BLACK);
}

static void encerrarPrograma(void) {
	uint32_t somaTempos = 0;

	int menorQuantidade =
			(qntTotalSaidas < qntTotalEntradas) ?
					qntTotalSaidas : qntTotalEntradas;

	for (int i = 0; i < menorQuantidade; i++) {
		somaTempos += (arrayTempoEntrada[i] - arrayTempoSaida[i]);
	}

	uint32_t tempoMedioSaidas = 0;
	if (menorQuantidade > 0)
		tempoMedioSaidas = (somaTempos / menorQuantidade) / 1000;

	uint32_t tempoFimAula = HAL_GetTick();
	uint32_t tempoTotalAula = (tempoFimAula - tempoInicioAula) / 1000;
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
	ST7789_Fill_Color(BLACK);
	ST7789_WriteString(110, 150, "+", Font_16x26, WHITE, BLACK);
	ST7789_WriteString(145, 180, "+", Font_16x26, GREEN, BLACK);
	ST7789_WriteString(110, 210, "-", Font_16x26, WHITE, BLACK);
	ST7789_WriteString(75, 180, "-", Font_16x26, RED, BLACK);
	ST7789_WriteString(30, 100, "0", Font_16x26, RED, BLACK);
	ST7789_WriteString(190, 100, "3", Font_16x26, GREEN, BLACK);
	ST7789_WriteString(90, 26, "00/", Font_16x26, WHITE, BLACK);
	char numStr[3];
	itoa(qntAlunosReg, numStr, 10);
	ST7789_WriteString(138, 26, numStr, Font_16x26, WHITE, BLACK);
	while (1) {
		int botoesPressionados = (botEsqLer == 0) + (botDirLer == 0)
				+ (botCmaLer == 0) + (botBxoLer == 0);
		HAL_Delay(50);

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
