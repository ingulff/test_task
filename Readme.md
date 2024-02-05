### Build options

- `TT_BUILD_TESTS` - включает тесты в сборку проекта. По умолчанию `ON`
- `TT_HOST_IP` - указывает ip на котором будет запускаться сервер вместо заданного `localhost` или `127.0.0.1` по умолчанию
- `TT_PORT_IP` - задает порт который будет работы сервера, по умолчанию значение `1234`
- `TT_THREADS_NUMBER` - задает число потоков для работы сервера, по умолчанию равно `4`

**Примеры вызовов cmake**  
```bash
$ cmake -G Ninja -S ./ -B build/linux - -DTT_THREADS_NUMBER=1 -DTT_PORT_IP=880

# сборка со всеми опциями по умолчанию:
$ cmake -S . -B  build/linux/out
$ cmake --build build/linux/out --parallel 8
```

Сборка проверена:
- ubuntu 22.04
  - cmake 3.28.2
  - clang & clang++  15.0.7
- Windows 11
  - cmake 3.20.21032501-MSVC_2
  - Visual Studio 16 2019
  - MSVC 19.29.30153.0

**@NOTE**  
- перед сборкой добавьте в 3rdparty_config.cmake:
	- `BOOST_INCLUDE` - директория где установлена библиотека boost
	- `SSL_INSTALL_PATH` - директория где установлена openss библиотека (или другая, но тогда еще нужно скорректировать fing_package(OpenSSL)  
  		для вашей библиотеки в 3rdparty.cmake) eсли используете OpenSSL укажите USE_OPEN_SSL как ON

### Tests

- перед запуском тестов нужно запустить отдельно процесс сервера
- **Опции сборки тестов**:
    - `TT_EXTERNAL_UPLOAD_SOURCE_DIR` - добавить для тестов путь, который будет указан как параметр  
    запуска сервера. Без этой опции будет будет использовать `./resource/upload` от корневой  
    директории проекта
    - `TT_EXTERNAL_DOWNLOAD_SOURCE_DIR` - добавить путь, куда будут скачиваться файлы, по умолчаниб  
    будет использоваться  `./resource/downloads` от корневой директории проекта
    **@NOTE** проверка сейчас реализована как контрольная сумма отправляемого файла и полученного в ответе от сервера и   
    полученные файлы не записываются на диск 
    - `TT_EXTERNAL_CONFIG_FILEPATH` - добавить путь к файлу (с именем файла) с ключами для валидации подключения.   
    По умолчанию используется `./config`

**@WARN**  
Для корректноый работы тестов важно, аргументы запуска сервера совпадали с `TT_EXTERNAL_UPLOAD_SOURCE_DIR` и `TT_EXTERNAL_CONFIG_FILEPATH`  
в случае если они не принимают значения по умолчанию.

**Примеры вызовов cmake**  
```bash
$ cmake -G Ninja -S ./ -B build/linux -DTT_EXTERNAL_UPLOAD_SOURCE_DIR=path-to-dir -DTT_EXTERNAL_UPLOAD_SOURCE_DIR=path-to-dir
```

**@NOT**  
Тесты могут занимать несколько минут, так как в некорых выполняется примерно 1000 запросов


### Using wget
Примеры комманд wget, предполагая, что сервер работает на `127.0.0.1:1234`
```bash
# запустив сервер с корневой директории проекта:
$ build/linux/out/source/http_server ${PWD}/resource/upload ${PWD}/configs/ids.txt

# несуществующий файл без secret
$ wget https://127.0.0.1:1234/qwwerty --no-check-certificate

# несуществующий файл с фейк secret
$ wget https://127.0.0.1:1234/qwwerty -d --header="secret:80" --no-check-certificate

# несуществующий файл с secret
$ wget https://127.0.0.1:1234/qwwerty -d --header="secret:1910" --no-check-certificate

# существующий файл без secret
$ wget https://127.0.0.1:1234/image/img_jpg_1.jpg --no-check-certificate

# существующий файл с фейк secret
$ wget https://127.0.0.1:1234/img_jpg_1.jpg -d --header="secret:80" --no-check-certificate

# существующий файл с фейк secret
$ wget https://127.0.0.1:1234/img_jpg_1.jpg -d --header="secret:1910" --no-check-certificate
```
