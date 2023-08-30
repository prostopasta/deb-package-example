# Пример структуры директорий для сборки deb-пакета

Для создания deb-пакета вам потребуется пакет "devscripts". Если он еще не установлен, выполните команду:

    sudo apt install devscripts

Создайте каталог для вашего пакета, назовём его myprogram:

    mkdir myprogram

Зайдите в этот каталог:

    cd myprogram

Создайте каталоги, которые будут имитировать файловую систему:

    mkdir -p usr/bin etc/devops

Скомпилируйте файл программы program.c и поместите исполняемый файл в usr/bin:

    gcc program.c -o usr/bin/program

Создайте файл конфигурации в etc/devops:

    touch etc/devops/program.conf

Создайте каталог DEBIAN в корневом каталоге вашего пакета. Этот каталог будет использоваться для управления информацией о пакете:

    mkdir DEBIAN

В директории DEBIAN создайте файл control и заполните его следующим образом:

```
Package: myprogram
Version: 1.0
Section: base
Priority: optional
Architecture: all
Depends: libc6 (>= 2.7)
Maintainer: Your Name <your@mail.address>
Description: My program
```

Если ваша программа использует сервис systemd, необходимо создать файл сервиса:

    mkdir -p lib/systemd/system

А затем создать в этом каталоге файл сервиса lib/systemd/system/myprogram.service:

```
[Unit]
Description=My program

[Service]
ExecStart=/usr/bin/program

[Install]
WantedBy=multi-user.target
```

После создания всех необходимых файлов и каталогов вернитесь в корневой каталог вашего пакета и соберите сам DEB-пакет:
    
    cd ..
    dpkg-deb --build myprogram

Теперь у вас должен быть файл myprogram.deb.