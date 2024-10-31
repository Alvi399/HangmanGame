#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <ctime>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;
class AbstractPlayer {
    public:
        virtual void readLeaderboard() = 0;
        virtual void updateLeaderboard(string name, int skor) = 0;
        virtual void displayLeaderboard() = 0;
        int skor[10] = {10,20,30,40,50,60,70,80,90,100};
        string topPlayer[10][2];
        

};
class IMekanisme {
public:
    virtual void drawHangman(int a) = 0;
    virtual void buatSoal() = 0;
    virtual void inisialisasiSoal() = 0;
    virtual void getAbjad(const string& kata) = 0;
    virtual void cekInput(char kataAsli) = 0; 
};

class Data: public IMekanisme, public AbstractPlayer {
public:
    void drawHangman(int kesempatan_param) override {
        switch (kesempatan_param) {
            case 5:
                cout << "   ___" << endl;
                cout << "  |     |" << endl;
                cout << "  |     O" << endl;
                cout << "  |" << endl;
                cout << "  |" << endl;
                break;
            case 4:
                cout << "   ___" << endl;
                cout << "  |     |" << endl;
                cout << "  |     O" << endl;
                cout << "  |     |" << endl;
                break;
            case 3:
                cout << "   ___" << endl;
                cout << "  |     |" << endl;
                cout << "  |     O" << endl;
                cout << "  |    /|" << endl;
                break;
            case 2:
                cout << "   ___" << endl;
                cout << "  |     |" << endl;
                cout << "  |     O" << endl;
                cout << "  |    /|\\" << endl;
                break;
            case 1:
                cout << "   ___" << endl;
                cout << "  |     |" << endl;
                cout << "  |     O" << endl;
                cout << "  |    /|\\" << endl;
                cout << "  |    /" << endl;
                break;
            case 0:
                cout << "   ___" << endl;
                cout << "  |     |" << endl;
                cout << "  |     O" << endl;
                cout << "  |    /|\\" << endl;
                cout << "  |    / \\" << endl;
                break;
        }
    } 

    void inisialisasiSoal() override {
        ifstream file ("./bankSoal.txt");
        string line;
        if (!file.is_open()) {
            cout << "File terkunci" << endl;
            return;
        }
        int i = 0, j = 0;
        while (getline(file, line)) {
            bankSoal[i][j] = line;
            j++;
            if (j == 11) {
                i++;
                j = 0;
            }
            if (i == 5) break;
        }
        file.close();
    }

    void buatSoal() override {
        int randomIndexRow = rand() % 5;
        int randomIndexCol = rand() % 10 + 1;
        int randomIndexColSkor = rand() % 10;
        Soal[0] = bankSoal[randomIndexRow][0];
        Soal[1] = bankSoal[randomIndexRow][randomIndexCol];
        Soal[2] = to_string(skor[randomIndexColSkor]);
    }

    void getAbjad(const string& kata) override {
        memset(abjadTebakan, 0, sizeof(abjadTebakan));
        int length = kata.length();
        if (length > 15) {
            cout << "Kata terlalu panjang, maksimum 15 karakter." << endl;
            return;
        }

        set<char> usedLetters(kata.begin(), kata.end());
        vector<int> indices(15);
        for (int i = 0; i < 15; i++) indices[i] = i;
        random_shuffle(indices.begin(), indices.end());

        for (int i = 0; i < length; i++) abjadTebakan[indices[i]] = kata[i];

        for (int i = length; i < 15; i++) {
            char randomLetter;
            do {
                randomLetter = 'a' + rand() % 26;
            } while (usedLetters.find(randomLetter) != usedLetters.end());
            abjadTebakan[indices[i]] = randomLetter;
            usedLetters.insert(randomLetter);
        }
    }

    void cekInput(char kataAsli) override {
        string soal = Soal[1];
        bool ditemukan = false;
        // Update progres_menjawab jika huruf ditemukan
        for (int i = 0; i < soal.size(); i++) {
            if (kataAsli == soal[i]) {
                progres_menjawab[i] = kataAsli;
                ditemukan = true;
            }
        }
        // Hapus huruf dari jangkauan abjad
        for (int j = 0; j < 15; j++) {
            if (abjadTebakan[j] == kataAsli) {
                abjadTebakan[j] = '-'; // Gantilah huruf dengan '-' untuk menandakan bahwa huruf tersebut tidak lagi dapat ditebak
                break; // Hentikan pencarian setelah huruf ditemukan
            }
        }
        // Kurangi kesempatan jika huruf tidak ditemukan
        if (!ditemukan) {
            kesempatan -= 1; // Kurangi kesempatan jika huruf tidak ditemukan
            cout << "Huruf '" << kataAsli << "' tidak ditemukan." << endl;
        }
    }
    void readLeaderboard() override {
        string path = "./leaderBoard.txt";
        ifstream file(path);
        if (!file.is_open()) {
            cout << "Failed to open leaderboard file." << endl;
            return;
        }
        string line;
        int i = 0;
        while (getline(file, line)) {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                topPlayer[i][0] = line.substr(0, pos);
                topPlayer[i][1] = line.substr(pos + 1);
                i++;
            }
            if (i == 10) break;
        }
        file.close();
    }

    void bubbleSort() {
        for (int i = 0; i < 10 - 1; ++i) {
            for (int j = 0; j < 10 - 1 - i; ++j) {
                // Jika skor saat ini lebih kecil dari skor berikutnya, tukar posisi
                if (stoi(topPlayer[j][1]) < stoi(topPlayer[j + 1][1])) {
                    swap(topPlayer[j], topPlayer[j + 1]);
                }
            }
        }
    }

    void updateLeaderboard(string username, int skor) override {
        // Konversi skor terendah di leaderboard (baris terakhir) ke integer untuk perbandingan
        int skorTerendah = stoi(topPlayer[9][1]);

        // Jika skor baru lebih besar dari skor terendah, kita periksa leaderboard
        if (skor > skorTerendah) {
            bool namaDitemukan = false;

            // Cek apakah username sudah ada di leaderboard
            for (int i = 0; i < 10; ++i) {
                if (topPlayer[i][0] == username) {
                    namaDitemukan = true;
                    // Jika nama ada, cek apakah skor baru lebih tinggi dari skor yang tersimpan
                    if (skor > stoi(topPlayer[i][1])) {
                        topPlayer[i][1] = to_string(skor); // Update skor
                    }
                    break;
                }
            }

            // Jika username belum ada di leaderboard, ganti entri terendah
            if (!namaDitemukan) {
                topPlayer[9][0] = username;
                topPlayer[9][1] = to_string(skor);
            }

            // Mengurutkan leaderboard menggunakan bubble sort
            bubbleSort();
        }
    }

    void displayLeaderboard() override {
        cout <<"===========TOP PLAYER==========" << endl;
        cout <<"|No|      Nama         |Skor  |" << endl;
        cout << "==============================" <<endl;
        for (int i = 0; i < 10; i++) {
            cout << i + 1 << ". " << topPlayer[i][0] << " " << topPlayer[i][1] << endl;
        }
    }


    char kataAsli;
    char abjadTebakan[15];
    string Soal[3]; //index 1: topik, 2:soal, 3:skor_soal
    string bankSoal[5][11];
    int kesempatan = 5;
    vector<char> progres_menjawab;
};

class Game : public Data {
public:
    Game(string username_param) : username(username_param), terjawab(false), gameOver(false), skorPlayer(0) {}
    
    void mainGame() {
        // inisialisasiSoal();
        // // for (int i = 0; i < 5; i++)
        // // {
        // //     for (int j = 0; j < 11; j++)
        // //     {
        // //         cout << bankSoal[i][j] << endl;
        // //     }
        // // }
        // buatSoal(); 
        // cout << Soal[0] << " : " << Soal[1] << " : " << Soal[2] << endl;
        
        while (!gameOver) {
            inisialisasiSoal();
            buatSoal();
            progres_menjawab = vector<char>(Soal[1].size(), '-');
            getAbjad(Soal[1]);
            
            while (!terjawab && kesempatan > 0) {
                cout << "Topik: " << Soal[0] <<"      Skor Soal: "<< Soal[2]<<"      Total Score: "<< skorPlayer<<"      Kesempatan: "<<kesempatan << endl;
                drawHangman(kesempatan);

                // Cetak progres_menjawab di layar setiap kali
                for (char c : progres_menjawab) {
                    cout << c << " ";
                }
                cout << endl;

                cout << "Jangkauan Huruf: ";
                for (char c : abjadTebakan) {
                    cout << c << " ";
                }
                cout << endl;

                cout << "Masukkan tebakan huruf: ";
                cin >> kataAsli;

                cekInput(kataAsli);

                // Cek apakah semua karakter dalam progres_menjawab sudah terjawab
                if (std::all_of(progres_menjawab.begin(), progres_menjawab.end(), [](char c) { return c != '-'; })) {
                    terjawab = true;
                    string resume;
                    skorPlayer += stoi(Soal[2]);
                    cout << "Selamat! Kamu berhasil menjawab semua kata." << endl;
                }

                if (kesempatan == 0 && !terjawab) {
                    gameOver = true;
                    cout << "Sayang sekali, kamu gagal menjawab. Jawabannya adalah: " << Soal[1] << endl;
                }
                cout << endl << endl;
            }

            // Reset status game untuk soal berikutnya jika ada
            if (terjawab) {
                terjawab = false;
                gameOver = false;
            }
        }
    }

    void endPlay() {
            // skorPlayer = 1000;
            system("cls"); // for Windows
            drawHangman(kesempatan);
            cout << "<=======================GAME OVER=============================>\n";
            readLeaderboard();
            updateLeaderboard(username, skorPlayer);
            displayLeaderboard();
            cout << "skor kamu: " << skorPlayer << " Dengan username: " <<username;
    }

private:
    string username;
    int skorPlayer;
    bool terjawab;
    bool gameOver;
};

int main() {
    system("cls");
    srand(time(0));
    cout << "---------------------------------------------\n";
    cout << "==================HANGMAN 2==================\n"; 
    cout << "---------------------------------------------\n";
    string username;
    cout << "Masukan username anda: ";
    cin >> username;
    system("cls");
    Game Hangman(username);
    Hangman.mainGame();
    Hangman.endPlay();
    
    return 0;
}
