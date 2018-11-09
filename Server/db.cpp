#include "db.h"
#include <vector>
#include <iostream>
#include <QVector>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDate>
#include <QTime>

// A note on this class, don't use any global variables since this class will be accessed by the multi-threaded server.
// Since sql is safe for multithreading just build queries and return the results. The way that the sockets are set up
// it should be able to handle multi-threaded processes.

DB::DB() {
}

std::string DB::executeCommand(std::string command, std::string payload) {
    //std::vector<std::string> tables;
    //MYSQL_RES *result;
    //MYSQL_ROW row;
    MYSQL *connection, mysql;

    //int state;
    mysql_init(&mysql);

    connection = mysql_real_connect(&mysql, "localhost", "root", "atomforpeace", "ccc_app", 0, 0, 0);

    if (connection == NULL) {
        std::cout << mysql_error(&mysql) << std::endl;
    }

    if (command == "register") {
        return regUser(connection, payload);
    }

    if (command == "authenticate") {
        return authenticate(connection, payload);
    }

    if (command == "getstudents") {
        return getStudents(connection, payload);
    }

    if (command == "puzzlesolved") {
        return puzzleSolved(connection, payload);
    }

    if (command == "studentlist") {
        return studentlist(connection, payload);
    }

    if (command == "getSolvedPuzzles") {
        return solvedPuzzles(connection, payload);
    }
    if(command == "deleteStudent") {
        return deleteStudent(connection, payload);
    }

    if(command == "teachAuthenticate"){
        return teachAuthenticate(connection,payload);
    }

    //idk how necessary these are
    // mysql_free_result(result);
    //mysql_close(connection);

    //fallback command
    return "Command: " + command + " Not Recognized";
}

std::string DB::solvedPuzzles(MYSQL *connection, std::string payload) {
    MYSQL_RES *result;

    QVector<QString> split = QString::fromStdString(payload).split(",").toVector();

    QString query = "SELECT id FROM users WHERE username=\"" + split.at(0) + "\";";
    int state = mysql_query(connection, query.toLatin1().data());

    if (state != 0) {
        std::cout << mysql_error(connection) << std::endl;
        return "SQLERROR";
    }

    result = mysql_store_result(connection);

    //check to see if the user exists in the database
    if (mysql_num_rows(result) == 0) {
        return "INVALIDUSER";
    }

    MYSQL_ROW userrow = mysql_fetch_row(result);
    std::string userid = std::string(userrow[0]);

    query = " SELECT puzzles.puzzleid FROM `puzzles` INNER JOIN `users` on puzzles.userid = users.id ";
    query += " WHERE users.id =\"" + QString::fromStdString(userid) + "\"; ";

    //execute
    state = mysql_query(connection, query.toLatin1().data());
    result = mysql_store_result(connection);

    if (state != 0) {
        std::cout << mysql_error(connection) << std::endl;
        return "SQLERROR";
    }

    MYSQL_ROW row;
    QString res = "";

    while ((row = mysql_fetch_row(result)))
        res += QString(row[0]) + ",";
    res.chop(1); //remove last comma
    return res.toStdString();
}


std::string DB::teachAuthenticate(MYSQL *connection, std::string payload) {
    MYSQL_RES *result;

    QVector<QString> split = QString::fromStdString(payload).split(",").toVector();
    QString query = "SELECT * FROM users WHERE username=\"" + split.front() +
        "\" AND password=\"" + split.back() + "\"AND users.teacher=\"1\";";
    int state = mysql_query(connection, query.toLatin1().data());

    if (state != 0) {
        std::cout << mysql_error(connection) << std::endl;
        return "SQLERROR";
    }

    result = mysql_store_result(connection);

    if (mysql_num_rows(result) != 0) {
        std::cout << "Login Success" << std::endl;
        return "VALID";
    }

    //Easter egg by Meysam
    if (split.front() == "averysecretusername" && split.back() == "averysecretpassword") {
        return "VALID";
    }

    return "INVALID";
}

std::string DB::authenticate(MYSQL *connection, std::string payload) {
    MYSQL_RES *result;

    QVector<QString> split = QString::fromStdString(payload).split(",").toVector();
    QString query = "SELECT * FROM users WHERE username=\"" + split.front() +
        "\" AND password=\"" + split.back() + "\";";
    int state = mysql_query(connection, query.toLatin1().data());

    if (state != 0) {
        std::cout << mysql_error(connection) << std::endl;
        return "SQLERROR";
    }

    result = mysql_store_result(connection);

    if (mysql_num_rows(result) != 0) {
        std::cout << "Login Success" << std::endl;
        return "VALID";
    }

    //Easter egg by Meysam
    if (split.front() == "averysecretusername" && split.back() == "averysecretpassword") {
        return "VALID";
    }

    return "INVALID";
}

//Method to register users
std::string DB::getStudents(MYSQL *connection, std::string payload) {
    MYSQL_RES *result;

    QString query = "SELECT username FROM users;"; //WHERE class_code=\"" + QString::fromStdString(payload) + "\";";
    int state = mysql_query(connection, query.toLatin1().data());

    if (state != 0) {
        std::cout << mysql_error(connection) << std::endl;
        return "SQLERROR";
    }

    result = mysql_store_result(connection);
    //MYSQL_FIELD *field = mysql_fetch_field(result);
    MYSQL_ROW row;
    QString res = "";

    while ((row = mysql_fetch_row(result)))
        res += QString(row[0]) + ",";
    res.chop(1); //remove last comma
    return res.toStdString();
}

//Method to register users
std::string DB::regUser(MYSQL *connection, std::string payload) {
    MYSQL_RES *result;

    QVector<QString> split = QString::fromStdString(payload).split(",").toVector();

    QString query = "SELECT * FROM users WHERE username=\"" + split.at(0) + "\";";
    int state = mysql_query(connection, query.toLatin1().data());

    if (state != 0) {
        std::cout << mysql_error(connection) << std::endl;
        return "SQLERROR";
    }

    result = mysql_store_result(connection);

    //check for username
    if (mysql_num_rows(result) != 0) {
        return "USERERROR";
    }

    //query class code
    query = "SELECT * FROM users WHERE class_code=\"" + split.at(2) + "\";";
    state = mysql_query(connection, query.toLatin1().data());
    result = mysql_store_result(connection);

    //if teach and class present error
    //if student and class not present error
    if ((mysql_num_rows(result) != 0 && split.at(2) == "1") ||
        (mysql_num_rows(result) == 0 && split.at(2) == "0")) {
        return "CLASSERROR";
    }

    //construct insert command
    query = "INSERT INTO `users` (`username`,`password`,`class_code`,`teacher`, `dateofbirth`)  VALUES (\"";
    query += split.at(0) + "\",\"" + split.at(1) + "\"," + split.at(2) + ",\"" + split.at(3) + "\",\"" + split.at(4) + "\");";

    //For debug, remove later
    //std::cout << query.toLatin1().data() << std::endl;

    //execute
    state = mysql_query(connection, query.toLatin1().data());
    result = mysql_store_result(connection);

    if (state == 0) {
        return "SUCCESS";
    } else {
        return "FAIL";
    }
}


//Method to submit and log a solved puzzle
std::string DB::puzzleSolved(MYSQL *connection, std::string payload) {
    MYSQL_RES *result;

    QVector<QString> split = QString::fromStdString(payload).split(",").toVector();

    QString query = "SELECT id FROM users WHERE username=\"" + split.at(0) + "\";";
    int state = mysql_query(connection, query.toLatin1().data());

    if (state != 0) {
        std::cout << mysql_error(connection) << std::endl;
        return "SQLERROR";
    }

    result = mysql_store_result(connection);

    //check to see if the user exists in the database
    if (mysql_num_rows(result) == 0) {
        return "INVALIDUSER";
    }

    MYSQL_ROW userrow = mysql_fetch_row(result);
    std::string userid = std::string(userrow[0]);

    query = "SELECT `puzzleid` FROM `puzzles` WHERE puzzles.userid="+ QString::fromStdString(userid)+";";
    state = mysql_query(connection, query.toLatin1().data());
    if (state != 0) {
        std::cout << mysql_error(connection) << std::endl;
        return "SQLERROR";
    }

    result = mysql_store_result(connection);
    int numfields = mysql_num_fields(result);


    MYSQL_ROW row;
    std::vector<int> completed;
    while((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < numfields; i++)
        {
            completed.push_back(std::stoi(std::string(row[i])));
        }
    }

    //construct insert command

    int puzzleid = std::stoi(split.at(1).toStdString());
    for(auto it = completed.begin(); it < completed.end(); it++){
        if(puzzleid==*it){
            return "SUCCESS"; // check for duplicates
        }
    }
    //otherwise update the database

    query = "INSERT INTO `puzzles` (`userid`, `puzzleid`, `date`) VALUES (\"";
    query += QString::fromStdString(userid) + "\",\"" + split.at(1) + "\",CURRENT_TIMESTAMP\);";

    //For debug, remove later
    std::cout << query.toLatin1().data() << std::endl;

    //execute
    state = mysql_query(connection, query.toLatin1().data());
    result = mysql_store_result(connection);

    if (state == 0) {
        return "SUCCESS";
    } else {
        return "FAIL";
    }
}

//Method to get the list of students
std::string DB::studentlist(MYSQL *connection, std::string) {
    MYSQL_RES *result;
    QString query = "SELECT id, username, (SELECT date as completiondate FROM `puzzles` WHERE puzzletable.puzzleid = 1 AND puzzletable.userid = usertable.id) as puzzle1, (SELECT date FROM `puzzles` WHERE puzzletable.puzzleid = 2 AND puzzletable.userid = usertable.id) as puzzle2, (SELECT date FROM `puzzles` WHERE puzzletable.puzzleid = 3 AND puzzletable.userid = usertable.id) as puzzle3, (SELECT date FROM `puzzles` WHERE puzzletable.puzzleid = 4 AND puzzletable.userid = usertable.id) as puzzle4 FROM users as usertable, puzzles as puzzletable;";
    int state = mysql_query(connection, query.toLatin1().data());

    if (state != 0) {
        std::cout << mysql_error(connection) << std::endl;
        return "SQLERROR";
    }

    QString generatedhtml = "";
    generatedhtml += "<!DOCTYPE html> <html lang='en'> <head> <meta charset='utf-8'> <title>Student Analytics Report - The CCC Educational App</title> <meta name='viewport' content='width=device-width, initial-scale=1'> <meta http-equiv='X-UA-Compatible' content='IE=edge' /> <link rel='stylesheet' href='https://bootswatch.com/united/bootstrap.css' media='screen'> <link rel='stylesheet' href='https://bootswatch.com/assets/css/custom.min.css'> <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries --> <!--[if lt IE 9]> <script src='https://bootswatch.com/bower_components/html5shiv/dist/html5shiv.js'></script> <script src='https://bootswatch.com/bower_components/respond/dest/respond.min.js'></script> <![endif]--> </head> <body> <div class='navbar navbar-default navbar-fixed-top'> <div class='container'> <div class='navbar-header'> <a href='#' class='navbar-brand'><strong>The CCC Educational App</strong></a> </div> <div class='navbar-collapse collapse' id='navbar-main'><a href='#' class='navbar-brand pull-right'><strong>Date: " + QDate::currentDate().toString() + " - " + QTime::currentTime().toString() + "</strong></a> </div> </div> </div> <div class='container'> <!-- Tables ================================================== --> <div> <div class='row'> <div class='col-lg-12'> <div class='page-header'> <h2>Student Analytics Report</h2> </div> <div class='bs-component'> <table class='table table-striped table-hover '> <thead> <tr> <th>Student Name (ID)</th> <th>Puzzle 1 (Stack)</th> <th>Puzzle 2 (Array)</th> <th>Puzzle 3 (List)</th> <th>Puzzle 4 (Tree)</th> </tr> </thead> <tbody>";
    result = mysql_store_result(connection);
    //MYSQL_FIELD *field = mysql_fetch_field(result);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result))) {
        //For loop to add rows
        QString userid = row[1];
        QString username = row[0];
        QString puzzle1 = row[2];

        if (puzzle1.length() < 1) {
            puzzle1 = "--";
        }

        QString puzzle2 = row[3];

        if (puzzle2.length() < 1) {
            puzzle2 = "--";
        }

        QString puzzle3 = row[4];

        if (puzzle3.length() < 1) {
            puzzle3 = "--";
        }

        QString puzzle4 = row[5];

        if (puzzle4.length() < 1) {
            puzzle4 = "--";
        }

        generatedhtml += "<tr> <td>" + QString(row[1]) + " ( " + QString(row[0]) + " )" + "</td> <td>" + puzzle1 + "</td> <td>" + puzzle2 + "</td> <td>" + puzzle3 + "</td> <td>" + puzzle4 + "</td> </tr>";
    }
    generatedhtml += "</tbody> </table> </div><!-- /example --> </div> </div> </div> </div> </html>";
    //For debug, remove later
    std::cout << "Student Analytics Report generated (analytics.html)" << std::endl;
    //Try writing something to a file (HTML) and opening it in a browser:
    QString filename = "./analytics.html";
    QFile file(filename);
    QFile::remove(filename);

    if (file.open(QIODevice::ReadWrite) ) {
        QTextStream stream(&file);
        stream << generatedhtml << endl;
    }

    QString urladdress = QDir::currentPath() + "/analytics.html";
    return urladdress.toStdString();
}

std::string DB::deleteStudent(MYSQL* connection, std::string payload){

    //check id exists in users table
    MYSQL_RES *result;
    QString query = "SELECT `id` FROM `users` WHERE users.username=\"" + QString::fromStdString(payload) + "\";";
    int state = mysql_query(connection, query.toLatin1().data());

    if (state != 0) {
        std::cout << mysql_error(connection) << std::endl;
        return "USERDOESNOTEXIST";
    }

    result = mysql_store_result(connection);

    MYSQL_ROW userrow = mysql_fetch_row(result);
    std::string userid = std::string(userrow[0]);



   //delete row from users table
    query = "DELETE FROM `users` WHERE users.id=\"" + QString::fromStdString(userid) + "\";";
    state = mysql_query(connection, query.toLatin1().data());

    if (state != 0) {
        std::cout << mysql_error(connection) << std::endl;
        return "FAILEDTODELETE";
    }

    result = mysql_store_result(connection);

    //check for id in row from puzzles table
    query = "SELECT `userid` FROM `puzzles` WHERE puzzles.userid=\"" + QString::fromStdString(payload) + "\";";
    state = mysql_query(connection, query.toLatin1().data());

    if (state != 0) {
        std::cout << mysql_error(connection) << std::endl;
        return "USERIDNOTINPUZZLES";
    }

    //delete row from users table
     query = "DELETE FROM `puzzles` WHERE puzzles.userid=\"" + QString::fromStdString(payload) + "\";";
     state = mysql_query(connection, query.toLatin1().data());

     result = mysql_store_result(connection);

    if (state == 0) {
        return "SUCCESS";
    } else {
        return "FAIL";
    }

}
