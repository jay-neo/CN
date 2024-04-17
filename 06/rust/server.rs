use std::io::{Read, Write};
use std::net::{TcpListener, TcpStream};


fn calculate_date_time() -> String {
    use std::time::{SystemTime};
    let since_the_epoch = SystemTime::now().duration_since(SystemTime::UNIX_EPOCH).expect("Time went backwards");
    let mut seconds = since_the_epoch.as_secs(); 

    // Fix Error
    let timezone_offset = 7 * 3600 + 47 * 60;
    if seconds >= timezone_offset {
        seconds += timezone_offset;
    }   

    let years = 1970 + seconds / 31556952;
    let mut remaining_seconds = seconds % 31556952;

    // Calculate months
    let mut month = 1;
    let mut days_in_month = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
    // Leap year
    if years % 4 == 0 && (years % 100 != 0 || years % 400 == 0) {
        days_in_month[1] = 29;
    }
    for days in days_in_month.iter() {
        if remaining_seconds < days * 86400 {
            break;
        }
        remaining_seconds -= days * 86400;
        month += 1;
    }

    // Calculate days
    let days = remaining_seconds / 86400 + 1;
    remaining_seconds %= 86400;

    // Calculate hours
    let hours = remaining_seconds / 3600;
    remaining_seconds %= 3600;

    // Calculate minutes
    let minutes = remaining_seconds / 60;
    remaining_seconds %= 60;

    // Calculate seconds
    let seconds = remaining_seconds;

    format!("{:02}-{:02}-{} {:02}:{:02}:{:02}", days, month, years, hours, minutes, seconds)
}



fn handle_client(mut stream: TcpStream) {

    let peer_addr = stream.peer_addr().expect("Failed to get peer address");
    println!("Client connected from {}", peer_addr);

    loop {
        // this line reads data from the stream and stores it in the buffer.
        let mut buffer = [0; 1024];
        match stream.read(&mut buffer) {
            Ok(bytes_received) => {
                if bytes_received == 0 {
                    println!("Client disconnected from {}", peer_addr);
                    return;
                }
                 // this line converts the data in the buffer into a UTF-8 enccoded string.
                let request = String::from_utf8_lossy(&buffer[..bytes_received]);
                let response = if request.trim() == "time" {
                    calculate_date_time()
                } else {
                    "Bad Request".to_string()
                };

                stream.write(response.as_bytes()).expect("Failed to write response!");

                if request.trim() == "exit" {
                    println!("Client disconnected from {}", peer_addr);
                    return;
                }
            }
            Err(err) => {
                eprintln!("Client {} Error receiving data: {}", peer_addr, err);
                return;
            }
        }

    }
    
}


fn main() {
    let ip = "127.0.0.1";
    let port = 8080;
    let address = format!("{}:{}", ip, port);
    
    let listener = TcpListener::bind(&address).expect("Failed to bind to address");
    println!("Server listening on {}", address);

    for stream in listener.incoming(){
        match stream{
            Ok(stream) => {
                std::thread::spawn(|| handle_client(stream));
            }
            Err(e) => {
                eprintln!("Failed to establish connection: {}", e);
            }
        }
    }
}