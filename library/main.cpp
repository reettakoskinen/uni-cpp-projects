#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <vector>
#include <algorithm>

/* Kirjasto
 *
 * Kuvaus:
 *   Ohjelma toteuttaa kirjaston, joka lukee kirjastojen tiedot
 *  tiedostosta ja tallentaa ne map tietorakenteeseen, joka
 *  koostuu merkkijono ja struct tyypeistä.
 *   Käyttäjä pystyy tekemään erilaisia hakuja liittyen kirjastojen
 *  kokoelmiin. Jos käyttäjän syöttämän tiedoston lukeminen onnistuu,
 *  käyttäjä voi etsiä mitä kirjastoja tietorankeenteessa on. Mitä materiaaleja
 *  kussakin kirjastossa on. Etsiä tietyn kirjailijan kirjoja ja saada
 *  tietää, mitä kirjoja on saatavilla ja kuinka pitkä varausjono niillä on
 *  kussakin kirjastossa tai tiedon, että kirja on hyllyssä lainattavissa.
 *  Käyttäjä voi myös etsiä tiettyä kirjaa ja saada tietää, onko se lainattavissa
 *  tai missä kirjastoissa sillä on lyhyin varausjono. Viimeisenä käyttäjä
 *  voi etsiä, mitkä kaikki kirjat kaikista kirjoista ovat tällä hetkellä
 *  hyllyssä lainattavissa.
 *   Ohjelma tarkistaa, ovatko käyttäjän antamat komennot hyväksyttäviä.
 * Ohjelma päättyy, kun käyttäjä antaa lopetuskomennon "quit".
 *
 * Ohjelman kirjoittaja
 * Nimi: Reetta Koskinen
 */

struct Book
{
    std::string author;
    std::string title;
    int reservations;
};

using Libraries = std::map<std::string, std::vector<Book>>;

std::vector<std::string> split(const std::string& s, const char delimiter, bool ignore_empty = false) {
    //Jakaa merkkijonona saadun lauseen sanoiksi. Saa parametreinään merkkijonon, jakaja merkin
    //sekä tiedon jätetäänkö tyhjät merkit huomioimatta. Palauttaa vektorin,
    //joka sisältää erotellut sanat
    std::vector<std::string> result;
    std::string tmp = s;

    while(tmp.find(delimiter) != std::string::npos) {
        std::string new_part = tmp.substr(0, tmp.find(delimiter));
        tmp = tmp.substr(tmp.find(delimiter)+1, tmp.size());
        if(not (ignore_empty and new_part.empty())) {
            result.push_back(new_part);
        }
    }
    if(not (ignore_empty and tmp.empty())) {
        result.push_back(tmp);
    }
    return result;
}

bool unknown_input(Libraries lib, std::string library_name, std::string author_name = "") {
    //Tarkistaa löytyykö käyttäjän syöttämä kirjasto tai kirjailija tietorakenteesta.
    //Saa parametreinä tietorakenteen, käyttäjän syöttämän kirjasto nimen
    //ja mahdollisesti kirjailijan nimen. Palauttaa true jos kirjasto tai kirjailija on
    //tuntematon ja true jos molemmat löytyvät tietorakenteesta.
    if (lib. find(library_name) == lib.end()) {
        std::cout << "Error: unknown library" << std::endl;
        return true;
    }
    if (author_name != "") {
        std::set<std::string> authors;
        for (std::vector<Book>::iterator i = lib.at(library_name).begin(); i != lib.at(library_name).end(); ++i) {
            authors.insert(i->author);
        }
         if (authors.find(author_name) == authors.end()) {
            std::cout << "Error: unknown author" << std::endl;
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool right_amount_parameters (std::vector<std::string> input, uint correct_amount) {
    //Tarkistaa onko käyttäjän antamassa haku käskyssä oikea määrä parametrejä.
    //Saa parametreinä käyttäjän antaman syötteen ja parametrien oikean määrän
    //kyseessä olevalle käskylle. Palauttaa true jos parametrejä on oikea määrä, muuten false.
    if (input.size() != correct_amount) {
        std::cout << "Error: wrong number of parameters" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

void print_libraries(Libraries lib) {
    //Tulostaa kaikki kirjastot. Saa parametrinä tietorakenteen,
    //johon kirjastojen tiedot on tallennettu.
    for (Libraries::iterator i = lib.begin(); i != lib.end(); ++i) {
        std::cout << i->first << std::endl;
    }
}

void print_materials(Libraries lib, std::string library_name) {
    //Tulostaa kaikki haetussa kirjastossa olevat kirjat. Saa parametreinä
    //tietorakenteen ja halutun kirjaston nimen.
    if (unknown_input(lib, library_name)) {
        return;
    }
    else {
        std::map<std::string, std::vector<std::string>> books;

        for (std::vector<Book>::iterator i = lib.at(library_name).begin(); i != lib.at(library_name).end(); ++i) {
            books.insert({i->author, {}});
            books.at(i->author).push_back(i->title);
            sort(books.at(i->author).begin(), books.at(i->author).end());
        }
        for (auto& info_pair : books) {
            for (std::string& book : info_pair.second) {
                std::cout << info_pair.first << ": " << book << std::endl;
            }
        }
    }
}

void print_books(Libraries lib, std::string library_name, std::string author_name) {
    //Tulostaa kaikki haetun tekijän kirjat, jotka löytyvät haetusta kirjastosta
    //ja kirjojen varaustiedot. Saa parametreinä tietorakenteen, halutun kirjaston nimen
    //ja halutun kirjailijan nimen.
    if (unknown_input(lib, library_name, author_name)) {
        return;
    }
    else {
        for (std::vector<Book>::iterator i = lib.at(library_name).begin(); i != lib.at(library_name).end(); ++i) {
            if (i->author == author_name) {
                if (i->reservations == 0) {
                    std::cout << i->title << " --- " << "on the shelf" << std::endl;
                }
                else {
                    std::cout << i->title << " --- " << i->reservations << " reservations" << std::endl;
                }
            }
        }
    }
}

void print_reservable(Libraries lib, std::string author_name, std::string book_name) {
    //Tulostaa lyhyimmän varausjonon halutun tekijän halutulle kirjalle ja sen alle
    //kirjastot, joista kirja löytyy tällä samanpituisella varausjonolla.
    //Saa parametreinä tietorakenteen, halutun kirjailijan nimen
    //ja halutun kirjan nimen.
    std::map<std::string, int> reservations;
    std::set<std::string> authors;
    std::set<std::string> books;
    int min = 100;

    for (Libraries::iterator i = lib.begin(); i != lib.end(); ++i) {
        std::string library_name = i->first;
        for (auto book : lib.at(library_name)) {
            authors.insert(book.author);
            books.insert(book.title);
            if (book.title == book_name and book.author == author_name) {
                reservations.insert({library_name, book.reservations});
                if (book.reservations < min) {
                    min = book.reservations;
                }
            }
        }
    }
    if (authors.find(author_name) == authors.end() or books.find(book_name) == books.end()) {
        std::cout << "Book is not a library book" << std::endl;
    }
    //Jos varausjono on 100 tai pidempi, min pysyy muuttumattomana.
    else if (min == 100) {
        std::cout << "Book is not reservable from any library" << std::endl;
    }
    else {
        if (min == 0) {
            std::cout << "on the shelf" << std::endl;
        }
        else {
        std::cout << min << " reservations" << std::endl;
        }
        for (auto& library : reservations) {
            if (library.second == min) {
                std::cout << "--- " << library.first << std::endl;
            }
        }
    }
}

void print_loanable_books(Libraries lib) {
    //Tulostaa kaikkien kirjastojen ne kirjat, jotka ovat heti lainattavissa.
    //Saa parametrinä tietorakenteen.
    std::map<std::string, std::vector<std::string>> books_info;

    for (Libraries::iterator i = lib.begin(); i != lib.end(); ++i) {
        std::string library_name = i->first;
        for (auto& book : lib.at(library_name)) {
            if (book.reservations == 0) {
                books_info.insert({book.author, {}});
                books_info.at(book.author).push_back(book.title);
                sort(books_info.at(book.author).begin(), books_info.at(book.author).end());
            }
        }
    }
    for (auto& author_book_pair : books_info) {
        for (auto& book_name : books_info.at(author_book_pair.first)) {
            std::cout << author_book_pair.first << ": ";
            std::cout << book_name << std::endl;
        }
    }
}

int main() {
    Libraries lib;

    std::string file_name;
    std::cout << "Input file: ";
    getline(std::cin, file_name);

    //Tarkista aukeaako tiedosto.
    std::ifstream file(file_name);
    if (not file) {
        std::cout << "Error: input file cannot be opened" << std::endl;
        return EXIT_FAILURE;
    }

    //Erottele kirjastojen ja kirjojen tiedot tiedoston riveiltä.
    std::string line;
    bool invalid_line = false;
    while ( getline(file, line) ) {
        std::vector<std::string> strings = split(line, ';', true);

        //Tarkista onko rivin tiedot oikein.
        if (strings.size() != 4) {
            invalid_line = true;
        }
        for (std::string& word : strings) {
            if (word.length() == 0) {
                invalid_line = true;
            }
        }
        if (invalid_line) {
            std::cout << "Error: empty field" << std::endl;
            return EXIT_FAILURE;
        }
        std::string library_name = strings.at(0);
        std::string author = strings.at(1);
        std::string book_name = strings.at(2);
        int reservations;
        if (strings.at(3) == "on-the-shelf"){
            reservations = 0;
        }
        else {
            reservations = stoi(strings.at(3));
        }

        //Tarkista onko kirjasto jo tietokannassa, jos ei lisää se.
        if (lib.find(library_name) == lib.end()) {
            lib.insert({library_name, {}});
        }
        //Lisää kirja kirjastoon.
        Book book_instance = {author, book_name, reservations};
        lib.at(library_name).push_back(book_instance);
    }
    file.close();

    //Pyydä käyttäjältä syötettä kunnes käyttäjä lopettaa ohjelman.
    bool keep_asking = true;
    while (keep_asking) {
        std::string input;
        std::cout << "lib> ";
        getline(std::cin, input);

        std::vector<std::string> inputs = split(input, ' ', true);

        if (inputs.at(0) == "quit") {
            if (right_amount_parameters(inputs, 1)) {
                keep_asking = false;
            }
        }

        else if (inputs.at(0) == "libraries") {
            if (right_amount_parameters(inputs, 1)) {
                print_libraries(lib);
            }
        }

        else if (inputs.at(0) == "material") {
            if (right_amount_parameters(inputs, 2)) {
                std::string library_name = inputs.at(1);
                print_materials(lib, library_name);
            }
        }

        else if (inputs.at(0) == "books") {
            if (right_amount_parameters(inputs, 3)) {
                std::string library_name = inputs.at(1);
                std::string author_name = inputs.at(2);
                print_books(lib, library_name, author_name);
            }
        }

        else if (inputs.at(0) == "reservable") {
            if (inputs.size() < 3) {
                std::cout << "Error: wrong number of parameters" << std::endl;
            }
            else {
                std::string author_name = inputs.at(1);
                std::string book_name = inputs.at(2);
                //Lisää "book_name" merkkijonoon kaikki loput merkkijonot vektorista, jos kirjan nimi koostuu useasta sanasta.
                int lenght = inputs.size();
                if (lenght > 3) {
                    for (int i = 3; i < lenght; i++) {
                        book_name = book_name + " " + inputs.at(i);
                    }
                    //Poista " " merkit, jos ne löytyy.
                    if(book_name.find('"') != std::string::npos) {
                        book_name.pop_back();
                        book_name.erase(0, 1);
                    }
                }
                print_reservable(lib, author_name, book_name);
            }
        }

        else if (inputs.at(0) == "loanable") {
            if (right_amount_parameters(inputs, 1)) {
                print_loanable_books(lib);
            }
        }

        else {
            std::cout << "Error: unknown command" << std::endl;
        }
     }
    return 0;
}
