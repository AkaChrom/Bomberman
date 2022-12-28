#include "jeu.h"

// structure du socket du client de cette application
SOCKET csock;
// informations techniques du socket du client de cette application
SOCKADDR_IN csin;

/**
 * permet à l'os de savoir que l'application va travaillé avec des scoket
 * utilisation de la librairie winsock (version 2)
 */
void init_winsock() {

    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);
}

/**
 * > initialise le socket du client 
 * > lance l'écoute sur son port assigné
 */
void init_socket() {

    // adresse ip du server distant
    char addr_ip[16];
    // selection de l'adresse ip de l'hôte distant
    printf("Entrer l'@Ip de l'autre joueur : ");
    //scanf("%s",&addr_ip);
    // création du socket
    csock = socket(AF_INET, SOCK_STREAM, 0);
    // adresse du server distant
    csin.sin_addr.s_addr = inet_addr("127.0.0.1");//&addr_ip);
    csin.sin_family = AF_INET;
    // port d'écoute
    csin.sin_port = htons(PORT_D);
}

/**
 * connexion du client de cette application avec un server distant
 * (méthode bloquante jusqu'au succès d'une connexion)
 */
void start_connexion() {    
    printf("SLT C MOI\n");
    // attente d'acceptation d'une connexion d'un client   
    while (connect(csock, (SOCKADDR *)&csin, sizeof(csin)) == SOCKET_ERROR) {
        printf("TENTATIVE DE CONNEXION ...\n");
        Sleep(100);
    } 
}

/**
 * envoi les données au client 
 */
void recv_infos() {
    // buffer servant à contenir le futur message
    char buffer[255];
    recv(csock, buffer, sizeof(buffer), 0);
    printf("MSG : %s",buffer);
}

/**
 * réceptionne les données du client
 */
void send_infos() {
     send(csock, "salut à toi aussi, c'est le client\r\n", 38, 0);
}

void test_net() {

    init_winsock();
    init_socket();
    start_connexion();

    while (1)
    {
        send_infos();
        recv_infos();
    }

}