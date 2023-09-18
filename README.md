# Пример структуры директорий для сборки deb-пакета

Для создания deb-пакета вам потребуется пакет "devscripts". Если он еще не установлен, выполните команду:

    sudo apt install devscripts

Создайте каталог для вашего пакета, назовём его myprogram:

    mkdir myserver

Зайдите в этот каталог:

    cd myserver

Создайте каталоги, которые будут имитировать файловую систему:

    mkdir -p usr/bin etc/devops

Скомпилируйте файл программы program.c и поместите исполняемый файл в usr/bin:

    gcc myserver.c -o usr/bin/myserver

Создайте файл конфигурации в etc/devops:

    touch etc/devops/myserver.conf

Создайте каталог DEBIAN в корневом каталоге вашего пакета. Этот каталог будет использоваться для управления информацией о пакете:

    mkdir DEBIAN

В директории DEBIAN создайте файл control и заполните его следующим образом:

```
Package: myserver
Version: 1.0
Section: base
Priority: optional
Architecture: all
Depends: libc6 (>= 2.7)
Maintainer: Your Name <your@mail.address>
Description: My server
```

Если ваша программа использует сервис systemd, необходимо создать файл сервиса:

    mkdir -p lib/systemd/system

А затем создать в этом каталоге файл сервиса lib/systemd/system/myserver.service:

```
[Unit]
Description=My server

[Service]
ExecStart=/usr/bin/myserver

[Install]
WantedBy=multi-user.target
```

После создания всех необходимых файлов и каталогов вернитесь в корневой каталог вашего пакета и соберите сам DEB-пакет:
    
    cd ..
    dpkg-deb --build myserver

Теперь у вас должен быть файл myserver.deb.

# Установка пакета

    sudo dpkg -i myserver.deb 
    Selecting previously unselected package myserver.
    (Reading database ... 232497 files and directories currently installed.)
    Preparing to unpack myserver.deb ...
    Unpacking myserver (1.0) ...
    Setting up myserver (1.0) ...


# Старт сервиса

    sudo systemctl start myserver
    sudo systemctl status myserver

    ● myserver.service - Simple Network Server with TCP Listener
        Loaded: loaded (/lib/systemd/system/myserver.service; disabled; vendor preset: enabled)
        Active: active (running) since Mon 2023-09-18 18:52:28 UTC; 1s ago
    Main PID: 60312 (myserver)
        Tasks: 1 (limit: 4625)
        Memory: 200.0K
            CPU: 2ms
        CGroup: /system.slice/myserver.service
                └─60312 /usr/bin/myserver

# Проверка что сервис запустился и слушает на порту 8080

    sudo ss -ntlp
    State   Recv-Q   Send-Q     Local Address:Port      Peer Address:Port  Process                                                    
    LISTEN  0        511            127.0.0.1:38151          0.0.0.0:*      users:(("node",pid=43552,fd=18))                          
    LISTEN  0        4096             0.0.0.0:111            0.0.0.0:*      users:(("rpcbind",pid=461,fd=4),("systemd",pid=1,fd=43))  
    LISTEN  0        128            127.0.0.1:8080           0.0.0.0:*      users:(("myserver",pid=60312,fd=3))                       
    LISTEN  0        4096       127.0.0.53%lo:53             0.0.0.0:*      users:(("systemd-resolve",pid=494,fd=13))                 
    LISTEN  0        128              0.0.0.0:22             0.0.0.0:*      users:(("sshd",pid=666,fd=3))                             
    LISTEN  0        4096                [::]:111               [::]:*      users:(("rpcbind",pid=461,fd=6),("systemd",pid=1,fd=45))  
    LISTEN  0        128                 [::]:22                [::]:*      users:(("sshd",pid=666,fd=4))                             

# Проверка установки соединения

    nc -4 localhost 8080
    hello
    hello