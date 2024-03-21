use std::net::TcpStream;
use std::io::{self, Write, ErrorKind};

const SERVER_IP: &str = "127.0.0.1";
const SERVER_PORT: u16 = 8080;


fn main() {
    let server_addr = format!("{}:{}", SERVER_IP, SERVER_PORT);

    let mut client = TcpStream::connect(server_addr).expect("Failed to connect to server");

    let mut msg = String::new();
    loop {
        print!("Enter your message: ");
        io::stdout().flush().unwrap(); // to ignore newline
    
        io::stdin().read_line(&mut msg).expect("Error: Failed to read line");

        match client.write_all(msg.as_bytes()) {
            Ok(_) => {}
            Err(ref e) if e.kind() == ErrorKind::BrokenPipe => {
                println!("Connection closed by server");
                break;
            }
            Err(e) => panic!("Failed to send message: {:?}", e),
        }

        if msg.trim() == "exit" {
            break;
        }
        
        msg.clear();
    }

    println!("Happy terminating from client side");
}
