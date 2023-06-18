#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

using namespace std;

struct Data {
vector<double> veri;
char harf;
};

double minkowski_uzaklik(Data& a, Data& b) {
double mesafe = 0;
for (int i = 0; i < a.veri.size(); i++) {
mesafe += pow(abs(a.veri[i] - b.veri[i]), 3);
}
return pow(mesafe, 1.0/3);
}

vector<Data> read_data(string filename) {
    ifstream file(filename);
    vector<Data> data;
    string line;
    while (getline(file, line)) {
        Data d;
        stringstream ss(line);
        string veri;
        while (getline(ss, veri, ',')) {
            if (d.veri.size() < 10) {
                d.veri.push_back(stod(veri));
            } else {
                d.harf = veri[0];
            }
        }
        data.push_back(d);
    }
    return data;
}


char knn(vector<Data>& data, Data& a, int k) {
    vector<pair<double, char>> mesafeler;
    for (auto& d : data) {
        mesafeler.push_back({minkowski_uzaklik(d, a), d.harf});
    }
    sort(mesafeler.begin(), mesafeler.end());


    map<char, int> counts;
    for (const auto& d : data) {
        counts[d.harf] = 0;
    }

    for (int i = 0; i < k; i++) {
        counts[mesafeler[i].second]++;
    }

    int max_count = 0;
    char max_target = mesafeler[0].second;
    for (const auto& pair : counts) {
        if (pair.second > max_count) {
            max_count = pair.second;
            max_target = pair.first;
        }
    }

    return max_target;
}
void cross_validation(vector<Data>& data, int k) {
random_shuffle(data.begin(), data.end());
int dosya = data.size() / 5;
double basari = 0;
for (int i = 0; i < 5; i++) {
vector<Data> train_data, test_data;
for (int j = 0; j < data.size(); j++) {
if (j >= i*dosya && j < (i+1)*dosya) {
test_data.push_back(data[j]);
} else {
train_data.push_back(data[j]);
}
}
int dogruluk = 0;
for (auto& d : test_data) {
char tahmin = knn(train_data, d, k);
if (tahmin == d.harf) {
dogruluk++;
}
}
double success_rate = (double) dogruluk / test_data.size();
        basari += success_rate;
        cout << "Iterasyon " << i+1 << " Basari: " << success_rate << endl;
    }
    double average_success_rate = basari / 5;
    cout << "Ortalama Basari: " << average_success_rate << endl;
}

int main() {
vector<Data> data = read_data("avila.txt");
int k;
cout << "k degeri: ";
cin >> k;
cross_validation(data, k);
return 0;
}
