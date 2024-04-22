use std::net::{UdpSocket};

const SERVER_IP: &str = "127.0.0.1";
const SERVER_PORT: u16 = 8080;

fn handle_udpsocket(socket: &UdpSocket) {
    loop {
        let mut buffer = [0; 1024];
        match socket.recv_from(&mut buffer) {
            Ok((bytes_received, client_addr)) => {
                if bytes_received == 0 {
                    println!("Client Error from {}", client_addr);
                    continue;
                }
                
                let request = String::from_utf8_lossy(&buffer[..bytes_received]);
                
                println!("Client {} Request = {}", client_addr, request);

                socket
                    .send_to(request.as_bytes(), &client_addr)
                    .expect("Failed to write response!");

            }
            Err(err) => {
                eprintln!("Error receiving data: {}", err);
            }
        }
    }
}


fn main() {
    let address = format!("{}:{}", SERVER_IP, SERVER_PORT);
    
    let socket = UdpSocket::bind(&address).expect("Failed to bind to address");
    println!("Server listening on {}", address);

    handle_udpsocket(&socket);

    println!("Server terminated");
}