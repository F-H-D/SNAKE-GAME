#include <iostream> //standard liblary
#include <conio.h> // fungsi getch()
#include <fstream> //database
#include <limits> //untuk menghilangkan buffer
#include <windows.h> //ganti warna dan
#include <iomanip> // membuat space tabel history
#include <time.h> //membuat srand yang variatif
#define max_1 3
#define max_2 20

using namespace std;
// deklarasi main game
bool gameOver;
const int height=25;
const int width=50;
const int w_rand = width+46;
const int h_rand = height-2;
int x, y, fruitX, fruitY, score_smt;
int tailX[5000], tailY[5000], nTail;
enum eDirection {STOP=0, UP,LEFT, DOWN, RIGHT};
eDirection dir;
int boomX[max_2], boomY[max_2];

//deklarasi menu
int pil;
char yes_no;
enum option {START=1, HISTORY, EXIT};

//deklarasi history
int ketemu, posisi, i, j, k;
string nama[max_2];
int score [max_2][max_1];
int* higest_score = new int [max_2];
int tail[max_2];
int head=0;
int last_nama;

//fungsi 
void text_snake();
void text_history();
void text_keluar();
void Setup();
void Draw();
void Input();
void Logic();
void start_game(string &nama_start);
void tampilan_gameover();
void logic_gameover(char &c, int &chosen);
void gotoxy(int x, int y);
void tampilan_menu();
void logic_menu(char &c, int &chosen);
void best_score();
void searching_proses(string &nama_start);
void tampilan_history();
void logic_history(char &c, int &chosen);
void history_tabel();
void write_data();
void read_data();
void hapus_data(int &nomor);
struct nyawa_ular;
void input_nyawa_ular();
void main_lagi();
void keluar();
void selection_ascending();
void selection_descending();

int main (){
    read_data();
    menu:
    string nama_start;
    char c=0;
    int chosen=2;
    system("cls");
    tampilan_menu();
    while (c!=char(13))
    {
        logic_menu(c, chosen);
    }
    //pilihan menu utama start
    if (chosen==0)
    {
        start:
        char c=0;
        int chosen=2;
        system("cls");
        if(last_nama == 20){
            gotoxy(36,4);
            cout << "HISTORY PENYIMPANAN SUDAH MENCAPAI TITIK MAKSIMAL";
            gotoxy(30,5);
            cout << "HAPUS BEBERAPA DATA PADA HISTORY AGAR KERJA KERASMU TERSIMPAN\n\n";
            getch();
            goto menu;
        }
        start_game(nama_start);
        searching_proses(nama_start);
        best_score();
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        tampilan_gameover();
        logic_gameover(c, chosen);
        while (c!=char(13))
        {
        logic_gameover(c, chosen);
        }
        //pilihan menu setelah game over try again
        if (chosen==0){
            goto start;
        }
        //pilihan menu setelah game over back to menu utama
        if (chosen==2){
            goto menu;
        }
        //pilihan menu setelah game over exit
        if (chosen==4){
            write_data();
            system("cls");
            text_keluar();
            cout << endl;
            getch();
        }
    }
    //pilihan menu utama history
    if (chosen==2)
    {
        history:
        c=0;
        chosen = 2;
        history_tabel();
        tampilan_history();
        while (c!=char(13))
        {
        logic_history(c, chosen);
        }
        //pilihan menu history higest score
        if (chosen==0){
            selection_descending();
            goto history;
        }
        //pilihan menu history lowest score
        if (chosen==2){
            selection_ascending();
            goto history;
        }
        //pilihan menu history hapus data
        if (chosen==4){
            int nomor;
            system("cls");
            history_tabel();
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            gotoxy(53, last_nama+14);
            cout << "NUMBER : "; cin >> nomor;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            hapus_data(nomor);
            goto history;
        }
        //pilihan menu history back to menu utama
        if (chosen==6){
            system("cls");
            goto menu;
        }
        //pilihan menu history exit
        if (chosen==8){
            write_data();
            system("cls");
            text_keluar();
            cout << endl << endl;
            getch();
        }
    }
    //pilihan menu utama exit
    if (chosen==4)
    {
        write_data();
        system("cls");
        text_keluar();
        cout << endl;
        getch();
    }
}

//pengurutan dari kecil ke besar
void selection_ascending(){
    for (int j=0; j<last_nama; j++){
        int maks = j;
        for(k=j+1; k<last_nama; k++){
            if(higest_score[maks]>higest_score[k])
            maks = k;
        }
        if(higest_score[j] != higest_score[maks]){
            int tmp_higest_score = higest_score[j];
            higest_score[j] = higest_score[maks];
            higest_score[maks] = tmp_higest_score;

            string tmp_nama = nama[j];
            nama[j] = nama[maks];
            nama[maks] = tmp_nama;

            for(int m=0; m<max_1; m++) {
                int tmp_score = score[j][m];
                score[j][m] = score[maks][m];
                score[maks][m] = tmp_score;
            }
        }
    }
}
//pengurutan dari besar ke kecil
void selection_descending(){
       for (int j=0; j<last_nama; j++){
        int maks = j;
        for(k=j+1; k<last_nama; k++){
            if(higest_score[maks]<higest_score[k])
            maks = k;
        }
        if(higest_score[j] != higest_score[maks]){
            int tmp_higest_score = higest_score[j];
            higest_score[j] = higest_score[maks];
            higest_score[maks] = tmp_higest_score;

            string tmp_nama = nama[j];
            nama[j] = nama[maks];
            nama[maks] = tmp_nama;

            for(int m=0; m<max_1; m++) {
                int tmp_score = score[j][m];
                score[j][m] = score[maks][m];
                score[maks][m] = tmp_score;
            }
        }
    }
}

//database 
//menulis data ke file
void write_data(){
    ofstream data;
    data.open("FINAL PROJECT.txt", ios::trunc);
    data << last_nama << endl;
    for(int i=0; i<last_nama; i++){
        data << nama[i] << endl;
        data << tail[i] << endl;
        data << higest_score[i] << endl;
        for(int j=0; j<max_1; j++){
            data << score[i][j] << endl;
        }
        data << endl;
    }
    data.close();    
}
//membaca data dari file dan disimpan ke variabel
void read_data(){
    ifstream data;
    string x;
    data.open("FINAL PROJECT.txt");
    data >> last_nama;
    for(int i=0; i<last_nama; i++){
        data >> nama[i];
        data >> tail[i];
        data >> higest_score[i];
        for(int j=0; j<max_1; j++){
            data >> score[i][j];
        }
        getline(data,x);
    }
    data.close();
}
//penghapusan data pada tabel history
void hapus_data(int &nomor){
    fstream data;
    for(int i=0; i<last_nama; i++){
        if(i==nomor-1){
            data.open("FINAL PROJECT.txt", ios::trunc| ios::out);
            data << last_nama-1 << endl;
            for(int i=0; i<last_nama; i++){
                if(i==nomor-1){
                    data <<endl;
                }else{
                    data << nama[i] << endl;
                    data << tail[i] << endl;
                    data << higest_score[i] << endl;
                    for(int j=0; j<max_1; j++){
                        data << score[i][j] << endl;
                    }
                    data << endl;
                }
            }
            data.close();
            string x;
            data.open("FINAL PROJECT.txt", ios::in);
            data >> last_nama;
            for(int i=0; i<last_nama; i++){
                data >> nama[i];
                data >> tail[i];
                data >> higest_score[i];
                for(int j=0; j<max_1; j++){
                    data >> score[i][j];
                }
                getline(data,x);
            }
            data.close();
        }
    }
}

//nyawa ular
//menambah nyawa ular
struct nyawa_ular{
    int atas;
    char stack[max_1];
}T;
//menampilkan jumlah nyawa ular
void input_nyawa_ular(){
   while(T.atas<max_1){
        T.stack[T.atas]= 'A';
        T.atas++;
   }
   T.atas--;
}

//gotoxy
//membuat posisi untuk awalan text output
void gotoxy(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//menu utama
//menampilkan isi dari menu utama
void tampilan_menu(){
    text_snake();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(61,10);
    cout<<" START ";
    gotoxy(61,12);
    cout<<"HISTORY";
    gotoxy(61,14);
    cout<<" EXIT  ";
}
//anak panah bergerak dalam menu utama
void logic_menu(char &c, int &chosen){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
    gotoxy(58,10+chosen);
    cout<<"=>";
    gotoxy(69,10+chosen);
    cout<<"<=";
    gotoxy(0,0);
    c=getch();
    gotoxy(69,10+chosen);
    cout<<"  ";
    gotoxy(58,10+chosen);
    cout<<"  ";
    if (c=='s')
    {
        if (chosen<4)
        {
            chosen+=2;
        }
    }
    if (c=='w')
    {
        if (chosen>0)
        {
            chosen-=2;
        }
    }
}

//text
void random_color_text(){
    srand(time(0));
    int color,color_new;
    color = rand() % 16;
    if(color!=0){
        color_new = color;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color_new);
}
//text SNAKE
void text_snake(){
    random_color_text();
    gotoxy(29,1);
    cout << char(24) << "\t\t";
    cout << "/|=====   |\\\\   ||   /|====|\\   ||   //   ||=====";
    cout << "\t  " << char(24);
    gotoxy(29,2);
    cout << "W\t\t";
    cout << "||        ||\\\\  ||   ||    ||   ||  //    ||     ";
    cout << "\t  W";
    gotoxy(29,3);
    cout << " \t\t";
    cout << "\\|===|\\   || \\\\ ||   ||====||   ||===     ||=====";
    cout << "\t   ";
    gotoxy(29,4);
    cout << "S\t\t";
    cout << "     ||   ||  \\\\||   ||    ||   ||  \\\\    ||     ";
    cout << "\t  S";
    gotoxy(29,5);
    cout << char(25) << "\t\t";
    cout << "=====|/   ||   \\\\|   ||    ||   ||   \\\\   ||=====";
    cout << "\t  " << char(25);
    
}
//text HISTORY
void text_history(){
    random_color_text();
    gotoxy(12,1);
    cout << char(24) << "\t      ";
    cout << "||    ||   ||   /|=====   ========    /|====|\\    ||===\\\\   \\\\      //";
    cout << "\t      " << char(24);
    gotoxy(12,2);
    cout << "W\t      ";
    cout << "||    ||   ||   ||           ||      //      \\\\   ||    ||   \\\\    // ";
    cout << "\t      W";
    gotoxy(12,3);
    cout << " \t      ";
    cout << "||====||   ||   \\|===|\\      ||      ||      ||   ||===//     \\|==|/   ";
    cout << "\t       ";
    gotoxy(12,4);
    cout << "S\t      ";
    cout << "||    ||   ||        ||      ||      \\\\      //   ||   \\\\       ||    ";
    cout << "\t      S";
    gotoxy(12,5);
    cout << char(25) << "\t      ";
    cout << "||    ||   ||   =====|/      ||       \\|====|/    ||    \\\\      ||    ";
    cout << "\t      " << char(25);
}
//text THANKYOU
void text_keluar(){
    random_color_text();
    gotoxy(15,1);
    cout << "========  ||    ||   /|====|\\   |\\\\   ||   ||   //   \\\\      //    /|====|\\    ||    ||";
    gotoxy(15,2);
    cout << "   ||     ||    ||   ||    ||   ||\\\\  ||   ||  //     \\\\    //    //      \\\\   ||    ||";
    gotoxy(15,3);
    cout << "   ||     ||====||   ||====||   || \\\\ ||   ||===       \\|==|/     ||      ||   ||    ||";
    gotoxy(15,4);
    cout << "   ||     ||    ||   ||    ||   ||  \\\\||   ||  \\\\        ||       \\\\      //   ||    ||";
    gotoxy(15,5);
    cout << "   ||     ||    ||   ||    ||   ||   \\\\|   ||   \\\\       ||        \\|====|/    \\|====|/";
}

//history
//proses mencari best score
void best_score(){
    for (i=0; i<last_nama; i++){
        int k = *(higest_score + i);
        for (j=0; j<tail[i]; j++){
            if( k == 0 || k < score[i][j]){
                *(higest_score + i) = score[i][j];
            }
        }
    }
}
//searching proses untuk mencari posisi array dari nama
void searching_proses(string &nama_start){
    //proses searching nama agar data tidak double
    for(i=0; i<last_nama; i++){
        if(nama[i] == nama_start){
            ketemu = 1;
            posisi = i;
            i = last_nama;
        }
    //memulai proses queque pada score 
    }if(ketemu == 0){
        j = last_nama;
        int ta = tail[j];
        nama[j] = nama_start;
        last_nama++;
        score[j][ta] = score_smt;
        tail[j]++;
    }else{
        int t = tail[posisi];
        if(t!=max_1){
            score[posisi][t] = score_smt;
            tail[posisi]++;
            ketemu=0;
        }else{
            head++;
            for(k=head; k<=t; k++){
                score[posisi][k-1] = score [posisi][k];
            }
            head--;
            t--;
            score[posisi][t] = score_smt;
            t++;
            ketemu=0;
        }
    }
}
//menampilkan isi dari menu history
void tampilan_history(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(51,last_nama+14);
    cout<<" HIGEST SCORE ";
    gotoxy(51,last_nama+16);
    cout<<" LOWEST SCORE ";
    gotoxy(51,last_nama+18);
    cout<<"  CLEAR DATA  ";
    gotoxy(51,last_nama+20);
    cout<<" BACK TO MENU ";
    gotoxy(51,last_nama+22);
    cout<<"     EXIT     ";
}
//anak panah bergerak dalam menu history
void logic_history(char &c, int &chosen){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
    gotoxy(49,last_nama+14+chosen);
    cout<<"=>";
    gotoxy(65,last_nama+14+chosen);
    cout<<"<=";
    gotoxy(0,0);
    c=getch();
    gotoxy(49,last_nama+14+chosen);
    cout<<"  ";
    gotoxy(65,last_nama+14+chosen);
    cout<<"  ";
    if (c=='s')
    {
        if (chosen<8)
        {
            chosen+=2;
        }
    }
    if (c=='w')
    {
        if (chosen>0)
        {
            chosen-=2;
        }
    }
}
//output untuk menampilkan history
void history_tabel(){
    system("cls");
    text_history();
    gotoxy(8,8);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    for(int i=0; i<100; i++){
        cout << "=";
    }
    gotoxy(8,9);
    cout << "||" << setiosflags(ios::left) << setw(6)<< "  NO";
    cout << "||" << setiosflags(ios::left) << setw(20) << "         NAMA";
    for (int j=0; j<max_1; j++){
        cout << "||" << setiosflags(ios::left) << setw(9) << "   SCORE KE-";
        cout << setiosflags(ios::left) << setw(3) << j+1;
    }
    cout << "||" << setiosflags(ios::left) << setw(15) << "   BEST SCORE" << "||";
    gotoxy(8,10);
    for (j=0; j<100; j++){
        cout << "=";
    }
    for (int j=0; j<last_nama; j++){
        gotoxy(8,j+11);
        cout << setiosflags(ios::left) << setw(2);
        cout << "||  " << setiosflags(ios::left) << setw(4) << j+1;
        cout << setiosflags(ios::left) << setw(2);
        cout << "||  " << setiosflags(ios::left) << setw(18) << nama[j];
        for (k=0; k<max_1; k++){
            cout << setiosflags(ios::left) << setw(2);
            cout << "||  " << setiosflags(ios::left) << setw(13) << score[j][k];
        }
        cout << setiosflags(ios::left) << setw(2);
        cout << "||  " << setiosflags(ios::left) << setw(13) << higest_score[j] << "||\n";
    }
    gotoxy(8,last_nama+11);
    for (j=0; j<100; j++){
        cout << "=";
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
    gotoxy(8,last_nama+12);
    cout << "*batas maksimal data adalah 20";
}

//inti game
//persiapan sebelum memulai game
void Setup(){
    nTail=0;
    score_smt=0;
    gameOver=false;
    dir=STOP;
    x=width;
    y=height/2;
    srand(time(0));
    fruitX = rand() %w_rand;
    fruitY = rand() %h_rand;
    input_nyawa_ular();
    for(int s=0; s<max_2; s++){
        boomX[s] = rand() %w_rand;
        boomY[s] = rand() %h_rand;
    }

}
//membuat tampilan pada game
void Draw(){
    system("cls");
    text_snake();
    gotoxy(17,8);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
    cout << "SCORE: " << score_smt << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    gotoxy(109,8);
    for(int i=0; i<=T.atas; i++){
        cout << T.stack[i];
    }
    gotoxy(23,10);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout << "A";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    cout << " --> SNAKE LIFE";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
    gotoxy(43,10);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),1);
    cout << "O";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    cout << " --> SNAKE HEAD";
    gotoxy(63,10);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9);
    cout << "o";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    cout << " --> SNAKE TAIL";
    gotoxy(83,10);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout << "B";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    cout << " --> BOOM";
    gotoxy(97,10);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9);
    cout << "F";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    cout << " --> FOOD";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
    gotoxy(15,11);
    for(int i=0; i<width; i++){
        cout << "# ";
    }cout << endl;

    for (int i=0; i<height; i++){
        for (int j=0; j<width+50; j++){
            if(j==0){
                gotoxy(15,i+12);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
                cout << "#";
            }else if(j==width+48){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
                cout << "#";
            }else if(i==y && j==x){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),1);
                cout << "O";
            }else if(i==fruitY && j==fruitX){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9);
                cout <<"F";
            }else{

                bool print = false;

                for(int k=0; k<nTail; k++){
                    if(i==tailY[k] && j==tailX[k]){
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9);
                        cout << "o";
                        print = true;
                    }
                }
                for(int s=0; s<max_2; s++){
                    if(i==boomY[s] && j==boomX[s]){
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
                        cout <<"B";
                        print = true;
                    }
                }if (print==false){
                    cout << " ";
                }
            }
        }cout << "\n";
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
    gotoxy(15,36);
    for(int i=0; i<width; i++){
        cout << "# ";
    }
}
//menampung input dari a s w d
void Input(){
    if(_kbhit()){
        switch (_getch())
        {
        case 'w':
            dir=UP;
            break;
        case 'a':
            dir=LEFT;
            break;
        case 's':
            dir=DOWN;
            break;
        case 'd':
            dir=RIGHT;
            break;
        default:
            break;
        }
    }
}
//logika saat game berjalan
void Logic(){

    int prevX = tailX[0];
    int prevY = tailY[0];
    tailX[0]=x;
    tailY[0]=y;
    int prev2X, prev2Y;

    for(int i=1; i<nTail; i++){
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch(dir){
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        default:
            break;
    }
    if(x<0 || x>width+48 || y<0 || y>height-1){
        if(T.atas==0){
            T.atas--;
            Draw();
            gameOver=true;
            T.atas++;
        //hapus nyawa ular
        }else{
            x=width;
            y=height/2;
            T.atas--;
        }
        
    }
    for(int i=0; i<nTail; i++){
        if(x==tailX[i] && y==tailY[i]){
            if(T.atas==0){
                T.atas--;
                Draw();
                gameOver=true;
                T.atas++;
            //hapus nyawa ular
            }else{
                x=width;
                y=height/2;
                T.atas--;
            }
        }
    }
    if(x==fruitX && y==fruitY){
        score_smt+=10;
        srand(time(0));
        fruitX = rand() %w_rand;
        fruitY = rand() %h_rand;
        nTail++;
    }
    for(int s=0; s<max_2; s++){
        if(x==boomX[s] && y==boomY[s]){
            if(T.atas==0){
                T.atas--;
                Draw();
                gameOver=true;
                T.atas++;
            //hapus nyawa ular
            }else{
                x=width;
                y=height/2;
                T.atas--;
            }
        }
    }
    
    if (gameOver == true){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),78);
        gotoxy(61,23);
        cout << "GAME OVER";
    }
}
// berisi keseluruhan dari inti game termasuk setup, draw, input dan logic
void start_game(string &nama_start){
    text_snake();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(56,10);
    cout << "YOUR NAME : "; 
    cin >> nama_start;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    Setup();
    while(!gameOver){
        Draw();
        Input();
        Logic();
        if(dir==UP || dir==DOWN){
            Sleep(30);
        }else{}
    }
}
//menampilkan isi dari menu setelah game over
void tampilan_gameover(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(58,38);
    cout << "   TRY AGAIN  ";
    gotoxy(58,40);
    cout << "  BACK TO MENU ";
    gotoxy(58,42);
    cout << "      EXIT     ";
}
// anak panah bergerak dalam menu setelah game over
void logic_gameover(char &c, int &chosen){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
    gotoxy(57,38+chosen);
    cout<<"=>";
    gotoxy(73,38+chosen);
    cout<<"<=";
    gotoxy(0,0);
    c=getch();
    gotoxy(57,38+chosen);
    cout<<"  ";
    gotoxy(73,38+chosen);
    cout<<"  ";
    if (c=='s')
    {
        if (chosen<4)
        {
            chosen+=2;
        }
    }
    if (c=='w')
    {
        if (chosen>0)
        {
            chosen-=2;
        }
    }
}