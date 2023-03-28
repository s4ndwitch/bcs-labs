# bsc-lab13

1. В manjaro директория с файлами apache находится в /etc/httpd/conf/.
2. В ней создавай и .htpasswd, и .htgroup, ибо пользователь http не имеет доступа к файлам других пользователей.
3. Разрешай в conf файле AllowOverride, всё как делал.
4. Корневая директория файлов находится в /srv/http/.
5. Создавай .htaccess и прочую как уже делал.
6. Код пятого пункта приложу.
7. Сначала установи github.com/apache/apr по инструкции, которая там же лежит.
8. Затем склонируй github.com/apache/httpd и замени support/passwd_common.c на приложенный файл. Добавленный код помечен комментарием // LAB //, найдёшь по поиску.
9. В файле build/config_vars.mk замени -ldb-6.2 на -ldb.
9. Сконфигурируй и скомпилируй в том же порядке, как и apr, но не устанавливай.
10. Бинарник будет лежать в support/htpasswd.

Использовано:
https://github.com/zhicheng/base64
https://github.com/apache/httpd
