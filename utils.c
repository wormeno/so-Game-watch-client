/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

//TODO
/*
 * Recibe un paquete a serializar, y un puntero a un int en el que dejar
 * el tamaño del stream de bytes serializados que devuelve
 */
void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	int offset = 0;
	*bytes = paquete->buffer->size + 2*sizeof(int);
	void* aEnviar = malloc(*bytes);
	memcpy(aEnviar + offset , &(paquete->codigo_operacion),sizeof(int));
	offset += sizeof(int);
	memcpy(aEnviar + offset , &(paquete->buffer->size),sizeof(int));
	offset += sizeof(int);
	memcpy(aEnviar + offset , paquete->buffer->stream,paquete->buffer->size);
	return aEnviar;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

//TODO
void enviar_mensaje(char* mensaje, int socket_cliente)
{
	int estado=0,bytes = 0;
	t_buffer* buffer = (t_buffer*) malloc(sizeof(t_buffer));
	buffer->size =  strlen(mensaje) +1; // ta;anio del mensaje
	void* stream =  malloc(buffer->size);
	memcpy(stream , mensaje,buffer->size);
	buffer->stream = stream;
	//Armo el paquete
	t_paquete* paquete = (t_paquete*) malloc(sizeof(t_paquete));
	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = buffer;
	//Serializo

	void* aEnviar =  serializar_paquete(paquete,&bytes);
	//envio
	estado = send(socket_cliente, aEnviar, bytes, 0);

	printf("envio \n");
	switch (estado) {
			case -1:
				printf("EnviarMensaje -> Error al enviar.\n");
				break;
			case 0:
				printf("EnviarMensaje -> No se pudo enviar nada.\n");
				break;
			default:
				printf("EnviarMensaje -> Paquete Enviado - %d Bytes transferidos.\n", estado);
				break;
	}
	printf("\n");
	//libero
	free(stream);
	free(buffer);
	free(paquete);
	free(aEnviar);
}

//TODO
char* recibir_mensaje(int socket_cliente)
{
	char* mensaje;
	int cod_op,size;
	recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL); // Primero recibimos el codigo de operacion
	switch( cod_op ) {
		case MENSAJE:
			recv(socket_cliente, &size, sizeof(int), MSG_WAITALL);//recibimos el buffer: su tama;o
			printf("tamanio %d\n",size);
			mensaje= malloc(size);
			recv(socket_cliente, mensaje, size, MSG_WAITALL);
			break;
		default:
			printf("RecibirMensaje -> Error OpCode: %d.\n", cod_op);
			break;
	}
	return mensaje;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
