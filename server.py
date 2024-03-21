import socket

# DEKLARASI KONSTANTA
PORT = 4567

# DEKLARASI VARIABEL
flag_kirim = 1
PORT_CLIENT = [0] * 40  # maksimum menampung 40 client
nClient = 0  # menyatakan banyaknya client

nama_developer1 = "Bostang Palaguna"
nama_developer2 = "Karma Kunga"
nama_developer3 = "Emmanuella Pramudita Rumanti"
NIM_developer1 = "13220055"
NIM_developer2 = "13220028"
NIM_developer3 = "13220031"


# INISIASI FUNGSI
def response(buff, input):
    if input == "nama1":
        buff = nama_developer1
    elif input == "nama2":
        buff = nama_developer2
    elif input == "nama3":
        buff = nama_developer3
    elif input == "NIM1":
        buff = NIM_developer1
    elif input == "NIM2":
        buff = NIM_developer2
    elif input == "NIM3":
        buff = NIM_developer3
    else:
        buff = "Perintah tidak diketahui"

# ALGORITMA UTAMA
def main():
    # setup server
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('127.0.0.1', PORT))
    server_socket.listen(10)
    print("[+]Listening....")

    while True:
        client_socket, addr = server_socket.accept()
        flag_kirim = 1
        print(f"Connection accepted from {addr[0]}:{addr[1]}")

        PORT_CLIENT[nClient] = addr[1]
        print(f"Assigning client{nClient+1} to port {addr[1]}")
        nClient += 1

        if nClient > 40:
            print(f"Maximum number of clients reached. Connection from {addr[0]}:{addr[1]} rejected.")
            client_socket.close()
            continue

        while True:
            if flag_kirim:
                buffer = client_socket.recv(1024).decode()
                if buffer == "selesai":
                    flag_kirim = 0
                    for i in range(40):
                        if addr[1] == PORT_CLIENT[i]:
                            print(f"Client{i+1} Memutus koneksi...")
                else:
                    for i in range(40):
                        if addr[1] == PORT_CLIENT[i]:
                            print(f"Client{i+1} ({addr[0]}:{addr[1]}): {buffer}")

                    response_buffer = response(buffer, buffer)
                    client_socket.send(response_buffer.encode())
                    print(f"Server: {response_buffer}")
                buffer = ""

        client_socket.close()

if __name__ == "__main__":
    main()
