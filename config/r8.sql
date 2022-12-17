DROP table if EXISTS wexingroup;
DROP table if EXISTS orderrecord;
DROP table if EXISTS payrecord;
DROP table if EXISTS categy;
DROP table if EXISTS menu;
DROP table if EXISTS originalOrder;
DROP table if EXISTS groupkeyword;
DROP table if EXISTS greetState;


/* DEFAULT (datetime(CURRENT_TIMESTAMP,'localtime')) */

create table wexingroup(id INTEGER PRIMARY KEY AUTOINCREMENT, user varchar(255), phone varchar(56), name varchar(255));

create table orderrecord(id INTEGER PRIMARY KEY AUTOINCREMENT, user varchar(255), phone varchar(56), gendatetime DATETIME , sordernum varchar(12), ordernum int,  groupid int, status int,  nickname varchar(255), ordertime varchar(128), paytime varchar(128), formatdetail varchar(255), orderdetail varchar(255), price int);

create table categy(id INTEGER PRIMARY KEY AUTOINCREMENT,  user varchar(255), phone varchar(56),  name varchar(255), price int);

create table menu(id INTEGER PRIMARY KEY AUTOINCREMENT, user varchar(255), phone varchar(56),  name varchar(255), price float, categy int, descx varchar(255), defaultselect int);

create table originalOrder(phone varchar(56), groupid int , ordertime varchar(64), txt text(5000000));

create table groupkeyword(user varchar(255), phone varchar(255), keyword varchar(255));



create table greetState(user varchar(255), phone varchar(255), hello varchar(1204), endtime varchar(255), attention varchar(255), goodbye varchar(255));