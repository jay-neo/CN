<div align="center"><h1>FTP</h1></div>


FTP (file transfer protocol) is an internet protocol that is used for transferring files between client and server over the internet or a computer network. It is similar to other internet protocols like SMTP which is used for emails and HTTP which is used for websites. FTP server enables the functionality of transferring files between server and client. A client connects to the server with credentials and depending upon the permissions it has, it can either read files or upload files to the server as well




<h2 align="center">FTP Server</h2>

## Windows


https://4sysops.com/archives/install-and-configure-an-ftp-server-with-powershell/
https://gist.github.com/PrateekKumarSingh/10891d3b9636435da46fb08170a11960

https://stackoverflow.com/questions/59546334/powershell-add-asp-net-4-8-via-powershell

https://www.reddit.com/r/PowerShell/comments/13kd6sb/how_to_create_an_ftp_connection_and_file_download/?rdt=57072


### FTP Server Setup

### FTP Site Creation

### Access FTP Site

Using File Explorer

https://www.youtube.com/watch?v=NQeCUpT0Yns
https://www.youtube.com/watch?v=VFVKs4YwkF0
https://www.youtube.com/watch?v=D0gseNd1ZYQ












## Linux

### Debian / Fedora / Arch

#### Installation & Configuration

Step 1: Installation
```sh
sudo <package_manager> install vsftpd
```
Step 2: `vsftpd` file configuration
```sh
sudo nano /etc/vsftpd.conf
```
edit the file
```
listen=NO
listen_ipv6=YES
anonymous_enable=NO
local_enable=YES
write_enable=YES
local_umask=022
dirmessage_enable=YES
use_localtime=YES
xferlog_enable=YES
connect_from_port_20=YES
chroot_local_user=YES
secure_chroot_dir=/var/run/vsftpd/empty
pam_service_name=vsftpd
rsa_cert_file=/etc/ssl/certs/ssl-cert-snakeoil.pem
rsa_private_key_file=/etc/ssl/private/ssl-cert-snakeoil.key
ssl_enable=NO
pasv_enable=Yes
pasv_min_port=10000
pasv_max_port=10100
allow_writeable_chroot=YES

local_root=/ftp 
allow_writeable_chroot=YES
chroot_local_user=YES
chroot_list_enable=YES
chroot_list_file=/etc/vsftpd.chroot_list
```
`listen_port=2121`

Step 3: Firewall rules configuration
for Debian based distros
```sh
sudo ufw allow from any to any port 20,21 proto tcp
```
for RHEL based distros
```sh
sudo firewall-cmd --zone=public --permanent --add-service=ftp
```

Step 4: Restart the `vsftpd` server
```sh
sudo systemctl restart vsftpd
```

#### Creating FTP User

```sh
sudo useradd -m <ftpuser>
```

```sh
sudo passwd <ftpuser>
```

#### Create the FTP folder and set permissions

```sh
sudo mkdir /ftp
```

```sh
sudo chown adminuser /ftp
```
```sh
sudo systemctl restart --now vsftpd
```


#### To enable SSL/TLS 

```sh
sudo nano /etc/vsftpd.conf
```
Go to the end of the file and remove the following lines
```
rsa_cert_file=/etc/ssl/certs/ssl-cert-snakeoil.pem
rsa_private_key_file=/etc/ssl/private/ssl-cert-snakeoil.key
ssl_enable=NO
```

And, paste the following lines
```
rsa_cert_file=/etc/ssl/private/vsftpd.pem
rsa_private_key_file=/etc/ssl/private/vsftpd.pem
ssl_enable=YES
allow_anon_ssl=NO
force_local_data_ssl=YES
force_local_logins_ssl=YES
ssl_tlsv1=YES
ssl_sslv2=NO
ssl_sslv3=NO
require_ssl_reuse=NO
ssl_ciphers=HIGH
```

Save the changes and finally, restart the vsftpd service by typing in
```sh
sudo systemctl restart --now vsftpd
```

### RHEL

[in RedHat](https://www.computernetworkingnotes.com/linux-tutorials/how-to-configure-ftp-server-in-redhat-linux.html)
[using SELinux rules in CentOS](https://www.tecmint.com/install-ftp-server-in-centos-7)
[SSL/TLS in RedHat Linux](https://www.tecmint.com/setup-secure-ftp-file-transfer-using-ssl-tls-in-rhel-8/)
[Security in FTP](https://likegeeks.com/ftp-server-linux/#FTP_server_security)














<h2 align="center">FTP Client</h2>

## FTP cli

## Using Web Browser





### Resources


https://www.youtube.com/watch?v=cNj62Eujlug
https://www.youtube.com/watch?v=ke3tnd73aUI
https://www.youtube.com/watch?v=VFVKs4YwkF0
https://www.youtube.com/watch?v=5B53m03pmx0
https://www.youtube.com/watch?v=r0C5HIKAmio
