# README #
10.02 1)Это очень ранняя версия программы. Пока что могут работать много клиентов, но они не отличимы друг от друга. 
Сервер получает сообщение, выводит его у себя, и отправляет обратно клиенту.
Также работает команда "/exit".

16.02 2)Попробовал наладить работу сервера и клиентов, клиенты теперь передают свои имена серверу и тот их запоминает в структуре.
Проблема - не работает функция, пришлось код напрямую в программу записать. Из-за этого иногда ломаются сообщения. 
Клиенты работают хорошо, но первый почему-то получается "мёртвым" - он не может ни отправить сообщение, ни получить. У остальных такой проблемы нет.

21.02 3)Доделал задание. Функционал не большой, но чат работает. 
Вариант: Простой чат. 
Процесс сборки: 1)Cкачиваем файлы: sockmain.c, client.c, sock.h, func.c, makefile. 2)Запускаем терминал из папки с файлами, пишем: make 3)Происходит компиляция всех файлов.
Процесс запуска: 1)Запускаем терминал: ./sock.exe 2)Последовательно открываем новые терминалы, пишем ./client.exe и записываем никнейм, можем вводить сообщения.
Описание: func.c - функция проверки строк на равенство (можно другие функции добавить), sockmain.c - main от программы-сервера, client.c - main от программы-клиента,
makefile - файл компиляции и сборки, sock.h - содержит описание структур, функций и константы.
Библиотеки: #include <sys/ioctl.h> - URL: https://www.opennet.ru/man.shtml?topic=ioctl&category=2 - нужна для отсоединения клиентов.
			#include <arpa/inet.h> - URL: http://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html - нужна для заполнения полей структуры адреса сервера 
			(домена, IP, порта).
Команда /help говорит, что можно использовать команду /users - но её нет, забыл удалить строку.
Её можно реализовать, но у меня возникли проблемы с выходом из двойного цикла.

23.02 4)Исправил большинство замечаний. Программу можно проверять, процесс запуска остался без изменений, только при запуске сервера и клиента нужно указывать одинаковый порт (например: ./sock.exe 8080 и ./client.exe 8080).
Без правки остались только 2-3 пункта (1-имена могут повторяться, 2-сервер не может сам написать комманду /shutdown и завершиться, 
3-при выходе пользователя его никнейм не печатается). /shutdown - пробовал написать через fork() - столкнулся с проблемой, что сокет закрывается, но вот выйти из процесса, а потом из самой программы не удаётся.
Повторение имён исправить можно - но из-за моей реализации это немного проблематично (нужно отлавливать момент на котором клиент уже подключён и ввёл имя, потом отправлять запрос серверу, чтобы он пробежался по массиву и сравнил имена и вернуть результат клиенту)
Решил интерпретировать команду /users, как историю всех посещений (т.е. печатаются ники тех клиентов, которые когда-либо заходили в чат). Поставил функции обработчики сигналов, которые позволяют
безопасно завершать работу сервера и клиентов, убрал "залипание" порта. Попробовал приглашать к вводу клиентов с помощью специального символа - не получилось.
Ограничение на размер сообщения у каждого клиента сделал персональными, если он вводит больше символов, чем может - сервер просит его повторить ввод, разбив его сообщение на более маленькие.

Долго мучался с Gitom, создал ветку, попытался сделать merge, вроде получилось (по крайней мере, файлы появились здесь).

Провёл ещё один тест, в историю посещения не вносится информация о том, что клиент сменил никнейм (можно исправить также проходя массив и сравнивая имена).
Заметил, что makefile не добавился в новый commit (version 2). Он остался без изменений, т.е. компилировать нужно с его помощью.

28.02 4)Полностью исправил все замечания. Порт проверяется (не стал делать жёсткую проверку, просто сделал так, чтобы нельзя было вводить буквы, символы и последовательности более чем из 4 чисел).
Устранил проблему с кол-вом клиентов (ошибка была в массиве), протестировал ограничение на ввод - работает правильно (проверял символ в символ), попробую протестировать в машзале.
Команда /users теперь полностью работает (клиенты добавляются и удаляются, никнеймы меняются). Удалил задержку перед отправкой сообщения (в команде /nick оставил, чтобы компьютер успел соединить все строки).
Процесс запуска остался тем же, изменились только файлы sockmain.c и client.c (также необходимо иметь файлы sock.h, makefile, func.c).