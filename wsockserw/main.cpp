#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib") // Загружаем ws2_32.dll

struct modbus{
    unsigned short transaction_id=0,protocol_id =0, length;
    unsigned char unit_id=0;
    unsigned char function_code;
    char data[MAXBYTE-8];
    };

int main(){
    modbus package;
    char bytes[sizeof(modbus)];
 // Инициализируем DLL
    WSADATA wsaData;
    WSAStartup( MAKEWORD(2, 2), &wsaData);

 // Создаем сокет
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

 // Привязываем сокет
    sockaddr_in sockAddr;
 memset (& sockAddr, 0, sizeof (sockAddr)); // Каждый байт заполняется 0
 sockAddr.sin_family = PF_INET; // Использовать IPv4-адрес
 sockAddr.sin_addr.s_addr = inet_addr ("127.0.0.1"); // Определенный IP-адрес (в вузе 0.0.0.0)
 sockAddr.sin_port = htons (502); // Порт
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

 // Входим в состояние мониторинга
    listen(servSock, 20);

 // Получение клиентского запроса
    SOCKADDR clntAddr;
    int nSize;
    SOCKET clntSock;

    while(1){
        nSize = sizeof(SOCKADDR);
        clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
        recv(clntSock, bytes, sizeof(modbus), NULL);
        memcpy(&package, bytes, sizeof(modbus));
        switch (package.function_code)
        {
            case 65:
                {
                    strcpy(package.data, "Server is working!\n");
                    package.length = sizeof(package.data);
                    //кодируем структуру
                    memcpy(bytes, &package, sizeof(modbus));//закодировали в массив байт
                }
            break;
            case 66:
                {
                    char name[MAXBYTE-8] = "hello, ";
                    strcat(name, package.data);
                    strcpy(package.data, name);
                    package.length = sizeof(package.data);
                    //кодируем структуру
                    memcpy(bytes, &package, sizeof(modbus));//закодировали в массив байт
                }
            break;
            case 67:
                {
                    int rez = 1;
                    int val = std::atoi(package.data);
                    for (int i = 1; i <= val; i++)
                    {
                        rez *=i;
                    }
                    itoa ( rez,package.data, 10);
                    package.length = sizeof(package.data);
                    memcpy(bytes, &package, sizeof(modbus));//закодировали в массив байт
                }
            break;
        };
        send(clntSock, bytes, sizeof(modbus), NULL);
    }

 // Отправляем данные клиенту

 // Закрываем сокет
    closesocket(clntSock);
    closesocket(servSock);

 // Прекращаем использование DLL
    WSACleanup();

    return 0;
}
