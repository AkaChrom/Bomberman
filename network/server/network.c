#include "jeu.h"

// structure du socket du serveur de cette application
SOCKET sock;
// structure du socket du client de l'application distante
SOCKET csock;

/**
 * permet à l'os de savoir que l'application va travaillé avec des scoket
 * utilisation de la librairie winsock (version 2)
 */
void init_winsock() {

    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);
}

/**
 * > initialise le socket du server 
 * > lance l'écoute sur son port assigné
 */
void init_socket() {

    // informations techniques du socket du serveur
    SOCKADDR_IN sin;   
    // création du socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    // adresse du pc
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_family = AF_INET;
    // port d'écoute
    sin.sin_port = htons(PORT);
    // attache le socket au port
    bind(sock, (SOCKADDR *)&sin, sizeof(sin));
    // démarre l'ecoute du port sur le socket
    listen(sock, 0);
}

/**
 * > attente d'une connexion avec un client distant
 * (méthode bloquante jusqu'au succès d'une connexion)
 */
void start_connexion() {
    // informations techniques du socket du client
    SOCKADDR_IN csin;
    // taille de la informations 
    int sinsize = sizeof(csin);
    // attente d'acceptation d'une connexion d'un client
    while ((csock = accept(sock, (SOCKADDR *)&csin, &sinsize)) == INVALID_SOCKET) {
        printf("\n ERREUR : CONNEXION AU SOCKET NON ACCEPTEE");
    }
}

/**
 * envoi les données au client 
 */
void send_infos() {
     send(csock, "COUCOU c'est le serveur!\r\n", 27, 0);
}

/**
 * réceptionne les données du client
 */
void recv_infos() {
    // buffer servant à contenir le futur message
    char buffer[255];
    recv(csock, buffer, sizeof(buffer), 0);
    printf("MSG : %s",buffer);
}

void test_net() {

    init_winsock();
    init_socket();
    start_connexion();

    while (1)
    {
        send_infos();
        recv_infos();
        Sleep(1000);
    }
    

}