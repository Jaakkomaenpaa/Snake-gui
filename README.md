# Snake_gui

Ohjelmointi 2 -kurssin viimeinen projekti. 

Tehtävänantona oli rakentaa Qt-käyttöliittymä valmiiksi annetulle ASCII-grafiikalla terminaalissa pelattavalle matopelille.
Omaa tuotostani ovat mainwindow.cpp ja mainwindow.hh. Tarkempia ohjeita pelin pelaamiseen ja toimintaperiaatteeseeen 
löytyy instructions.txt -tiedostosta. 

Peliin on lisätty vielä kurssin jälkeen uusi pisteytysjärjestelmä: pelaajien pisteet ja nimimerkit tallennetaan tiedostoon, 
josta käyttöliittymä hakee tiedot ja esittää parhaat pistesaavutukset sekä niiden saajien nimimerkit. Pisteytys on vielä
hieman keskeneräinen toiminto, sillä tällä hetkellä käyttöliittymä pitäisi sulkea jokaisen pelin jälkeen, jotta pisteet 
tallentuisivat.

Kommentointi on myös vielä vähäistä uusien ominaisuuksien kohdalta.

------------------------------------------------------------------------------------------------------------------------

The last project of Programming 2 course.

The assignment was to build a Qt user interface for an existing snake game that could be played in the terminal with
ASCII graphics. My commitments are mainwindow.cpp and mainwindow.hh. More specific instructions for playing the game
and the execution can be found in instructions.txt file. Unfortunately the instructions are only in Finnish at the
moment.

After the course, a new point calculating and storing method has been added: the user can choose a username and it
will be stored in a separate file with the points achieved in one game. The interface fetches the data from the file
and displays the best scores and the users that have achieved them. The scoring is a bit incomplete function, since
now the interface has to be closed after every game to store the points.

Commenting is also minimal with the new features.
