/* COMP.CS.110 Projekti 4: snake_gui
 *
 * Rakentaa käyttöliittymän annetulle koodipohjalle.
 * Käyttöliittymässä voi pelata perinteistä snake -peliä,
 * jonka tarkemmat toiminnot ja peliohjeet löytyvät
 * tiedostosta instructions.txt.
 *
 * Toimii käytännössä niin, että liikuttaa koodipohjan matoa
 * taustalla ja hakee sieltä aina tarvittavat tiedot, kuten
 * objektien sijainnit pelilaudalla. Käyttöliittymän positiot
 * on kerrottu kymmenellä, jotta objekteista ei tulisi liian
 * pieniä.
 *
 * Ohjelman kirjoittaja
 *  Nimi: Jaakko Mäenpää
 *  Opiskelijanumero: 50344046
 *  Käyttäjätunnus: fxjama
 *  E-Mail: jaakko.maenpaa@tuni.fi
 *
*/



#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "gameboard.hh"
#include "point.hh"

#include<fstream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    score_ = 0;
    highScore_ = 0;

    createUserInterface();
    getScores();
}

MainWindow::~MainWindow()
{
    deleteGameArea();
    delete ui;
}

void MainWindow::createUserInterface()
{

    // Määritellään tarvittavat widgetit
    QLabel* seedLabel = new QLabel(this);
    seedLineEdit_ = new QLineEdit(this);
    QLabel* playerNameLabel = new QLabel(this);
    playerNameLineEdit_ = new QLineEdit(this);
    QLabel* widthLabel = new QLabel(this);
    QLabel* heightLabel = new QLabel(this);
    scoreLabel_ = new QLabel(this);
    playerLabel_ = new QLabel(this);
    QLabel* highScoreHeaderLabel = new QLabel(this);
    winLoseLabel_ = new QLabel(this);
    pauseTextLabel_ = new QLabel(this);
    widthComboBox_ = new QComboBox(this);
    heightComboBox_ = new QComboBox(this);
    test_ = new QLabel(this);
    pauseButton_ = new QPushButton(this);
    continueButton_ = new QPushButton(this);
    createButton_ = new QPushButton(this);
    resetButton_ = new QPushButton(this);
    timer_ = new QTimer(this);

    // Kirjoitetaan niille tekstit
    seedLabel->setText("Seed:");
    playerNameLabel->setText("Player name:");
    widthLabel->setText("Width:");
    heightLabel->setText("Height:");
    scoreLabel_->setText("Score: " + QString::number(score_));
    highScoreHeaderLabel->setText("Top " + QString::number(HIGHSCORES)
                                  + " scores:");

    createButton_->setText("Start game");
    resetButton_->setText("Reset");
    pauseButton_->setText("Pause");
    continueButton_->setText("Continue");

    // Sijoitetaan widgetit paikoilleen
    seedLabel->setGeometry(10, 75, 80, 20);
    playerNameLabel->setGeometry(10, 10, 80, 20);
    playerNameLineEdit_->setGeometry(10, 35, 120, 30);
    seedLineEdit_->setGeometry(10, 100, 80, 30);
    widthLabel->setGeometry(10, 135, 80, 20);
    heightLabel->setGeometry(90, 135, 80, 20);
    widthComboBox_->setGeometry(10, 155, 80, 30);
    heightComboBox_->setGeometry(90, 155, 80, 30);
    createButton_->setGeometry(10, 190, 80, 30);
    resetButton_->setGeometry(90, 190, 80, 30);
    scoreLabel_->setGeometry(200, 10, 80, 30);
    playerLabel_->setGeometry(300, 10, 120, 30);
    highScoreHeaderLabel->setGeometry(10, 300, 100, 30);
    pauseButton_->setGeometry(10, 225, 80, 30);
    continueButton_->setGeometry(90, 225, 80, 30);


    timer_->setSingleShot(false);

    // Yhdistetään napit oikeisiin metodeihin
    connect(timer_, &QTimer::timeout, this, &MainWindow::playGame);
    connect(createButton_, &QPushButton::clicked, this,
            &MainWindow::createGameArea);
    connect(resetButton_, &QPushButton::clicked, this,
            &MainWindow::resetGame);
    connect(pauseButton_, &QPushButton::clicked, this,
            &MainWindow::pauseGame);
    connect(continueButton_, &QPushButton::clicked, this,
            &MainWindow::continueGame);

    // Poistetaan käytöstä napit, joita ei tällä hetkellä tarvita
    pauseButton_->setDisabled(true);
    continueButton_->setDisabled(true);
    resetButton_->setDisabled(true);

    // Lisätään pudotusvalikoihin pelialueen mahdolliset koot
    for (int widthSize : areaSizes_)
    {
        widthComboBox_->addItem(QString::number(widthSize));
        heightComboBox_->addItem(QString::number(widthSize));
    }
}

void MainWindow::createGameArea()
{

    if (playerNameLineEdit_->text().isEmpty())
    {
        playerName_ = "guest";
    }
    else
    {
        playerName_ = playerNameLineEdit_->text().toStdString();
    }
    playerLabel_->setText("Player: " + QString::fromStdString(playerName_));

    // Määritellään uudelleen nappien käytettävyys
    createButton_->setDisabled(true);
    pauseButton_->setDisabled(false);
    resetButton_->setDisabled(false);

    // Haetaan widgeteistä pelialueen luomiseen tarvittavat tiedot
    areaWidth_ = widthComboBox_->currentText().toInt();
    areaHeight_ = heightComboBox_->currentText().toInt();
    seed_ = seedLineEdit_->text().toInt();

    // Jonka jälkeen asetetaan nekin disabled-tilaan
    seedLineEdit_->clearFocus();
    seedLineEdit_->setDisabled(true);
    playerNameLineEdit_->clearFocus();
    playerNameLineEdit_->setDisabled(true);
    widthComboBox_->setDisabled(true);
    heightComboBox_->setDisabled(true);

    // Luodaan käyttöliittymässä näkyvä pelialue
    scene_ = new QGraphicsScene(this);
    gameArea_ = new QGraphicsView(this);
    gameArea_->setGeometry(200, 40, areaWidth_ + 2, areaHeight_ + 2);
    gameArea_->setScene(scene_);
    scene_->setSceneRect(0, 0, areaWidth_ - 1, areaHeight_ - 1);

    winLoseLabel_->setGeometry(200, areaHeight_ + 45, 80, 30);
    pauseTextLabel_->setGeometry(300, areaHeight_ + 45, 80, 30);

    // Luodaan taustalla oleva pelialue
    int gameBoardWidth = areaWidth_ / 10;
    int gameBoardHeight = areaHeight_ / 10;
    gameBoard_ = new GameBoard(gameBoardWidth,gameBoardHeight, seed_);

    // Haetaan ruuan sijainti taustalta
    Point foodStartPos = gameBoard_->getFood();
    int foodXPos = foodStartPos.getXPos();
    int foodYPos = foodStartPos.getYPos();

    // Piirretään häntä- ja ruokaobjektit
    QPen blackPen(Qt::black);   
    QBrush redBrush(Qt::red);
    QBrush greenBrush(Qt::darkGreen);
    food_ = scene_->addEllipse(0, 0, 10, 10,blackPen, redBrush);
    snakeTail_ = scene_->addRect(0,0, 10, 10, blackPen, greenBrush);

    // Haetaan madolle pää, joka luodaan automaattisesti kohtaan (0,0)
    std::string filename = "snakeHead.png";
    QPixmap image(QString::fromStdString(filename));
    image = image.scaled(11, 11);
    snakeHead_ = scene_->addPixmap(image);

    // Piilotetaan objektit, koska ne luodaan ensin kohtaan (0, 0), jotta
    // myöhemmin käytettävä metodi setPos() toimii oikein
    snakeHead_->hide();
    snakeTail_->hide();
    food_->hide();

    // Päivitetään ruuan viimeisin sijainti
    latestFoodX_ = foodXPos;
    latestFoodY_ = foodYPos;

    // Aloitetaan pelin pyöritys
    timer_->start(DURATION);
    gameArea_->show();
}

void MainWindow::playGame()
{
    // Asetetaan objektit näkyviin vasta nyt, etteivät ne näy aluksi
    // pelialueen yläkulmassa
    snakeHead_->show();
    snakeTail_->show();
    food_->show();

    moveHeadTailFood();

    // Päivitetään pisteitä sitä mukaa kun ruokaa syödään
    if (eatsFood())
    {
        ++score_;
        scoreLabel_->setText("Score: " + QString::number(score_));
    }

    // Liikutetaan madon runkoa
    std::deque< Point >snake = gameBoard_->snake_;
    if (snake.size() >= 2)
    {
        snake.pop_back();
        snake.pop_front();
        int snakeIndex = 0;
        for (Point snakePoint : snake)
        {
            snakeBody_.at(snakeIndex)->setPos(snakePoint.getXPos() * 10,
                                              snakePoint.getYPos() * 10);
            snakeBody_.at(snakeIndex)->show();
            ++snakeIndex;
        }
    }

    // Päivitetään taas ruuan edellinen sijainti
    latestFoodX_ = currentFoodX_;
    latestFoodY_ = currentFoodY_;

    // Tarkistetaan voitto- tai häviötilanne
    checkWinOrLoss();
}

void MainWindow::deleteGameArea()
{ 
    // Poistetaan mahdollisesti muistia vievät komponentit
    delete gameArea_;
    delete gameBoard_;

    scene_->removeItem(snakeHead_);
    scene_->removeItem(snakeTail_);
    scene_->removeItem(food_);
    for (QGraphicsRectItem* snakePart : snakeBody_)
    {
        scene_->removeItem(snakePart);
    }


    scene_->deleteLater();
    delete snakeHead_;
    delete snakeTail_;
    delete food_;

    snakeHead_ = nullptr;
    scene_ = nullptr;
    snakeTail_ = nullptr;
    food_ = nullptr;
    gameArea_ = nullptr;
    gameBoard_ = nullptr;

    for (QGraphicsRectItem* snakePart : snakeBody_)
    {
        delete snakePart;
        snakePart = nullptr;
    }   
}

void MainWindow::moveHeadTailFood()
{
    // Liikutetaan matoa valmiissa pohjassa ja haetaan sen perusteella
    // pää-, häntä- ja ruokaobjektien sijainnit
    gameBoard_->moveSnake(direction_);

    Point currentHeadPoint = gameBoard_->getHead();
    int currentHeadX = currentHeadPoint.getXPos();
    int currentHeadY = currentHeadPoint.getYPos();

    Point currentFoodPoint = gameBoard_->getFood();
    currentFoodX_ = currentFoodPoint.getXPos();
    currentFoodY_ = currentFoodPoint.getYPos();

    Point currentTailPoint = gameBoard_->getTail();
    int currentTailX = currentTailPoint.getXPos();
    int currentTailY = currentTailPoint.getYPos();

    // Liikutetaan päätä, häntää ja ruokaa (jos tarvitsee) käyttöliittymässä
    snakeHead_->setPos(currentHeadX * 10, currentHeadY * 10);
    food_->setPos(currentFoodX_ * 10, currentFoodY_ * 10);
    snakeTail_->setPos(currentTailX * 10, currentTailY * 10);


}

void MainWindow::checkWinOrLoss()
{
    // Häviötilanteessa maalataan alue punaiseksi
    if (gameBoard_->gameLost())
    {
        QBrush redBrush(Qt::red);
        winLoseLabel_->setText("You lost :(");
        gameArea_->setBackgroundBrush(redBrush);
    }
    // Voitossa vihreäksi
    if (gameBoard_->gameWon())
    {
        QBrush greenBrush(Qt::green);
        winLoseLabel_->setText("You won!");
        gameArea_->setBackgroundBrush(greenBrush);
    }
}

bool MainWindow::eatsFood()
{
    // Jos ruoka on liikkunut, eli se on syöty, lisätään madolle runko-osa
    if (currentFoodX_ != latestFoodX_ and currentFoodY_ != latestFoodY_)
    {
        QPen bluePen(Qt::darkBlue);
        QBrush blueBrush(Qt::blue);
        QGraphicsRectItem* snakePart = scene_->addRect(0, 0, 10, 10, bluePen,
                                                       blueBrush);
        // Piilotetaan osa, koska se luodaan ensin yläkulmaan
        snakePart->hide();
        snakeBody_.push_back(snakePart);
        return true;
    }
    return false;
}

void MainWindow::resetGame()
{

    // Nollataan kaikki tarvittavat komponentit
    deleteGameArea();
    direction_ = "";   
    snakeBody_ = {};
    continueButton_->setDisabled(true);
    winLoseLabel_->clear();
    pauseTextLabel_->clear();
    createGameArea();
    // Päivitetään pisteluetteloa
    saveScore();
    score_ = 0;
    scoreLabel_->setText("Score: " + QString::number(score_));
}

void MainWindow::pauseGame()
{
    // Asetetaan napit oikeisiin tiloihin
    timer_->stop();
    continueButton_->setDisabled(false);
    pauseButton_->setDisabled(true);
    seedLineEdit_->setDisabled(false);
    playerNameLineEdit_->setDisabled(false);
    widthComboBox_->setDisabled(false);
    heightComboBox_->setDisabled(false);
    pauseTextLabel_->setText("Paused");
}

void MainWindow::continueGame()
{
    // Asetetaan napit oikeisiin tiloihin
    timer_->start(DURATION);
    pauseButton_->setDisabled(false);
    continueButton_->setDisabled(true);
    seedLineEdit_->setDisabled(true);
    playerNameLineEdit_->setDisabled(true);
    widthComboBox_->setDisabled(true);
    heightComboBox_->setDisabled(true);
    pauseTextLabel_->clear();
}

void MainWindow::getScores()
{
    playerScores_ = {};

    std::ifstream scoreFile(SCOREFILE);
    std::string row;
    while(getline(scoreFile, row))
    {
        std::string::size_type pos = row.find(SPLITMARK);
        int score = stoi(row.substr(pos + 1));
        row.erase(pos);
        std::string playerName = row;
        playerScores_.push_back({score, playerName});
    }

    sort(playerScores_.rbegin(), playerScores_.rend());

    int scoresLength = 5;
    if(playerScores_.size() < 5)
    {
        scoresLength = playerScores_.size();
    }

    int plus = 0;
    for(int i = 0; i < scoresLength; ++i)
    {
        QLabel* highScoreLabel = new QLabel(this);
        QString player = QString::fromStdString(playerScores_.at(i).second);
        QString score = QString::number(playerScores_.at(i).first);
        highScoreLabel->setText(QString::number(i + 1) + ". " + score + 
                                ": " + player);
        highScoreLabel->setGeometry(10, 330+plus, 120, 20);
        plus += 25;
    }
}

void MainWindow::saveScore()
{
    bool scoreModified = false;

    if (playerScores_.size() < SCORES)
    {
        playerScores_.push_back({score_, playerName_});
        if (playerScores_.size() < 5)
        {
            scoreModified = true;
        }
    }

    else if (score_ > playerScores_.at(SCORES - 1).first)
    {
        playerScores_.at(SCORES - 1) = {score_, playerName_};
        if (score_ > playerScores_.at(4).first)
        {
            scoreModified = true;
        }
    }
    sort(playerScores_.rbegin(), playerScores_.rend());

    if (scoreModified)
    {
        std::ofstream scoreFile(SCOREFILE);
        for (std::pair<int, std::string > playerScore : playerScores_)
        {
            scoreFile << playerScore.second << SPLITMARK << playerScore.first
                      << std::endl;
        }
        getScores();
    }

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W)
    {
        direction_ = "w";
    }
    if (event->key() == Qt::Key_A)
    {
        direction_ = "a";
    }
    if (event->key() == Qt::Key_S)
    {
        direction_ = "s";
    }
    if (event->key() == Qt::Key_D)
    {
        direction_ = "d";

    }
}



