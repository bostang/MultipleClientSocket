'''
EL4236 Perancangan Perangkat Lunak Jaringan 2023/2024
*Hari dan Tanggal : Rabu, 6 Maret 2024
*Nama (NIM) 1     : Karma Kunga (13220028)
*Nama (NIM) 2     : Bostang Palaguna (13220055)
*Nama File        : client.c
*Deskripsi        : sourcecode server untuk program komunikasi TCP socket
*Programmer       : Bostang Palaguna
'''

import socket

# HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
HOST = '10.6.101.147' # Current IPv4 server  
PORT = 4567        # Port to listen on (non-privileged ports are > 1023)
MAX_BUFFER_SIZE = 80

def handle_connection(sockfd):
   """Handles communication with the server."""
   while True:
       try:
           data = input("Client: ").encode()
           sockfd.sendall(data)

           response = sockfd.recv(MAX_BUFFER_SIZE).decode()
           if response != "selesai":
            print("Server:", response)

           if response.startswith("selesai"):
               print("Memutus koneksi...")
               break

       except (ConnectionError, KeyboardInterrupt):
           print("Connection closed.")
           break

def main():
   """Creates a client socket and connects to the server."""
   with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sockfd:
       try:
           sockfd.connect((HOST, PORT))
           print("Connected to the server.")

           handle_connection(sockfd)

       except ConnectionRefusedError:
           print("Connection with the server failed.")

if __name__ == "__main__":
   main()

# Referensi
    # https://github.com/RPKQ/CNS_HW2