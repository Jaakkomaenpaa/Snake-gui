/* COMP.CS.110 Projekti 4: snake_gui
 *
 * Määrittelee käyttöliittymän rakentavan luokan
 *
 * Ohjelman kirjoittaja
 *  Nimi: Jaakko Mäenpää
 *  Opiskelijanumero: 50344046
 *  Käyttäjätunnus: fxjama
 *  E-Mail: jaakko.maenpaa@tuni.fi
 *
*/


#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "gameboard.hh"
#include "point.hh"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QGraphicsView>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QKeyEvent>
#include <deque>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Rakentaja
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Tunnistaa näppäinpainallukset ja muokkaa direction_ -muuttujaa
    // niiden mukaisesti
    void keyPressEvent(QKeyEvent* event);

private slots:

    // Luo näytön eli napit, tekstikentät, tekstit jne.
    void createUserInterface();
    // Luo pelialueen
    void createGameArea();
    // Pelin mainloop, saa signaalin QTimer-oliolta annetuin väliajoin
    // (DURATION)
    void playGame();
    // Poistaa pelialueen ja muistia vievät komponentit
    void deleteGameArea();
    // Liikuttaa madon päätä, häntää ja ruokaa
    void moveHeadTailFood();
    // Tarkistaa pelin tilanteen voitolta ja häviöltä
    void checkWinOrLoss();
    // Tarkistaa, syökö mato ruuan tietyllä hetkellä
    bool eatsFood();
    // Resetoi pelin
    void resetGame();
    // Pysäyttää pelin
    void pauseGame();
    // Jatkaa peliä
    void continueGame();
    
    void getScores();
    
    void saveScore();

private:
    Ui::MainWindow *ui;

    // Alue jossa objekteja liikutetaan
    QGraphicsScene* scene_;
    // Näkyvillä oleva pelialue
    QGraphicsView* gameArea_;

    QGraphicsRectItem* snakeTail_;
    QGraphicsPixmapItem* snakeHead_;
    QGraphicsEllipseItem* food_;

    QComboBox* widthComboBox_;
    QComboBox* heightComboBox_;
    QLineEdit* seedLineEdit_;
    QLineEdit* playerNameLineEdit_;

    QLabel* test_;
    QLabel* winLoseLabel_;
    QLabel* scoreLabel_;
    QLabel* pauseTextLabel_;
    QLabel* playerLabel_;

    // Kutsuu createGameArea() -metodia
    QPushButton* createButton_;

    QPushButton* resetButton_;
    QPushButton* pauseButton_;
    QPushButton* continueButton_;

    GameBoard* gameBoard_;

    QTimer* timer_;

    int areaWidth_;
    int areaHeight_;
    int seed_;
    int score_;
    int highScore_;

    // Pitävät kirjaa ruuan sijainnista taustalla
    int latestFoodX_;
    int latestFoodY_;
    int currentFoodX_;
    int currentFoodY_;

    std::string playerName_;
    
    // Pitää kirjaa annetusta suunnasta
    std::string direction_;

    // Aika, jonka välein timer_ kutsuu playGamea
    const int DURATION = 200;
    
    // Käyttöliittymässä näytettävien pisteiden määrä
    const unsigned long int HIGHSCORES = 5;
    
    // Tiedostossa pidettävien pisteiden määrä
    const unsigned long int SCORES = 20;

    const char SPLITMARK = '|';

    const std::string SCOREFILE = "scores.txt";

    // Saatavilla olevat pelialueen korkeudet ja leveydet
    const std::vector< int > areaSizes_ = {
        150,
        200,
        250,
        300,
        350,
        400
    };
    
    std::vector< std::pair< int, std::string >> playerScores_;
    
    // Vektori johon talletetaan madon osat, paitsi pää ja häntä
    std::vector< QGraphicsRectItem* > snakeBody_;
};
#endif // MAINWINDOW_HH
