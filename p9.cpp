//Вариант 24

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Time {
    int hours;
    int minutes;
};

enum type {
    charter, transit, stikovka
};

struct flytable {
    string pp;
    Time vo;
    Time vp;
    Time vf;
    int cost;
    type raise;
};


void print(flytable p) {
    cout << p.pp << " " << p.vo.hours << ":" << p.vo.minutes << " " << p.vp.hours << ":" << p.vp.minutes << " " << p.vf.hours << ":" << p.vf.minutes << " " << p.cost << " " << p.raise << endl;
}

void print(flytable p[], int length) {
    for (size_t i = 0; i < length; i++)
    {
        print(p[i]);
    }
}

void krasivaya(flytable flighst[], int length, flytable*& fluit, int& length_f, char K, type raise) {
    int count = 0;
    for (size_t i = 0; i < length; i++)
    {
        if ((flighst[i].pp[0] == K) && (flighst[i].raise == raise)) {
            count++;
        }
    }
    fluit = new  flytable[count];
    length_f = count;
    for (size_t i = 0, j = 0; i < length; i++)
    {
        if ((flighst[i].pp[0] == K) && (flighst[i].raise == raise)) {
            fluit[j] = flighst[i];
            j++;
        }
    }
}

void puzirok(flytable*& fluit, int length) {
    for (size_t i = 0; i < length; i++)
    {
        for (size_t j = 0; j < length - 1; j++)
        {
            if (fluit[j].vf.hours < fluit[j + 1].vf.hours) {
                flytable aux = fluit[j];
                fluit[j] = fluit[j + 1];
                fluit[j + 1] = aux;
            }
            else if (fluit[j].vf.hours == fluit[j + 1].vf.hours) {
                if (fluit[j].vf.minutes < fluit[j + 1].vf.minutes) {
                    flytable aux = fluit[j];
                    fluit[j] = fluit[j + 1];
                    fluit[j + 1] = aux;
                }
            }
        }
    }
}

void vremya_nochi_22_6(flytable f[], type charterniy, int length, int count) {
    for (size_t i = 0; i < length; i++)
    {
        if (((f[i].vo.hours >= 22) || (f[i].vo.hours <= 6)) && (f[i].raise == charterniy)) {
            print(f[i]);
            count++;
            if (count == 5) return;
        }
    }
}

void izmenenie_reisa(flytable f[], Time vremya_valeta, string punkt_posadki, int length, string pp, Time vo, Time vp, Time vf, int cost, type raise) {
    for (size_t i = 0; i < length; i++)
    {
        if ((f[i].pp == punkt_posadki) && (f[i].vo.hours == vremya_valeta.hours) && (f[i].vo.minutes == vremya_valeta.minutes)) {
            f[i].pp = pp;
            f[i].vo = vo;
            f[i].vp = vp;
            f[i].vf = vf;
            f[i].cost = cost;
            f[i].raise = raise;
            return;
        }
    }
}

void stikov_reis(flytable flights[], flytable*& f, int cost, type stikovka, int length, int& count) {
    count = 0;
    for (size_t i = 0; i < length; i++)
    {
        if ((flights[i].raise == stikovka) && (flights[i].cost < cost)) count++;
    }
    f = new flytable[count];
    for (size_t i = 0, j = 0; i < length; i++)
    {
        if ((flights[i].raise == stikovka) && (flights[i].cost < cost)) {
            f[j] = flights[i];
            print(f[j]);
            j++;
        }
    }
}

void zap_file(flytable f[], int length) {
    ofstream fout("rez_9.txt");
    for (size_t i = 0; i < length; i++)
    {
        fout << f[i].pp << " | Отправление: " << f[i].vo.hours << ":" << f[i].vo.minutes << " | Цена: " << f[i].cost << endl;
    }
    fout.close();
    cout << "Структура записана в файл rez_9.txt";
}

void read_file() {
    string str;
    ifstream fin("rez_9.txt");
    if (fin.is_open()) {
        cout << "Файл прочитан:" << endl;
        while (!fin.eof()) {
            getline(fin, str);
            cout << str << endl;
        }
    }
    fin.close();
}

void zap_bin_file(flytable f[], int length) {
    ofstream fout("data.bin", ios::binary | ios::out);
    if (!fout.is_open()) return;

    for (int i = 0; i < length; i++) {
        size_t str_size = f[i].pp.size();

        fout.write((char*)&str_size, sizeof(str_size));
        fout.write(f[i].pp.c_str(), str_size);

        fout.write((char*)&f[i].vo, sizeof(Time));
        fout.write((char*)&f[i].vp, sizeof(Time));
        fout.write((char*)&f[i].vf, sizeof(Time));
        fout.write((char*)&f[i].cost, sizeof(int));
        fout.write((char*)&f[i].raise, sizeof(type));
    }
    fout.close();
    cout << "Бинарный файл записан." << endl;
}


void read_bin_file(int length) {
    ifstream fin("data.bin", ios::binary | ios::in);
    if (!fin.is_open()) return;

    cout << "Чтение из бинарного файла" << endl;
    for (int i = 0; i < length; i++) {
        flytable temp;
        size_t str_size;


        fin.read((char*)&str_size, sizeof(str_size));
        char* buffer = new char[str_size + 1];
        fin.read(buffer, str_size);
        buffer[str_size] = '\0';
        temp.pp = buffer;
        delete[] buffer;

        fin.read((char*)&temp.vo, sizeof(Time));
        fin.read((char*)&temp.vp, sizeof(Time));
        fin.read((char*)&temp.vf, sizeof(Time));
        fin.read((char*)&temp.cost, sizeof(int));
        fin.read((char*)&temp.raise, sizeof(type));

        print(temp);
    }
    fin.close();
}


int main() {
    setlocale(LC_ALL, "ru");
    const int LENGTH = 20;
    flytable flights[20] = {
        {"Канск", {8, 0}, {10, 30}, {2, 30}, 5000, transit},
        {"Красноярск", {12, 0}, {13, 45}, {1, 45}, 4500, charter},
        {"Москва", {6, 0}, {8, 20}, {2, 20}, 8000, stikovka},
        {"Киров", {23, 0}, {0, 30}, {1, 30}, 3000, transit},
        {"Сочи", {9, 0}, {11, 0}, {2, 0}, 7000, charter},
        {"Курск", {22, 0}, {23, 30}, {1, 30}, 2800, transit},
        {"Калуга", {5, 0}, {6, 45}, {1, 45}, 3200, transit},
        {"Новгород", {14, 0}, {16, 0}, {2, 0}, 4000, stikovka},
        {"Кемерово", {20, 0}, {22, 15}, {2, 15}, 5500, charter},
        {"Псков", {7, 0}, {9, 0}, {2, 0}, 3500, transit},
        {"Кострома", {18, 0}, {19, 45}, {1, 45}, 3100, transit},
        {"Коломна", {10, 0}, {11, 30}, {1, 30}, 2900, transit},
        {"Тверь", {15, 0}, {16, 30}, {1, 30}, 2700, stikovka},
        {"Клин", {21, 0}, {22, 30}, {1, 30}, 2600, transit},
        {"Владивосток", {1, 0}, {8, 0}, {7, 0}, 12000, charter},
        {"Когалым", {23, 30}, {1, 0}, {1, 30}, 4000, transit},
        {"Краснодар", {11, 0}, {13, 0}, {2, 0}, 4800, charter},
        {"Рязань", {13, 0}, {14, 30}, {1, 30}, 2500, stikovka},
        {"Калининград", {3, 0}, {5, 30}, {2, 30}, 6000, transit},
        {"Казань", {17, 0}, {18, 45}, {1, 45}, 4200, charter} };

    zap_file(flights, LENGTH);
    cout << endl << endl;
    read_file();
    cout << endl << endl;
    zap_bin_file(flights, LENGTH);
    cout << endl << endl;
    read_bin_file(LENGTH);
    cout << endl << endl;
    flytable* f = nullptr;
    int length_f;
    krasivaya(flights, LENGTH, f, length_f, 'К', transit);
    cout << "Вывод структуры с названием на К:" << endl;
    print(f, length_f);

    puzirok(f, length_f);
    cout << endl << "Вывод структуры после сортировки пузырьком:" << endl;
    print(f, length_f);

  
    delete[] f;
    f = nullptr;

    cout << endl << "Вывод структуры после сортировки по времени:" << endl;
    vremya_nochi_22_6(flights, charter, LENGTH, 0);

    cout << endl << "Вывод структуры после изменения рейса:" << endl;
    izmenenie_reisa(flights, { 15,0 }, "Тверь", LENGTH, "Чита", { 12,0 }, { 15,15 }, { 3,15 }, 2600, stikovka);
    print(flights, LENGTH);

    cout << endl << "Вывод структуры с одним типом:" << endl;

    stikov_reis(flights, f, 3000, stikovka, LENGTH, length_f);
    print(f, length_f);


    delete[] f;

    return 0;
}