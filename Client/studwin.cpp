#include "studwin.h"
#include "ui_studwin.h"
#include "client.h"
#include <vector>
#include "qstandarditemmodel.h"


StudWin::StudWin(QWidget *parent) :
    QWidget(parent),

    ui(new Ui::StudWin) {
    ui->setupUi(this);
    layout_0 = new QGridLayout(this);
    ui->mainGameWidget->setLayout(layout_0);
    pw=nullptr;
    levelshow = true;
    currPuzz = 0;
    ui->puzzle1->setStyleSheet("Background-color: #3daee9;");
    QObject::connect(this->ui->logoutButton, &QPushButton::clicked, this, &StudWin::studLogoutButton);
}

void StudWin::switched(){

    if(pw!=nullptr)
    {
       layout_0->removeWidget(pw);
       delete pw;
    }
    pw = new PuzzleWindow;
    puzzles.clear();
    setupLevels();
    pw->setPuzzle(puzzles.front());
    layout_0->addWidget(pw);
    this->ui->logoutButton->setCheckable(true);
}

StudWin::StudWin(Client *client, QWidget *parent) : StudWin(parent) {
    this->client = client;
}

StudWin::~StudWin() {
    delete ui;
    for (auto it = puzzles.begin(); it < puzzles.end(); it++) {
        delete *it;
    }
}

void StudWin::setupLevels() {
    puzzles.push_back(new StackPuzzle(size()));
    puzzles.push_back(new ArrayPuzzle(size()));
    puzzles.push_back(new ListPuzzle(size()));
    puzzles.push_back(new TreePuzzle(size()));
}

std::vector<bool> StudWin::getSolvedList() {
    std::vector<bool> solvedlist;
    for (auto it = puzzles.begin(); it < puzzles.end(); it++) {
        Puzzle *puzz = *it;
        solvedlist.push_back(puzz->solved());
    }
    return solvedlist;
}

void StudWin::on_hideButton_clicked() {
    if (levelshow) {
        ui->levelFrame->hide();
        levelshow = false;
        ui->hideButton->setText(QString("Show Levels"));
    } else {
        ui->levelFrame->showNormal();
        ui->hideButton->setText(QString("Hide Levels"));
        levelshow = true;
    }
}

//Move to controller?
void StudWin::studLogoutButton() {
  //  Client *par = client;
    Client *par = client;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(static_cast<QWidget *>(this), "Really QUIT??", "Quit?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
        UserSocket sock(sf::IpAddress::LocalHost, 11777, par->getSessionId());
        try {
            sock.deauthenticate();
        } catch (authenticationexception ex) {
        }
    } else {
        //recover
    }
}

void StudWin::on_checkBox_stateChanged(int arg1) {
    //There may be better way to get directory
    Q_INIT_RESOURCE(sprites);
    QFile musicfile(":rc/music.ogg");
    musicfile.copy(QString("music.ogg"));

    if (arg1 == 0) {
        music.stop();
    } else {
        if (!music.openFromFile("music.ogg")) {
            std::cout << "music broke" << std::endl;
        } else {
            music.setVolume(50);
            music.play();
        }
    }
}

std::vector<bool> StudWin::getUnlockedPuzzles() {
    UserSocket sock(sf::IpAddress::LocalHost, 11777,client->getSessionId());
    Message msg = sock.sendPayload("getSolvedPuzzles", client->username);
    QVector<QString> completedPuzzles = QString::fromStdString(msg.payload).split(",").toVector();
    return convertStringsToBools(completedPuzzles);
}

std::vector<bool> StudWin::convertStringsToBools(QVector<QString> strBools) {
    std::vector<bool>bools;
    std::vector<int> completed;
    for(auto it = strBools.begin(); it < strBools.end(); it++){
        std::string parse = it->toStdString();
        if(parse != ""){
            completed.push_back(std::stoi(parse));
        }
     }

    for (int i = 0; i < 4; i++) {
        bool found = false;
        for(auto it = completed.begin(); it < completed.end(); it++){
            if(*it==i){
                bools.push_back(true);
                found = true;
            }
        }
        if(!found){
            bools.push_back(false);
        }

    } // should always return a set of 4 bools
   return bools;
}

void StudWin::updatePuzzles(){
    std::vector<bool> unlocked = getUnlockedPuzzles();
    std::vector<QPushButton*> buttons;
    buttons.push_back(ui->puzzle1);
    buttons.push_back(ui->puzzle2);
    buttons.push_back(ui->puzzle3);
    buttons.push_back(ui->puzzle4);

    bool last = false;
    int i = 0;
    for(auto it = unlocked.begin(); it < unlocked.end(); it++){
         if(*it){
             last=true;
         }else{
             if(last){
                 unlocked[i]=true;
                 break;
             }else{
                 break; // if the last wasn't unlocked and this one isn't unlocked then we're done.
             }
         }
         i++;
    }

    if(!last){ // if nothing is unlocked, unlock the firs puzzle
        unlocked[0]=true;
    }

    i = 0;
    for(auto it = unlocked.begin(); it < unlocked.end(); it++){
        if(*it){
           unlocked[i]=true;
           buttons[i]->setDisabled(false);
           buttons[i]->setStyleSheet("Background-color: black;");
        }else{
           buttons[i]->setDisabled(true);
           buttons[i]->setStyleSheet("Background-color: grey;");
        }
        i++;
    }
    buttons[currPuzz]->setStyleSheet("Background-color: #3daee9;");
}


void StudWin::setCurrentUsername(QString currentUsername) {
    this->currentUsername = currentUsername;
    ui->userLabel->setText("Welcome: " + this->currentUsername);
}

void StudWin::on_puzzle1_clicked()
{
    currPuzz = 0;
    updatePuzzles();
    pw->setPuzzle(puzzles[0]);
    pw->setFocus();
}

void StudWin::on_puzzle2_clicked()
{
    currPuzz = 1;
    updatePuzzles();
    pw->setPuzzle(puzzles[1]);
    pw->setFocus();
}

void StudWin::on_puzzle3_clicked()
{
    currPuzz = 2;
    updatePuzzles();
    pw->setPuzzle(puzzles[2]);
    pw->setFocus();
    ui->puzzle3->setStyleSheet("Background-color: #3daee9;");
}

void StudWin::on_puzzle4_clicked()
{
    currPuzz = 3;
    updatePuzzles();
    pw->setPuzzle(puzzles[3]);
    pw->setFocus();
    ui->puzzle4->setStyleSheet("Background-color: #3daee9;");
}
