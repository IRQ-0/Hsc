# System Web Monitor (SWM) for Windows
Pice of software that spits out information about:
- CPU usage
- Memory usage
- IP configuration

Also features:
- Accounts (unable to see stats if user is not logged in)

Project developement is not planned now, but we will see what will happen in future.

# Compilation:
1. Make sure that you have ``G++`` compiler installed (https://gcc.gnu.org/) as well as downloaded MySQL development kit for C++ (https://dev.mysql.com/downloads/connector/cpp/)
2. Open ``main.cpp`` file and configure lines from ``11`` to ``21``
Example configuration:\
``// Server configuration``\
``#define IP "127.0.0.1"`` -> Set IP address that server will listen on\
``#define PORT 8000`` -> Set port that server will listen on\
``#define LOG_FOLDER "c:\\logs"`` -> Set path to log folder (not needed if logs are off)\
``#define HTML_FOLDER "c:\\httpHome"`` -> Set path to server home (like ``htdocs`` on Xampp)\
``// Mysql DB params``\
``#define DB_USER "admin"`` -> Set MySQL username\
``#define DB_PASSWORD "admin"`` -> Set MySQL password\
``#define DB_HOST "127.0.0.1"`` -> Set MySQL host\
``#define DB_DATABASE "testdb"`` -> Set MySQL database name\
``#define DB_PORT 3306`` -> Set MySQL port
3. Run ``make.bat`` file. If it pitints ``OK`` at the end, everything is done successfully (it will also run server).

# DB configuartion
You have to properly set up database in order to allow program to use it later. It will basicly contain all information about accounts.
1. Open MySQL prompt and connect to your database.
2. Run command: 
```
CREATE TABLE users (
 id INT NOT NULL AUTO_INCREMENT,
 username varchar(255),
 password varchar(255),
 lastlogin varchar(255),
 primary key (id)
 );
 ```
3. Add new user by running fallowing command:
 ```
 INSERT INTO users (username, password) values ('USERNAME', 'PASSWORD');
```
4. Check last login date by running fallowin command:
```
SELECT lastlogin FROM users WHERE username = 'USERNAME';
```
5. Delete user by following command:
 ```
 DELETE FROM users WHERE username = 'USERNAME';
 ```
 6. Change username:
 ```
 UPDATE users SET username = 'NEW USERNAME' WHERE username = 'OLD USERNAME';
 ```
 
 7. Change password:
 ```
 UPDATE users SET password = 'NEW PASSWORD' WHERE password = 'OLD PASSWORD';
 ```
 
Don't use it plz
