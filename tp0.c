/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;
	//char* mensaje = malloc(32*sizeof(char));
	char* mensaje= "Mensaje muy muy largo pero recontra demasiadolargolarguisimo";
	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	//Loggear "soy un log"
	printf("Ingrese su mensaje;.\n-> ");
	//scanf("%s",mensaje);
	printf("Ingresastes: %s  \n", mensaje);
	log_info(logger,"INICIO PROGRAMA\n Envio mensaje");
	log_info(logger,mensaje);

	config = leer_config();

	/*------------------------------PARTE 3-----------------------------------------------------*/

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	//crear conexion
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	//enviar mensaje
	conexion =  crear_conexion(ip,puerto);
	enviar_mensaje(mensaje,conexion);

	//recibir mensaje
	log_info(logger,"Ahora recibo mensaje");
	char* mensajeRecibido = recibir_mensaje(conexion);

	//printf("recibido %s\n",mensajeRecibido);

	//loguear mensaje recibido
	log_info(logger, mensajeRecibido);
	free(mensajeRecibido);



	log_info(logger,"FIN PROGRAMA");
	terminar_programa(conexion, logger, config);
	printf("memoria liberada.\n");/*//*/

}

//TODO
t_log* iniciar_logger(void)
{
	return log_create("tp0.log","tp0.c",1, LOG_LEVEL_TRACE);
}

//TODO
t_config* leer_config(void)
{
	return config_create("tp0.config");
}

//TODO
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	liberar_conexion(conexion);
		printf("Conexion liberada.\n");

	log_destroy(logger);
		printf("Logger destruido.\n");

		config_destroy(config);
		printf("Config destruido\n");
}
